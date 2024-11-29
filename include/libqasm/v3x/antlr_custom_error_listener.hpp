#pragma once

#include <antlr4-runtime.h>

#include <exception>
#include <optional>
#include <string>

namespace cqasm::v3x::parser {

class AntlrCustomErrorListener : public antlr4::BaseErrorListener {
    /**
     * Name of the file being parsed.
     */
    std::optional<std::string> file_name_;

    void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offending_symbol, size_t line,
        size_t char_position_in_line, const std::string& msg, std::exception_ptr e) override;

public:
    explicit AntlrCustomErrorListener(const std::optional<std::string>& file_name);
    void syntaxError(size_t line, size_t char_position_in_line, const std::string& msg);
};

}  // namespace cqasm::v3x::parser
