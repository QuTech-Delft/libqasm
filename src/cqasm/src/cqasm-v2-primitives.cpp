/** \file
 * Implementation for \ref include/cqasm-v2-primitives.hpp "cqasm-v2-primitives.hpp".
 */

#include "cqasm-v2-primitives.hpp"

namespace cqasm {
namespace v2 {
namespace primitives {

template <>
Str initialize<Str>() { return ""; }

template <>
Bool initialize<Bool>() { return false; }

template <>
Int initialize<Int>() { return 0; }

template <>
Real initialize<Real>() { return 0.0; }

template <>
Complex initialize<Complex>() { return 0.0; }

} // namespace primitives
} // namespace v2
} // namespace cqasm
