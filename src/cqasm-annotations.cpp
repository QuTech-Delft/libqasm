/** \file
 * Implementation for \ref include/cqasm-annotations.hpp "cqasm-annotations.hpp".
 */

#include "cqasm-annotations.hpp"
#include "cqasm-annotations-constants.hpp"

#include <iostream>


namespace cqasm::annotations {

/**
 * Constructs a source location object.
 */
SourceLocation::SourceLocation(
    const std::optional<std::string> &file_name_,
    std::uint32_t first_line_,
    std::uint32_t first_column_,
    std::uint32_t last_line_,
    std::uint32_t last_column_)
: file_name{ file_name_ }
, first_line{ first_line_ }
, first_column{ first_column_ }
, last_line{ last_line_ }
, last_column{ last_column_ } {

    if (file_name.has_value() && file_name.value().empty()) {
        file_name = std::nullopt;
    }
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
        first_column = column;
    } else if (line == first_line && column < first_column) {
        first_column = column;
    }
    if (line > last_line) {
        last_line = line;
        last_column = column;
    } else if (line == last_line && column > last_column) {
        last_column = column;
    }
}

/**
 * Stream << overload for source location objects.
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &object) {
    // Print file name.
    os << object.file_name.value_or(unknown_file_name);

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

} // namespace cqasm::annotations
