/** \file
 * Implementation for \ref include/cqasm-parse-helper.hpp "cqasm-parse-helper.hpp".
 */

#include "cqasm-parse-helper.hpp"
#include "cqasm-parser.hpp"
#include "cqasm-lexer.hpp"

namespace cqasm {
namespace parser {

/**
 * Parse the given file.
 */
ParseResult parse_file(const std::string &filename) {
    return std::move(ParseHelper(filename, "", true).result);
}

/**
 * Parse using the given file pointer.
 */
ParseResult parse_file(FILE *file, const std::string &filename) {
    return std::move(ParseHelper(filename, file).result);
}

/**
 * Parse the given string. A filename may be given in addition for use within
 * error messages.
 */
ParseResult parse_string(const std::string &data, const std::string &filename) {
    return std::move(ParseHelper(filename, data, false).result);
}

/**
 * Parse a string or file with flex/bison. If use_file is set, the file
 * specified by filename is read and data is ignored. Otherwise, filename
 * is used only for error messages, and data is read instead. Don't use
 * this directly, use parse().
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    const std::string &data,
    bool use_file
) : filename(filename) {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    if (use_file) {
        fptr = fopen(filename.c_str(), "r");
        if (!fptr) {
            std::ostringstream sb;
            sb << "Failed to open input file " << filename << ": "
               << strerror(errno);
            push_error(sb.str());
            return;
        }
        yyset_in(fptr, (yyscan_t)scanner);
    } else {
        buf = yy_scan_string(data.c_str(), (yyscan_t)scanner);
    }

    // Do the actual parsing.
    parse();

}

/**
 * Construct the analyzer internals for the given filename, and analyze
 * the file.
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    FILE *fptr
) : filename(filename) {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    yyset_in(fptr, (yyscan_t)scanner);

    // Do the actual parsing.
    parse();

}

/**
 * Initializes the scanner. Returns whether this was successful.
 */
bool ParseHelper::construct() {
    int retcode = yylex_init((yyscan_t*)&scanner);
    if (retcode) {
        std::ostringstream sb;
        sb << "Failed to construct scanner: " << strerror(retcode);
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
    int retcode = yyparse((yyscan_t) scanner, *this);
    if (retcode == 2) {
        std::ostringstream sb;
        sb << "Out of memory while parsing " << filename;
        push_error(sb.str());
        return;
    } else if (retcode) {
        std::ostringstream sb;
        sb << "Failed to parse " << filename;
        push_error(sb.str());
        return;
    }
    if (result.errors.empty() && !result.root.is_complete()) {
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
        yy_delete_buffer((YY_BUFFER_STATE)buf, (yyscan_t)scanner);
    }
    if (scanner) {
        yylex_destroy((yyscan_t)scanner);
    }
}

/**
 * Pushes an error.
 */
void ParseHelper::push_error(const std::string &error) {
    result.errors.push_back(error);
}

/**
 * Constructs a source location object.
 */
SourceLocation::SourceLocation(
    const std::string &filename,
    uint32_t first_line,
    uint32_t first_column,
    uint32_t last_line,
    uint32_t last_column
) :
    filename(filename),
    first_line(first_line),
    first_column(first_column),
    last_line(last_line),
    last_column(last_column)
{
    if (last_line < first_line) {
        last_line = first_line;
    }
    if (last_line == first_line && last_column < first_column) {
        last_column = first_column;
    }
}

/**
 * Expands the location range to contain the given location in the source
 * file.
 */
void SourceLocation::expand_to_include(uint32_t line, uint32_t column) {
    if (line < first_line) {
        first_line = line;
    }
    if (line == first_line && column < first_column) {
        first_column = column;
    }
    if (line > last_line) {
        last_line = line;
    }
    if (line == last_line && column > last_column) {
        last_column = column;
    }
}

} // namespace parser
} // namespace cqasm

/**
 * Stream << overload for source location objects.
 */
std::ostream& operator<<(std::ostream& os, const cqasm::parser::SourceLocation& object) {

    // Print filename.
    os << object.filename;

    // Special case for when only the source filename is known.
    if (!object.first_line) {
        return os;
    }

    // Print line number.
    os << ":" << object.first_line;

    // Special case for when only line numbers are known.
    if (!object.first_column) {

        // Print last line too, if greater.
        if (object.last_line > object.first_line) {
            os << ".." << object.last_line;
        }

        return os;
    }

    // Print column.
    os << ":" << object.first_column;

    if (object.last_line == object.first_line) {

        // Range is on a single line. Only repeat the column number.
        if (object.last_column > object.first_column) {
            os << ".." << object.last_column;
        }

    } else if (object.last_line > object.first_line) {

        // Range is on multiple lines. Repeat both line and column number.
        os << ".." << object.last_line << ":" << object.last_column;

    }

    return os;
}
