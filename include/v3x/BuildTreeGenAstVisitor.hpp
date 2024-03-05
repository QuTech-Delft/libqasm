#pragma once

#include "v3x/BuildCustomAstVisitor.hpp"
#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"

#include <antlr4-runtime.h>
#include <any>
#include <optional>


namespace cqasm::v3x::parser { class CustomErrorListener; }


namespace cqasm::v3x::parser {

class BuildTreeGenAstVisitor : public BuildCustomAstVisitor {
    /**
     * Name of the file being parsed.
     */
    std::optional<std::string> file_name_;

    /**
     * Error listener.
     */
    CustomErrorListener* error_listener_p_;

    [[nodiscard]] std::int64_t get_int_value(size_t line, size_t char_position_in_line, const std::string &text) const;
    [[nodiscard]] double get_float_value(size_t line, size_t char_position_in_line, const std::string &text) const;

    bool get_bool_value(antlr4::tree::TerminalNode *node) const;
    std::int64_t get_int_value(antlr4::tree::TerminalNode *node) const ;
    double get_float_value(antlr4::tree::TerminalNode *node) const;

    tree::Maybe<ast::IntegerLiteral> getArraySize(CqasmParser::ArraySizeDeclarationContext *context);

    template <typename Context>
    tree::One<ast::Variable> visitVariable(Context *context) {
        auto ret = tree::make<ast::Variable>(
            tree::One<ast::Identifier>{ tree::make<ast::Identifier>(context->IDENTIFIER()->getText()) },
            std::any_cast<tree::One<ast::Type>>(context->type()->accept(this))
        );
        setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
        return ret;
    }

    template <typename Context>
    tree::One<ast::Variable> visitClassicalVariable(Context *context) {
        auto ret = tree::make<ast::Variable>(
            tree::One<ast::Identifier>{ tree::make<ast::Identifier>(context->IDENTIFIER()->getText()) },
            std::any_cast<tree::One<ast::Type>>(context->classicalType()->accept(this))
        );
        setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
        return ret;
    }

    template <typename RetExpressionType, typename Context>
    std::any visitBinaryExpression(Context *context, antlr4::Token *token) {
        auto ret = tree::make<RetExpressionType>(
            std::any_cast<tree::One<ast::Expression>>(context->expression(0)->accept(this)),
            std::any_cast<tree::One<ast::Expression>>(context->expression(1)->accept(this))
        );
        setNodeAnnotation(ret, token);
        return tree::One<ast::Expression>{ ret };
    }

public:
    std::any visitProgram(CqasmParser::ProgramContext *context) override;
    std::any visitStatementSeparator(CqasmParser::StatementSeparatorContext *context) override;
    std::any visitVersion(CqasmParser::VersionContext *context) override;
    std::any visitGlobalBlock(CqasmParser::GlobalBlockContext *context) override;
    std::any visitLocalBlock(CqasmParser::LocalBlockContext *context) override;
    std::any visitGlobalBlockStatement(CqasmParser::GlobalBlockStatementContext *context) override;
    std::any visitLocalBlockStatement(CqasmParser::LocalBlockStatementContext *context) override;
    std::any visitVariableDeclaration(CqasmParser::VariableDeclarationContext *context) override;
    std::any visitVariableDefinition(CqasmParser::VariableDefinitionContext *context) override;
    std::any visitVariableInitialization(CqasmParser::VariableInitializationContext *context) override;
    std::any visitFunctionDeclaration(CqasmParser::FunctionDeclarationContext *context) override;
    std::any visitParameters(CqasmParser::ParametersContext *context) override;
    std::any visitAssignmentStatement(CqasmParser::AssignmentStatementContext *context) override;
    std::any visitReturnStatement(CqasmParser::ReturnStatementContext *context) override;
    std::any visitExpressionStatement(CqasmParser::ExpressionStatementContext *context) override;
    std::any visitGate(CqasmParser::GateContext *context) override;
    std::any visitMeasureInstruction(CqasmParser::MeasureInstructionContext *context) override;
    std::any visitType(CqasmParser::TypeContext *context) override;
    std::any visitQubitType(CqasmParser::QubitTypeContext *context) override;
    std::any visitBitType(CqasmParser::BitTypeContext *context) override;
    std::any visitAxisType(CqasmParser::AxisTypeContext *context) override;
    std::any visitBoolType(CqasmParser::BoolTypeContext *context) override;
    std::any visitIntType(CqasmParser::IntTypeContext *context) override;
    std::any visitFloatType(CqasmParser::FloatTypeContext *context) override;
    std::any visitArraySizeDeclaration(CqasmParser::ArraySizeDeclarationContext *context) override;

    std::any visitExpressionList(CqasmParser::ExpressionListContext *context) override;
    std::any visitIndexList(CqasmParser::IndexListContext *context) override;
    std::any visitIndexItem(CqasmParser::IndexItemContext *context) override;
    std::any visitIndexRange(CqasmParser::IndexRangeContext *context) override;
    std::any visitParensExpression(CqasmParser::ParensExpressionContext *context) override;
    std::any visitUnaryPlusMinusExpression(CqasmParser::UnaryPlusMinusExpressionContext *context) override;
    std::any visitBitwiseNotExpression(CqasmParser::BitwiseNotExpressionContext *context) override;
    std::any visitLogicalNotExpression(CqasmParser::LogicalNotExpressionContext *context) override;
    std::any visitPowerExpression(CqasmParser::PowerExpressionContext *context) override;
    std::any visitProductExpression(CqasmParser::ProductExpressionContext *context) override;
    std::any visitAdditionExpression(CqasmParser::AdditionExpressionContext *context) override;
    std::any visitShiftExpression(CqasmParser::ShiftExpressionContext *context) override;
    std::any visitComparisonExpression(CqasmParser::ComparisonExpressionContext *context) override;
    std::any visitEqualityExpression(CqasmParser::EqualityExpressionContext *context) override;
    std::any visitBitwiseAndExpression(CqasmParser::BitwiseAndExpressionContext *context) override;
    std::any visitBitwiseXorExpression(CqasmParser::BitwiseXorExpressionContext *context) override;
    std::any visitBitwiseOrExpression(CqasmParser::BitwiseOrExpressionContext *context) override;
    std::any visitLogicalAndExpression(CqasmParser::LogicalAndExpressionContext *context) override;
    std::any visitLogicalXorExpression(CqasmParser::LogicalXorExpressionContext *context) override;
    std::any visitLogicalOrExpression(CqasmParser::LogicalOrExpressionContext *context) override;
    std::any visitTernaryConditionalExpression(CqasmParser::TernaryConditionalExpressionContext *context) override;
    std::any visitFunctionCall(CqasmParser::FunctionCallContext *context) override;
    std::any visitIndex(CqasmParser::IndexContext *context) override;
    std::any visitIdentifier(CqasmParser::IdentifierContext *context) override;
    std::any visitAxisInitializationList(CqasmParser::AxisInitializationListContext *context) override;
    std::any visitInitializationList(CqasmParser::InitializationListContext *context) override;
    std::any visitBooleanLiteral(CqasmParser::BooleanLiteralContext *context) override;
    std::any visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) override;
    std::any visitFloatLiteral(CqasmParser::FloatLiteralContext *context) override;

    explicit BuildTreeGenAstVisitor(const std::optional<std::string> &file_name);
    void addErrorListener(CustomErrorListener *errorListener) override;
    void syntaxError(size_t line, size_t char_position_in_line, const std::string &text) const override;
    void setNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const override;
    void expandNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const override;
};

}  // namespace cqasm::v3x::parser
