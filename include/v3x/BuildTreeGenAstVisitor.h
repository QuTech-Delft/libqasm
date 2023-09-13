#pragma once

#include "antlr4-runtime.h"
#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"

#include <any>


namespace cqasm::v3x::parser {

class  BuildTreeGenAstVisitor : public CqasmParserVisitor {
public:
    std::any visitProgram(CqasmParser::ProgramContext *context) override;
    std::any visitVersion(CqasmParser::VersionContext *context) override;
    std::any visitStatement(CqasmParser::StatementContext *context) override;
    std::any visitMapStatement(CqasmParser::MapStatementContext *context) override;
    std::any visitVarStatement(CqasmParser::VarStatementContext *context) override;
    std::any visitInstruction(CqasmParser::InstructionContext *context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitExpression(CqasmParser::ExpressionContext *context) override;
    std::any visitArrayElements(CqasmParser::ArrayElementsContext *context) override;
    std::any visitArrayIndex(CqasmParser::ArrayIndexContext *context) override;
};

}  // namespace cqasm::v3x::parser
