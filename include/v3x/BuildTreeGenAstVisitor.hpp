#pragma once

#include "v3x/BuildCustomAstVisitor.hpp"
#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"

#include <any>


namespace cqasm::v3x::parser {

class  BuildTreeGenAstVisitor : public BuildCustomAstVisitor {
    /**
     * Name of the file being parsed.
     */
    std::string file_name_;

    std::int64_t get_integer_literal_value(antlr4::tree::TerminalNode *node);
    double get_float_literal_value(antlr4::tree::TerminalNode *node);
public:
    std::any visitProgram(CqasmParser::ProgramContext *context) override;
    std::any visitVersion(CqasmParser::VersionContext *context) override;
    std::any visitQubits(CqasmParser::QubitsContext *context) override;
    std::any visitStatement(CqasmParser::StatementContext *context) override;
    std::any visitMapping(CqasmParser::MappingContext *context) override;
    std::any visitVariable(CqasmParser::VariableContext *context) override;
    std::any visitInstruction(CqasmParser::InstructionContext *context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitExpression(CqasmParser::ExpressionContext *context) override;
    std::any visitIndex(CqasmParser::IndexContext *context) override;

    explicit BuildTreeGenAstVisitor(const std::string &file_name = "<unknown>")
    : file_name_{ file_name } {}
};

}  // namespace cqasm::v3x::parser
