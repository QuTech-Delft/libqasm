#pragma once

#include <antlr4-runtime.h>
#include <exception>
#include <string>


namespace cqasm::v3x::parser {

class CustomErrorListener : public antlr4::BaseErrorListener {
    /**
     * Name of the file being parsed.
     */
    std::string file_name_;

    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const std::string &msg, std::exception_ptr e) override;

public:
    explicit CustomErrorListener(std::string file_name = annotations::unknown_file_name);
    void syntaxError(size_t line, size_t charPositionInLine, const std::string &msg);
};

}  // namespace cqasm::v3x::parser
