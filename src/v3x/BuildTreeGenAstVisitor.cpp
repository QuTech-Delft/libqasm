#include "cqasm-annotations.hpp"
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

BuildTreeGenAstVisitor::BuildTreeGenAstVisitor(std::string file_name)
: file_name_{ std::move(file_name) }
, error_listener_p_{ nullptr } {}

void BuildTreeGenAstVisitor::addErrorListener(CustomErrorListener *errorListener) {
    error_listener_p_ = errorListener;
}

void BuildTreeGenAstVisitor::syntaxError(size_t line, size_t char_position_in_line, const std::string &text) {
    assert(error_listener_p_);
    error_listener_p_->syntaxError(line, char_position_in_line, text);
}

void BuildTreeGenAstVisitor::setNodeAnnotation(ast::One<ast::Node> node, antlr4::Token *token) {
    auto token_size = token->getStopIndex() - token->getStartIndex() + 1;
    // ANTLR provides a zero-based character position in line
    // We change it here to a one-based index, which is the more human-readable, and the common option in text editors
    node->set_annotation(cqasm::annotations::SourceLocation{
        file_name_,
        static_cast<uint32_t>(token->getLine()),
        static_cast<uint32_t>(token->getCharPositionInLine() + 1),
        static_cast<uint32_t>(token->getLine()),
        static_cast<uint32_t>(token->getCharPositionInLine() + 1 + token_size)
    });
}

std::int64_t BuildTreeGenAstVisitor::get_int_value(size_t line, size_t char_position_in_line, const std::string &text) {
    try {
        return std::stoll(text);
    } catch (std::out_of_range&) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the INTEGER_LITERAL range", text));
    }
    return {};
}

std::int64_t BuildTreeGenAstVisitor::get_int_value(antlr4::tree::TerminalNode *node) {
    const auto &token = node->getSymbol();
    assert(token->getType() == CqasmParser::INTEGER_LITERAL);
    return get_int_value(token->getLine(), token->getCharPositionInLine(), node->getText());
}

double BuildTreeGenAstVisitor::get_float_value(size_t line, size_t char_position_in_line, const std::string &text) {
    try {
        return std::stod(text);
    } catch (std::out_of_range&) {
        syntaxError(line, char_position_in_line, fmt::format("value '{}' is out of the FLOAT_LITERAL range", text));
    }
    return {};
}

double BuildTreeGenAstVisitor::get_float_value(antlr4::tree::TerminalNode *node) {
    const auto &token = node->getSymbol();
    const auto &text = node->getText();
    assert(token->getType() == CqasmParser::FLOAT_LITERAL);
    return get_float_value(token->getLine(), token->getCharPositionInLine(), text);
}

std::any BuildTreeGenAstVisitor::visitProgram(CqasmParser::ProgramContext *context) {
    auto ret = cqasm::tree::make<Program>();
    ret->version = std::any_cast<One<Version>>(visitVersion(context->version()));
    ret->statements = std::any_cast<One<StatementList>>(visitStatements(context->statements()));
    return ret;
}

std::any BuildTreeGenAstVisitor::visitVersion(CqasmParser::VersionContext *context) {
    auto ret = cqasm::tree::make<Version>();
    const auto &token = context->VERSION_NUMBER()->getSymbol();
    const auto &text = context->VERSION_NUMBER()->getText();
    const std::regex pattern{ "([0-9]+)(?:\\.([0-9]+))?" };
    std::smatch matches{};
    std::regex_match(text, matches, pattern);
    ret->items.push_back(get_int_value(token->getLine(), token->getCharPositionInLine(), matches[1]));
    if (matches[2].matched) {
        ret->items.push_back(get_int_value(token->getLine(), token->getCharPositionInLine() + matches.position(2),
            matches[2]));
    }
    setNodeAnnotation(ret, token);
    return ret;
}

std::any BuildTreeGenAstVisitor::visitStatements(CqasmParser::StatementsContext *context) {
    auto ret = cqasm::tree::make<StatementList>();
    const auto &statements = context->statement();
    std::for_each(statements.begin(), statements.end(), [this, &ret](auto &statement_ctx) {
        ret->items.add(std::any_cast<One<Statement>>(statement_ctx->accept(this)));
    });
    return ret;
}

std::any BuildTreeGenAstVisitor::visitStatementSeparator(CqasmParser::StatementSeparatorContext *) {
    return {};
}

// tree-gen AST has support for many variable declarations on the same line,
// but parser grammar only allows one variable declaration per line at the moment

