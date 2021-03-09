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

} // namespace primitives
} // namespace v1
} // namespace cqasm
