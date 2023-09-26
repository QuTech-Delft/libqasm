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
                file_name_,
                token->getLine(),
                token->getCharPositionInLine(),
                text
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
                file_name_,
                token->getLine(),
                token->getCharPositionInLine(),
                text
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
        ret->statements->items.add(std::any_cast<One<Statement>>(visitStatement(statement_ctx)));
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
        std::any_cast<One<Expression>>(visitExpression(context->expression())).get_ptr()
    };
}

std::any BuildTreeGenAstVisitor::visitStatement(CqasmParser::StatementContext *context) {
    auto ret = One<Statement>{};
    if (auto mapping_ctx = context->mapping()) {
        ret = cqasm::tree::make<Mapping>(std::any_cast<Mapping>(visitMapping(mapping_ctx)));
    } else if (auto variable_ctx = context->variable()) {
        ret = cqasm::tree::make<Variables>(std::any_cast<Variables>(visitVariable(variable_ctx)));
    } else if (auto instruction_ctx = context->instruction()) {
        ret = cqasm::tree::make<Bundle>(Many<Instruction>{
            std::any_cast<One<Instruction>>(visitInstruction(instruction_ctx))
        });
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitMapping(CqasmParser::MappingContext *context) {
    auto ret = Mapping{};
    ret.alias = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret.expr = std::any_cast<One<Expression>>(visitExpression(context->expression()));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVariable(CqasmParser::VariableContext *context) {
    return Variables{
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER(0)->getText()) },  // names
        cqasm::tree::make<Identifier>(context->IDENTIFIER(1)->getText())  // typ
    };
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    auto ret = cqasm::tree::make<Instruction>();
    ret->name = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->operands = cqasm::tree::make<ExpressionList>(std::any_cast<ExpressionList>(
        visitExpressionList(context->expressionList())));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext *context) {
    auto ret = ExpressionList{};
    const auto &expressions = context->expression();
    std::for_each(expressions.begin(), expressions.end(), [this, &ret](auto &expression_ctx) {
        ret.items.add(std::any_cast<One<Expression>>(visitExpression(expression_ctx)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitExpression(CqasmParser::ExpressionContext *context) {
    auto ret = One<Expression>{};
    if (auto *integer_literal_node = context->INTEGER_LITERAL()) {
        auto integer_literal = get_integer_literal_value(integer_literal_node);
        ret = cqasm::tree::make<IntegerLiteral>(integer_literal);
    } else if (auto *float_literal_node = context->FLOAT_LITERAL()) {
        auto float_literal = get_float_literal_value(float_literal_node);
        ret = cqasm::tree::make<FloatLiteral>(float_literal);
    } else if (auto *id_node = context->IDENTIFIER()) {
        auto id = id_node->getText();
        ret = cqasm::tree::make<Identifier>(std::move(id));
    } else if (auto index_ctx = context->index()) {
        ret = cqasm::tree::make<Index>(std::any_cast<Index>(visitIndex(index_ctx)));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = Index{};
    auto id = context->IDENTIFIER()->getText();
    ret.expr = cqasm::tree::make<Identifier>(std::move(id));
    ret.indices = cqasm::tree::make<IndexList>();
    auto index_item = cqasm::tree::make<IndexItem>(std::any_cast<One<Expression>>(
        visitExpression(context->expression())));
    ret.indices->items.add(index_item);
    return ret;
}

}  // namespace cqasm::v3x::parser
