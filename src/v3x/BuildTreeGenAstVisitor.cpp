#include "cqasm-error.hpp"
#include "cqasm-tree.hpp"
#include "v1x/cqasm-ast.hpp"
#include "v3x/BuildTreeGenAstVisitor.h"

#include <cassert>  // assert
#include <charconv>  // from_chars
#include <stdexcept>  // runtime_error
#include <system_error>  // errc


namespace cqasm::v3x::parser {

using namespace cqasm::v1x::ast;
using namespace cqasm::error;

template <typename T>
T get_terminal_node_value(antlr4::tree::TerminalNode *node) {
    auto text = node->getText();
    T ret{};
    auto [ptr, ec] = std::from_chars(text.c_str(), text.c_str() + text.size(), ret);
    if (ec == std::errc::invalid_argument) {
        throw AnalysisError{ "terminal node is not of the expected type." };
    } else if (ec == std::errc::result_out_of_range) {
        throw AnalysisError{ "terminal node is out of range" };
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    auto ret = One<Program>{};
    ret->version = std::any_cast<One<Version>>(visitVersion(context->version()));
    ret->num_qubits = std::any_cast<Maybe<Expression>>(visitExpression(context->expression()));
    ret->statements = cqasm::tree::make<StatementList>();
    for (size_t i{ 0 }; i < context->statement().size(); ++i) {
        auto statement_ast = visitStatement(context->statement(i));
        ret->statements->items.add(std::any_cast<One<Statement>>(statement_ast));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = One<Version>{};
    for (size_t i{ 0 }; i < context->INTEGER_LITERAL().size(); ++i) {
        auto number = get_terminal_node_value<std::int64_t>(context->INTEGER_LITERAL(i));
        ret->items.push_back(number);
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitStatement(CqasmParser::StatementContext *context) {
    if (auto ast = context->mapping()) {
        return cqasm::tree::make<Mapping>(std::any_cast<Mapping>(visitMapping(ast)));
    } else if (auto ast = context->variable()) {
        return cqasm::tree::make<Variables>(std::any_cast<Variables>(visitVariable(ast)));
    } else if (auto ast = context->instruction()) {
        return cqasm::tree::make<Instruction>(std::any_cast<Instruction>(visitInstruction(ast)));
    } else {
        throw AnalysisError{ "unknown statement type" };
    }
}

std::any BuildTreeGenAstVisitor::visitMapping(CqasmParser::MappingContext *context) {
    auto ret = Mapping{};
    ret.alias = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret.expr = std::any_cast<One<Expression>>(visitExpression(context->expression()));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVariable(CqasmParser::VariableContext *context) {
    auto ret = Variables{};
    assert(context->IDENTIFIER().size() == 2);
    ret.names.add(cqasm::tree::make<Identifier>(context->IDENTIFIER(0)->getText()));
    ret.typ = cqasm::tree::make<Identifier>(context->IDENTIFIER(1)->getText());
    return ret;
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    auto ret = Instruction{};
    ret.name = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    auto expression_list_ast = visitExpressionList(context->expressionList());
    ret.operands = cqasm::tree::make<ExpressionList>(std::any_cast<ExpressionList>(expression_list_ast));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext *context) {
    auto ret = ExpressionList{};
    for (size_t i{ 0 }; i < context->expression().size(); ++i) {
        auto expression_ast = visitExpression(context->expression(i));
        ret.items.add(std::any_cast<One<Expression>>(expression_ast));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitExpression(CqasmParser::ExpressionContext *context) {
    if (auto integer_literal_ast = context->INTEGER_LITERAL()) {
        auto integer_literal = get_terminal_node_value<std::int64_t>(integer_literal_ast);
        return cqasm::tree::make<IntegerLiteral>(integer_literal);
    } else if (auto float_literal_ast = context->FLOAT_LITERAL()) {
        auto float_literal = get_terminal_node_value<double>(float_literal_ast);
        return cqasm::tree::make<FloatLiteral>(float_literal);
    } else if (auto id_ast = context->IDENTIFIER()) {
        auto id = id_ast->getText();
        return cqasm::tree::make<Identifier>(std::move(id));
    } else if (auto index_ast = context->index()) {
        return cqasm::tree::make<Index>(std::any_cast<Index>(visitIndex(index_ast)));
    } else {
        throw AnalysisError{ "unknown expression type" };
    }
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = Index{};
    auto id = context->IDENTIFIER()->getText();
    ret.expr = cqasm::tree::make<Identifier>(std::move(id));
    ret.indices = cqasm::tree::make<IndexList>();
    auto expression_ast = visitExpression(context->expression());
    auto index_item = cqasm::tree::make<IndexItem>(std::any_cast<One<Expression>>(expression_ast));
    ret.indices->items.add(std::move(index_item));
    return ret;
}

}  // namespace cqasm::v3x::parser
