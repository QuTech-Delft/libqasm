#include "v3x/BuildTreeGenAstVisitor.hpp"

#include <antlr4-runtime.h>

#include <algorithm>  // for_each
#include <cassert>  // assert
#include <cstdint>  // uint32_t
#include <regex>
#include <stdexcept>  // runtime_error
#include <string>  // stod, stoll

#include "cqasm-annotations.hpp"
#include "cqasm-tree.hpp"
#include "v3x/CustomErrorListener.hpp"
#include "v3x/cqasm-ast.hpp"
#include "v3x/cqasm-types.hpp"

namespace cqasm::v3x::parser {

using namespace cqasm::v3x::ast;
using namespace cqasm::error;

BuildTreeGenAstVisitor::BuildTreeGenAstVisitor(const std::optional<std::string> &file_name)
: file_name_{ file_name }
, error_listener_p_{ nullptr } {
    if (file_name_.has_value() && file_name_.value().empty()) {
        file_name_ = std::nullopt;
    }
}

void BuildTreeGenAstVisitor::addErrorListener(CustomErrorListener *errorListener) {
    error_listener_p_ = errorListener;
}

void BuildTreeGenAstVisitor::syntaxError(size_t line, size_t char_position_in_line, const std::string &text) const {
    assert(error_listener_p_);
    error_listener_p_->syntaxError(line, char_position_in_line, text);
}

/**
 * ANTLR provides a zero-based character position in line
 * We change it here to a one-based index, which is the more human-readable, and the common option in text editors
 */
void BuildTreeGenAstVisitor::setNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const {
    auto token_size = token->getStopIndex() - token->getStartIndex() + 1;
    node->set_annotation(annotations::SourceLocation{ file_name_,
        { { static_cast<std::uint32_t>(token->getLine()),
              static_cast<std::uint32_t>(token->getCharPositionInLine() + 1) },
            { static_cast<std::uint32_t>(token->getLine()),
                static_cast<std::uint32_t>(token->getCharPositionInLine() + 1 + token_size) } } });
}

void BuildTreeGenAstVisitor::expandNodeAnnotation(const One<Node> &node, antlr4::Token *token) const {
    auto token_size = token->getStopIndex() - token->getStartIndex() + 1;
    if (auto source_location = node->get_annotation_ptr<annotations::SourceLocation>()) {
        source_location->expand_to_include(
            annotations::SourceLocation::Index{ static_cast<std::uint32_t>(token->getLine()),
                static_cast<std::uint32_t>(token->getCharPositionInLine() + 1 + token_size) });
    }
}

std::int64_t BuildTreeGenAstVisitor::get_int_value(
    size_t line, size_t char_position_in_line, const std::string &text) const {
    try {
        return std::stoll(text);
    } catch (std::out_of_range &) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the INTEGER_LITERAL range", text));
    }
    return {};
}

double BuildTreeGenAstVisitor::get_float_value(
    size_t line, size_t char_position_in_line, const std::string &text) const {
    try {
        return std::stod(text);
    } catch (std::out_of_range &) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the FLOAT_LITERAL range", text));
    }
    return {};
}

bool BuildTreeGenAstVisitor::get_bool_value(antlr4::tree::TerminalNode *node) const {
    assert(node->getSymbol()->getType() == CqasmParser::BOOLEAN_LITERAL);
    return node->getText() == "true";
}

std::int64_t BuildTreeGenAstVisitor::get_int_value(antlr4::tree::TerminalNode *node) const {
    const auto token = node->getSymbol();
    assert(token->getType() == CqasmParser::INTEGER_LITERAL);
    return get_int_value(token->getLine(), token->getCharPositionInLine(), node->getText());
}

double BuildTreeGenAstVisitor::get_float_value(antlr4::tree::TerminalNode *node) const {
    const auto token = node->getSymbol();
    const auto &text = node->getText();
    assert(token->getType() == CqasmParser::FLOAT_LITERAL);
    return get_float_value(token->getLine(), token->getCharPositionInLine(), text);
}

std::any BuildTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    auto ret = tree::make<Program>();
    ret->version = std::any_cast<One<Version>>(visitVersionSection(context->versionSection()));
    ret->block = context->globalBlockSection()
        ? std::any_cast<One<GlobalBlock>>(visitGlobalBlockSection(context->globalBlockSection()))
        : tree::make<GlobalBlock>();
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersionSection(CqasmParser::VersionSectionContext *context) {
    return visitVersion(context->version());
}

