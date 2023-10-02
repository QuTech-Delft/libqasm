#include "cqasm-error.hpp"
#include "v3x/CustomErrorListener.hpp"

#include <fmt/format.h>
#include <stdexcept>


namespace cqasm::v3x::parser {

void CustomErrorListener::syntaxError(antlr4::Recognizer * /* recognizer */, antlr4::Token * /* offendingSymbol */,
    size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr /* e */) {

    throw std::runtime_error{
        fmt::format("{}:{}:{}: {}", file_name_, line, charPositionInLine, msg)
    };
}

}  // namespace cqasm::v3x::parser
