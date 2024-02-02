/** \file
 * Implementation for \ref include/v1x/cqasm-parse-helper.hpp "v1x/cqasm-parse-helper.hpp".
 */

#include "v1x/cqasm-parse-helper.hpp"
#include "v1x/cqasm-parser.hpp"
#include "v1x/cqasm-lexer.hpp"

namespace cqasm {
namespace v1x {
namespace parser {

/**
 * Parse the given file path.
 */
ParseResult parse_file(const std::string &file_path) {
    return ParseHelper(file_path, "", true).result;
}

/**
 * Parse using the given file pointer.
 */
ParseResult parse_file(FILE *file, const std::string &file_name) {
    return ParseHelper(file_name, file).result;
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
ParseResult parse_string(const std::string &data, const std::string &file_name) {
    return ParseHelper(file_name, data, false).result;
}

/**
 * Parse a string or file with flex/bison.
 * If use_file is set, the file specified by file_path is read and data is ignored.
 * Otherwise, file_path is used only for error messages, and data is read instead.
 * Don't use this directly, use parse().
 */
ParseHelper::ParseHelper(const std::string &file_path, const std::string &data, bool use_file)
: file_name(file_path) {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    if (use_file) {
        fptr = fopen(file_path.c_str(), "r");
        if (!fptr) {
            std::ostringstream sb;
            sb << "Failed to open input file " << file_path << ": "
               << strerror(errno);
            push_error(sb.str());
            return;
        }
        cqasm_v1x_set_in(fptr, (yyscan_t)scanner);
    } else {
        buf = cqasm_v1x__scan_string(data.c_str(), (yyscan_t)scanner);
    }

    // Do the actual parsing.
    parse();
}

/**
 * Construct the analyzer internals for the given file_name, and analyze the file.
 */
ParseHelper::ParseHelper(const std::string &file_name, FILE *fptr)
: file_name(file_name) {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    cqasm_v1x_set_in(fptr, (yyscan_t)scanner);

    // Do the actual parsing.
    parse();
}

/**
 * Initializes the scanner.
 * Returns whether this was successful.
 */
bool ParseHelper::construct() {
    if (int ret_code = cqasm_v1x_lex_init((yyscan_t*)&scanner); ret_code) {
        std::ostringstream sb;
        sb << "Failed to construct scanner: " << strerror(ret_code);
        push_error(sb.str());
        return false;
    } else {
        return true;
    }
}

/**
 * Does the actual parsing.
 */
void ParseHelper::parse() {
    if (int ret_code = cqasm_v1x_parse((yyscan_t) scanner, *this); ret_code == 2) {
        std::ostringstream sb;
        sb << "Out of memory while parsing " << file_name;
        push_error(sb.str());
        return;
    } else if (ret_code) {
        std::ostringstream sb;
        sb << "Failed to parse " << file_name;
        push_error(sb.str());
        return;
    }
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw std::runtime_error("internal error: no parse errors returned, but AST is incomplete. AST was dumped.");
    }
}

/**
 * Destroys the analyzer.
 */
ParseHelper::~ParseHelper() {
    if (fptr) {
        fclose(fptr);
    }
    if (buf) {
        cqasm_v1x__delete_buffer((YY_BUFFER_STATE)buf, (yyscan_t)scanner);
    }
    if (scanner) {
        cqasm_v1x_lex_destroy((yyscan_t)scanner);
    }
}

/**
 * Pushes an error.
 */
void ParseHelper::push_error(const std::string &error) {
    result.errors.push_back(error);
}

} // namespace parser
} // namespace v1x
} // namespace cqasm
