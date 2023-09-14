#include "cqasm-error.hpp"
#include "v1x/cqasm-ast.hpp"
#include "v3x/BuildTreeGenAstVisitor.h"

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

    for (size_t i{ 0 }; i < context->statement().size(); ++i) {
        auto statement_ast = visitStatement(context->statement(i));
        ret->statements->items.add(std::any_cast<One<Statement>>(statement_ast));
    }

    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = One<Version>{};

    for (size_t i{ 0 }; i < context->INTEGER_LITERAL().size(); ++i) {
        ret->items.push_back(get_terminal_node_value<std::int64_t>(context->INTEGER_LITERAL(i)));
    }

    return ret;
}

std::any BuildTreeGenAstVisitor::visitStatement(CqasmParser::StatementContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitMapStatement(CqasmParser::MapStatementContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitVarStatement(CqasmParser::VarStatementContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitExpressionList(CqasmParser::ExpressionListContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitExpression(CqasmParser::ExpressionContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitArrayElements(CqasmParser::ArrayElementsContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitArrayIndex(CqasmParser::ArrayIndexContext * /* context */) {
    throw std::runtime_error{ "Unimplemented" };
}

}  // namespace cqasm::v3x::parser
