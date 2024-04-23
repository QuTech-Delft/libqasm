/** \file
 * Implementation for \ref include/cqasm-annotations.hpp "cqasm-annotations.hpp".
 */

#include "cqasm-annotations.hpp"

#include <algorithm>  // min, max
#include <iostream>

#include "cqasm-annotations-constants.hpp"

namespace cqasm::annotations {

SourceLocation::Range::Range(const Index &f, const Index &l)
: first{ f }
, last{ l } {
    last = std::max<Index>(last, first);
}

/**
 * Constructs a source location object.
 */
SourceLocation::SourceLocation(const std::optional<std::string> &file_name_, const Range &range_)
: file_name{ file_name_ }
, range{ range_ } {
    if (file_name.has_value() && file_name.value().empty()) {
        file_name = std::nullopt;
    }
}

/**
 * Expands the location range to contain the given location in the source file.
 */
void SourceLocation::expand_to_include(const Index &index) {
    range.first = std::min<Index>(range.first, index);
    range.last = std::max<Index>(range.last, index);
}

/**
 * Stream << overload for source location objects.
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &object) {
    // Print file name.
    os << object.file_name.value_or(unknown_file_name);

    // Special case for when only the source file name is known.
    if (!object.range.first.line) {
        return os;
    }

    // Print line number.
    os << ":" << object.range.first.line;

    // Special case for when only line numbers are known.
    if (!object.range.first.column) {
        // Print last line too, if greater.
        if (object.range.last.line > object.range.first.line) {
            os << ".." << object.range.last.line;
        }
        return os;
    }

    // Print column.
    os << ":" << object.range.first.column;

    if (object.range.last.line == object.range.first.line) {
        // Range is on a single line. Only repeat the column number.
        if (object.range.last.column > object.range.first.column) {
            os << ".." << object.range.last.column;
        }
    } else if (object.range.last.line > object.range.first.line) {
        // Range is on multiple lines. Repeat both line and column number.
        os << ".." << object.range.last.line << ":" << object.range.last.column;
    }

    return os;
}

}  // namespace cqasm::annotations
