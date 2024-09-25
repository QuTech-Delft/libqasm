#include "v3x/AnalyzeTreeGenAstVisitor.hpp"

#include <algorithm>  // any_of, for_each, transform
#include <any>
#include <cassert>  // assert
#include <iterator>  // back_inserter
#include <range/v3/view/tail.hpp>  // tail

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-instruction.hpp"

namespace cqasm::v3x::analyzer {

AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{} {}

std::any AnalyzeTreeGenAstVisitor::visit_node(ast::Node & /* node */) {
    throw error::AnalysisError{ "unimplemented" };
}

std::any AnalyzeTreeGenAstVisitor::visit_program(ast::Program &program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.api_version;
    result_.root->version = std::any_cast<tree::One<semantic::Version>>(visit_version(*program_ast.version));
    auto [block, variables] = std::any_cast<GlobalBlockReturnT>(visit_global_block(*program_ast.block));
    result_.root->block = std::move(block);
    result_.root->variables = variables;
    return result_;
}

std::any AnalyzeTreeGenAstVisitor::visit_version(ast::Version &node) {
    auto ret = tree::make<semantic::Version>();
    try {
        // Check API version
        for (auto item : node.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (node.items != analyzer_.api_version) {
            throw error::AnalysisError{
                fmt::format("the only cQASM version supported is {}, but the cQASM file is version {}",
                    analyzer_.api_version,
                    node.items)
            };
        }

        ret->items = node.items;
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));

        // Default to API version in case the version in the AST is broken
        ret->items = analyzer_.api_version;
    }
    ret->copy_annotation<parser::SourceLocation>(node);
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_global_block(ast::GlobalBlock &node) {
    visit_block(node);
    return GlobalBlockReturnT{ analyzer_.current_block(), analyzer_.current_variables() };
}

std::any AnalyzeTreeGenAstVisitor::visit_annotated(ast::Annotated &node) {
    auto ret = tree::Any<semantic::AnnotationData>();
    for (const auto &annotation_data_ast : node.annotations) {
        ret.add(std::any_cast<ast::One<semantic::AnnotationData>>(visit_annotation_data(*annotation_data_ast)));
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_annotation_data(ast::AnnotationData &node) {
    auto ret = tree::make<semantic::AnnotationData>();
    try {
        ret->interface = node.interface->name;
        ret->operation = node.operation->name;
        for (const auto &expression_ast : node.operands->items) {
            try {
                ret->operands.add(std::any_cast<values::Value>(expression_ast->visit(*this)));
            } catch (error::AnalysisError &err) {
                err.context(node);
                result_.errors.push_back(std::move(err));
            }
        }
        ret->copy_annotation<parser::SourceLocation>(node);
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_variable(ast::Variable &node) {
    auto ret = tree::make<semantic::Variable>();
    try {
        // Build semantic type from syntactic type
        auto type = build_semantic_type(node.typ);

        // Construct variable
        // Use the location tag of the identifier to record where the variable was defined
        const auto &identifier = node.name;
        ret->name = identifier->name;
        ret->typ = type.clone();
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(*identifier);

        // Add the variable to the current scope
        analyzer_.add_variable_to_current_scope(ret);

        // Register the variable
        analyzer_.register_variable(identifier->name, tree::make<values::VariableRef>(ret));
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

/**
 * Promote a value to a given type,
 * or error if the promotion is not valid
 */
values::Value promote_or_error(const values::Value &rhs_value, const types::Type &lhs_type) {
    if (auto rhs_promoted_value = values::promote(rhs_value, lhs_type); !rhs_promoted_value.empty()) {
        return rhs_promoted_value;
    }
    throw error::AnalysisError{
        fmt::format("type of right-hand side ({}) could not be coerced to left-hand side ({})",
            values::type_of(rhs_value), lhs_type)
    };
}

/**
 * Convenience function for extracting the types of a list of variables.
 */
types::Types types_of(const tree::Any<semantic::Variable> &variables) {
    types::Types types{};
    for (const auto &variable : variables) {
        types.add(variable->typ.clone());
    }
    return types;
}

std::any AnalyzeTreeGenAstVisitor::visit_gate_instruction(ast::GateInstruction &node) {
    auto ret = tree::make<semantic::GateInstruction>();
    try {
        // Set gate and operand list
        auto gate = std::any_cast<tree::One<semantic::ModifiableGate>>(node.gate->visit(*this));
        auto operands = std::any_cast<values::Values>(node.operands->visit(*this));

        // Resolve the instruction
        ret.set(analyzer_.resolve_instruction(gate, operands));

        // Add the statement to the current scope
        analyzer_.add_statement_to_current_scope(ret);
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

void check_modifiable_gate(const tree::One<semantic::ModifiableGate> &gate) {
    if ((gate->name != instruction::pow_gate_modifier_name && !gate->operands.empty()) ||
        (gate->name == instruction::pow_gate_modifier_name &&
            !(gate->operands.size() == 1 &&
                values::check_promote(values::type_of(gate->operands[0]), tree::make<types::Float>())))) {
        throw error::AnalysisError{
            fmt::format("failed to resolve modifiable gate '{} with argument pack ({})",
                gate->full_name,
                values::types_of(gate->operands)
            )
        };
    }
}

void resolve_modifiable_gate_operands(const tree::One<semantic::ModifiableGate> &gate) {
    if (gate->name == instruction::pow_gate_modifier_name) {
        gate->operands[0] = promote(gate->operands[0], tree::make<types::Float>());
    }
}

void calculate_modifiable_gate_names(tree::One<semantic::ModifiableGate> &gate) {
    if (gate->modified_gate.empty()) {
        gate->full_name = gate->name;
        gate->resolution_name = gate->name;
        gate->terminal_name = gate->name;
        return;
    }
    gate->full_name = fmt::format("{}_{}", gate->name, gate->modified_gate->full_name);
    if (gate->name != instruction::inv_gate_modifier_name &&
        gate->name != instruction::pow_gate_modifier_name &&
        gate->name != instruction::ctrl_gate_modifier_name) {
        throw error::AnalysisError{
            fmt::format("invalid gate modifier: found '{}', expecting 'inv', 'pow' or 'ctrl'", gate->name) };
    }
    if (gate->modified_gate->resolution_name.starts_with(instruction::unitary_2_gate_prefix)) {
        throw error::AnalysisError{ "trying to apply a gate modifier to a multi-qubit gate" };
    }
    if (gate->name == instruction::inv_gate_modifier_name or gate->name == instruction::pow_gate_modifier_name) {
        gate->resolution_name = fmt::format("{}_{}", instruction::unitary_1_gate_prefix, gate->modified_gate->terminal_name);
    } else if (gate->name == instruction::ctrl_gate_modifier_name) {
        gate->resolution_name = fmt::format("{}_{}", instruction::unitary_2_gate_prefix, gate->modified_gate->terminal_name);
    }
    gate->terminal_name = gate->modified_gate->terminal_name;
}

std::any AnalyzeTreeGenAstVisitor::visit_modifiable_gate(ast::ModifiableGate &node) {
    auto ret = tree::make<semantic::ModifiableGate>();
    try {
        ret->name = node.name->name;
        if (!node.modified_gate.empty()) {
            ret->modified_gate =
                std::any_cast<tree::One<semantic::ModifiableGate>>(node.modified_gate->visit(*this)).get_ptr();
        }
        ret->operands = std::any_cast<values::Values>(visit_expression_list(*node.operands));
        calculate_modifiable_gate_names(ret);

        // Specific checks
        check_modifiable_gate(ret);

        // Resolve the gate
        resolve_modifiable_gate_operands(ret);
        
        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

void check_qubit_and_bit_indices_have_same_size(const values::Values &operands) {
    size_t qubit_indices_size{};
    size_t bit_indices_size{};
    // Instruction operands can be, either variables references or index references
    // Variables can be of type qubit, bit, qubit array, or bit array
    // Qubits and bits have a single index, arrays have a size
    // Index references point to a qubit array or bit array
    for (const auto &operand : operands) {
        if (auto variable_ref = operand->as_variable_ref()) {
            const auto &variable = *variable_ref->variable;
            if (variable.typ->as_qubit()) {
                qubit_indices_size += 1;
            } else if (auto qubit_array = variable.typ->as_qubit_array()) {
                qubit_indices_size += qubit_array->size;
            } else if (variable.typ->as_bit()) {
                bit_indices_size += 1;
            } else if (auto bit_array = variable.typ->as_bit_array()) {
                bit_indices_size += bit_array->size;
            }
        } else if (auto index_ref = operand->as_index_ref()) {
            const auto &variable = *index_ref->variable;
            if (variable.typ->as_qubit() || variable.typ->as_qubit_array()) {
                qubit_indices_size += index_ref->indices.size();
            } else if (variable.typ->as_bit() || variable.typ->as_bit_array()) {
                bit_indices_size += index_ref->indices.size();
            }
        }
    }
    if (qubit_indices_size != bit_indices_size) {
        throw error::AnalysisError{ "qubit and bit indices have different sizes" };
    }
}

void check_non_gate_instruction(const tree::One<semantic::NonGateInstruction> &instruction) {
    if (instruction->name == instruction::measure_instruction_name) {
        check_qubit_and_bit_indices_have_same_size(instruction->operands);
    }
}

std::any AnalyzeTreeGenAstVisitor::visit_non_gate_instruction(ast::NonGateInstruction &node) {
    auto ret = tree::make<semantic::NonGateInstruction>();
    try {
        const auto &name = node.name->name;
        const auto &operands = std::any_cast<values::Values>(visit_expression_list(*node.operands));

        // Resolve the instruction
        ret.set(analyzer_.resolve_instruction(name, operands));

        // Specific checks
        check_non_gate_instruction(ret);

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);

        // Add the statement to the current scope
        analyzer_.add_statement_to_current_scope(ret);
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_expression_list(ast::ExpressionList &node) {
    auto ret = values::Values();
    std::transform(
        node.items.begin(), node.items.end(), std::back_inserter(ret.get_vec()), [this](const auto &expression_ast) {
            return std::any_cast<values::Value>(visit_expression(*expression_ast));
        });
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_expression(ast::Expression &node) {
    try {
        auto ret = std::any_cast<values::Value>(node.visit(*this));
        ret->copy_annotation<parser::SourceLocation>(node);
        return ret;
    } catch (error::AnalysisError &err) {
        err.context(node);
        throw;
    }
}

/**
 * Convenience function for visiting a function call given the function's name and arguments
 */
values::Value AnalyzeTreeGenAstVisitor::visit_function_call(
    const tree::One<ast::Identifier> &name, const tree::Maybe<ast::ExpressionList> &arguments) {
    auto function_arguments = values::Values();
    if (!arguments.empty()) {
        std::for_each(
            arguments->items.begin(), arguments->items.end(), [&function_arguments, this](const auto node_argument) {
                function_arguments.add(std::any_cast<values::Value>(visit_expression(*node_argument)));
            });
    }
    const auto function_name = name->name;
    auto ret = analyzer_.resolve_function(function_name, function_arguments);
    if (ret.empty()) {
        throw error::AnalysisError{ "function implementation returned empty value" };
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_function_call(ast::FunctionCall &node) {
    return visit_function_call(node.name, node.arguments);
}

/**
 * Convenience function for visiting unary operators
 */
std::any AnalyzeTreeGenAstVisitor::visit_unary_operator(
    const std::string &name, const tree::One<ast::Expression> &expression) {
    return visit_function_call(tree::make<ast::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<ast::ExpressionList>{
            tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ expression }).get_ptr() });
}

/**
 * Convenience function for visiting binary operators
 */
std::any AnalyzeTreeGenAstVisitor::visit_binary_operator(
    const std::string &name, const tree::One<ast::Expression> &lhs, const tree::One<ast::Expression> &rhs) {
    return visit_function_call(tree::make<ast::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<ast::ExpressionList>{
            tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ lhs, rhs }).get_ptr() });
}

std::any AnalyzeTreeGenAstVisitor::visit_unary_minus_expression(ast::UnaryMinusExpression &node) {
    return visit_unary_operator("-", node.expr);
}

std::any AnalyzeTreeGenAstVisitor::visit_bitwise_not_expression(ast::BitwiseNotExpression &node) {
    return visit_unary_operator("~", node.expr);
}

std::any AnalyzeTreeGenAstVisitor::visit_logical_not_expression(ast::LogicalNotExpression &node) {
    return visit_unary_operator("!", node.expr);
}

std::any AnalyzeTreeGenAstVisitor::visit_power_expression(ast::PowerExpression &node) {
    return visit_binary_operator("**", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_product_expression(ast::ProductExpression &node) {
    return visit_binary_operator("*", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_division_expression(ast::DivisionExpression &node) {
    return visit_binary_operator("/", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_modulo_expression(ast::ModuloExpression &node) {
    return visit_binary_operator("%", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_addition_expression(ast::AdditionExpression &node) {
    return visit_binary_operator("+", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_subtraction_expression(ast::SubtractionExpression &node) {
    return visit_binary_operator("-", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_shift_left_expression(ast::ShiftLeftExpression &node) {
    return visit_binary_operator("<<", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_shift_right_expression(ast::ShiftRightExpression &node) {
    return visit_binary_operator(">>", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_gt_expression(ast::CmpGtExpression &node) {
    return visit_binary_operator(">", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_lt_expression(ast::CmpLtExpression &node) {
    return visit_binary_operator("<", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_ge_expression(ast::CmpGeExpression &node) {
    return visit_binary_operator(">=", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_le_expression(ast::CmpLeExpression &node) {
    return visit_binary_operator("<=", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_eq_expression(ast::CmpEqExpression &node) {
    return visit_binary_operator("==", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_cmp_ne_expression(ast::CmpNeExpression &node) {
    return visit_binary_operator("!=", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_bitwise_and_expression(ast::BitwiseAndExpression &node) {
    return visit_binary_operator("&", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_bitwise_xor_expression(ast::BitwiseXorExpression &node) {
    return visit_binary_operator("^", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_bitwise_or_expression(ast::BitwiseOrExpression &node) {
    return visit_binary_operator("|", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_logical_and_expression(ast::LogicalAndExpression &node) {
    return visit_binary_operator("&&", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_logical_xor_expression(ast::LogicalXorExpression &node) {
    return visit_binary_operator("^^", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_logical_or_expression(ast::LogicalOrExpression &node) {
    return visit_binary_operator("||", node.lhs, node.rhs);
}

std::any AnalyzeTreeGenAstVisitor::visit_ternary_conditional_expression(ast::TernaryConditionalExpression &node) {
    return visit_function_call(tree::make<ast::Identifier>("operator?:"),
        tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ node.cond, node.if_true, node.if_false }));
}

/**
 * Check out of range accesses from any index in an input list to an array of a given size
 */
void check_out_of_range(const IndexListT &indices, primitives::Int size) {
    for (const auto &index_item : indices) {
        if (index_item->value < 0 || index_item->value >= size) {
            throw error::AnalysisError{ fmt::format("index {} out of range (size {})", index_item->value, size) };
        }
    }
}

std::any AnalyzeTreeGenAstVisitor::visit_index(ast::Index &node) {
    try {
        auto expression = std::any_cast<values::Value>(visit_expression(*node.expr));
        auto variable_ref_ptr = expression->as_variable_ref();
        const auto variable_link = variable_ref_ptr->variable;
        const auto variable_type = variable_link->typ;
        if (variable_type->as_qubit_array() || variable_type->as_bit_array()) {
            auto indices = std::any_cast<IndexListT>(visit_index_list(*node.indices));
            check_out_of_range(indices, types::size_of(variable_type));
            auto ret = tree::make<values::IndexRef>(variable_link, indices);
            return values::Value{ ret };
        } else {
            throw error::AnalysisError{ fmt::format(
                "indexation is not supported for value of type '{}'", values::type_of(expression)) };
        }
    } catch (error::AnalysisError &err) {
        err.context(node);
        throw;
    }
}

std::any AnalyzeTreeGenAstVisitor::visit_index_list(ast::IndexList &index_list_ast) {
    auto ret = IndexListT{};
    for (const auto &index_entry : index_list_ast.items) {
        if (auto index_item = index_entry->as_index_item()) {
            // Single index
            ret.add(std::any_cast<tree::One<IndexT>>(visit_index_item(*index_item)));
        } else if (auto index_range = index_entry->as_index_range()) {
            // Range notation
            ret.extend(std::any_cast<IndexListT>(visit_index_range(*index_range)));
        } else {
            throw std::runtime_error{ "unknown IndexEntry AST node" };
        }
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_index_item(ast::IndexItem &index_item_ast) {
    auto index_item = visit_const_int(*index_item_ast.index);
    auto index_value_sp = tree::make<IndexT>(index_item);
    index_value_sp->copy_annotation<parser::SourceLocation>(index_item_ast);
    return index_value_sp;
}

std::any AnalyzeTreeGenAstVisitor::visit_index_range(ast::IndexRange &index_range_ast) {
    auto first = visit_const_int(*index_range_ast.first);
    auto last = visit_const_int(*index_range_ast.last);
    if (first > last) {
        throw error::AnalysisError("last index is lower than first index", &index_range_ast);
    }
    IndexListT ret{};
    for (auto index = first; index <= last; index++) {
        auto index_value_sp = tree::make<IndexT>(index);
        index_value_sp->copy_annotation<parser::SourceLocation>(index_range_ast);
        ret.add(index_value_sp);
    }
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_identifier(ast::Identifier &node) {
    return analyzer_.resolve_variable(node.name);
}

/**
 * If any element of the initialization list is not a const boolean, const int, or const float, throw an error
 */
void check_initialization_list_element_type(const values::Value &value) {
    if (!(value->as_const_bool() || value->as_const_int() || value->as_const_float())) {
        throw error::AnalysisError{ "expecting a const bool, const int, or const float value" };
    }
}

std::any AnalyzeTreeGenAstVisitor::visit_boolean_literal(ast::BooleanLiteral &node) {
    auto ret = tree::make<values::ConstBool>(node.value);
    return values::Value{ ret };
}

std::any AnalyzeTreeGenAstVisitor::visit_integer_literal(ast::IntegerLiteral &node) {
    auto ret = tree::make<values::ConstInt>(node.value);
    return values::Value{ ret };
}

std::any AnalyzeTreeGenAstVisitor::visit_float_literal(ast::FloatLiteral &node) {
    auto ret = tree::make<values::ConstFloat>(node.value);
    return values::Value{ ret };
}

/**
 * Shorthand for parsing an expression to a constant integer.
 */
primitives::Int AnalyzeTreeGenAstVisitor::visit_const_int(ast::Expression &expression) {
    if (auto int_value = visit_as<types::Int>(expression); !int_value.empty()) {
        if (auto const_int_value = int_value->as_const_int()) {
            return const_int_value->value;
        }
        throw error::AnalysisError{ "integer must be constant", &expression };
    }
    throw error::AnalysisError{ "expected an integer", &expression };
}

}  // namespace cqasm::v3x::analyzer
