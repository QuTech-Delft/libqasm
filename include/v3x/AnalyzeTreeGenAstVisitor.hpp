#pragma once

#include "v3x/CqasmParserVisitor.h"


namespace cqasm::v3x::analyzer {

class AnalyzeTreeGenAstVisitor {
public:
    std::any visitProgram(CqasmParser::ProgramContext *context);
    std::any visitVersion(CqasmParser::VersionContext *context);
    std::any visitStatements(CqasmParser::StatementsContext *context);
    std::any visitStatementSeparator(CqasmParser::StatementSeparatorContext *context);
    std::any visitQubitTypeDefinition(CqasmParser::QubitTypeDefinitionContext *context);
    std::any visitBitTypeDefinition(CqasmParser::BitTypeDefinitionContext *context);
    std::any visitMeasureStatement(CqasmParser::MeasureStatementContext *context);
    std::any visitInstruction(CqasmParser::InstructionContext *context);
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context);
    std::any visitIndexList(CqasmParser::IndexListContext *context);
    std::any visitIndexItem(CqasmParser::IndexItemContext *context);
    std::any visitIndexRange(CqasmParser::IndexRangeContext *context);
    std::any visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context);
    std::any visitFloatLiteral(CqasmParser::FloatLiteralContext *context);
    std::any visitIdentifier(CqasmParser::IdentifierContext *context);
    std::any visitIndex(CqasmParser::IndexContext *context);
};

}  // namespace cqasm::v3x::analyzer