std::any BuildTreeGenAstVisitor::visitQubitTypeDefinition(CqasmParser::QubitTypeDefinitionContext *context) {
    auto array_size_definition_ctx = context->arraySizeDefinition();
    auto size = (array_size_definition_ctx)
        ? tree::Maybe<IntegerLiteral>{ std::any_cast<One<IntegerLiteral>>(array_size_definition_ctx->accept(this)).get_ptr() }
        : tree::Maybe<IntegerLiteral>{};
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->QUBIT_TYPE()->getText()),
        size
    );
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitBitTypeDefinition(CqasmParser::BitTypeDefinitionContext *context) {
    auto array_size_definition_ctx = context->arraySizeDefinition();
    auto size = (array_size_definition_ctx)
        ? tree::Maybe<IntegerLiteral>{ std::any_cast<One<IntegerLiteral>>(array_size_definition_ctx->accept(this)).get_ptr() }
        : tree::Maybe<IntegerLiteral>{};
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->BIT_TYPE()->getText()),
        size
    );
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitAxisTypeDefinition(CqasmParser::AxisTypeDefinitionContext *context) {
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->AXIS_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{}
    );
    // TODO: initialization
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitBoolTypeDefinition(CqasmParser::BoolTypeDefinitionContext *context) {
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->BOOL_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{}
    );
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitBoolArrayTypeDefinition(CqasmParser::BoolArrayTypeDefinitionContext *context) {
    auto size = std::any_cast<One<IntegerLiteral>>(visitArraySizeDefinition(context->arraySizeDefinition()));
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->BOOL_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{ size.get_ptr() }
    );
    // TODO: initialization
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIntTypeDefinition(CqasmParser::IntTypeDefinitionContext *context) {
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->INT_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{}
    );
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIntArrayTypeDefinition(CqasmParser::IntArrayTypeDefinitionContext *context) {
    auto size = std::any_cast<One<IntegerLiteral>>(visitArraySizeDefinition(context->arraySizeDefinition()));
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->INT_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{ size.get_ptr() }
    );
    // TODO: initialization
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloatTypeDefinition(CqasmParser::FloatTypeDefinitionContext *context) {
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->FLOAT_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{}
    );
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloatArrayTypeDefinition(CqasmParser::FloatArrayTypeDefinitionContext *context) {
    auto size = std::any_cast<One<IntegerLiteral>>(visitArraySizeDefinition(context->arraySizeDefinition()));
    auto ret = cqasm::tree::make<Variables>(
        Many<Identifier>{ cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText()) },
        cqasm::tree::make<Identifier>(context->FLOAT_TYPE()->getText()),
        tree::Maybe<IntegerLiteral>{ size.get_ptr() }
    );
    // TODO: initialization
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitArraySizeDefinition(CqasmParser::ArraySizeDefinitionContext *context) {
    auto int_ctx = context->INTEGER_LITERAL();
    return cqasm::tree::make<IntegerLiteral>(get_int_value(int_ctx));
}

std::any BuildTreeGenAstVisitor::visitAxisTypeInitializer(CqasmParser::AxisTypeInitializerContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitArrayTypeInitializer(CqasmParser::ArrayTypeInitializerContext *context) {
    (void) context;
    throw std::runtime_error{ "Unimplemented" };
}

std::any BuildTreeGenAstVisitor::visitMeasureInstruction(CqasmParser::MeasureInstructionContext *context) {
    auto ret = cqasm::tree::make<Instruction>();
    ret->name = cqasm::tree::make<Identifier>(context->MEASURE()->getText());
    ret->condition = cqasm::tree::Maybe<Expression>{};
    ret->operands = cqasm::tree::make<ExpressionList>();
    ret->operands->items.add(std::any_cast<One<Expression>>(context->expression(1)->accept(this)));
    ret->operands->items.add(std::any_cast<One<Expression>>(context->expression(0)->accept(this)));
    const auto &token = context->MEASURE()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Statement>{ ret };
}

std::any BuildTreeGenAstVisitor::visitInstruction(CqasmParser::InstructionContext *context) {
    auto ret = cqasm::tree::make<Instruction>();
    ret->name = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->condition = cqasm::tree::Maybe<Expression>{};
    ret->operands = std::any_cast<One<ExpressionList>>(visitExpressionList(context->expressionList()));
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
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

std::any BuildTreeGenAstVisitor::visitIntegerLiteral(CqasmParser::IntegerLiteralContext *context) {
    auto value = get_int_value(context->INTEGER_LITERAL());
    auto ret = cqasm::tree::make<IntegerLiteral>(value);
    const auto &token = context->INTEGER_LITERAL()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitFloatLiteral(CqasmParser::FloatLiteralContext *context) {
    auto value = get_float_value(context->FLOAT_LITERAL());
    auto ret = cqasm::tree::make<FloatLiteral>(value);
    const auto &token = context->FLOAT_LITERAL()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIdentifier(CqasmParser::IdentifierContext *context) {
    auto ret = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Expression>{ ret };
}

std::any BuildTreeGenAstVisitor::visitIndex(CqasmParser::IndexContext *context) {
    auto ret = cqasm::tree::make<Index>();
    ret->expr = cqasm::tree::make<Identifier>(context->IDENTIFIER()->getText());
    ret->indices = std::any_cast<One<IndexList>>(visitIndexList(context->indexList()));
    const auto &token = context->IDENTIFIER()->getSymbol();
    setNodeAnnotation(ret, token);
    return One<Expression>{ ret };
}

}  // namespace cqasm::v3x::parser
