#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-semantic-gen.hpp"


namespace cqasm::v3x::analyzer {

class AnalyzeTreeGenAstVisitor {
    const Analyzer &analyzer_;
    AnalysisResult result_;

public:
    AnalysisResult visitProgram(const ast::Program &ast);
    void visitVersion(const ast::Version &ast);
    void visitStatementList(const ast::StatementList &ast);
    void visitVariables(const ast::Variables &ast);
    void visitMeasureStatement(const ast::MeasureStatement &ast);
    tree::Maybe<semantic::Instruction> visitInstruction(const ast::Instruction &ast);
    tree::Maybe<values::Value> visitExpressionList(const ast::ExpressionList &ast);
    tree::Many<values::ConstInt> visitIndexList(const ast::IndexList &ast);
    values::ConstInt visitIndexItem(const ast::IndexItem &ast);
    tree::Many<values::ConstInt> visitIndexRange(const ast::IndexRange &ast);
    values::ConstInt visitIntegerLiteral(const ast::IntegerLiteral &ast);
    values::ConstReal visitFloatLiteral(const ast::FloatLiteral &ast);
    values::Value visitIdentifier(const ast::Identifier &ast);
    values::Value visitIndex(const ast::Index &ast);

    explicit AnalyzeTreeGenAstVisitor(const Analyzer &analyzer);
};

}  // namespace cqasm::v3x::analyzer
