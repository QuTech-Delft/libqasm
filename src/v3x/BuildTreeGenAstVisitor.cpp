#include "cqasm-tree.hpp"
#include "v1x/cqasm-ast.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"

#include <algorithm>  // for_each
#include <antlr4-runtime.h>
#include <stdexcept>  // runtime_error
#include <string>  // stod, stoll


namespace cqasm::v3x::parser {

using namespace cqasm::v1x::ast;
using namespace cqasm::error;

std::int64_t BuildTreeGenAstVisitor::get_integer_literal_value(antlr4::tree::TerminalNode *node) {
    auto text = node->getText();
    std::int64_t ret{};
    try {
        ret = std::stoll(text);
    } catch (std::out_of_range&) {
        const auto &token = node->getSymbol();
        throw std::runtime_error{
            fmt::format("{}:{}:{}: value '{}' is out of the INTEGER_LITERAL range",
                file_name_, token->getLine(), token->getCharPositionInLine(), text
        )};
    }
    return ret;
}

double BuildTreeGenAstVisitor::get_float_literal_value(antlr4::tree::TerminalNode *node) {
    auto text = node->getText();
    double ret{};
    try {
        ret = std::stod(text);
    } catch (std::out_of_range&) {
        const auto &token = node->getSymbol();
        throw std::runtime_error{
            fmt::format("{}:{}:{}: value '{}' is out of the FLOATING_LITERAL range",
                file_name_, token->getLine(), token->getCharPositionInLine(), text
        )};
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    auto ret = cqasm::tree::make<Program>();
    ret->version = std::any_cast<One<Version>>(visitVersion(context->version()));
    if (auto qubits_ctx = context->qubits()) {
        ret->num_qubits = std::any_cast<Maybe<Expression>>(visitQubits(qubits_ctx));
    }
    ret->statements = cqasm::tree::make<StatementList>();
    const auto &statements = context->statement();
    std::for_each(statements.begin(), statements.end(), [this, &ret](auto &statement_ctx) {
        ret->statements->items.add(std::any_cast<One<Statement>>(statement_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = cqasm::tree::make<Version>();
    const auto& integer_literals = context->INTEGER_LITERAL();
    std::for_each(integer_literals.begin(), integer_literals.end(), [this, &ret](auto *integer_literal_node) {
        auto number = get_integer_literal_value(integer_literal_node);
        ret->items.push_back(number);
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitQubits(CqasmParser::QubitsContext *context) {
    return Maybe<Expression>{
        std::any_cast<One<Expression>>(context->expression()->accept(this)).get_ptr()
    };
}

std::any BuildTreeGenAstVisitor::visitMapping(CqasmParser::MappingContext *context) {
    auto ret = cqasm::tree::make<Mapping>();
    ret->alias = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->expr = std::any_cast<One<Expression>>(context->expression()->accept(this));
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitVariable(CqasmParser::VariableContext *context) {
    auto ret = cqasm::tree::make<Variables>();
    ret->names = Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER(0)->getText()) };
    ret->typ = cqasm::tree::make<Identifier>(context->IDENTIFIER(1)->getText());
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    auto ret = cqasm::tree::make<Bundle>();
    auto instruction = cqasm::tree::make<Instruction>();
    instruction->name = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    instruction->operands = std::any_cast<One<ExpressionList>>(visitExpressionList(context->expressionList()));
    ret->items.add(instruction);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext *context) {
    auto ret = cqasm::tree::make<ExpressionList>();
    const auto &expressions = context->expression();
    std::for_each(expressions.begin(), expressions.end(), [this, &ret](auto &expression_ctx) {
        ret->items.add(std::any_cast<One<Expression>>(expression_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) {
    auto value = get_integer_literal_value(context->INTEGER_LITERAL());
    auto ret = cqasm::tree::make<IntegerLiteral>(value);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloatLiteral(CqasmParser::FloatLiteralContext *context) {
    auto value = get_float_literal_value(context->FLOAT_LITERAL());
    auto ret = cqasm::tree::make<FloatLiteral>(value);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIdentifier(CqasmParser::IdentifierContext *context) {
    auto ret = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = cqasm::tree::make<Index>();
    ret->expr = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->indices = cqasm::tree::make<IndexList>();
    auto index_item = cqasm::tree::make<IndexItem>(std::any_cast<One<Expression>>(
        context->expression()->accept(this)));
    ret->indices->items.add(index_item);
    return One<Expression>{ ret };
}

}  // namespace cqasm::v3x::parser
