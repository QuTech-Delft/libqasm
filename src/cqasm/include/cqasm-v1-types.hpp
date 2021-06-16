/** \file
 * Defines the \ref cqasm::v1::types "types" of \ref cqasm::v1::values "values"
 * available within cQASM's type system, as well as some utility functions.
 */

#pragma once

#include "cqasm-v1-types-gen.hpp"

namespace cqasm {
namespace v1 {

/**
 * Namespace for the classes representing the types available within cQASM.
 */
namespace types {

/**
 * A cQASM type.
 */
using Type = tree::One<TypeBase>;

/**
 * Zero or more cQASM types.
 */
using Types = tree::Any<TypeBase>;

/**
 * Constructs a set of types from a shorthand string representation. In it,
 * each character represents one type. The supported characters are as follows:
 *
 *  - Q = qubit
 *  - B = assignable bit/boolean (measurement register)
 *  - b = bit/boolean
 *  - a = axis (x, y, or z)
 *  - i = integer
 *  - r = real
 *  - c = complex
 *  - u = complex matrix of size 4^n, where n is the number of qubits in
 *        the parameter list (automatically deduced)
 *  - s = (quoted) string
 *  - j = json
 *
 * In general, lowercase means the parameter is only read and can thus be a
 * constant, while uppercase means it is mutated.
 *
 * Note that complex matrices with different constraints and real matrices of
 * any kind cannot be specified this way. You'll have to construct and add
 * those manually.
 */
Types from_spec(const std::string &spec);

/**
 * Returns whether the `actual` type matches the constraints of the `expected`
 * type.
 */
bool type_check(const Type &expected, const Type &actual);

/**
 * Stream << overload for a single type.
 */
std::ostream &operator<<(std::ostream &os, const Type &type);

/**
 * Stream << overload for zero or more types.
 */
std::ostream &operator<<(std::ostream &os, const Types &types);

} // namespace types
} // namespace v1
} // namespace cqasm
