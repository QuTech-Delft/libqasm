#include "cqasm-error.hpp"
#include "v3x/CustomErrorListener.hpp"

#include <cstdint>  // uint32_t
#include <stdexcept>


namespace cqasm::v3x::parser {

CustomErrorListener::CustomErrorListener(const std::optional<std::string> &file_name)
: file_name_{ file_name }
{
    if (file_name_.has_value() && file_name_.value().empty()) {
        file_name_ = std::nullopt;
    }
}

void CustomErrorListener::syntaxError(
    antlr4::Recognizer * /* recognizer */,
    antlr4::Token *offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const std::string &msg, std::exception_ptr /* e */) {

    // ANTLR provides a zero-based character position in line
    // We change it here to a one-based index, which is the more human-readable, and the common option in text editors
    throw error::ParseError{
        msg,
        file_name_,
        static_cast<std::uint32_t>(line),
        static_cast<std::uint32_t>(charPositionInLine + 1),
        static_cast<std::uint32_t>(line),
        static_cast<std::uint32_t>(charPositionInLine + 1 + (offendingSymbol ? offendingSymbol->getText().size() : 0))
    };
}

void CustomErrorListener::syntaxError(size_t line, size_t charPositionInLine, const std::string &msg) {
    syntaxError(nullptr, nullptr, line, charPositionInLine, msg, nullptr);
}

}  // namespace cqasm::v3x::parser
