#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-analyzer.hpp"

#include <algorithm>  // for_each
#include <string_view>


namespace cqasm::v3x::analyzer {


AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{} {}

AnalysisResult AnalyzeTreeGenAstVisitor::visitProgram(const ast::Program &program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.api_version;
    visitVersion(*program_ast.version);
    visitStatements(*program_ast.statements);
    return result_;
}

void AnalyzeTreeGenAstVisitor::visitVersion(const ast::Version &version_ast) {
    try {
        // Default to API version in case the version in the AST is broken
        result_.root->version = tree::make<semantic::Version>();
        result_.root->version->items = analyzer_.api_version;

        // Check API version
        for (auto item : version_ast.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (version_ast.items > analyzer_.api_version) {
            throw error::AnalysisError(fmt::format(
                "the maximum cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.api_version,
                version_ast.items
            ));
        }

        // Save the file version
        result_.root->version->items = version_ast.items;
    } catch (error::AnalysisError &err) {
        err.context(version_ast);
        result_.errors.push_back(err.get_message());
    }
    result_.root->version->copy_annotation<parser::SourceLocation>(version_ast);
}

void AnalyzeTreeGenAstVisitor::visitStatements(const ast::StatementList &statement_list_ast) {
    for (const auto &statement : statement_list_ast.items) {
        try {
            if (auto variable = statement->as_variable()) {
                visitVariable(*variable);
            } else if (auto initialization = statement->as_initialization()) {
                if (auto semantic_statement = visitInitialization(*initialization); !semantic_statement.empty()) {
                    result_.root->statements.add(semantic_statement);
                }
            } else if (auto assignment_instruction = statement->as_assignment_instruction()) {
                if (auto semantic_statement = visitAssignmentInstruction(*assignment_instruction); !semantic_statement.empty()) {
                    result_.root->statements.add(semantic_statement);
                }
            } else if (auto instruction = statement->as_instruction()) {
                if (auto semantic_statement = visitInstruction(*instruction); !semantic_statement.empty()) {
                    result_.root->statements.add(semantic_statement);
                }
            } else {
                throw std::runtime_error("unexpected statement node");
            }
        } catch (error::AnalysisError &err) {
            err.context(*statement);
            result_.errors.push_back(err.get_message());
        }
    }
}

template <typename T, typename TArray>
types::Type AnalyzeTreeGenAstVisitor::visitVariableType(const ast::Variable &variable_ast,
    std::string_view type_name) const {

    if (variable_ast.size.empty()) {
        return tree::make<T>(1);
    } else if (variable_ast.size->value > 0) {
        return tree::make<TArray>(variable_ast.size->value);
    } else {
        throw error::AnalysisError{ fmt::format("declaring {} array of size <= 0", type_name) };
    }
}

void AnalyzeTreeGenAstVisitor::visitVariable(const ast::Variable &variable_ast) {
    try {
        auto type_name = variable_ast.typ->name;
        types::Type type{};
        if (type_name == "qubit") {
            type = visitVariableType<types::Qubit, types::QubitArray>(variable_ast, "qubit");
        } else if (type_name == "bit") {
            type = visitVariableType<types::Bit, types::BitArray>(variable_ast, "bit");
        } else if (type_name == "axis") {
            type = tree::make<types::Axis>();
        } else if (type_name == "bool") {
            type = visitVariableType<types::Bool, types::BoolArray>(variable_ast, "bool");
        } else if (type_name == "int") {
            type = visitVariableType<types::Int, types::IntArray>(variable_ast, "int");
        } else if (type_name == "float") {
            type = visitVariableType<types::Real, types::RealArray>(variable_ast, "real");
        } else {
            throw error::AnalysisError("unknown type \"" + type_name + "\"");
        }

        // Construct variable
        // Use the location tag of the identifier to record where the variable was defined
        const auto &identifier = variable_ast.name;
        const auto &variable = tree::make<semantic::Variable>(identifier->name, type.clone());
        variable->copy_annotation<parser::SourceLocation>(*identifier);
        variable->annotations = visitAnnotations(variable_ast.annotations);
        result_.root->variables.add(variable);

        analyzer_.add_mapping(identifier->name, tree::make<values::VariableRef>(variable));
    } catch (error::AnalysisError &err) {
        err.context(variable_ast);
        result_.errors.push_back(err.get_message());
    }
}

bool check_qubit_and_bit_indices_have_same_size(const values::Values &operands) {
    size_t qubit_indices_size{};
    size_t bit_indices_size{};
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

tree::Maybe<semantic::Instruction> AnalyzeTreeGenAstVisitor::visitInstruction(
    const ast::Instruction &instruction_ast) {

    tree::Maybe<semantic::Instruction> node{};
    try {
        // Set operand list
        auto operands = values::Values();
        for (const auto &operand_expr : instruction_ast.operands->items) {
            operands.add(visitExpression(*operand_expr));
        }

        // Resolve the instruction
        node.set(analyzer_.resolve_instruction(instruction_ast.name->name, operands));

        // Check qubit and bit indices have the same size
        if (!node->instruction.empty() && node->instruction->request_qubit_and_bit_indices_have_same_size) {
            if (!check_qubit_and_bit_indices_have_same_size(operands)) {
                throw error::AnalysisError{ "qubit and bit indices have different sizes", &instruction_ast };
            }
        }

        // Set condition code
        node->condition.set(tree::make<values::ConstBool>(true));

        // Copy annotation data
        node->annotations = visitAnnotations(instruction_ast.annotations);
        node->copy_annotation<parser::SourceLocation>(instruction_ast);
    } catch (error::AnalysisError &err) {
        err.context(instruction_ast);
        result_.errors.push_back(err.get_message());
    }
    return node;
}

values::Value AnalyzeTreeGenAstVisitor::promoteValueToType(const values::Value &rhs_value, const types::Type &lhs_type) {
    // TODO: the code below should be generic, e.g., work as well for Bool/BoolArray, Real/RealArray, and so on
    if (auto rhs_value_ptr = rhs_value->as_const_int_array()) {
        if (lhs_type->as_int_array()) {
            const auto &rhs_value_items = rhs_value_ptr->value.get_vec();
            auto rhs_promoted_value = cqasm::tree::make<values::ConstIntArray>();
            std::for_each(rhs_value_items.begin(), rhs_value_items.end(),
                [&rhs_promoted_value](const auto &rhs_value) {
                    rhs_promoted_value->value.add(promoteValueToType(rhs_value, tree::make<types::Int>()));
            });
            return rhs_promoted_value;
        } else {
            throw error::AnalysisError{ fmt::format(
                "type of right-hand side ({}) could not be coerced to left-hand side ({})",
                values::type_of(rhs_value), types::Type(tree::make<types::Int>())) };
        }
    } else if (auto rhs_promoted_value = values::promote(rhs_value, lhs_type); rhs_promoted_value.empty()) {
        throw error::AnalysisError{ fmt::format(
            "type of right-hand side ({}) could not be coerced to left-hand side ({})",
            values::type_of(rhs_value), lhs_type) };
    } else {
        return rhs_promoted_value;
    }
}

tree::Maybe<semantic::AssignmentInstruction> AnalyzeTreeGenAstVisitor::visitInitialization(
    const ast::Initialization &initialization_ast) {

    tree::Maybe<semantic::AssignmentInstruction> ret{};
    try {
        // Analyze the right-hand side operand
        //
        // Initialization instructions check the right-hand side operand first
        // In order to avoid code such as 'int i = i' being correct
        const auto &rhs_value = visitExpression(*initialization_ast.rhs);

        // Add the variable declaration
        visitVariable(*initialization_ast.var);

        // Analyze the left-hand side operand
        // Left-hand side is always assignable for an initialization
        const auto &lhs_value = visitExpression(*initialization_ast.var->name);

        // Check if right-hand side operand needs be promoted
        auto rhs_size = values::range_of(rhs_value);
        auto lhs_size = values::range_of(lhs_value);
        if (rhs_size != lhs_size) {
            throw error::AnalysisError{ fmt::format(
                "trying to initialize a variable of size {} with a value of size {}",
                lhs_size, rhs_size) };
        } else {
            const auto &lhs_type = values::type_of(lhs_value);
            const auto &promoted_rhs_value = promoteValueToType(rhs_value, lhs_type);
            ret.emplace(lhs_value, promoted_rhs_value);
        }

        // Set condition code
        ret->condition.set(tree::make<values::ConstBool>(true));

        // Copy annotation data
        ret->annotations = visitAnnotations(initialization_ast.annotations);
        ret->copy_annotation<parser::SourceLocation>(initialization_ast);
    } catch (error::AnalysisError &err) {
        err.context(initialization_ast);
        result_.errors.push_back(err.get_message());
    }
    return ret;
}

tree::Maybe<semantic::AssignmentInstruction> AnalyzeTreeGenAstVisitor::visitAssignmentInstruction(
    const ast::AssignmentInstruction &instruction_ast) {

    tree::Maybe<semantic::AssignmentInstruction> ret{};
    try {
        // Analyze the operands
        const auto &lhs_value = visitExpression(*instruction_ast.lhs);
        const auto &rhs_value = visitExpression(*instruction_ast.rhs);

        // Check assignability of the left-hand side
        if (bool assignable = lhs_value->as_reference(); !assignable) {
            throw error::AnalysisError{ "left-hand side of assignment statement must be assignable" };
        } else {
            ret->lhs = lhs_value;
        }

        // Check if right-hand side operand needs be promoted
        // TODO: factor this code and the same code in visitInitialization out?
        auto rhs_size = values::range_of(rhs_value);
        auto lhs_size = values::range_of(lhs_value);
        if (rhs_size != lhs_size) {
            throw error::AnalysisError{ fmt::format(
                "trying to initialize a variable of size {} with an initialization list of size {}",
                lhs_size, rhs_size) };
        } else {
            const auto &lhs_type = values::type_of(lhs_value);
            const auto &promoted_rhs_value = promoteValueToType(rhs_value, lhs_type);
            ret.emplace(lhs_value, promoted_rhs_value);
        }

        // Set condition code
        ret->condition.set(tree::make<values::ConstBool>(true));

        // Copy annotation data
        ret->annotations = visitAnnotations(instruction_ast.annotations);
        ret->copy_annotation<parser::SourceLocation>(instruction_ast);
    } catch (error::AnalysisError &err) {
        err.context(instruction_ast);
        result_.errors.push_back(err.get_message());
    }
    return ret;
}

values::Value AnalyzeTreeGenAstVisitor::visitExpression(const ast::Expression &expression_ast) {
    values::Value ret{};
    try {
        if (auto integer_literal = expression_ast.as_integer_literal()) {
            ret.set(tree::make<values::ConstInt>(integer_literal->value));
        } else if (auto float_literal = expression_ast.as_float_literal()) {
            ret.set(tree::make<values::ConstReal>(float_literal->value));
        } else if (auto identifier = expression_ast.as_identifier()) {
            ret.set(analyzer_.resolve_mapping(identifier->name));
        } else if (auto index = expression_ast.as_index()) {
            ret.set(visitIndex(*index));
        } else if (auto initialization_list = expression_ast.as_initialization_list()) {
            ret.set(visitInitializationList(*initialization_list));
        } else {
            throw std::runtime_error{ "unexpected expression node" };
        }
    } catch (error::AnalysisError &err) {
        err.context(expression_ast);
        throw;
    }
    if (ret.empty()) {
        throw std::runtime_error{ "visitExpression returned an empty value" };
    }
    ret->copy_annotation<parser::SourceLocation>(expression_ast);
    return ret;
}

values::Value AnalyzeTreeGenAstVisitor::visitIndex(const ast::Index &index_ast) {
    auto expression = visitExpression(*index_ast.expr);
    auto variable_ref_ptr = expression->as_variable_ref();
    const auto &variable_link = variable_ref_ptr->variable;
    const auto &variable = *variable_link;
    if (auto qubit_array = variable.typ->as_qubit_array()) {
        auto indices = visitIndexList(*index_ast.indices, qubit_array->size);
        return tree::make<values::IndexRef>(variable_link, indices);
    } else if (auto bit_array = variable.typ->as_bit_array()) {
        auto indices = visitIndexList(*index_ast.indices, bit_array->size);
        return tree::make<values::IndexRef>(variable_link, indices);
    } else {
        throw error::AnalysisError{ fmt::format(
            "indexation is not supported for value of type '{}'", values::type_of(expression)), &index_ast };
    }
}

tree::Many<values::ConstInt> AnalyzeTreeGenAstVisitor::visitIndexList(
    const ast::IndexList &index_list_ast, size_t size) {

    tree::Many<values::ConstInt> ret;
    for (const auto &index_entry : index_list_ast.items) {
        if (auto index_item = index_entry->as_index_item()) {
            // Single index
            auto index_item_value_sp = visitIndexItem(*index_item, size);
            ret.add(index_item_value_sp);
        } else if (auto index_range = index_entry->as_index_range()) {
            // Range notation
            ret = visitIndexRange(*index_range, size);
        } else {
            throw std::runtime_error{ "unknown IndexEntry AST node" };
        }
    }
    return ret;
}

tree::One<values::ConstInt> AnalyzeTreeGenAstVisitor::visitIndexItem(
    const ast::IndexItem &index_item_ast, size_t size) {

    auto index_item = visitConstInt(*index_item_ast.index);
    if (index_item < 0 || static_cast<unsigned long>(index_item) >= size) {
        throw error::AnalysisError{
            fmt::format("index {} out of range (size {})", index_item, size), &index_item_ast };
    }
    auto index_value_sp = tree::make<values::ConstInt>(index_item);
    index_value_sp->copy_annotation<parser::SourceLocation>(index_item_ast);
    return index_value_sp;
}

tree::Many<values::ConstInt> AnalyzeTreeGenAstVisitor::visitIndexRange(
    const ast::IndexRange &index_range_ast, size_t size) {

    auto first = visitConstInt(*index_range_ast.first);
    if (first < 0 || static_cast<unsigned long>(first) >= size) {
        throw error::AnalysisError{
            fmt::format("index {} out of range (size {})", first, size), &*index_range_ast.first };
    }
    auto last = visitConstInt(*index_range_ast.last);
    if (last < 0 || static_cast<unsigned long>(last) >= size) {
        throw error::AnalysisError{
            fmt::format("index {} out of range (size {})", last, size), &*index_range_ast.last };
    }
    if (first > last) {
        throw error::AnalysisError("last index is lower than first index", &index_range_ast);
    }
    tree::Many<values::ConstInt> ret{};
    for (auto index = first; index <= last; index++) {
        auto index_value_sp = tree::make<values::ConstInt>(index);
        index_value_sp->copy_annotation<parser::SourceLocation>(index_range_ast);
        ret.add(index_value_sp);
    }
    return ret;
}

values::Value AnalyzeTreeGenAstVisitor::visitInitializationList(
    const ast::InitializationList &initialization_list_ast) {

    // TODO: the code below should be generic, e.g., work as well for Bool/BoolArray, Real/RealArray, and so on
    if (initialization_list_ast.expr_list->items.empty()) {
        throw error::AnalysisError{ "initialization list should have at least one element" };
    }
    const auto &items = initialization_list_ast.expr_list->items.get_vec();
    const auto &first_value = visitExpression(*items[0]);
    if (types::type_check(values::type_of(first_value), tree::make<types::Int>())) {
        auto ret = tree::make<values::ConstIntArray>();
        ret->value.add(first_value);
        std::for_each(std::next(items.begin()), items.end(), [this, &ret](const auto &item) {
            const auto &value = visitExpression(*item);
            if (types::type_check(values::type_of(value), tree::make<types::Int>())) {
                ret->value.add(value);
            } else {
                throw error::AnalysisError{ fmt::format("expecting value of type ({}) but found ({})",
                    types::Type(tree::make<types::Int>()), values::type_of(value)) };
            }
        });
        return ret;
    } else {
        throw error::AnalysisError{ "unimplemented" };
    }
}

tree::Any<semantic::AnnotationData> AnalyzeTreeGenAstVisitor::visitAnnotations(
    const tree::Any<ast::AnnotationData> &annotations_ast) {

    auto ret = tree::Any<semantic::AnnotationData>();
    for (const auto &annotation_ast : annotations_ast) {
        try {
            auto annotation = tree::make<semantic::AnnotationData>();
            annotation->interface = annotation_ast->interface->name;
            annotation->operation = annotation_ast->operation->name;
            for (const auto &expression_ast : annotation_ast->operands->items) {
                try {
                    annotation->operands.add(visitExpression(*expression_ast));
                } catch (error::AnalysisError &e) {
                    e.context(*annotation_ast);
                    result_.errors.push_back(e.get_message());
                }
            }
            annotation->copy_annotation<parser::SourceLocation>(*annotation_ast);
            ret.add(annotation);
        } catch (error::AnalysisError &err) {
            err.context(*annotation_ast);
            result_.errors.push_back(err.get_message());
        }
    }
    return ret;
}

/**
 * Shorthand for parsing an expression and promoting it to the given type,
 * constructed in-place with the type_args parameter pack.
 * Returns empty when the cast fails.
 */
template <class Type, class... TypeArgs>
values::Value AnalyzeTreeGenAstVisitor::analyze_as(const ast::Expression &expression, TypeArgs... type_args) {
    return values::promote(visitExpression(expression), tree::make<Type>(type_args...));
}

/**
 * Shorthand for parsing an expression to a constant integer.
 */
primitives::Int AnalyzeTreeGenAstVisitor::visitConstInt(const ast::Expression &expression) {
    try {
        auto value = analyze_as<types::Int>(expression);
        if (value.empty()) {
            throw error::AnalysisError("expected an integer");
        }
        if (auto int_value = value->as_const_int()) {
            return int_value->value;
        } else {
            throw error::AnalysisError("integer must be constant");
        }
    } catch (error::AnalysisError &e) {
        e.context(expression);
        throw;
    }
}

}  // namespace cqasm::v3x::analyzer
