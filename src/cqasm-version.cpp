/** \file
 * Implementation for \ref include/cqasm-version.hpp "cqasm-version.hpp".
 */

#include "cqasm-version.hpp"

#include <fmt/format.h>

#include <memory>

namespace cqasm::version {

/**
 * Constructs a version object from a string.
 */
Version::Version(const std::string &version) {
    if (version.empty()) {
        return;
    }
    size_t next = 0;
    size_t last = 0;
    while ((next = version.find('.', last)) != std::string::npos) {
        push_back(std::stoi(version.substr(last, next - last)));
        last = next + 1;
    }
    push_back(std::stoi(version.substr(last)));
    for (auto component : *this) {
        if (component < 0) {
            throw std::invalid_argument("version component below zero");
        }
    }
}

/**
 * Constructs a version object from an array of chars.
 */
Version::Version(const char *version)
: Version(std::string{ version }) {}

/**
 * Compares this version against the other version.
 * Returns:
 *   1 if this version is newer than the other,
 *   -1 if this version is older than the other, or
 *   0 if both versions are the same.
 * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
 */
int Version::compare(const Version &other) const {
    for (size_t i = 0; i < this->size() || i < other.size(); i++) {
        auto lhs = (i < this->size()) ? (*this)[i] : 0;
        auto rhs = (i < other.size()) ? other[i] : 0;
        if (lhs > rhs) {
            return 1;
        }
        if (lhs < rhs) {
            return -1;
        }
    }
    return 0;
}

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object) {
    return os << fmt::format("{}", fmt::join(object, "."));
}

}  // namespace cqasm::version
