#include "libqasm/v3x/syntactic_analyzer.hpp"

#include <antlr4-runtime.h>

#include <algorithm>  // for_each
#include <cassert>  // assert
#include <cstdint>  // uint32_t
#include <regex>
#include <stdexcept>  // runtime_error
#include <string>  // stod, stoll

#include "libqasm/annotations.hpp"
#include "libqasm/tree.hpp"
#include "libqasm/v3x/antlr_custom_error_listener.hpp"
#include "libqasm/v3x/ast.hpp"
#include "libqasm/v3x/types.hpp"

namespace cqasm::v3x::parser {

using namespace cqasm::v3x::ast;
using namespace cqasm::error;

SyntacticAnalyzer::SyntacticAnalyzer(const std::optional<std::string>& file_name)
: file_name_{ file_name }
, error_listener_p_{ nullptr } {
    if (file_name_.has_value() && file_name_.value().empty()) {
        file_name_ = std::nullopt;
    }
}

void SyntacticAnalyzer::addErrorListener(AntlrCustomErrorListener* error_listener) {
    error_listener_p_ = error_listener;
}

void SyntacticAnalyzer::syntaxError(size_t line, size_t char_position_in_line, const std::string& text) const {
    assert(error_listener_p_);
    error_listener_p_->syntaxError(line, char_position_in_line, text);
}

/**
 * ANTLR provides a zero-based character position in line
 * We change it here to a one-based index, which is the more human-readable, and the common option in text editors
 */
void SyntacticAnalyzer::setNodeAnnotation(const ast::One<ast::Node>& node, antlr4::Token* token) const {
    auto token_size = token->getStopIndex() - token->getStartIndex() + 1;
    node->set_annotation(annotations::SourceLocation{ file_name_,
        { { static_cast<std::uint32_t>(token->getLine()),
              static_cast<std::uint32_t>(token->getCharPositionInLine() + 1) },
            { static_cast<std::uint32_t>(token->getLine()),
                static_cast<std::uint32_t>(token->getCharPositionInLine() + 1 + token_size) } } });
}

void SyntacticAnalyzer::expandNodeAnnotation(const One<Node>& node, antlr4::Token* token) const {
    auto token_size = token->getStopIndex() - token->getStartIndex() + 1;
    if (auto source_location = node->get_annotation_ptr<annotations::SourceLocation>()) {
        source_location->expand_to_include(
            annotations::SourceLocation::Index{ static_cast<std::uint32_t>(token->getLine()),
                static_cast<std::uint32_t>(token->getCharPositionInLine() + 1 + token_size) });
    }
}

void SyntacticAnalyzer::copyNodeAnnotation(const ast::One<ast::Node>& from, const ast::One<ast::Node>& to) const {
    to->copy_annotation<annotations::SourceLocation>(*from);
}

std::int64_t SyntacticAnalyzer::get_int_value(
    size_t line, size_t char_position_in_line, const std::string& text) const {
    try {
        return std::stoll(text);
    } catch (std::out_of_range&) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the INTEGER_LITERAL range", text));
    }
    return {};
}

double SyntacticAnalyzer::get_float_value(size_t line, size_t char_position_in_line, const std::string& text) const {
    try {
        return std::stod(text);
    } catch (std::out_of_range&) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the FLOAT_LITERAL range", text));
    }
    return {};
}

bool SyntacticAnalyzer::get_bool_value(antlr4::tree::TerminalNode* node) const {
    assert(node->getSymbol()->getType() == CqasmParser::BOOLEAN_LITERAL);
    return node->getText() == "true";
}

std::int64_t SyntacticAnalyzer::get_int_value(antlr4::tree::TerminalNode* node) const {
    const auto token = node->getSymbol();
    assert(token->getType() == CqasmParser::INTEGER_LITERAL);
    return get_int_value(token->getLine(), token->getCharPositionInLine(), node->getText());
}

