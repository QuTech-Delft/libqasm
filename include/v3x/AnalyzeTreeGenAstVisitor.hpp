#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-semantic-gen.hpp"


namespace cqasm::v3x::analyzer {

class AnalyzeTreeGenAstVisitor {
    const Analyzer &analyzer_;
    AnalysisResult result_;

public:
    AnalysisResult visitProgram(const ast::Program &program_ast);
    void visitVersion(const ast::Version &version_ast);
    void visitStatementList(const ast::StatementList &statement_list_ast);
    void visitVariables(const ast::Variables &variables_ast);
    void visitMeasureStatement(const ast::MeasureStatement &ast);
    tree::Maybe<semantic::Instruction> visitInstruction(const ast::Instruction &ast);
    tree::Maybe<values::Value> visitExpressionList(const ast::ExpressionList &ast);
    values::Value visitExpression(const ast::Expression &ast);
    tree::Many<values::ConstInt> visitIndexList(const ast::IndexList &ast);
    values::ConstInt visitIndexItem(const ast::IndexItem &ast);
    tree::Many<values::ConstInt> visitIndexRange(const ast::IndexRange &ast);
    values::ConstInt visitIntegerLiteral(const ast::IntegerLiteral &ast);
    values::ConstReal visitFloatLiteral(const ast::FloatLiteral &ast);
    values::Value visitIdentifier(const ast::Identifier &ast);
    values::Value visitIndex(const ast::Index &ast);
    tree::Any<semantic::AnnotationData> visitAnnotations(const tree::Any<ast::AnnotationData> &annotations_ast);

    explicit AnalyzeTreeGenAstVisitor(const Analyzer &analyzer);
};

}  // namespace cqasm::v3x::analyzer
