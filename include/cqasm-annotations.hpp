/** \file
 * Contains annotation objects used within the trees by libqasm.
 */

#pragma once

#include <string>

namespace cqasm {

/**
 * Namespace for tree annotation objects used by libqasm.
 */
namespace annotations {

/**
 * Source location annotation object, containing source file line numbers etc.
 */
class SourceLocation {
public:

    /**
     * The name of the source file.
     */
    std::string filename;

    /**
     * The first line of the range, or 0 if unknown.
     */
    uint32_t first_line;

    /**
     * The first column of the range, or 0 if unknown.
     */
    uint32_t first_column;

    /**
     * The last line of the range, or 0 if unknown.
     */
    uint32_t last_line;

    /**
     * The last column of the range, or 0 if unknown.
     */
    uint32_t last_column;

    /**
     * Constructs a source location object.
     */
    SourceLocation(
        const std::string &filename,
        uint32_t first_line = 0,
        uint32_t first_column = 0,
        uint32_t last_line = 0,
        uint32_t last_column = 0
    );

    /**
     * Expands the location range to contain the given location in the source
     * file.
     */
    void expand_to_include(uint32_t line, uint32_t column = 1);

};

/**
 * Stream << overload for source location objects.
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &object);

} // namespace annotations
} // namespace cqasm
