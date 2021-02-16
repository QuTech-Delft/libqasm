/** \file
 * Implementation for \ref include/cqasm-v1-primitives.hpp "cqasm-v1-primitives.hpp".
 */

#include "cqasm-v1-primitives.hpp"

namespace cqasm {
namespace v1 {
namespace primitives {

template <>
Str initialize<Str>() { return ""; }

template <>
Bool initialize<Bool>() { return false; }

template <>
Axis initialize<Axis>() { return Axis::X; }

template <>
Int initialize<Int>() { return 0; }

template <>
Real initialize<Real>() { return 0.0; }

/**
 * Constructs a version object from a string, defaulting to 1.0.
 */
Version::Version(const std::string &version) {
    size_t next, last = 0;
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
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
 */
int Version::compare(const Version &other) const {
    for (size_t i = 0; i < this->size() || i < other.size(); i++) {
        auto lhs = i < this->size() ? (*this)[i] : 0;
        auto rhs = i < other.size() ? other[i] : 0;
        if (lhs > rhs) return 1;
        if (lhs < rhs) return -1;
    }
    return 0;
}

/**
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
 */
int Version::compare(const std::string &other) const {
    return compare(Version(other));
}

/**
 * Stream << overload for axis nodes.
 */
std::ostream &operator<<(std::ostream &os, const Axis &axis) {
    switch (axis) {
        case Axis::X:
            os << "X";
            break;
        case Axis::Y:
            os << "Y";
            break;
        case Axis::Z:
            os << "Z";
            break;
    }
    return os;
}

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object) {
    bool first = true;
    for (auto item : object) {
        if (first) {
            first = false;
        } else {
            os << ".";
        }
        os << item;
    }
    return os;
}

} // namespace primitives
} // namespace v1
} // namespace cqasm
