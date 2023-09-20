#include "cqasm-tree.hpp"
#include "v1x/cqasm-ast.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"

#include <cassert>  // assert
#include <stdexcept>  // runtime_error
#include <string>  // stod, stoll


namespace cqasm::v3x::parser {

using namespace cqasm::v1x::ast;
using namespace cqasm::error;

std::int64_t get_integer_literal_value(antlr4::tree::TerminalNode *node) {
    auto text = node->getText();
    std::int64_t ret{};
    try {
        ret = std::stoll(text);
    } catch (std::invalid_argument&) {
        throw std::runtime_error{ "terminal node is not of the expected INTEGER_LITERAL type." };
    } catch (std::out_of_range&) {
        throw std::runtime_error{ "terminal node is out of the INTEGER_LITERAL range" };
    }
    return ret;
}

double get_float_literal_value(antlr4::tree::TerminalNode *node) {
    auto text = node->getText();
    double ret{};
    try {
        ret = std::stod(text);
    } catch (std::invalid_argument&) {
        throw std::runtime_error{ "terminal node is not of the expected FLOAT_LITERAL type." };
    } catch (std::out_of_range&) {
        throw std::runtime_error{ "terminal node is out of the FLOAT_LITERAL range" };
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
    for (size_t i{ 0 }; i < context->statement().size(); ++i) {
        ret->statements->items.add(std::any_cast<One<Statement>>(visitStatement(context->statement(i))));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = cqasm::tree::make<Version>();
    for (size_t i{ 0 }; i < context->INTEGER_LITERAL().size(); ++i) {
        auto number = get_integer_literal_value(context->INTEGER_LITERAL(i));
        ret->items.push_back(number);
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitQubits(CqasmParser::QubitsContext *context) {
    auto ret = Maybe<Expression>{};
    if (auto qubits_ctx = context->QUBITS(); qubits_ctx->getText() == "qubits") {
        ret.set(std::any_cast<One<Expression>>(visitExpression(context->expression())).get_ptr());
    }
    return ret;
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
    } else {
        throw std::runtime_error{ "unknown statement type" };
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitMapping(CqasmParser::MappingContext *context) {
    auto ret = Mapping{};
    if (auto mapping_ctx = context->MAP(); mapping_ctx->getText() == "map") {
        ret.alias = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
        ret.expr = std::any_cast<One<Expression>>(visitExpression(context->expression()));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVariable(CqasmParser::VariableContext *context) {
    auto ret = Variables{};
    if (auto variable_ctx = context->VAR(); variable_ctx->getText() == "var") {
        assert(context->IDENTIFIER().size() == 2);
        ret.names.add(cqasm::tree::make<Identifier>(context->IDENTIFIER(0)->getText()));
        ret.typ = cqasm::tree::make<Identifier>(context->IDENTIFIER(1)->getText());

    }
    return ret;
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
    for (size_t i{ 0 }; i < context->expression().size(); ++i) {
        ret.items.add(std::any_cast<One<Expression>>(visitExpression(context->expression(i))));
    }
    return ret;
}

std::any BuildTreeGenAstVisitor::visitExpression(CqasmParser::ExpressionContext *context) {
    auto ret = One<Expression>{};
    if (auto integer_literal_ast = context->INTEGER_LITERAL()) {
        auto integer_literal = get_integer_literal_value(integer_literal_ast);
        ret = cqasm::tree::make<IntegerLiteral>(integer_literal);
    } else if (auto float_literal_ast = context->FLOAT_LITERAL()) {
        auto float_literal = get_float_literal_value(float_literal_ast);
        ret = cqasm::tree::make<FloatLiteral>(float_literal);
    } else if (auto id_ast = context->IDENTIFIER()) {
        auto id = id_ast->getText();
        ret = cqasm::tree::make<Identifier>(std::move(id));
    } else if (auto index_ctx = context->index()) {
        ret = cqasm::tree::make<Index>(std::any_cast<Index>(visitIndex(index_ctx)));
    } else {
        throw std::runtime_error{ "unknown expression type" };
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
    ret.indices->items.add(std::move(index_item));
    return ret;
}

}  // namespace cqasm::v3x::parser
