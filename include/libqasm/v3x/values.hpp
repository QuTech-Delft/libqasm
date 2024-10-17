/** \file
 * Defines classes representing the \ref cqasm::v3x::values "values"
 * (collective name for constants, references, and dynamically evaluated expressions)
 * available within cQASM's type system, as well as some utility functions.
 */

#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>  // all_of, for_each

#include "libqasm/v3x/ast.hpp"
#include "libqasm/v3x/types.hpp"
#include "libqasm/v3x/values_generated.hpp"

/**
 * Namespace for the classes representing the values
 * (including dynamically evaluated expressions or references to registers) available within cQASM.
 */
namespace cqasm::v3x::values {

/**
 * A cQASM value, either known at compile-time or an expression for something only known at runtime.
 */
using Value = tree::One<ValueBase>;

/**
 * Zero or more cQASM values.
 */
using Values = tree::Any<ValueBase>;

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Also checks assignability of the value if the type says the value must be assignable.
 * Returns null if the check/promotion fails,
 * otherwise returns the constructed value by way of a smart pointer.
 * If the type was an exact match, this may return the given value without modification or a clone thereof.
 */
Value promote(const Value& value, const types::Type& type);

/**
 * Checks if a from_type can be promoted to a to_type.
 */
bool check_promote(const types::Type& from_type, const types::Type& to_type);

/**
 * Returns the element type of the given type.
 * Throws an error if the given type is not of array type.
 */
types::Type element_type_of(const types::Type& type);

/**
 * Returns the type of the given value.
 */
types::Type type_of(const Value& value);

/**
 * Returns the types of the given values.
 */
types::Types types_of(const Values& values);

/**
 * Returns the number of elements of the given value.
 */
primitives::Int size_of(const Value& value);

/**
 * Throws an AnalysisError if the given value is not a constant,
 * i.e. if it doesn't have a known value at this time.
 */
void check_const(const Value& value);

/**
 * Throws an AnalysisError if any of the given values are not a constant,
 * i.e. if it doesn't have a known value at this time.
 */
void check_const(const Values& values);

/**
 * Stream << overload for a single value.
 */
std::ostream& operator<<(std::ostream& os, const Value& value);

/**
 * Stream << overload for zero or more values.
 */
std::ostream& operator<<(std::ostream& os, const Values& values);

}  // namespace cqasm::v3x::values

template <>
struct fmt::formatter<cqasm::v3x::values::Value> : fmt::ostream_formatter {};
template <>
struct fmt::formatter<cqasm::v3x::values::Values> : fmt::ostream_formatter {};
