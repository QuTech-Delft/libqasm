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

    std::int64_t get_int_value(size_t line, size_t char_position_in_line, const std::string &text);
    std::int64_t get_int_value(antlr4::tree::TerminalNode *node);
    double get_float_value(size_t line, size_t char_position_in_line, const std::string &text);
    double get_float_value(antlr4::tree::TerminalNode *node);
public:
    std::any visitProgram(CqasmParser::ProgramContext *context) override;
    std::any visitVersion(CqasmParser::VersionContext *context) override;
    std::any visitStatements(CqasmParser::StatementsContext *context) override;
    std::any visitStatementSeparator(CqasmParser::StatementSeparatorContext *context) override;
    std::any visitQubitTypeDefinition(CqasmParser::QubitTypeDefinitionContext *context) override;
    std::any visitBitTypeDefinition(CqasmParser::BitTypeDefinitionContext *context) override;
    std::any visitMeasureStatement(CqasmParser::MeasureStatementContext *context) override;
    std::any visitInstruction(CqasmParser::InstructionContext *context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitIndexList(CqasmParser::IndexListContext *context) override;
    std::any visitIndexItem(CqasmParser::IndexItemContext *context) override;
    std::any visitIndexRange(CqasmParser::IndexRangeContext *context) override;
    std::any visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) override;
    std::any visitFloatLiteral(CqasmParser::FloatLiteralContext *context) override;
    std::any visitIdentifier(CqasmParser::IdentifierContext *context) override;
    std::any visitIndex(CqasmParser::IndexContext *context) override;

    explicit BuildTreeGenAstVisitor(const std::string &file_name = "<unknown>")
    : file_name_{ file_name } {}
};

}  // namespace cqasm::v3x::parser
