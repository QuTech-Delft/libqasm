#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-semantic-gen.hpp"


namespace cqasm::v3x::analyzer {

class AnalyzeTreeGenAstVisitor {
    Analyzer &analyzer_;
    AnalysisResult result_;

    template <typename T, typename TArray>
    [[nodiscard]] types::Type visitVariableType(const ast::Variable &variable_ast, std::string_view type_name) const;

    template <typename ConstTypeArray>
    [[nodiscard]] static tree::One<ConstTypeArray> buildArrayValueFromPromotedValues(
        const values::Values &values, const types::Type &type);

    [[nodiscard]] static values::Value buildValueFromPromotedValues(
        const values::Values &values, const types::Type &type);

public:
    AnalysisResult visitProgram(const ast::Program &program_ast);
    void visitVersion(const ast::Version &version_ast);
    void visitStatements(const ast::StatementList &statement_list_ast);
    void visitVariable(const ast::Variable &variable_ast);
    tree::Maybe<semantic::AssignmentInstruction> visitInitialization(const ast::Initialization &initializaion_ast);
    tree::Maybe<semantic::AssignmentInstruction> visitAssignmentInstruction(const ast::AssignmentInstruction &ast);
    tree::Maybe<semantic::Instruction> visitMeasureInstruction(const ast::MeasureInstruction &ast);
    tree::Maybe<semantic::Instruction> visitInstruction(const ast::Instruction &ast);
    values::Value visitExpression(const ast::Expression &ast);
    values::Value visitIndex(const ast::Index &ast);
    tree::Many<values::ConstInt> visitIndexList(const ast::IndexList &ast, size_t size);
    tree::One<values::ConstInt> visitIndexItem(const ast::IndexItem &ast, size_t size);
    tree::Many<values::ConstInt> visitIndexRange(const ast::IndexRange &ast, size_t size);
    values::Value visitInitializationList(const ast::InitializationList &initialization_list_ast);

    tree::Any<semantic::AnnotationData> visitAnnotations(const tree::Any<ast::AnnotationData> &annotations_ast);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack.
     * Returns empty when the cast fails.
     */
    template<class Type, class... TypeArgs>
    values::Value analyze_as(const ast::Expression &expression, TypeArgs... type_args);

    /**
     * Shorthand for parsing an expression to a constant integer.
     */
    primitives::Int visitConstInt(const ast::Expression &expression);

    explicit AnalyzeTreeGenAstVisitor(Analyzer &analyzer);
};

}  // namespace cqasm::v3x::analyzer
