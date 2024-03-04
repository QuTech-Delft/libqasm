/** \file
 * Contains annotation objects used within the trees by libqasm.
 */

#pragma once

#include <cstdint>
#include <fmt/ostream.h>
#include <optional>
#include <string>


namespace cqasm::annotations {

/**
 * Source location annotation object, containing source file line numbers etc.
 */
struct SourceLocation {
    struct Index {
        std::uint32_t line = 0;
        std::uint32_t column = 0;

        bool operator==(const Index &other) const = default;
        auto operator<=>(const Index &other) const = default;
    };

    struct Range {
        Index first;
        Index last;

        Range() = default;
        Range(const Index &f, const Index &l) : first{ f } , last{ l } { last = std::max<Index>(last, first); }
        Range(const Range &other) = default;
        Range(Range &&other) noexcept = default;
        Range& operator=(const Range &other) = default;
        Range& operator=(Range &&other) noexcept = default;

        bool operator==(const Range &other) const = default;
        auto operator<=>(const Range &other) const = default;
    };

    /**
     * The name of the source file.
     */
    std::optional<std::string> file_name;

    /**
     * The source location range.
     */
    Range range;

    /**
     * Constructs a source location object.
     */
    SourceLocation() = default;
    explicit SourceLocation(const std::optional<std::string> &file_name, const Range &range);
    SourceLocation(const SourceLocation &other) = default;
    SourceLocation(SourceLocation &&other) noexcept = default;
    SourceLocation& operator=(const SourceLocation &other) = default;
    SourceLocation& operator=(SourceLocation &&other) noexcept = default;

    bool operator==(const SourceLocation &other) const = default;
    auto operator<=>(const SourceLocation &other) const = default;

    /**
     * Expands the location range to contain the given location in the source file.
     */
    void expand_to_include(const Index &last);
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
