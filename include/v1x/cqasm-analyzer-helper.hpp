#pragma once

#include "v1x/cqasm-analysis-result.hpp"
#include "v1x/cqasm-analyzer.hpp"
#include "v1x/cqasm-scope.hpp"

#include <list>
#include <string>
#include <utility>  // pair


namespace cqasm::v1x::analyzer {

/**
 * Helper class for analyzing a single AST.
 * This contains the stateful information that Analyzer can't have (to allow Analyzer to be reused).
 */
class AnalyzerHelper {
public:
    /**
     * The analyzer associated with this helper.
     */
    const Analyzer &analyzer;

    /**
     * The analysis result being constructed.
     */
    AnalysisResult result;

    /**
     * Scope stack. back() is the global scope, front() is the current scope.
     */
    std::list<Scope> scope_stack;

    /**
     * List of all goto instructions in the program, for name resolution when all other analysis completes.
     */
    std::list<std::pair<tree::Maybe<semantic::GotoInstruction>, std::string>> gotos;

    /**
     * Analyzes the given AST using the given analyzer.
     */
    AnalyzerHelper(const Analyzer &analyzer, const ast::Program &ast);

    /**
     * Parses the version tag.
     * Any semantic errors encountered are pushed into the result error vector.
     */
    void analyze_version(const ast::Version &ast);

    /**
     * Checks the qubits statement and updates the scope accordingly.
     * Any semantic errors encountered are pushed into the result error vector.
     */
    void analyze_qubits(const ast::Expression &count);

    /**
     * Returns a reference to the subcircuit that's currently being built.
     * If there is no subcircuit yet, a default one is created, using the source location annotation on the source node.
     */
    tree::Maybe<semantic::Subcircuit> get_current_subcircuit(const tree::Annotatable &source);

    /**
     * Returns a reference to the current scope.
     */
    Scope &get_current_scope();

    /**
     * Returns a reference to the global scope.
     */
    Scope &get_global_scope();

    /**
     * Returns a reference to the block that's currently being built (1.2+).
     */
    tree::Maybe<semantic::Block> get_current_block(const tree::Annotatable &source);

    /**
     * Adds an analyzed statement to the current block (1.2+).
     */
    void add_to_current_block(const tree::Maybe<semantic::Statement> &stmt);

    /**
     * Analyzes the given statement list,
     * adding the analyzed statements to the current subcircuit (API 1.0/1.1) or block (API 1.2+).
     */
    void analyze_statements(const ast::StatementList &statements);

    /**
     * Analyzes a statement list corresponding to a structured control-flow subblock (1.2+).
     * Handles the requisite scoping, then defers to analyze_statements().
     */
    tree::Maybe<semantic::Block> analyze_subblock(const ast::StatementList &statements, bool is_loop);

    /**
     * Analyzes the given bundle and, if valid, adds it to the current subcircuit using API version 1.0/1.1.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the subcircuit.
     */
    void analyze_bundle(const ast::Bundle &bundle);

    /**
     * Analyzes the given bundle and, if valid, adds it to the current scope/block using API version 1.2+.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the block.
     */
    void analyze_bundle_ext(const ast::Bundle &bundle);

    /**
     * Analyzes the given instruction.
     * If an error occurs, the message is added to the result error vector, and an empty Maybe is returned.
     */
    tree::Maybe<semantic::Instruction> analyze_instruction(const ast::Instruction &insn);

    /**
     * Analyzes the given cQASM 1.2+ set instruction.
     * If an error occurs, the message is added to the result error vector, and an empty Maybe is returned.
     */
    tree::Maybe<semantic::SetInstruction> analyze_set_instruction(const ast::Instruction &insn);

    /**
     * Analyzes the given two operands as lhs and rhs of a set instruction.
     * Used for the actual set instruction as well as the assignments in the header of a C-style for loop.
     */
    tree::Maybe<semantic::SetInstruction> analyze_set_instruction_operands(
        const ast::Expression &lhs_expr,
        const ast::Expression &rhs_expr
    );

    /**
     * Analyzes the given cQASM 1.2+ goto instruction.
     * If an error occurs, the message is added to the result error vector, and an empty Maybe is returned.
     */
    tree::Maybe<semantic::GotoInstruction> analyze_goto_instruction(const ast::Instruction &insn);

    /**
     * Analyzes the error model meta-instruction and, if valid, adds it to the analysis result.
     * If an error occurs, the message is added to the result error vector, and nothing is added.
     */
    void analyze_error_model(const ast::Instruction &insn);

    /**
     * Analyzes the given mapping and, if valid, adds it to the current scope.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the scope.
     */
    void analyze_mapping(const ast::Mapping &mapping);

