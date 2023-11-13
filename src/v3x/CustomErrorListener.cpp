#include "cqasm-error.hpp"
#include "v3x/CustomErrorListener.hpp"

#include <fmt/format.h>
#include <stdexcept>


namespace cqasm::v3x::parser {

CustomErrorListener::CustomErrorListener(const std::string &file_name)
: file_name_{ file_name } {}

void CustomErrorListener::syntaxError(size_t line, size_t charPositionInLine, const std::string &msg) {
    syntaxError(nullptr, nullptr, line, charPositionInLine, msg, nullptr);
}

void CustomErrorListener::syntaxError(antlr4::Recognizer * /* recognizer */, antlr4::Token * /* offendingSymbol */,
    size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr /* e */) {

    // ANTLR provides a zero-based character position in line
    // We change it here to a one-based index, which is the more human-readable, and the common option in text editors
    throw std::runtime_error{
        fmt::format("{}:{}:{}: {}", file_name_, line, charPositionInLine + 1, msg)
    };
}

}  // namespace cqasm::v3x::parser
