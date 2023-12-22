#pragma once

#include "cqasm-tree.hpp"
#include "v3x/BuildCustomAstVisitor.hpp"
#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"
#include "v3x/CustomErrorListener.hpp"
#include "v3x/cqasm-ast.hpp"

#include <any>


namespace cqasm::v3x::parser {

class  BuildTreeGenAstVisitor : public BuildCustomAstVisitor {
    /**
     * Name of the file being parsed.
     */
    std::string file_name_;

    /**
     * Error listener.
     */
    CustomErrorListener* error_listener_p_;

    [[nodiscard]] std::int64_t get_int_value(size_t line, size_t char_position_in_line, const std::string &text) const;
    [[nodiscard]] double get_float_value(size_t line, size_t char_position_in_line, const std::string &text) const;

    bool get_bool_value(antlr4::tree::TerminalNode *node) const;
    std::int64_t get_int_value(antlr4::tree::TerminalNode *node) const ;
    double get_float_value(antlr4::tree::TerminalNode *node) const;

    std::any visitBoolTypeDefinition(CqasmParser::BoolTypeDeclarationContext *context);
    std::any visitBoolTypeInitialization(CqasmParser::BoolTypeDeclarationContext *context);
    std::any visitIntTypeDefinition(CqasmParser::IntTypeDeclarationContext *context);
    std::any visitIntTypeInitialization(CqasmParser::IntTypeDeclarationContext *context);
    std::any visitFloatTypeDefinition(CqasmParser::FloatTypeDeclarationContext *context);
    std::any visitFloatTypeInitialization(CqasmParser::FloatTypeDeclarationContext *context);

public:
    std::any visitProgram(CqasmParser::ProgramContext *context) override;
    std::any visitVersion(CqasmParser::VersionContext *context) override;
    std::any visitStatements(CqasmParser::StatementsContext *context) override;
    std::any visitStatementSeparator(CqasmParser::StatementSeparatorContext *context) override;
    std::any visitQubitTypeDeclaration(CqasmParser::QubitTypeDeclarationContext *context) override;
    std::any visitBitTypeDeclaration(CqasmParser::BitTypeDeclarationContext *context) override;
    std::any visitAxisTypeDeclaration(CqasmParser::AxisTypeDeclarationContext *context) override;
    std::any visitBoolTypeDeclaration(CqasmParser::BoolTypeDeclarationContext *context) override;
    std::any visitIntTypeDeclaration(CqasmParser::IntTypeDeclarationContext *context) override;
    std::any visitFloatTypeDeclaration(CqasmParser::FloatTypeDeclarationContext *context) override;
    std::any visitArraySizeDeclaration(CqasmParser::ArraySizeDeclarationContext *context) override;
    std::any visitMeasureInstruction(CqasmParser::MeasureInstructionContext *context) override;
    std::any visitInstruction(CqasmParser::InstructionContext *context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitIndexList(CqasmParser::IndexListContext *context) override;
    std::any visitIndexItem(CqasmParser::IndexItemContext *context) override;
    std::any visitIndexRange(CqasmParser::IndexRangeContext *context) override;
    std::any visitBooleanLiteral(CqasmParser::BooleanLiteralContext *context) override;
    std::any visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) override;
    std::any visitFloatLiteral(CqasmParser::FloatLiteralContext *context) override;
    std::any visitIdentifier(CqasmParser::IdentifierContext *context) override;
    std::any visitIndex(CqasmParser::IndexContext *context) override;
    std::any visitAxisInitializationList(CqasmParser::AxisInitializationListContext *context) override;
    std::any visitInitializationList(CqasmParser::InitializationListContext *context) override;

    explicit BuildTreeGenAstVisitor(std::string file_name = "<unknown>");
    void addErrorListener(CustomErrorListener *errorListener) override;
    void syntaxError(size_t line, size_t char_position_in_line, const std::string &text) const override;
    void setNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const override;
};

}  // namespace cqasm::v3x::parser