double SyntacticAnalyzer::get_float_value(antlr4::tree::TerminalNode* node) const {
    const auto token = node->getSymbol();
    const auto& text = node->getText();
    assert(token->getType() == CqasmParser::FLOAT_LITERAL);
    return get_float_value(token->getLine(), token->getCharPositionInLine(), text);
}

std::any SyntacticAnalyzer::visitProgram(CqasmParser::ProgramContext* context) {
    auto ret = tree::make<Program>();
    ret->version = std::any_cast<One<Version>>(visitVersionSection(context->versionSection()));
    ret->block = context->globalBlockSection()
        ? std::any_cast<One<GlobalBlock>>(visitGlobalBlockSection(context->globalBlockSection()))
        : tree::make<GlobalBlock>();
    return ret;
}

std::any SyntacticAnalyzer::visitVersionSection(CqasmParser::VersionSectionContext* context) {
    return visitVersion(context->version());
}

std::any SyntacticAnalyzer::visitGlobalBlockSection(CqasmParser::GlobalBlockSectionContext* context) {
    auto ret = tree::make<GlobalBlock>();
    const auto& statements = context->globalBlockStatement();
    std::for_each(statements.begin(), statements.end(), [this, &ret](const auto& statement_ctx) {
        ret->statements.add(std::any_cast<One<Statement>>(statement_ctx->accept(this)));
    });
    return ret;
}

std::any SyntacticAnalyzer::visitEofSection(CqasmParser::EofSectionContext*) {
    return {};
}

std::any SyntacticAnalyzer::visitStatementSeparator(CqasmParser::StatementSeparatorContext*) {
    return {};
}

