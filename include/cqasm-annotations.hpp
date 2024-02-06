/** \file
 * Contains annotation objects used within the trees by libqasm.
 */

#pragma once

#include <cstdint>
#include <fmt/ostream.h>
#include <string>


namespace cqasm::annotations {

static constexpr const char* unknown_file_name = "<unknown file name>";

/**
 * Source location annotation object, containing source file line numbers etc.
 */
class SourceLocation {
public:
    /**
     * The name of the source file.
     */
    std::string file_name;

    /**
     * The first line of the range, or 0 if unknown.
     */
    std::uint32_t first_line;

    /**
     * The first column of the range, or 0 if unknown.
     */
    std::uint32_t first_column;

    /**
     * The last line of the range, or 0 if unknown.
     */
    std::uint32_t last_line;

    /**
     * The last column of the range, or 0 if unknown.
     */
    std::uint32_t last_column;

    /**
     * Constructs a source location object.
     */
    explicit SourceLocation(
        const std::string &file_name,
        std::uint32_t first_line = 0,
        std::uint32_t first_column = 0,
        std::uint32_t last_line = 0,
        std::uint32_t last_column = 0
    );

    /**
     * Expands the location range to contain the given location in the source file.
     */
    void expand_to_include(std::uint32_t line, std::uint32_t column = 1);

    /**
     * Checks if the name of the source file is known.
     */
    [[nodiscard]] bool file_name_known() const;
};

/**
 * Stream << overload for source location objects.
 */
std::ostream &operator<<(std::ostream &os, const SourceLocation &object);

} // namespace cqasm::annotations

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <> struct fmt::formatter<cqasm::annotations::SourceLocation> : ostream_formatter {};