std::any BuildTreeGenAstVisitor::visitGlobalBlockSection(CqasmParser::GlobalBlockSectionContext *context) {
    auto ret = tree::make<GlobalBlock>();
    const auto &statements = context->globalBlockStatement();
    std::for_each(statements.begin(), statements.end(), [this, &ret](const auto &statement_ctx) {
        ret->statements.add(std::any_cast<One<Statement>>(statement_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitEofSection(CqasmParser::EofSectionContext *) {
    return {};
}

std::any BuildTreeGenAstVisitor::visitStatementSeparator(CqasmParser::StatementSeparatorContext *) {
    return {};
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = tree::make<Version>();
    const auto token = context->VERSION_NUMBER()->getSymbol();
    const auto &text = context->VERSION_NUMBER()->getText();
    const std::regex pattern{ "([0-9]+)(?:\\.([0-9]+))?" };
    std::smatch matches{};
    std::regex_match(text, matches, pattern);
    ret->items.push_back(get_int_value(token->getLine(), token->getCharPositionInLine(), matches[1]));
    if (matches[2].matched) {
        ret->items.push_back(
            get_int_value(token->getLine(), token->getCharPositionInLine() + matches.position(2), matches[2]));
    }
    setNodeAnnotation(ret, token);
    return ret;
}

std::any BuildTreeGenAstVisitor::visitGlobalBlockStatement(CqasmParser::GlobalBlockStatementContext *context) {
    if (auto variable_definition_ctx = context->variableDefinition(); variable_definition_ctx) {
        return variable_definition_ctx->accept(this);
    } else if (auto instruction_ctx = context->instruction(); instruction_ctx) {
        return instruction_ctx->accept(this);
    }
    throw error::AnalysisError{ "unknown global block statement type" };
}

std::any BuildTreeGenAstVisitor::visitVariableDefinition(CqasmParser::VariableDefinitionContext *context) {
    return One<Statement>{ visitVariable(context) };
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    if (auto gate_instruction_ctx = context->gateInstruction(); gate_instruction_ctx) {
        return gate_instruction_ctx->accept(this);
    } else if (auto measure_instruction_ctx = context->measureInstruction(); measure_instruction_ctx) {
        return measure_instruction_ctx->accept(this);
    } else if (auto reset_instruction_ctx = context->resetInstruction(); reset_instruction_ctx) {
        return reset_instruction_ctx->accept(this);
    }
    throw error::AnalysisError{ "unknown instruction type" };
}

std::any BuildTreeGenAstVisitor::visitGateInstruction(CqasmParser::GateInstructionContext *context) {
    if (auto gate_modifier_ctx = context->gateModifier(); gate_modifier_ctx) {
        return gate_modifier_ctx->accept(this);
    } else if (auto gate_ctx = context->gate(); gate_ctx) {
        return gate_ctx->accept(this);
    }
    throw error::AnalysisError{ "unknown gate instruction type" };
}

std::any BuildTreeGenAstVisitor::visitInvGate(CqasmParser::InvGateContext *context) {
    auto ret = tree::make<InvGateModifier>();
    ret->name = tree::make<Keyword>(context->INV()->getText());
    ret->gate = std::any_cast<One<GateInstruction>>(context->gateInstruction()->accept(this));
    setNodeAnnotation(ret, context->INV()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitPowGate(CqasmParser::PowGateContext *context) {
    auto ret = tree::make<PowGateModifier>();
    ret->name = tree::make<Keyword>(context->POW()->getText());
    ret->gate = std::any_cast<One<GateInstruction>>(context->gateInstruction()->accept(this));
    ret->exponent = std::any_cast<One<Expression>>(context->expression()->accept(this));
    setNodeAnnotation(ret, context->POW()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitCtrlGate(CqasmParser::CtrlGateContext *context) {
    auto ret = tree::make<CtrlGateModifier>();
    ret->name = tree::make<Keyword>(context->CTRL()->getText());
    ret->gate = std::any_cast<One<GateInstruction>>(context->gateInstruction()->accept(this));
    ret->ctrl_qubit = std::any_cast<One<Expression>>(context->expression()->accept(this));
    setNodeAnnotation(ret, context->CTRL()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitGate(CqasmParser::GateContext *context) {
    auto ret = tree::make<Gate>();
    ret->name = tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->operands = std::any_cast<One<ExpressionList>>(visitExpressionList(context->expressionList()));
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitMeasureInstruction(CqasmParser::MeasureInstructionContext *context) {
    auto ret = tree::make<MeasureInstruction>();
    ret->name = tree::make<Keyword>(context->MEASURE()->getText());
    ret->lhs = std::any_cast<One<Expression>>(context->expression(0)->accept(this));
    ret->rhs = std::any_cast<One<Expression>>(context->expression(1)->accept(this));
    setNodeAnnotation(ret, context->MEASURE()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitResetInstruction(CqasmParser::ResetInstructionContext *context) {
    auto ret = tree::make<ResetInstruction>();
    ret->name = tree::make<Keyword>(context->RESET()->getText());
    ret->operand = context->expression()
        ? Maybe<Expression>{ std::any_cast<One<Expression>>(context->expression()->accept(this)).get_ptr() }
        : Maybe<Expression>{};
    setNodeAnnotation(ret, context->RESET()->getSymbol());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitType(CqasmParser::TypeContext *context) {
    return context->quantumType()->accept(this);
}

std::any BuildTreeGenAstVisitor::visitQubitType(CqasmParser::QubitTypeContext *context) {
    auto ret =
        tree::make<Type>(tree::make<Keyword>(types::qubit_type_name), getArraySize(context->arraySizeDeclaration()));
    setNodeAnnotation(ret, context->QUBIT_TYPE()->getSymbol());
    if (context->arraySizeDeclaration()) {
        expandNodeAnnotation(ret, context->arraySizeDeclaration()->CLOSE_BRACKET()->getSymbol());
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitBitType(CqasmParser::BitTypeContext *context) {
    auto ret =
        tree::make<Type>(tree::make<Keyword>(types::bit_type_name), getArraySize(context->arraySizeDeclaration()));
    setNodeAnnotation(ret, context->BIT_TYPE()->getSymbol());
    if (context->arraySizeDeclaration()) {
        expandNodeAnnotation(ret, context->arraySizeDeclaration()->CLOSE_BRACKET()->getSymbol());
    }
    return ret;
}

Maybe<IntegerLiteral> BuildTreeGenAstVisitor::getArraySize(CqasmParser::ArraySizeDeclarationContext *context) {
    return (context)
        ? Maybe<IntegerLiteral>{ std::any_cast<One<IntegerLiteral>>(context->accept(this)).get_ptr() }
        : Maybe<IntegerLiteral>{};
}

std::any BuildTreeGenAstVisitor::visitArraySizeDeclaration(CqasmParser::ArraySizeDeclarationContext *context) {
    return tree::make<IntegerLiteral>(get_int_value(context->INTEGER_LITERAL()));
}

std::any BuildTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext *context) {
    auto ret = tree::make<ExpressionList>();
    const auto &expressions = context->expression();
    std::for_each(expressions.begin(), expressions.end(), [this, &ret](auto &expression_ctx) {
        ret->items.add(std::any_cast<One<Expression>>(expression_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitIndexList(CqasmParser::IndexListContext *context) {
    auto ret = tree::make<IndexList>();
    const auto &index_entries = context->indexEntry();
    std::for_each(index_entries.begin(), index_entries.end(), [this, &ret](auto &index_entry_ctx) {
        auto index_entry = std::any_cast<One<IndexEntry>>(index_entry_ctx->accept(this));
        ret->items.add(index_entry);
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitIndexItem(CqasmParser::IndexItemContext *context) {
    return One<IndexEntry>{ tree::make<IndexItem>(
        std::any_cast<One<Expression>>(context->expression()->accept(this))) };
}

std::any BuildTreeGenAstVisitor::visitIndexRange(CqasmParser::IndexRangeContext *context) {
    return One<IndexEntry>{ tree::make<IndexRange>(std::any_cast<One<Expression>>(context->expression(0)->accept(this)),
        std::any_cast<One<Expression>>(context->expression(1)->accept(this))) };
}

std::any BuildTreeGenAstVisitor::visitParensExpression(CqasmParser::ParensExpressionContext *context) {
    return context->expression()->accept(this);
}

std::any BuildTreeGenAstVisitor::visitUnaryPlusMinusExpression(CqasmParser::UnaryPlusMinusExpressionContext *context) {
    if (context->PLUS()) {
        return std::any_cast<One<Expression>>(context->expression()->accept(this));
    }
    auto ret = tree::make<UnaryMinusExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->MINUS()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitBitwiseNotExpression(CqasmParser::BitwiseNotExpressionContext *context) {
    auto ret = tree::make<BitwiseNotExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->BITWISE_NOT_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitLogicalNotExpression(CqasmParser::LogicalNotExpressionContext *context) {
    auto ret = tree::make<LogicalNotExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->LOGICAL_NOT_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitPowerExpression(CqasmParser::PowerExpressionContext *context) {
    return visitBinaryExpression<PowerExpression>(context, context->POWER_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitProductExpression(CqasmParser::ProductExpressionContext *context) {
    if (context->PRODUCT_OP()) {
        return visitBinaryExpression<ProductExpression>(context, context->PRODUCT_OP()->getSymbol());
    } else if (context->DIVISION_OP()) {
        return visitBinaryExpression<DivisionExpression>(context, context->DIVISION_OP()->getSymbol());
    }
    return visitBinaryExpression<ModuloExpression>(context, context->MODULO_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitAdditionExpression(CqasmParser::AdditionExpressionContext *context) {
    if (context->PLUS()) {
        return visitBinaryExpression<AdditionExpression>(context, context->PLUS()->getSymbol());
    }
    return visitBinaryExpression<SubtractionExpression>(context, context->MINUS()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitShiftExpression(CqasmParser::ShiftExpressionContext *context) {
    if (context->SHL_OP()) {
        return visitBinaryExpression<ShiftLeftExpression>(context, context->SHL_OP()->getSymbol());
    }
    return visitBinaryExpression<ShiftRightExpression>(context, context->SHR_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitComparisonExpression(CqasmParser::ComparisonExpressionContext *context) {
    if (context->CMP_GT_OP()) {
        return visitBinaryExpression<CmpGtExpression>(context, context->CMP_GT_OP()->getSymbol());
    } else if (context->CMP_LT_OP()) {
        return visitBinaryExpression<CmpLtExpression>(context, context->CMP_LT_OP()->getSymbol());
    } else if (context->CMP_GE_OP()) {
        return visitBinaryExpression<CmpGeExpression>(context, context->CMP_GE_OP()->getSymbol());
    }
    return visitBinaryExpression<CmpLeExpression>(context, context->CMP_LE_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitEqualityExpression(CqasmParser::EqualityExpressionContext *context) {
    if (context->CMP_EQ_OP()) {
        return visitBinaryExpression<CmpEqExpression>(context, context->CMP_EQ_OP()->getSymbol());
    }
    return visitBinaryExpression<CmpNeExpression>(context, context->CMP_NE_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitBitwiseAndExpression(CqasmParser::BitwiseAndExpressionContext *context) {
    return visitBinaryExpression<BitwiseAndExpression>(context, context->BITWISE_AND_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitBitwiseXorExpression(CqasmParser::BitwiseXorExpressionContext *context) {
    return visitBinaryExpression<BitwiseXorExpression>(context, context->BITWISE_XOR_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitBitwiseOrExpression(CqasmParser::BitwiseOrExpressionContext *context) {
    return visitBinaryExpression<BitwiseOrExpression>(context, context->BITWISE_OR_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitLogicalAndExpression(CqasmParser::LogicalAndExpressionContext *context) {
    return visitBinaryExpression<LogicalAndExpression>(context, context->LOGICAL_AND_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitLogicalXorExpression(CqasmParser::LogicalXorExpressionContext *context) {
    return visitBinaryExpression<LogicalXorExpression>(context, context->LOGICAL_XOR_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitLogicalOrExpression(CqasmParser::LogicalOrExpressionContext *context) {
    return visitBinaryExpression<LogicalOrExpression>(context, context->LOGICAL_OR_OP()->getSymbol());
}

std::any BuildTreeGenAstVisitor::visitTernaryConditionalExpression(
    CqasmParser::TernaryConditionalExpressionContext *context) {
    auto ret =
        tree::make<TernaryConditionalExpression>(std::any_cast<One<Expression>>(context->expression(0)->accept(this)),
            std::any_cast<One<Expression>>(context->expression(1)->accept(this)),
            std::any_cast<One<Expression>>(context->expression(2)->accept(this)));
    setNodeAnnotation(ret, context->TERNARY_CONDITIONAL_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFunctionCall(CqasmParser::FunctionCallContext *context) {
    auto ret = tree::make<FunctionCall>();
    ret->name = tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->arguments = context->expressionList()
        ? Maybe<ExpressionList>{ std::any_cast<One<ExpressionList>>(context->expressionList()->accept(this)).get_ptr() }
        : Maybe<ExpressionList>{};
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = tree::make<Index>();
    ret->expr = tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->indices = std::any_cast<One<IndexList>>(visitIndexList(context->indexList()));
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIdentifier(CqasmParser::IdentifierContext *context) {
    auto ret = tree::make<Identifier>(context->IDENTIFIER()->getText());
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitBooleanLiteral(CqasmParser::BooleanLiteralContext *context) {
    auto value = get_bool_value(context->BOOLEAN_LITERAL());
    auto ret = tree::make<BooleanLiteral>(value);
    setNodeAnnotation(ret, context->BOOLEAN_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) {
    auto value = get_int_value(context->INTEGER_LITERAL());
    auto ret = tree::make<IntegerLiteral>(value);
    setNodeAnnotation(ret, context->INTEGER_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloatLiteral(CqasmParser::FloatLiteralContext *context) {
    auto value = get_float_value(context->FLOAT_LITERAL());
    auto ret = tree::make<FloatLiteral>(value);
    setNodeAnnotation(ret, context->FLOAT_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

}  // namespace cqasm::v3x::parser
