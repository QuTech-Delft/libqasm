#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-analyzer.hpp"


namespace cqasm::v3x::analyzer {


AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{} {}

AnalysisResult AnalyzeTreeGenAstVisitor::visitProgram(const ast::Program &program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.get_api_version();
    visitVersion(*program_ast.version);
    visitStatements(*program_ast.statements);
    return result_;
}

void AnalyzeTreeGenAstVisitor::visitVersion(const ast::Version &version_ast) {
    try {
        // Default to API version in case the version in the AST is broken
        result_.root->version = tree::make<semantic::Version>();
        result_.root->version->items = analyzer_.get_api_version();

        // Check API version
        for (auto item : version_ast.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (version_ast.items.more_than(analyzer_.get_api_version())) {
            throw error::AnalysisError(fmt::format(
                "the maximum cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.get_api_version(),
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
            if (auto variables = statement->as_variables()) {
                visitVariables(*variables);
            } else if (auto instruction = statement->as_instruction()) {
                result_.root->statements.add(visitInstruction(*instruction));
            } else {
                throw std::runtime_error("unexpected statement node");
            }
        } catch (error::AnalysisError &err) {
            err.context(*statement);
            result_.errors.push_back(err.get_message());
        }
    }
}

void AnalyzeTreeGenAstVisitor::visitVariables(const ast::Variables &variables_ast) {
    try {
        auto type_name = variables_ast.typ->name;
        types::Type type{};
        if (type_name == "qubit") {
            if (variables_ast.size.empty()) {
                type = tree::make<types::Qubit>();
            } else if (variables_ast.size->value > 0) {
                type = tree::make<types::QubitArray>(variables_ast.size->value);
            } else {
                throw error::AnalysisError("declaring qubit array of size <= 0");
            }
        } else if (type_name == "bit") {
            if (variables_ast.size.empty()) {
                type = tree::make<types::Bit>();
            } else if (variables_ast.size->value > 0) {
                type = tree::make<types::BitArray>(variables_ast.size->value);
            } else {
                throw error::AnalysisError("declaring bit array of size <= 0");
            }
        } else {
            throw error::AnalysisError("unknown type \"" + type_name + "\"");
        }

        // Construct the variables
        for (const auto &identifier : variables_ast.names) {
            // Construct variable
            // Use the location tag of the identifier to record where the variable was defined
            auto variable = tree::make<semantic::Variable>(identifier->name, type.clone());
            variable->copy_annotation<parser::SourceLocation>(*identifier);
            variable->annotations = visitAnnotations(variables_ast.annotations);
            result_.root->variables.add(variable);

            analyzer_.add_mapping(
                identifier->name,
                tree::make<values::VariableRef>(variable),
                tree::Maybe<ast::Mapping>()
            );
        }
    } catch (error::AnalysisError &err) {
        err.context(variables_ast);
        result_.errors.push_back(err.get_message());
    }
}

bool check_same_size_input_output_indices(const values::Values &operands) {
    size_t number_of_input_indices{};
    size_t number_of_output_indices{};
    for (const auto &operand : operands) {
        if (auto variable_ref = operand->as_variable_ref()) {
            const auto &variable = *variable_ref->variable;
            if (variable.typ->as_qubit()) {
                number_of_input_indices += 1;
            } else if (auto qubit_array = variable.typ->as_qubit_array()) {
                number_of_input_indices += qubit_array->size;
            } else if (variable.typ->as_bit()) {
                number_of_output_indices += 1;
            } else if (auto bit_array = variable.typ->as_bit_array()) {
                number_of_output_indices += bit_array->size;
            }
        } else if (auto index_ref = operand->as_index_ref()) {
            const auto &variable = *index_ref->variable;
            if (variable.typ->as_qubit() || variable.typ->as_qubit_array()) {
                number_of_input_indices += index_ref->indices.size();
            } else if (variable.typ->as_bit() || variable.typ->as_bit_array()) {
                number_of_output_indices += index_ref->indices.size();
            }
        }
    }
    return number_of_input_indices == number_of_output_indices;
}

tree::Maybe<semantic::Instruction> AnalyzeTreeGenAstVisitor::visitInstruction(
    const ast::Instruction &instruction_ast) {

    tree::Maybe<semantic::Instruction> node;
    try {
        // Set operand list
        auto operands = values::Values();
        for (const auto &operand_expr : instruction_ast.operands->items) {
            operands.add(visitExpression(*operand_expr));
        }
        // Append output operand list
        if (!instruction_ast.output_operands.empty()) {
            operands.add(visitExpression(*instruction_ast.output_operands));
        }

        // Resolve the instruction
        node.set(analyzer_.resolve_instruction(instruction_ast.name->name, operands));

        // Check number of input/qubit indices equals number of output/bit indices
        if (!node->instruction.empty() && node->instruction->request_same_size_input_output_indices) {
            if (!check_same_size_input_output_indices(operands)) {
                throw error::AnalysisError{
                    "number of input/qubit indices is different from number of output/bit indices", &instruction_ast };
            }
        }

        // Set condition code
        node->condition.set(tree::make<values::ConstBool>(true));

        // Copy annotation data
        node->annotations = visitAnnotations(instruction_ast.annotations);
        node->copy_annotation<parser::SourceLocation>(instruction_ast);
    } catch (error::AnalysisError &err) {
        err.context(instruction_ast);
        throw;
    }
    return node;
}

values::Value AnalyzeTreeGenAstVisitor::visitExpression(const ast::Expression &expression_ast) {
    values::Value retval;
    try {
        if (auto integer_literal = expression_ast.as_integer_literal()) {
            retval.set(tree::make<values::ConstInt>(integer_literal->value));
        } else if (auto float_literal = expression_ast.as_float_literal()) {
            retval.set(tree::make<values::ConstReal>(float_literal->value));
        } else if (auto identifier = expression_ast.as_identifier()) {
            retval.set(analyzer_.resolve_mapping(identifier->name));
        } else if (auto index = expression_ast.as_index()) {
            retval.set(visitIndex(*index));
        } else {
            throw std::runtime_error{ "unexpected expression node" };
        }
    } catch (error::AnalysisError &err) {
        err.context(expression_ast);
        throw;
    }
    if (retval.empty()) {
        throw std::runtime_error{ "visitExpression returned an empty value" };
    }
    retval->copy_annotation<parser::SourceLocation>(expression_ast);
    return retval;
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
