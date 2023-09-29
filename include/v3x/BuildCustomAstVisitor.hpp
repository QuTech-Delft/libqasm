 #pragma once

#include "v3x/CqasmParser.h"
#include "v3x/CqasmParserVisitor.h"

#include <any>


namespace cqasm::v3x::parser {

class BuildCustomAstVisitor : public CqasmParserVisitor {};

}  // namespace cqasm::v3x::parser
