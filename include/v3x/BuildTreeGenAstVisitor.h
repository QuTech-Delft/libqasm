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
    std::any visitMapping(CqasmParser::MappingContext *context) override;
    std::any visitVariable(CqasmParser::VariableContext *context) override;
    std::any visitInstruction(CqasmParser::InstructionContext *context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitExpression(CqasmParser::ExpressionContext *context) override;
    std::any visitIndex(CqasmParser::IndexContext *context) override;
};

}  // namespace cqasm::v3x::parser
