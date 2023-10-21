#include "v3x/AnalyzeTreeGenAstVisitor.hpp"


namespace cqasm::v3x::analyzer {

std::any AnalyzeTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitStatements(CqasmParser::StatementsContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitStatementSeparator(CqasmParser::StatementSeparatorContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitQubitTypeDefinition(CqasmParser::QubitTypeDefinitionContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitBitTypeDefinition(CqasmParser::BitTypeDefinitionContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitMeasureStatement(CqasmParser::MeasureStatementContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIndexList(CqasmParser::IndexListContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIndexItem(CqasmParser::IndexItemContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIndexRange(CqasmParser::IndexRangeContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitFloatLiteral(CqasmParser::FloatLiteralContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIdentifier(CqasmParser::IdentifierContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

std::any AnalyzeTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented"};
}

}  // namespace cqasm::v3x::analyzer
