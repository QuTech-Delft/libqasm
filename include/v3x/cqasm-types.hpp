/** \file
 * Defines the \ref cqasm::v3x::types "types" of \ref cqasm::v3x::values "values"
 * available within cQASM's type system, as well as some utility functions.
 */

#pragma once

#include "v3x/cqasm-types-gen.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>


/**
 * Namespace for the classes representing the types available within cQASM.
 */
namespace cqasm::v3x::types {

/**
 * A cQASM type.
 */
using Type = tree::One<TypeBase>;

/**
 * Zero or more cQASM types.
 */
using Types = tree::Any<TypeBase>;

/**
 * Constructs a set of types from a shorthand string representation.
 * In it, each character represents one type. The supported characters are as follows:
 *
 *  - Q = qubit
 *  - B = bit (measurement register)
 *  - V = qubit array
 *  - W = bit array
 *  - i = integer
 *  - r = real
 *  - c = complex
 */
Types from_spec(const std::string &spec);

/**
 * Returns whether the `actual` type matches the constraints of the `expected` type.
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

} // namespace cqasm::v3x::types


template <> struct fmt::formatter<cqasm::v3x::types::Type> : fmt::ostream_formatter {};
template <> struct fmt::formatter<cqasm::v3x::types::Types> : fmt::ostream_formatter {};
