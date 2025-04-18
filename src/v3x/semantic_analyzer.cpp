#include "libqasm/v3x/semantic_analyzer.hpp"

#include <algorithm>  // any_of, for_each, transform
#include <any>
#include <iterator>  // back_inserter

#include "libqasm/v3x/analyzer.hpp"
#include "libqasm/v3x/instruction.hpp"
#include "libqasm/v3x/instruction_set.hpp"
#include "libqasm/v3x/syntactic_generated.hpp"

namespace cqasm::v3x::analyzer {

using instruction::InstructionSet;

SemanticAnalyzer::SemanticAnalyzer(Analyzer& analyzer)
: analyzer_{ analyzer }
, result_{} {}

std::any SemanticAnalyzer::visit_node(syntactic::Node& /* node */) {
    throw error::AnalysisError{ "unimplemented" };
}

std::any SemanticAnalyzer::visit_program(syntactic::Program& program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.api_version;
    result_.root->version = std::any_cast<tree::One<semantic::Version>>(visit_version(*program_ast.version));
    auto [block, variables] = std::any_cast<GlobalBlockReturnT>(visit_global_block(*program_ast.block));
    result_.root->block = std::move(block);
    result_.root->variables = variables;
    return result_;
}

std::any SemanticAnalyzer::visit_version(syntactic::Version& node) {
    auto ret = tree::make<semantic::Version>();
    try {
        // Check API version
        for (auto item : node.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (node.items != analyzer_.api_version) {
            throw error::AnalysisError{ fmt::format(
                "the only cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.api_version,
                node.items) };
        }

        ret->items = node.items;
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));

        // Default to API version in case the version in the AST is broken
        ret->items = analyzer_.api_version;
    }
    ret->copy_annotation<parser::SourceLocation>(node);
    return ret;
}

std::any SemanticAnalyzer::visit_global_block(syntactic::GlobalBlock& node) {
    visit_block(node);
    return GlobalBlockReturnT{ analyzer_.current_block(), analyzer_.current_variables() };
}

std::any SemanticAnalyzer::visit_annotated(syntactic::Annotated& node) {
    auto ret = tree::Any<semantic::AnnotationData>();
    for (const auto& annotation_data_ast : node.annotations) {
        ret.add(std::any_cast<syntactic::One<semantic::AnnotationData>>(visit_annotation_data(*annotation_data_ast)));
    }
    return ret;
}

