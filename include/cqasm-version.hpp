/** \file
 * Defines utilities for detecting and dealing with cQASM language versions.
 */

#pragma once

#include "cqasm-error.hpp"

#include <cstdint>  // int64_t
#include <cstdio>  // FILE*
#include <fmt/ostream.h>
#include <memory>  // unique_ptr
#include <optional>
#include <ostream>
#include <string>
#include <vector>


/**
 * Namespace for detecting and dealing with cQASM language versions.
 */
namespace cqasm::version {

/**
 * Version number primitive used within the AST and semantic trees.
 */
class Version : public std::vector<std::int64_t> {
private:
    /**
     * Compares this version against the other version.
     * Returns:
     *   1 if this version is newer than the other,
     *   -1 if this version is older than the other, or
     *   0 if both versions are the same.
     * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
     */
    [[nodiscard]] int compare(const Version &other) const;

public:
    /**
     * Default constructor.
     */
    Version() = default;

    /**
     * Constructs a version object from a string.
     */
    Version(const std::string &version);

    /**
     * Constructs a version object from an array of chars.
     */
    Version(const char *version);

    [[nodiscard]] auto operator==(const Version &rhs) const {
        return compare(rhs) == 0;
    }
    [[nodiscard]] auto operator<=>(const Version &rhs) const {
        return compare(rhs) <=> 0;
    }
};

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object);

} // namespace cqasm::version

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <> struct fmt::formatter<cqasm::version::Version> : ostream_formatter {};