std::any SyntacticAnalyzer::visitVersion(CqasmParser::VersionContext* context) {
    auto ret = tree::make<Version>();
    const auto token = context->VERSION_NUMBER()->getSymbol();
    const auto& text = context->VERSION_NUMBER()->getText();
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

std::any SyntacticAnalyzer::visitGlobalBlockStatement(CqasmParser::GlobalBlockStatementContext* context) {
    if (auto variable_definition_ctx = context->variableDefinition(); variable_definition_ctx) {
        return variable_definition_ctx->accept(this);
    } else if (auto instruction_ctx = context->instruction(); instruction_ctx) {
        return instruction_ctx->accept(this);
    }
    throw error::AnalysisError{ "unknown global block statement type" };
}

std::any SyntacticAnalyzer::visitVariableDefinition(CqasmParser::VariableDefinitionContext* context) {
    return One<Statement>{ visitVariable(context) };
}

std::any SyntacticAnalyzer::visitInstruction(CqasmParser::InstructionContext* context) {
    if (auto gate_instruction_ctx = context->gateInstruction(); gate_instruction_ctx) {
        return gate_instruction_ctx->accept(this);
    } else if (auto non_gate_instruction_ctx = context->nonGateInstruction(); non_gate_instruction_ctx) {
        return non_gate_instruction_ctx->accept(this);
    }
    throw error::AnalysisError{ "unknown instruction type" };
}

std::any SyntacticAnalyzer::visitGateInstruction(CqasmParser::GateInstructionContext* context) {
    auto ret = tree::make<GateInstruction>();
    ret->gate = std::any_cast<One<Gate>>(context->gate()->accept(this));
    ret->operands = std::any_cast<One<ExpressionList>>(visitExpressionList(context->expressionList()));
    // Set the instruction annotation to the annotation of its gate
    copyNodeAnnotation(ret->gate, ret);
    return One<Statement>{ ret };
}

std::any SyntacticAnalyzer::visitInvGate(CqasmParser::InvGateContext* context) {
    auto ret = tree::make<Gate>();
    ret->name = tree::make<Identifier>(context->INV()->getText());
    ret->gate = std::any_cast<tree::One<Gate>>(context->gate()->accept(this)).get_ptr();
    setNodeAnnotation(ret, context->INV()->getSymbol());
    return ret;
}

std::any SyntacticAnalyzer::visitPowGate(CqasmParser::PowGateContext* context) {
    auto ret = tree::make<Gate>();
    ret->name = tree::make<Identifier>(context->POW()->getText());
    ret->gate = std::any_cast<tree::One<Gate>>(context->gate()->accept(this)).get_ptr();
    ret->parameter = std::any_cast<One<Expression>>(context->expression()->accept(this)).get_ptr();
    setNodeAnnotation(ret, context->POW()->getSymbol());
    return ret;
}

std::any SyntacticAnalyzer::visitCtrlGate(CqasmParser::CtrlGateContext* context) {
    auto ret = tree::make<Gate>();
    ret->name = tree::make<Identifier>(context->CTRL()->getText());
    ret->gate = std::any_cast<tree::One<Gate>>(context->gate()->accept(this)).get_ptr();
    setNodeAnnotation(ret, context->CTRL()->getSymbol());
    return ret;
}

std::any SyntacticAnalyzer::visitNamedGate(CqasmParser::NamedGateContext* context) {
    auto ret = tree::make<Gate>();
    ret->name = tree::make<Identifier>(context->IDENTIFIER()->getText());
    if (context->expression()) {
        ret->parameter = std::any_cast<One<Expression>>(context->expression()->accept(this)).get_ptr();
    }
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return ret;
}

std::any SyntacticAnalyzer::visitMeasureInstruction(CqasmParser::MeasureInstructionContext* context) {
    auto ret = tree::make<NonGateInstruction>();
    ret->name = tree::make<Keyword>(context->MEASURE()->getText());
    ret->operands = tree::make<ExpressionList>();
    ret->operands->items.add(std::any_cast<One<Expression>>(context->expression(0)->accept(this)));
    ret->operands->items.add(std::any_cast<One<Expression>>(context->expression(1)->accept(this)));
    setNodeAnnotation(ret, context->MEASURE()->getSymbol());
    return One<Statement>{ ret };
}

std::any SyntacticAnalyzer::visitResetInstruction(CqasmParser::ResetInstructionContext* context) {
    auto ret = tree::make<NonGateInstruction>();
    ret->name = tree::make<Keyword>(context->RESET()->getText());
    ret->operands = tree::make<ExpressionList>();
    if (context->expression()) {
        ret->operands->items.add(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    }
    setNodeAnnotation(ret, context->RESET()->getSymbol());
    return One<Statement>{ ret };
}

std::any SyntacticAnalyzer::visitType(CqasmParser::TypeContext* context) {
    return context->quantumType()->accept(this);
}

std::any SyntacticAnalyzer::visitQubitType(CqasmParser::QubitTypeContext* context) {
    auto ret =
        tree::make<Type>(tree::make<Keyword>(types::qubit_type_name), get_array_size(context->arraySizeDeclaration()));
    setNodeAnnotation(ret, context->QUBIT_TYPE()->getSymbol());
    if (context->arraySizeDeclaration()) {
        expandNodeAnnotation(ret, context->arraySizeDeclaration()->CLOSE_BRACKET()->getSymbol());
    }
    return ret;
}

std::any SyntacticAnalyzer::visitBitType(CqasmParser::BitTypeContext* context) {
    auto ret =
        tree::make<Type>(tree::make<Keyword>(types::bit_type_name), get_array_size(context->arraySizeDeclaration()));
    setNodeAnnotation(ret, context->BIT_TYPE()->getSymbol());
    if (context->arraySizeDeclaration()) {
        expandNodeAnnotation(ret, context->arraySizeDeclaration()->CLOSE_BRACKET()->getSymbol());
    }
    return ret;
}

Maybe<IntegerLiteral> SyntacticAnalyzer::get_array_size(CqasmParser::ArraySizeDeclarationContext* context) {
    return (context) ? Maybe<IntegerLiteral>{ std::any_cast<One<IntegerLiteral>>(context->accept(this)).get_ptr() }
                     : Maybe<IntegerLiteral>{};
}

std::any SyntacticAnalyzer::visitArraySizeDeclaration(CqasmParser::ArraySizeDeclarationContext* context) {
    return tree::make<IntegerLiteral>(get_int_value(context->INTEGER_LITERAL()));
}

std::any SyntacticAnalyzer::visitExpressionList(CqasmParser::ExpressionListContext* context) {
    auto ret = tree::make<ExpressionList>();
    const auto& expressions = context->expression();
    std::for_each(expressions.begin(), expressions.end(), [this, &ret](auto& expression_ctx) {
        ret->items.add(std::any_cast<One<Expression>>(expression_ctx->accept(this)));
    });
    return ret;
}

std::any SyntacticAnalyzer::visitIndexList(CqasmParser::IndexListContext* context) {
    auto ret = tree::make<IndexList>();
    const auto& index_entries = context->indexEntry();
    std::for_each(index_entries.begin(), index_entries.end(), [this, &ret](auto& index_entry_ctx) {
        auto index_entry = std::any_cast<One<IndexEntry>>(index_entry_ctx->accept(this));
        ret->items.add(index_entry);
    });
    return ret;
}

std::any SyntacticAnalyzer::visitIndexItem(CqasmParser::IndexItemContext* context) {
    return One<IndexEntry>{ tree::make<IndexItem>(
        std::any_cast<One<Expression>>(context->expression()->accept(this))) };
}

std::any SyntacticAnalyzer::visitIndexRange(CqasmParser::IndexRangeContext* context) {
    return One<IndexEntry>{ tree::make<IndexRange>(std::any_cast<One<Expression>>(context->expression(0)->accept(this)),
        std::any_cast<One<Expression>>(context->expression(1)->accept(this))) };
}

std::any SyntacticAnalyzer::visitParensExpression(CqasmParser::ParensExpressionContext* context) {
    return context->expression()->accept(this);
}

std::any SyntacticAnalyzer::visitUnaryPlusMinusExpression(CqasmParser::UnaryPlusMinusExpressionContext* context) {
    if (context->PLUS()) {
        return std::any_cast<One<Expression>>(context->expression()->accept(this));
    }
    auto ret = tree::make<UnaryMinusExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->MINUS()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitBitwiseNotExpression(CqasmParser::BitwiseNotExpressionContext* context) {
    auto ret = tree::make<BitwiseNotExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->BITWISE_NOT_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitLogicalNotExpression(CqasmParser::LogicalNotExpressionContext* context) {
    auto ret = tree::make<LogicalNotExpression>(std::any_cast<One<Expression>>(context->expression()->accept(this)));
    setNodeAnnotation(ret, context->LOGICAL_NOT_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitPowerExpression(CqasmParser::PowerExpressionContext* context) {
    return visitBinaryExpression<PowerExpression>(context, context->POWER_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitProductExpression(CqasmParser::ProductExpressionContext* context) {
    if (context->PRODUCT_OP()) {
        return visitBinaryExpression<ProductExpression>(context, context->PRODUCT_OP()->getSymbol());
    } else if (context->DIVISION_OP()) {
        return visitBinaryExpression<DivisionExpression>(context, context->DIVISION_OP()->getSymbol());
    }
    return visitBinaryExpression<ModuloExpression>(context, context->MODULO_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitAdditionExpression(CqasmParser::AdditionExpressionContext* context) {
    if (context->PLUS()) {
        return visitBinaryExpression<AdditionExpression>(context, context->PLUS()->getSymbol());
    }
    return visitBinaryExpression<SubtractionExpression>(context, context->MINUS()->getSymbol());
}

std::any SyntacticAnalyzer::visitShiftExpression(CqasmParser::ShiftExpressionContext* context) {
    if (context->SHL_OP()) {
        return visitBinaryExpression<ShiftLeftExpression>(context, context->SHL_OP()->getSymbol());
    }
    return visitBinaryExpression<ShiftRightExpression>(context, context->SHR_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitComparisonExpression(CqasmParser::ComparisonExpressionContext* context) {
    if (context->CMP_GT_OP()) {
        return visitBinaryExpression<CmpGtExpression>(context, context->CMP_GT_OP()->getSymbol());
    } else if (context->CMP_LT_OP()) {
        return visitBinaryExpression<CmpLtExpression>(context, context->CMP_LT_OP()->getSymbol());
    } else if (context->CMP_GE_OP()) {
        return visitBinaryExpression<CmpGeExpression>(context, context->CMP_GE_OP()->getSymbol());
    }
    return visitBinaryExpression<CmpLeExpression>(context, context->CMP_LE_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitEqualityExpression(CqasmParser::EqualityExpressionContext* context) {
    if (context->CMP_EQ_OP()) {
        return visitBinaryExpression<CmpEqExpression>(context, context->CMP_EQ_OP()->getSymbol());
    }
    return visitBinaryExpression<CmpNeExpression>(context, context->CMP_NE_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitBitwiseAndExpression(CqasmParser::BitwiseAndExpressionContext* context) {
    return visitBinaryExpression<BitwiseAndExpression>(context, context->BITWISE_AND_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitBitwiseXorExpression(CqasmParser::BitwiseXorExpressionContext* context) {
    return visitBinaryExpression<BitwiseXorExpression>(context, context->BITWISE_XOR_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitBitwiseOrExpression(CqasmParser::BitwiseOrExpressionContext* context) {
    return visitBinaryExpression<BitwiseOrExpression>(context, context->BITWISE_OR_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitLogicalAndExpression(CqasmParser::LogicalAndExpressionContext* context) {
    return visitBinaryExpression<LogicalAndExpression>(context, context->LOGICAL_AND_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitLogicalXorExpression(CqasmParser::LogicalXorExpressionContext* context) {
    return visitBinaryExpression<LogicalXorExpression>(context, context->LOGICAL_XOR_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitLogicalOrExpression(CqasmParser::LogicalOrExpressionContext* context) {
    return visitBinaryExpression<LogicalOrExpression>(context, context->LOGICAL_OR_OP()->getSymbol());
}

std::any SyntacticAnalyzer::visitTernaryConditionalExpression(
    CqasmParser::TernaryConditionalExpressionContext* context) {
    auto ret =
        tree::make<TernaryConditionalExpression>(std::any_cast<One<Expression>>(context->expression(0)->accept(this)),
            std::any_cast<One<Expression>>(context->expression(1)->accept(this)),
            std::any_cast<One<Expression>>(context->expression(2)->accept(this)));
    setNodeAnnotation(ret, context->TERNARY_CONDITIONAL_OP()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitFunctionCall(CqasmParser::FunctionCallContext* context) {
    auto ret = tree::make<FunctionCall>();
    ret->name = tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->arguments = context->expressionList()
        ? Maybe<ExpressionList>{ std::any_cast<One<ExpressionList>>(context->expressionList()->accept(this)).get_ptr() }
        : Maybe<ExpressionList>{};
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitIndex(CqasmParser::IndexContext* context) {
    auto ret = tree::make<Index>();
    ret->expr = tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->indices = std::any_cast<One<IndexList>>(visitIndexList(context->indexList()));
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitIdentifier(CqasmParser::IdentifierContext* context) {
    auto ret = tree::make<Identifier>(context->IDENTIFIER()->getText());
    setNodeAnnotation(ret, context->IDENTIFIER()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitBooleanLiteral(CqasmParser::BooleanLiteralContext* context) {
    auto value = get_bool_value(context->BOOLEAN_LITERAL());
    auto ret = tree::make<BooleanLiteral>(value);
    setNodeAnnotation(ret, context->BOOLEAN_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitIntegerLiteral(CqasmParser::IntegerLiteralContext* context) {
    auto value = get_int_value(context->INTEGER_LITERAL());
    auto ret = tree::make<IntegerLiteral>(value);
    setNodeAnnotation(ret, context->INTEGER_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

std::any SyntacticAnalyzer::visitFloatLiteral(CqasmParser::FloatLiteralContext* context) {
    auto value = get_float_value(context->FLOAT_LITERAL());
    auto ret = tree::make<FloatLiteral>(value);
    setNodeAnnotation(ret, context->FLOAT_LITERAL()->getSymbol());
    return One<Expression>{ ret };
}

}  // namespace cqasm::v3x::parser
