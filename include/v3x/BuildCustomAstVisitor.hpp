#pragma once

#include <any>

#include "cqasm-ast.hpp"
#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"

namespace antlr4 {
class Token;
}
namespace cqasm::v3x::parser {
class CustomErrorListener;
}

namespace cqasm::v3x::parser {

class BuildCustomAstVisitor : public CqasmParserVisitor {
public:
    virtual void addErrorListener(CustomErrorListener *errorListener) = 0;
    virtual void syntaxError(size_t line, size_t char_position_in_line, const std::string &text) const = 0;
    virtual void setNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const = 0;
    virtual void expandNodeAnnotation(const ast::One<ast::Node> &node, antlr4::Token *token) const = 0;
    virtual void copyNodeAnnotation(const ast::One<ast::Node> &from, const ast::One<ast::Node> &to) const = 0;
};

}  // namespace cqasm::v3x::parser
