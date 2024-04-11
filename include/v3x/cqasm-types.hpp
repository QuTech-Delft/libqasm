/** \file
 * Defines the \ref cqasm::v3x::types "types" of \ref cqasm::v3x::values "values"
 * available within cQASM's type system, as well as some utility functions.
 */

#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "v3x/cqasm-types-gen.hpp"

/**
 * Namespace for the classes representing the types available within cQASM.
 */
namespace cqasm::v3x::types {

constexpr const char *qubit_type_name = "qubit";
constexpr const char *bit_type_name = "bit";
constexpr const char *axis_type_name = "axis";
constexpr const char *bool_type_name = "bool";
constexpr const char *integer_type_name = "int";
constexpr const char *float_type_name = "float";
constexpr const char *complex_type_name = "complex";
constexpr const char *qubit_array_type_name = "qubit array";
constexpr const char *bit_array_type_name = "bit array";
constexpr const char *bool_array_type_name = "bool array";
constexpr const char *integer_array_type_name = "int array";
constexpr const char *float_array_type_name = "float array";

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
 *  - b = bool
 *  - i = int
 *  - f = float
 *  - V = qubit array
 */
Type from_spec(const char spec);
Types from_spec(const std::string &spec);

/**
 * Returns whether the `actual` type matches the constraints of the `expected` type.
 */
bool type_check(const Type &expected, const Type &actual);

/**
 * Returns the number of elements of the given type.
 */
primitives::Int size_of(const Type &type);

/**
 * Stream << overload for a single type.
 */
std::ostream &operator<<(std::ostream &os, const Type &type);

/**
 * Stream << overload for zero or more types.
 */
std::ostream &operator<<(std::ostream &os, const Types &types);

}  // namespace cqasm::v3x::types

template <>
struct fmt::formatter<cqasm::v3x::types::Type> : fmt::ostream_formatter {};
template <>
struct fmt::formatter<cqasm::v3x::types::Types> : fmt::ostream_formatter {};
