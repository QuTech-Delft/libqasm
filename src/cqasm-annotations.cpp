/** \file
 * Implementation for \ref include/cqasm-annotations.hpp "cqasm-annotations.hpp".
 */

#include <iostream>
#include "cqasm-annotations.hpp"

namespace cqasm {
namespace annotations {

/**
 * Constructs a source location object.
 */
SourceLocation::SourceLocation(
    const std::string &file_name,
    std::uint32_t first_line,
    std::uint32_t first_column,
    std::uint32_t last_line,
    std::uint32_t last_column)
: file_name{ !file_name.empty() ? file_name : unknown_file_name }
, first_line{ first_line }
, first_column{ first_column }
, last_line{ last_line }
, last_column{ last_column } {

    if (last_line < first_line) {
        last_line = first_line;
    }
    if (last_line == first_line && last_column < first_column) {
        last_column = first_column;
    }
}

/**
 * Expands the location range to contain the given location in the source file.
 */
void SourceLocation::expand_to_include(std::uint32_t line, std::uint32_t column) {
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

/**
 * Checks if the name of the source file is known.
 */
[[nodiscard]] bool SourceLocation::file_name_known() const {
    return !file_name.empty() && file_name != unknown_file_name;
}

/**
 * Stream << overload for source location objects.
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &object) {
    // Print file name.
    os << object.file_name;

    // Special case for when only the source file name is known.
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

} // namespace annotations
} // namespace cqasm
