#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-analyzer.hpp"

#include <algorithm>  // any_of, for_each
#include <any>
#include <cassert>  // assert
#include <range/v3/view/tail.hpp>  // tail


namespace cqasm::v3x::analyzer {

AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{}
{}

std::any AnalyzeTreeGenAstVisitor::visit_node(ast::Node &/* node */) {
    throw error::AnalysisError{ "unimplemented" };
}

std::any AnalyzeTreeGenAstVisitor::visit_program(ast::Program &program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.api_version;
    result_.root->version = std::any_cast<tree::One<semantic::Version>>(visit_version(*program_ast.version));
    auto [block, variables, functions] = std::any_cast<GlobalBlockReturnT>(visit_global_block(*program_ast.block));
    result_.root->block = block;
    result_.root->variables = variables;
    result_.root->functions = functions;
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
        if (node.items > analyzer_.api_version) {
            throw error::AnalysisError(fmt::format(
                "the maximum cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.api_version,
                node.items
            ));
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
    return GlobalBlockReturnT{ analyzer_.current_block(), analyzer_.current_variables(), analyzer_.global_functions() };
}

std::any AnalyzeTreeGenAstVisitor::visit_local_block(ast::LocalBlock &node) {
    visit_block(node);
    return LocalBlockReturnT{ analyzer_.current_block(), analyzer_.current_variables() };
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
        const auto identifier = node.name;
        ret->name = identifier->name;
        ret->typ = type.clone();
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(*identifier);

        // Add the variable to the current scope
        analyzer_.add_variable_to_current_scope(ret);

        // Register variable name in the current scope
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
    throw error::AnalysisError{ fmt::format(
        "type of right-hand side ({}) could not be coerced to left-hand side ({})",
        values::type_of(rhs_value), lhs_type) };
}

void do_assignment(
    tree::Maybe<semantic::AssignmentStatement> &assignment_statement,
    const values::Value &lhs_value,
    const values::Value &rhs_value) {

    // Check left and right-hand sides have the same size
    auto rhs_size = values::size_of(rhs_value);
    auto lhs_size = values::size_of(lhs_value);
    if (rhs_size != lhs_size) {
        throw error::AnalysisError{ fmt::format(
            "trying to initialize a lhs of size {} with a rhs of size {}",
            lhs_size, rhs_size) };
    }

    // Check if right-hand side operand needs be promoted
    const auto lhs_type = values::type_of(lhs_value);
    const auto promoted_rhs_value = promote_or_error(rhs_value, lhs_type);

    // Check axis types are not assigned [0, 0, 0]
    if (lhs_type->as_axis()) {
        if (values::check_all_of_array_values(rhs_value,
                [](const auto e){ return static_cast<double>(e->value) == 0.0; })) {
            throw error::AnalysisError{ "cannot set an axis variable type to [0.0, 0.0, 0.0]" };
        }
    }

    assignment_statement.emplace(lhs_value, promoted_rhs_value);
}

std::any AnalyzeTreeGenAstVisitor::visit_initialization(ast::Initialization &node) {
    auto ret = tree::Maybe<semantic::AssignmentStatement>();
    try {
        // Analyze the right-hand side operand
        //
        // Initialization instructions check the right-hand side operand first
        // In order to avoid code such as 'int i = i' being correct
        const auto rhs_value = std::any_cast<values::Value>(visit_expression(*node.rhs));

        // Add the variable declaration
        visit_variable(*node.var);

        // Analyze the left-hand side operand
        // Left-hand side is always assignable for an initialization
        const auto lhs_value = std::any_cast<values::Value>(visit_expression(*node.var->name));

        // Perform assignment
        do_assignment(ret, lhs_value, rhs_value);

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

bool AnalyzeTreeGenAstVisitor::current_block_has_return_statement() {
    return std::any_of(analyzer_.current_block()->statements.begin(), analyzer_.current_block()->statements.end(),
        [](const auto &statement) { return statement->as_return_statement(); });
}

void AnalyzeTreeGenAstVisitor::current_block_return_statements_promote_or_error(const tree::Maybe<types::Node> &return_type) {
    std::for_each(analyzer_.current_block()->statements.begin(), analyzer_.current_block()->statements.end(),
        [&return_type](const auto &statement) {
            if (auto return_statement = statement->as_return_statement(); return_statement) {
                assert(!return_statement->return_value.empty());
                const auto promoted_return_value = promote_or_error(return_statement->return_value, return_type);
                return_statement->return_value.set(promoted_return_value);
            }
    });
}

/**
 * Convenience function for extracting the types of a list of variables.
 */
types::Types types_of(const tree::Any<semantic::Variable> &variables) {
    types::Types types{};
    for (const auto &variable : variables) {
        types.add(variable->typ);
    }
    return types;
}

std::any AnalyzeTreeGenAstVisitor::visit_function(ast::Function &node) {
    auto ret = tree::make<semantic::Function>();
    try {
        analyzer_.push_scope();

        // Name
        const auto identifier = node.name;
        ret->name = identifier->name;

        // Return type
        if (!node.return_type.empty()) {
            ret->return_type = build_semantic_type(node.return_type);
        }

        // Parameters
        const auto &parameters = std::any_cast<LocalBlockReturnT>(visit_local_block(*node.parameters)).second;
        auto parameter_types = types_of(parameters);

        // Block
        auto [block, variables] = std::any_cast<LocalBlockReturnT>(visit_local_block(*node.block));
        ret->block = block;
        ret->variables = variables;

        // Return statement and return type checks
        if (current_block_has_return_statement() && node.return_type.empty()) {
            throw error::AnalysisError{ "found return statement but function does not have a return type" };
        } else if (!current_block_has_return_statement() && !node.return_type.empty()) {
            throw error::AnalysisError{ "function has a return type but return statement was not found" };
        } else if (current_block_has_return_statement() && !node.return_type.empty()) {
            current_block_return_statements_promote_or_error(ret->return_type);
        }

        // Copy annotation data
        ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
        ret->copy_annotation<parser::SourceLocation>(node);

        // Add the function to the current scope
        analyzer_.add_function_to_global_scope(ret);

        // Register function
        analyzer_.register_function(ret->name, parameter_types, tree::make<values::FunctionRef>(ret));
    } catch (error::AnalysisError &err) {
        err.context(node);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }

    analyzer_.pop_scope();
    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_assignment_statement(ast::AssignmentStatement &node) {
    auto ret = tree::Maybe<semantic::AssignmentStatement>();
    try {
        // Analyze the operands
        const auto lhs_value = std::any_cast<values::Value>(visit_expression(*node.lhs));
        const auto rhs_value = std::any_cast<values::Value>(visit_expression(*node.rhs));

        // Check assignability of the left-hand side
        if (bool assignable = lhs_value->as_reference(); !assignable) {
            throw error::AnalysisError{ "left-hand side of assignment statement must be assignable" };
        }

        // Perform assignment
        do_assignment(ret, lhs_value, rhs_value);

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

std::any AnalyzeTreeGenAstVisitor::visit_return_statement(ast::ReturnStatement &node) {
    auto ret = tree::make<semantic::ReturnStatement>(
        std::any_cast<values::Value>(visit_expression(*node.return_value)));

    // Copy annotation data
    ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(visit_annotated(*node.as_annotated()));
    ret->copy_annotation<parser::SourceLocation>(node);

    // Add the statement to the current scope
    analyzer_.add_statement_to_current_scope(ret);

    return ret;
}

std::any AnalyzeTreeGenAstVisitor::visit_expression_statement(ast::ExpressionStatement &node) {
    auto ret = tree::make<semantic::FunctionCallStatement>();
    try {
        if (auto function_call = node.expression->as_function_call(); function_call) {
            ret->return_value = std::any_cast<values::Value>(visit_function_call(*function_call)).get_ptr();

            // Copy annotation data
            ret->annotations = std::any_cast<tree::Any<semantic::AnnotationData>>(
                visit_annotated(*node.as_annotated()));
            // Expression statements get the source location information from their expressions
            ret->copy_annotation<parser::SourceLocation>(*node.expression);

            // Add the statement to the current scope
            analyzer_.add_statement_to_current_scope(ret);
        } else {
            throw error::AnalysisError{ "expression statement is not of function call type" };
        }
    } catch (error::AnalysisError &err) {
        // Expression statements get the source location information from their expressions
        err.context(*node.expression);
        result_.errors.push_back(std::move(err));
        ret.reset();
    }
    return ret;
}

bool check_qubit_and_bit_indices_have_same_size(const values::Values &operands) {
    size_t qubit_indices_size{};
    size_t bit_indices_size{};
    // Instruction operands can be, whether variables references or index references
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
    return qubit_indices_size == bit_indices_size;
}

std::any AnalyzeTreeGenAstVisitor::visit_gate(ast::Gate &node) {
    auto ret = tree::Maybe<semantic::Instruction>();
    try {
        // Set operand list
        auto operands = values::Values();
        for (const auto &operand_expr : node.operands->items) {
            operands.add(std::any_cast<values::Value>(visit_expression(*operand_expr)));
        }

        // Resolve the instruction
        ret.set(analyzer_.resolve_instruction(node.name->name, operands));

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

std::any AnalyzeTreeGenAstVisitor::visit_measure_instruction(ast::MeasureInstruction &node) {
    auto ret = tree::Maybe<semantic::Instruction>();
    try {
        // Set operand list
        // Notice operands have to be added in this order
        // Otherwise instruction resolution would fail
        auto operands = values::Values();
        operands.add(std::any_cast<values::Value>(visit_expression(*node.lhs)));
        operands.add(std::any_cast<values::Value>(visit_expression(*node.rhs)));

        // Resolve the instruction
        // For a measure instruction, this resolution will check that
        // the first operand is of bit type, and that
        // the second operand is of qubit type
        ret.set(analyzer_.resolve_instruction(node.name->name, operands));

        // Check qubit and bit indices have the same size
        if (!ret->instruction.empty()) {
            if (!check_qubit_and_bit_indices_have_same_size(operands)) {
                throw error::AnalysisError{ "qubit and bit indices have different sizes" };
            }
        }

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
    const tree::One<ast::Identifier> &name,
    const tree::Maybe<ast::ExpressionList> &arguments) {

    auto function_arguments = values::Values();
    if (!arguments.empty()) {
        std::for_each(arguments->items.begin(), arguments->items.end(),
            [&function_arguments, this](const auto node_argument) {
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
    const std::string &name,
    const tree::One<ast::Expression> &expression) {

    return visit_function_call(
        tree::make<ast::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<ast::ExpressionList>{
            tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ expression }).get_ptr() }
    );
}

/**
 * Convenience function for visiting binary operators
 */
std::any AnalyzeTreeGenAstVisitor::visit_binary_operator(
    const std::string &name,
    const tree::One<ast::Expression> &lhs,
    const tree::One<ast::Expression> &rhs) {

    return visit_function_call(
        tree::make<ast::Identifier>(std::string{ "operator" } + name),
        tree::Maybe<ast::ExpressionList>{
            tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ lhs, rhs }).get_ptr() }
    );
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
    return visit_function_call(
        tree::make<ast::Identifier>("operator?:"),
        tree::make<ast::ExpressionList>(tree::Any<ast::Expression>{ node.cond, node.if_true, node.if_false })
    );
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
 * Transform an input array into a const array of Type
 * Pre conditions:
 *   Type can only be Bool, Int, or Float
 *   All the values in the input array can be promoted to Type
 */
/* static */ values::Value AnalyzeTreeGenAstVisitor::build_value_from_promoted_values(
    const values::Values &values, const types::Type &type) {

    if (types::type_check(type, tree::make<types::Bool>())) {
        return build_array_value_from_promoted_values<values::ConstBoolArray>(values, type);
    } else if (types::type_check(type, tree::make<types::Int>())) {
        return build_array_value_from_promoted_values<values::ConstIntArray>(values, type);
    } else if (types::type_check(type, tree::make<types::Float>())) {
        return build_array_value_from_promoted_values<values::ConstFloatArray>(values, type);
    } else {
        throw error::AnalysisError{ "expecting Bool, Int, or Float type in initialization list" };
    }
}

/**
 * If any element of the initialization list is not a const boolean, const int, or const float, throw an error
 */
void check_initialization_list_element_type(const values::Value &value) {
    if (!(value->as_const_bool() || value->as_const_int() || value->as_const_float())) {
        throw error::AnalysisError{ "expecting a const bool, const int, or const float value" };
    }
}

std::any AnalyzeTreeGenAstVisitor::visit_initialization_list(ast::InitializationList &node) {
    try {
        // If initialization list is empty, throw an error
        const auto &expressions_ast = node.expr_list->items.get_vec();
        if (expressions_ast.empty()) {
            throw error::AnalysisError{ "initialization list is empty" };
        }

        // Set expression's highest type to the type of the first expression
        const auto first_value = std::any_cast<values::Value>(expressions_ast[0]->visit(*this));
        check_initialization_list_element_type(first_value);
        auto expressions_highest_type = values::type_of(first_value);

        // Build a list of expression values,
        // keeping the highest type to which we can promote (e.g., for a list of booleans and integers, integer)
        auto expressions_values = values::Values();
        expressions_values.add(first_value);
        for (const auto &current_expression_ast : ranges::views::tail(expressions_ast)) {
            const auto current_value = std::any_cast<values::Value>(visit_expression(*current_expression_ast));
            check_initialization_list_element_type(current_value);
            expressions_values.add(current_value);
            if (const auto current_value_type = values::type_of(current_value);
                values::check_promote(current_value_type, expressions_highest_type)) {
                continue;
            } else if (values::check_promote(expressions_highest_type, current_value_type)) {
                expressions_highest_type.set(current_value_type);
            } else {
                throw error::AnalysisError{
                    fmt::format("cannot perform a promotion between these two types: ({}) and ({})",
                        current_value_type, expressions_highest_type) };
            }
        }

        // Then return a Const<Type>Array value, where <Type> is the highest type to which we can promote
        return build_value_from_promoted_values(expressions_values, expressions_highest_type);
    } catch (error::AnalysisError &err) {
        err.context(node);
        throw;
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

}  // namespace cqasm::v3x::analyzer`
