#include "cqasm-tree.hpp"
#include "v3x/cqasm-ast.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"

#include <algorithm>  // for_each
#include <antlr4-runtime.h>
#include <cassert> // assert
#include <regex>
#include <stdexcept>  // runtime_error
#include <string>  // stod, stoll


namespace cqasm::v3x::parser {

using namespace cqasm::v3x::ast;
using namespace cqasm::error;

std::int64_t BuildTreeGenAstVisitor::get_int_value(size_t line, size_t char_position_in_line, const std::string &text) {
    try {
        return std::stoll(text);
    } catch (std::out_of_range&) {
        throw std::runtime_error{
            fmt::format("{}:{}:{}: value '{}' is out of the INT range",
                file_name_, line, char_position_in_line, text
        )};
    }
}

std::int64_t BuildTreeGenAstVisitor::get_int_value(antlr4::tree::TerminalNode *node) {
    const auto &token = node->getSymbol();
    assert(token->getType() == CqasmParser::INT);
    return get_int_value(token->getLine(), token->getCharPositionInLine(), node->getText());
}

double BuildTreeGenAstVisitor::get_float_value(size_t line, size_t char_position_in_line, const std::string &text) {
    try {
        return std::stod(text);
    } catch (std::out_of_range&) {
        throw std::runtime_error{
            fmt::format("{}:{}:{}: value '{}' is out of the FLOAT range",
                file_name_, line, char_position_in_line, text
            )};
    }
}

double BuildTreeGenAstVisitor::get_float_value(antlr4::tree::TerminalNode *node) {
    const auto &token = node->getSymbol();
    const auto &text = node->getText();
    assert(token->getType() == CqasmParser::FLOAT);
    return get_float_value(token->getLine(), token->getCharPositionInLine(), node->getText());
}

std::any BuildTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    auto ret = cqasm::tree::make<Program>();
    ret->version = std::any_cast<One<Version>>(visitVersion(context->version()));
    /*
    if (auto qubit_definition_ctx = context->qubitDefinition()) {
        ret->num_qubits = std::any_cast<Maybe<Expression>>(visitQubitDefinition(qubit_definition_ctx));
    }
    */
    ret->statements = cqasm::tree::make<StatementList>();
    const auto &statements = context->statement();
    std::for_each(statements.begin(), statements.end(), [this, &ret](auto &statement_ctx) {
        ret->statements->items.add(std::any_cast<One<Statement>>(statement_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = cqasm::tree::make<Version>();
    const auto &token = context->VERSION_NUMBER()->getSymbol();
    const auto &text = context->VERSION_NUMBER()->getText();
    const std::regex pattern{ "([0-9]+)\\.([0-9]+)" };
    std::smatch matches{};
    std::regex_match(text, matches, pattern);
    ret->items.push_back(get_int_value(token->getLine(), token->getCharPositionInLine(), matches[1]));
    ret->items.push_back(get_int_value(token->getLine(), token->getCharPositionInLine() + matches.position(2),
        matches[2]));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitQubitTypeDefinition(CqasmParser::QubitTypeDefinitionContext *context) {
    return cqasm::tree::make<Variable>(
        cqasm::tree::make<Identifier>(context->ID()->getText()),
        cqasm::tree::make<Identifier>(context->QUBIT_TYPE()->getText()),
        cqasm::tree::make<IntegerLiteral>(get_int_value(context->INT()))
    );
}

std::any BuildTreeGenAstVisitor::visitBitTypeDefinition(CqasmParser::BitTypeDefinitionContext *context) {
    return cqasm::tree::make<Variable>(
        cqasm::tree::make<Identifier>(context->ID()->getText()),
        cqasm::tree::make<Identifier>(context->BIT_TYPE()->getText()),
        cqasm::tree::make<IntegerLiteral>(get_int_value(context->INT()))
    );
}

std::any BuildTreeGenAstVisitor::visitMeasureStatement(CqasmParser::MeasureStatementContext *context) {
    auto ret = cqasm::tree::make<MeasureStatement>();
    ret->bits = std::any_cast<One<Expression>>(context->expression(0)->accept(this));
    ret->qubits = std::any_cast<One<Expression>>(context->expression(1)->accept(this));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    auto ret = cqasm::tree::make<Instruction>(
        cqasm::tree::make<Identifier>(context->ID()->getText()),
        std::any_cast<One<ExpressionList>>(visitExpressionList(context->expressionList()))
    );
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

std::any BuildTreeGenAstVisitor::visitIndexList(CqasmParser::IndexListContext *context) {
    auto ret = cqasm::tree::make<IndexList>();
    const auto &index_entries = context->indexEntry();
    std::for_each(index_entries.begin(), index_entries.end(), [this, &ret](auto &index_entry_ctx) {
        auto index_entry = std::any_cast<One<IndexEntry>>(index_entry_ctx->accept(this));
        ret->items.add(index_entry);
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitIndexItem(CqasmParser::IndexItemContext *context) {
    return One<IndexEntry>{ cqasm::tree::make<IndexItem>(
        std::any_cast<One<Expression>>(context->expression()->accept(this))
    )};
}

std::any BuildTreeGenAstVisitor::visitIndexRange(CqasmParser::IndexRangeContext *context) {
    return One<IndexEntry>{ cqasm::tree::make<IndexRange>(
        std::any_cast<One<Expression>>(context->expression(0)->accept(this)),
        std::any_cast<One<Expression>>(context->expression(1)->accept(this))
    )};
}

std::any BuildTreeGenAstVisitor::visitInt(CqasmParser::IntContext *context) {
    auto value = get_int_value(context->INT());
    auto ret = cqasm::tree::make<IntegerLiteral>(value);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloat(CqasmParser::FloatContext *context) {
    auto value = get_float_value(context->FLOAT());
    auto ret = cqasm::tree::make<FloatLiteral>(value);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitId(CqasmParser::IdContext *context) {
    auto ret = cqasm::tree::make<Identifier>(context->ID()->getText());
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = cqasm::tree::make<Index>();
    ret->expr = cqasm::tree::make<Identifier>(context->ID()->getText());
    ret->indices = std::any_cast<One<IndexList>>(visitIndexList(context->indexList()));
    return One<Expression>{ ret };
}

}  // namespace cqasm::v3x::parser
