/** \file
 * Implementation for \ref include/cqasm-primitives.hpp "cqasm-primitives.hpp".
 */

#include "cqasm-primitives.hpp"

namespace cqasm {
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

} // namespace primitives
} // namespace cqasm

/**
 * Stream << overload for axis nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Axis& axis) {
    switch (axis) {
        case ::cqasm::primitives::Axis::X:
            os << "X";
            break;
        case ::cqasm::primitives::Axis::Y:
            os << "Y";
            break;
        case ::cqasm::primitives::Axis::Z:
            os << "Z";
            break;
    }
    return os;
}

/**
 * Stream << overload for version nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Version& object) {
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
