#pragma once

#include <antlr4-runtime.h>

#include <any>
#include <optional>
#include <tuple>

#include "libqasm/v3x/CqasmParser.h"
#include "libqasm/v3x/CqasmParserVisitor.h"
#include "libqasm/v3x/syntactic_analyzer_base.hpp"

namespace cqasm::v3x::parser {
class AntlrCustomErrorListener;
}

namespace cqasm::v3x::parser {

class SyntacticAnalyzer : public BaseSyntacticAnalyzer {
    /**
     * Name of the file being parsed.
     */
    std::optional<std::string> file_name_;

    /**
     * Error listener.
     */
    AntlrCustomErrorListener* error_listener_p_;

    [[nodiscard]] std::int64_t get_int_value(size_t line, size_t char_position_in_line, const std::string& text) const;
    [[nodiscard]] double get_float_value(size_t line, size_t char_position_in_line, const std::string& text) const;

    bool get_bool_value(antlr4::tree::TerminalNode* node) const;
    std::int64_t get_int_value(antlr4::tree::TerminalNode* node) const;
    double get_float_value(antlr4::tree::TerminalNode* node) const;

    tree::Maybe<syntactic::IntegerLiteral> get_array_size(CqasmParser::ArraySizeDeclarationContext* context);

    template <typename Context>
    tree::One<syntactic::Variable> visitVariable(Context* context) {
        auto ret = tree::make<syntactic::Variable>(
            tree::One<syntactic::Identifier>{ tree::make<syntactic::Identifier>(context->IDENTIFIER()->getText()) },
            std::any_cast<tree::One<syntactic::Type>>(context->type()->accept(this)));
        setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
        return ret;
    }

    template <typename RetExpressionType, typename Context>
    std::any visitBinaryExpression(Context* context, antlr4::Token* token) {
        auto ret = tree::make<RetExpressionType>(
            std::any_cast<tree::One<syntactic::Expression>>(context->expression(0)->accept(this)),
            std::any_cast<tree::One<syntactic::Expression>>(context->expression(1)->accept(this)));
        setNodeAnnotation(ret, token);
        return tree::One<syntactic::Expression>{ ret };
    }

public:
    std::any visitProgram(CqasmParser::ProgramContext* context) override;
    std::any visitVersionSection(CqasmParser::VersionSectionContext* context) override;
    std::any visitGlobalBlockSection(CqasmParser::GlobalBlockSectionContext* context) override;
    std::any visitEofSection(CqasmParser::EofSectionContext* context) override;
    std::any visitStatementSeparator(CqasmParser::StatementSeparatorContext* context) override;
    std::any visitVersion(CqasmParser::VersionContext* context) override;
    std::any visitGlobalBlockStatement(CqasmParser::GlobalBlockStatementContext* context) override;
    std::any visitVariableDefinition(CqasmParser::VariableDefinitionContext* context) override;
    std::any visitInstruction(CqasmParser::InstructionContext* context) override;
    std::any visitMeasureInstruction(CqasmParser::MeasureInstructionContext* context) override;
    std::any visitResetInstruction(CqasmParser::ResetInstructionContext* context) override;
    std::any visitInitInstruction(CqasmParser::InitInstructionContext* context) override;
    std::any visitBarrierInstruction(CqasmParser::BarrierInstructionContext* context) override;
    std::any visitWaitInstruction(CqasmParser::WaitInstructionContext* context) override;
    std::any visitGateInstruction(CqasmParser::GateInstructionContext* context) override;
    std::any visitInvGate(CqasmParser::InvGateContext* context) override;
    std::any visitPowGate(CqasmParser::PowGateContext* context) override;
    std::any visitCtrlGate(CqasmParser::CtrlGateContext* context) override;
    std::any visitNamedGate(CqasmParser::NamedGateContext* context) override;
    std::any visitAsmDeclaration(CqasmParser::AsmDeclarationContext* context) override;
    std::any visitType(CqasmParser::TypeContext* context) override;
    std::any visitQubitType(CqasmParser::QubitTypeContext* context) override;
    std::any visitBitType(CqasmParser::BitTypeContext* context) override;
    std::any visitArraySizeDeclaration(CqasmParser::ArraySizeDeclarationContext* context) override;
    std::any visitExpressionList(CqasmParser::ExpressionListContext* context) override;
    std::any visitIndexList(CqasmParser::IndexListContext* context) override;
    std::any visitIndexItem(CqasmParser::IndexItemContext* context) override;
    std::any visitIndexRange(CqasmParser::IndexRangeContext* context) override;
    std::any visitParensExpression(CqasmParser::ParensExpressionContext* context) override;
    std::any visitUnaryPlusMinusExpression(CqasmParser::UnaryPlusMinusExpressionContext* context) override;
    std::any visitBitwiseNotExpression(CqasmParser::BitwiseNotExpressionContext* context) override;
    std::any visitLogicalNotExpression(CqasmParser::LogicalNotExpressionContext* context) override;
    std::any visitPowerExpression(CqasmParser::PowerExpressionContext* context) override;
    std::any visitProductExpression(CqasmParser::ProductExpressionContext* context) override;
    std::any visitAdditionExpression(CqasmParser::AdditionExpressionContext* context) override;
    std::any visitShiftExpression(CqasmParser::ShiftExpressionContext* context) override;
    std::any visitComparisonExpression(CqasmParser::ComparisonExpressionContext* context) override;
    std::any visitEqualityExpression(CqasmParser::EqualityExpressionContext* context) override;
    std::any visitBitwiseAndExpression(CqasmParser::BitwiseAndExpressionContext* context) override;
    std::any visitBitwiseXorExpression(CqasmParser::BitwiseXorExpressionContext* context) override;
    std::any visitBitwiseOrExpression(CqasmParser::BitwiseOrExpressionContext* context) override;
    std::any visitLogicalAndExpression(CqasmParser::LogicalAndExpressionContext* context) override;
    std::any visitLogicalXorExpression(CqasmParser::LogicalXorExpressionContext* context) override;
    std::any visitLogicalOrExpression(CqasmParser::LogicalOrExpressionContext* context) override;
    std::any visitTernaryConditionalExpression(CqasmParser::TernaryConditionalExpressionContext* context) override;
    std::any visitFunctionCall(CqasmParser::FunctionCallContext* context) override;
    std::any visitIndex(CqasmParser::IndexContext* context) override;
    std::any visitIdentifier(CqasmParser::IdentifierContext* context) override;
    std::any visitBooleanLiteral(CqasmParser::BooleanLiteralContext* context) override;
    std::any visitIntegerLiteral(CqasmParser::IntegerLiteralContext* context) override;
    std::any visitFloatLiteral(CqasmParser::FloatLiteralContext* context) override;

    explicit SyntacticAnalyzer(const std::optional<std::string>& file_name);
    void addErrorListener(AntlrCustomErrorListener* error_listener) override;
    void syntaxError(size_t line, size_t char_position_in_line, const std::string& text) const override;
    void setNodeAnnotation(const syntactic::One<syntactic::Node>& node, antlr4::Token* token) const override;
    void expandNodeAnnotation(const syntactic::One<syntactic::Node>& node, antlr4::Token* token) const override;
    void copyNodeAnnotation(
        const syntactic::One<syntactic::Node>& from, const syntactic::One<syntactic::Node>& to) const override;
};

}  // namespace cqasm::v3x::parser
