/** \file
 * Implementation for \ref include/v3x/cqasm-parse-helper.hpp "v3x/cqasm-parse-helper.hpp".
 */

#include "v3x/cqasm-parse-helper.hpp"

#include <stdexcept>  // runtime_error


namespace cqasm {
namespace v3x {
namespace parser {

/**
 * Parse the given file.
 */
ParseResult parse_file(const std::string & /* filename */) {
    throw std::runtime_error("Unimplemented");
}

/**
 * Parse using the given file pointer.
 */
ParseResult parse_file(FILE * /* file */, const std::string & /* filename */) {
    throw std::runtime_error("Unimplemented");
}

/**
 * Parse the given string. A filename may be given in addition for use within
 * error messages.
 */
ParseResult parse_string(const std::string & /* data */, const std::string & /* filename */) {
    throw std::runtime_error("Unimplemented");
}

/**
 * Parse a string or file with flex/bison. If use_file is set, the file
 * specified by filename is read and data is ignored. Otherwise, filename
 * is used only for error messages, and data is read instead. Don't use
 * this directly, use parse().
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    const std::string &,
    bool
) : filename(filename) {}

/**
 * Construct the analyzer internals for the given filename, and analyze
 * the file.
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    FILE *
) : filename(filename) {}

/**
 * Initializes the scanner. Returns whether this was successful.
 */
bool ParseHelper::construct() {
    return true;
}

/**
 * Does the actual parsing.
 */
void ParseHelper::parse() {}

/**
 * Destroys the analyzer.
 */
ParseHelper::~ParseHelper() {}

/**
 * Pushes an error.
 */
void ParseHelper::push_error(const std::string &) {}

} // namespace parser
} // namespace v3x
} // namespace cqasm
