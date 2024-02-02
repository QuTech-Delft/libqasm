#include "cqasm-annotations.hpp"
#include "cqasm-error.hpp"
#include "v3x/CustomErrorListener.hpp"

#include <cstdint>  // uint32_t
#include <memory>  // make_shared
#include <stdexcept>


namespace cqasm::v3x::parser {

CustomErrorListener::CustomErrorListener(std::string file_name)
: file_name_{ std::move(file_name) } {}

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
        std::make_shared<annotations::SourceLocation>(
            file_name_,
            static_cast<std::uint32_t>(line),
            static_cast<std::uint32_t>(charPositionInLine + 1),
            static_cast<std::uint32_t>(line),
            static_cast<std::uint32_t>(charPositionInLine + 1 + offendingSymbol->getText().size())
        )
    };
}

void CustomErrorListener::syntaxError(size_t line, size_t charPositionInLine, const std::string &msg) {
    syntaxError(nullptr, nullptr, line, charPositionInLine, msg, nullptr);
}

}  // namespace cqasm::v3x::parser