    /**
     * Analyzes the given declaration of one or more variables and, if valid, adds them to the current scope.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the scope.
     */
    void analyze_variables(const ast::Variables &variables);

    /**
     * Analyzes the given subcircuit header and, if valid, adds it to the subcircuit list.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the result.
     */
    void analyze_subcircuit(const ast::Subcircuit &subcircuit);

    /**
     * Analyzes the given structured control-flow statement and,
     * if valid, adds it to the current scope/block using API version 1.2+.
     * If an error occurs, the message is added to the result error vector, and nothing is added to the block.
     */
    void analyze_structured(const ast::Structured &structured);

    /**
     * Analyzes the given if-else chain.
     * Only intended for use as a helper function within analyze_structured().
     */
    tree::Maybe<semantic::IfElse> analyze_if_else(const ast::IfElse &if_else);

    /**
     * Analyzes the given C-style for loop.
     * Only intended for use as a helper function within analyze_structured().
     */
    tree::Maybe<semantic::ForLoop> analyze_for_loop(const ast::ForLoop &for_loop);

    /**
     * Analyzes the given static for loop.
     * Only intended for use as a helper function within analyze_structured().
     */
    tree::Maybe<semantic::ForeachLoop> analyze_foreach_loop(const ast::ForeachLoop &foreach_loop);

    /**
     * Analyzes the given while loop.
     * Only intended for use as a helper function within analyze_structured().
     */
    tree::Maybe<semantic::WhileLoop> analyze_while_loop(const ast::WhileLoop &while_loop);

    /**
     * Analyzes the given repeat-until loop.
     * Only intended for use as a helper function within analyze_structured().
     */
    tree::Maybe<semantic::RepeatUntilLoop> analyze_repeat_until_loop(const ast::RepeatUntilLoop &repeat_until_loop);

    /**
     * Analyzes the given list of annotations.
     * Any errors found result in the annotation being skipped and an error being appended to the result error vector.
     */
    tree::Any<semantic::AnnotationData> analyze_annotations(
        const tree::Any<ast::AnnotationData> &annotations);

    /**
     * Parses any kind of expression.
     * Always returns a filled value or throws an exception.
     */
    values::Value analyze_expression(const ast::Expression &expression);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack.
     * Returns empty when the cast fails.
     */
    template <class Type, class... TypeArgs>
    values::Value analyze_as(const ast::Expression &expression, TypeArgs... type_args) {
        return values::promote(analyze_expression(expression), tree::make<Type>(type_args...));
    }

    /**
     * Shorthand for parsing an expression to a constant integer.
     */
    primitives::Int analyze_as_const_int(const ast::Expression &expression);

    /**
     * Parses a matrix. Always returns a filled value or throws an exception.
     */
    values::Value analyze_matrix(const ast::MatrixLiteral &matrix_lit);

    /**
     * Helper for parsing a matrix.
     * Highly templated to avoid repeating the same code for different kinds of matrices,
     * but bear in mind that the template parameters are codependent.
     * Returns empty on failure.
     */
    template <class ElType, class ElVal, class MatLit, class MatVal>
    values::Value analyze_matrix_helper(
        size_t num_rows, size_t num_cols,
        const std::vector<values::Value> &vals
    ) {
        auto matrix = MatLit(num_rows, num_cols);
        for (size_t row = 0; row < num_rows; row++) {
            for (size_t col = 0; col < num_cols; col++) {
                auto val = values::promote(vals[row * num_cols + col], tree::make<ElType>());
                if (val.empty()) {
                    return {};
                } else {
                    auto val_real = val.template as<ElVal>();
                    if (val_real.empty()) {
                        return {};
                    } else {
                        matrix.at(row + 1, col + 1) = val_real->value;
                    }
                }
            }
        }
        return tree::make<MatVal>(matrix);
    }

    /**
     * Parses an index operator. Always returns a filled value or throws an
     * error.
     */
    values::Value analyze_index(const ast::Index &index);

    /**
     * Parses an index list.
     */
    tree::Many<values::ConstInt> analyze_index_list(
        const ast::IndexList &index_list, size_t size);

    /**
     * Parses a function. Always returns a filled value or throws an exception.
     */
    values::Value analyze_function(
        const ast::Identifier &name,
        const ast::ExpressionList &args);

    /**
     * Parses an operator. Always returns a filled value or throws an exception.
     */
    values::Value analyze_operator(
        const std::string &name,
        const tree::One<ast::Expression> &a,
        const tree::One<ast::Expression> &b = tree::One<ast::Expression>(),
        const tree::One<ast::Expression> &c = tree::One<ast::Expression>());
};

} // namespace cqasm::v1x::analyzer