std::any SemanticAnalyzer::visit_annotation_data(syntactic::AnnotationData& node) {
    auto ret = tree::make<semantic::AnnotationData>();
    try {
        ret->interface = node.interface->name;
        ret->operation = node.operation->name;
        for (const auto& expression_ast : node.operands->items) {
            try {
                ret->operands.add(std::any_cast<values::Value>(expression_ast->visit(*this)));
            } catch (error::AnalysisError& err) {
                err.context(node);
                result_.errors.push_back(std::move(err));
            }
        }
        ret->copy_annotation<parser::SourceLocation>(node);
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

std::any SemanticAnalyzer::visit_variable(syntactic::Variable& node) {
    auto ret = tree::make<semantic::Variable>();
    try {
        // Build semantic type from syntactic type
        auto type = build_semantic_type(node.typ);

        // Construct variable
        // Use the location tag of the identifier to record where the variable was defined
        const auto& identifier = node.name;
        ret->name = identifier->name;
        ret->typ = type.clone();
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(*identifier);

        // Add the variable to the current scope
        analyzer_.add_variable_to_current_scope(ret);

        // Register the variable
        analyzer_.register_variable(identifier->name, tree::make<values::VariableRef>(ret));
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

/**
 * Convenience function for extracting the types of a list of variables.
 */
types::Types types_of(const tree::Any<semantic::Variable>& variables) {
    types::Types types{};
    for (const auto& variable : variables) {
        types.add(variable->typ.clone());
    }
    return types;
}

/**
 * For a named gate, such as X or Rz, the terminal name will be the name of the gate.
 * For a composition of gate modifiers acting on a named gate, the terminal name will be the name of the named gate.
 */
std::string get_gate_terminal_name(const tree::One<semantic::Gate>& gate) {
    return gate->gate.empty() ? gate->name : get_gate_terminal_name(gate->gate);
}

std::string get_gate_resolution_name(const tree::One<semantic::Gate>& gate) {
    return gate->gate.empty() ? gate->name
                              : fmt::format("{}_{}",
                                    (InstructionSet::get_instance().is_single_qubit_gate_modifier(gate->name))
                                        ? InstructionSet::get_instance().single_qubit_gate_composition_prefix
                                        : InstructionSet::get_instance().two_qubit_gate_composition_prefix,
                                    get_gate_terminal_name(gate->gate));
}

void check_qubit_operands_indices_have_same_size(const values::Values& operands) {
    auto qubit_operands_indices_size = std::vector<size_t>(operands.size());
    // Instruction operands can be, either variable references or index references
    // Variables can be of type qubit or qubit array
    // Qubits have a single index, arrays have a size
    // Index references point to a qubit array
    for (size_t i = 0; i < operands.size(); ++i) {
        if (auto variable_ref = operands[i]->as_variable_ref()) {
            const auto& variable = *variable_ref->variable;
            if (variable.typ->as_qubit()) {
                qubit_operands_indices_size[i] += 1;
            } else if (auto qubit_array = variable.typ->as_qubit_array()) {
                qubit_operands_indices_size[i] += qubit_array->size;
            }
        } else if (auto index_ref = operands[i]->as_index_ref()) {
            const auto& variable = *index_ref->variable;
            if (variable.typ->as_qubit() || variable.typ->as_qubit_array()) {
                qubit_operands_indices_size[i] += index_ref->indices.size();
            }
        }
    }
    if (std::adjacent_find(qubit_operands_indices_size.begin(),
            qubit_operands_indices_size.end(),
            std::not_equal_to<>()) != qubit_operands_indices_size.end()) {
        throw error::AnalysisError{ "qubit operands indices have different sizes" };
    }
}

void check_gate_instruction(const tree::One<semantic::GateInstruction>& instruction) {
    check_qubit_operands_indices_have_same_size(instruction->operands);
}

std::any SemanticAnalyzer::visit_gate_instruction(syntactic::GateInstruction& node) {
    auto ret = tree::make<semantic::GateInstruction>();
    try {
        // Set gate and operands
        auto gate = std::any_cast<tree::One<semantic::Gate>>(node.gate->visit(*this));
        auto operands = std::any_cast<values::Values>(node.operands->visit(*this));

        // Resolve the instruction
        const auto& resolution_name = get_gate_resolution_name(gate);
        ret = analyzer_.resolve_instruction(resolution_name, gate, operands);

        // Specific checks
        check_gate_instruction(ret);

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);

        // Add the statement to the current scope
        analyzer_.add_statement_to_current_scope(ret);
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

bool is_two_qubit_gate(const tree::One<semantic::Gate>& gate) {
    const auto& resolution_name = get_gate_resolution_name(gate);
    return InstructionSet::get_instance().is_two_qubit_gate(resolution_name);
}

values::Values resolve_parameters(const std::string& instruction_name, const values::Values& parameters) {
    auto ret = values::Values{};
    const auto& instruction_set = InstructionSet::get_instance();
    const auto& param_types = instruction_set.get_instruction_param_types(instruction_name);
    if (param_types.has_value()) {
        std::for_each(param_types->begin(),
            param_types->end(),
            [i = 0, &instruction_name, &parameters, &ret](const auto& param_type) mutable {
                if (!values::check_promote(values::type_of(parameters[i]), types::from_spec(param_type))) {
                    throw error::AnalysisError{ fmt::format("failed to resolve '{}' with parameter type ({})",
                        instruction_name,
                        values::type_of(parameters[i])) };
                }
                ret.add(promote(parameters[i], types::from_spec(param_type)));
                i++;
            });
    }
    return ret;
}

void check_gate(const tree::One<semantic::Gate>& gate) {
    if (!gate->gate.empty() && is_two_qubit_gate(gate->gate)) {
        throw error::AnalysisError{ "trying to apply a gate modifier to a multi-qubit gate" };
    }
}

std::any SemanticAnalyzer::visit_gate(syntactic::Gate& node) {
    auto ret = tree::make<semantic::Gate>();
    try {
        ret->name = node.name->name;
        if (!node.gate.empty()) {
            ret->gate = std::any_cast<tree::One<semantic::Gate>>(visit_gate(*node.gate)).get_ptr();
        }
        ret->parameters = std::any_cast<values::Values>(visit_expression_list(*node.parameters));

        // Resolve the parameter
        ret->parameters = resolve_parameters(ret->name, ret->parameters);

        // Specific checks
        check_gate(ret);

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);
    } catch (error::AnalysisError& err) {
        err.context(node);
        throw;
    }
    return ret;
}

void check_qubit_and_bit_indices_have_same_size(const values::Values& operands) {
    size_t qubit_indices_size{};
    size_t bit_indices_size{};
    // Instruction operands can be, either variable references or index references
    // Variables can be of type qubit, bit, qubit array, or bit array
    // Qubits and bits have a single index, arrays have a size
    // Index references point to a qubit array or bit array
    for (const auto& operand : operands) {
        if (auto variable_ref = operand->as_variable_ref()) {
            const auto& variable = *variable_ref->variable;
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
            const auto& variable = *index_ref->variable;
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

void check_non_gate_instruction(const tree::One<semantic::NonGateInstruction>& instruction) {
    if (InstructionSet::get_instance().is_measure(instruction->name)) {
        check_qubit_and_bit_indices_have_same_size(instruction->operands);
    }
}

std::any SemanticAnalyzer::visit_non_gate_instruction(syntactic::NonGateInstruction& node) {
    auto ret = tree::make<semantic::NonGateInstruction>();
    try {
        ret->name = node.name->name;
        ret->operands = std::any_cast<values::Values>(visit_expression_list(*node.operands));

        // Resolve the instruction
        ret = analyzer_.resolve_instruction(ret->name, ret->operands);

        // Resolve the parameters
        if (!node.parameters.empty()) {
            ret->parameters =
                resolve_parameters(ret->name, std::any_cast<values::Values>(visit_expression_list(*node.parameters)));
        }

        // Specific checks
        check_non_gate_instruction(ret);

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);

        // Add the statement to the current scope
        analyzer_.add_statement_to_current_scope(ret);
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

std::any SemanticAnalyzer::visit_asm_declaration(syntactic::AsmDeclaration& node) {
    auto ret = tree::make<semantic::AsmDeclaration>();
    try {
        ret->backend_name = node.backend_name->name;
        ret->backend_code = node.backend_code;

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);

        // Add the statement to the current scope
        analyzer_.add_statement_to_current_scope(ret);
    } catch (error::AnalysisError& err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

std::any SemanticAnalyzer::visit_expression_list(syntactic::ExpressionList& node) {
    auto ret = values::Values();
    std::transform(
        node.items.begin(), node.items.end(), std::back_inserter(ret.get_vec()), [this](const auto& expression_ast) {
            return std::any_cast<values::Value>(visit_expression(*expression_ast));
        });
    return ret;
}

std::any SemanticAnalyzer::visit_expression(syntactic::Expression& node) {
    try {
        auto ret = std::any_cast<values::Value>(node.visit(*this));
        ret->copy_annotation<parser::SourceLocation>(node);
        return ret;
    } catch (error::AnalysisError& err) {
        err.context(node);
        throw;
    }
}

/**
 * Convenience function for visiting a function call given the function's name and arguments
 */
values::Value SemanticAnalyzer::visit_function_call(
    const tree::One<syntactic::Identifier>& name, const tree::Maybe<syntactic::ExpressionList>& arguments) {
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

std::any SemanticAnalyzer::visit_function_call(syntactic::FunctionCall& node) {
    return visit_function_call(node.name, node.arguments);
}

/**
 * Convenience function for visiting unary operators
 */
std::any SemanticAnalyzer::visit_unary_operator(
    const std::string& name, const tree::One<syntactic::Expression>& expression) {
    return visit_function_call(tree::make<syntactic::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<syntactic::ExpressionList>{
            tree::make<syntactic::ExpressionList>(tree::Any<syntactic::Expression>{ expression }).get_ptr() });
}

/**
 * Convenience function for visiting binary operators
 */
std::any SemanticAnalyzer::visit_binary_operator(
    const std::string& name, const tree::One<syntactic::Expression>& lhs, const tree::One<syntactic::Expression>& rhs) {
    return visit_function_call(tree::make<syntactic::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<syntactic::ExpressionList>{
            tree::make<syntactic::ExpressionList>(tree::Any<syntactic::Expression>{ lhs, rhs }).get_ptr() });
}

std::any SemanticAnalyzer::visit_unary_minus_expression(syntactic::UnaryMinusExpression& node) {
    return visit_unary_operator("-", node.expr);
}

std::any SemanticAnalyzer::visit_bitwise_not_expression(syntactic::BitwiseNotExpression& node) {
    return visit_unary_operator("~", node.expr);
}

std::any SemanticAnalyzer::visit_logical_not_expression(syntactic::LogicalNotExpression& node) {
    return visit_unary_operator("!", node.expr);
}

std::any SemanticAnalyzer::visit_power_expression(syntactic::PowerExpression& node) {
    return visit_binary_operator("**", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_product_expression(syntactic::ProductExpression& node) {
    return visit_binary_operator("*", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_division_expression(syntactic::DivisionExpression& node) {
    return visit_binary_operator("/", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_modulo_expression(syntactic::ModuloExpression& node) {
    return visit_binary_operator("%", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_addition_expression(syntactic::AdditionExpression& node) {
    return visit_binary_operator("+", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_subtraction_expression(syntactic::SubtractionExpression& node) {
    return visit_binary_operator("-", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_shift_left_expression(syntactic::ShiftLeftExpression& node) {
    return visit_binary_operator("<<", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_shift_right_expression(syntactic::ShiftRightExpression& node) {
    return visit_binary_operator(">>", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_gt_expression(syntactic::CmpGtExpression& node) {
    return visit_binary_operator(">", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_lt_expression(syntactic::CmpLtExpression& node) {
    return visit_binary_operator("<", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_ge_expression(syntactic::CmpGeExpression& node) {
    return visit_binary_operator(">=", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_le_expression(syntactic::CmpLeExpression& node) {
    return visit_binary_operator("<=", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_eq_expression(syntactic::CmpEqExpression& node) {
    return visit_binary_operator("==", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_cmp_ne_expression(syntactic::CmpNeExpression& node) {
    return visit_binary_operator("!=", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_bitwise_and_expression(syntactic::BitwiseAndExpression& node) {
    return visit_binary_operator("&", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_bitwise_xor_expression(syntactic::BitwiseXorExpression& node) {
    return visit_binary_operator("^", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_bitwise_or_expression(syntactic::BitwiseOrExpression& node) {
    return visit_binary_operator("|", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_logical_and_expression(syntactic::LogicalAndExpression& node) {
    return visit_binary_operator("&&", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_logical_xor_expression(syntactic::LogicalXorExpression& node) {
    return visit_binary_operator("^^", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_logical_or_expression(syntactic::LogicalOrExpression& node) {
    return visit_binary_operator("||", node.lhs, node.rhs);
}

std::any SemanticAnalyzer::visit_ternary_conditional_expression(syntactic::TernaryConditionalExpression& node) {
    return visit_function_call(tree::make<syntactic::Identifier>("operator?:"),
        tree::make<syntactic::ExpressionList>(
            tree::Any<syntactic::Expression>{ node.cond, node.if_true, node.if_false }));
}

/**
 * Check out of range accesses from any index in an input list to an array of a given size
 */
void check_out_of_range(const IndexListT& indices, primitives::Int size) {
    for (const auto& index_item : indices) {
        if (index_item->value < 0 || index_item->value >= size) {
            throw error::AnalysisError{ fmt::format("index {} out of range (size {})", index_item->value, size) };
        }
    }
}

std::any SemanticAnalyzer::visit_index(syntactic::Index& node) {
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
    } catch (error::AnalysisError& err) {
        err.context(node);
        throw;
    }
}

std::any SemanticAnalyzer::visit_index_list(syntactic::IndexList& index_list_ast) {
    auto ret = IndexListT{};
    for (const auto& index_entry : index_list_ast.items) {
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

std::any SemanticAnalyzer::visit_index_item(syntactic::IndexItem& index_item_ast) {
    auto index_item = visit_const_int(*index_item_ast.index);
    auto index_value_sp = tree::make<IndexT>(index_item);
    index_value_sp->copy_annotation<parser::SourceLocation>(index_item_ast);
    return index_value_sp;
}

std::any SemanticAnalyzer::visit_index_range(syntactic::IndexRange& index_range_ast) {
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

std::any SemanticAnalyzer::visit_identifier(syntactic::Identifier& node) {
    return analyzer_.resolve_variable(node.name);
}

/**
 * If any element of the initialization list is not a const boolean, const int, or const float, throw an error
 */
void check_initialization_list_element_type(const values::Value& value) {
    if (!(value->as_const_bool() || value->as_const_int() || value->as_const_float())) {
        throw error::AnalysisError{ "expecting a const bool, const int, or const float value" };
    }
}

std::any SemanticAnalyzer::visit_boolean_literal(syntactic::BooleanLiteral& node) {
    auto ret = tree::make<values::ConstBool>(node.value);
    return values::Value{ ret };
}

std::any SemanticAnalyzer::visit_integer_literal(syntactic::IntegerLiteral& node) {
    auto ret = tree::make<values::ConstInt>(node.value);
    return values::Value{ ret };
}

std::any SemanticAnalyzer::visit_float_literal(syntactic::FloatLiteral& node) {
    auto ret = tree::make<values::ConstFloat>(node.value);
    return values::Value{ ret };
}

/**
 * Shorthand for parsing an expression to a constant integer.
 */
primitives::Int SemanticAnalyzer::visit_const_int(syntactic::Expression& expression) {
    if (auto int_value = visit_as<types::Int>(expression); !int_value.empty()) {
        if (auto const_int_value = int_value->as_const_int()) {
            return const_int_value->value;
        }
        throw error::AnalysisError{ "integer must be constant", &expression };
    }
    throw error::AnalysisError{ "expected an integer", &expression };
}

}  // namespace cqasm::v3x::analyzer
