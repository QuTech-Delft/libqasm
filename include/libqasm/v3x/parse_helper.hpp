/** \file
 * Contains a helper class for the v3x lexer and parser,
 * as well as the entry points for invoking the parser directly,
 * in case you don't need semantic analysis.
 */

#pragma once

#include <memory>  // unique_ptr
#include <optional>
#include <string>

#include "libqasm/annotations.hpp"
#include "libqasm/v3x/antlr_scanner.hpp"
#include "libqasm/v3x/parse_result.hpp"

namespace cqasm::v3x::parser {

using SourceLocation = annotations::SourceLocation;

/**
 * Parse using the given file path.
 * Throws a ParseError if this fails.
 */
ParseResult parse_file(const std::string &file_path, const std::optional<std::string> &file_name);

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
ParseResult parse_string(const std::string &data, const std::optional<std::string> &file_name);

/**
 * Internal helper class for parsing cQASM files.
 */
class ParseHelper {
    /**
     * Scanner doing the actual parsing.
     */
    std::unique_ptr<ScannerAdaptor> scanner_up_;

    /**
     * Name of the file being parsed.
     */
    std::string file_name_;

public:
    explicit ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, const std::optional<std::string> &file_name);

    /**
     * Does the actual parsing.
     */
    ParseResult parse();
};

}  // namespace cqasm::v3x::parser
