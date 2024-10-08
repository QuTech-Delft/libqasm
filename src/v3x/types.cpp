/** \file
 * Implementation for \ref include/libqasm/v3x/types.hpp "libqasm/v3x/types.hpp".
 */

#include "libqasm/v3x/types.hpp"

#include <fmt/ostream.h>
#include <fmt/ranges.h>

namespace cqasm::v3x::types {

/**
 * Constructs a set of types from a shorthand string representation.
 * In it, each character represents one type. The supported characters are as follows:
 *
 *  - Q = qubit
 *  - B = bit (measurement register)
 *  - b = bool
 *  - i = int
 *  - f = float
 *  - V = qubit array
 *  - W = bit array
 */
Type from_spec(const char c) {
    switch (c) {
        case 'Q': return tree::make<types::Qubit>();
        case 'B': return tree::make<types::Bit>();
        case 'b': return tree::make<types::Bool>();
        case 'i': return tree::make<types::Int>();
        case 'f': return tree::make<types::Float>();
        case 'V': return tree::make<types::QubitArray>();
        case 'W': return tree::make<types::BitArray>();
        default: throw std::invalid_argument("unknown type code encountered");
    }
}

Types from_spec(const std::string &spec) {
    Types types;
    for (auto c : spec) {
        types.add(from_spec(c));
    }
    return types;
}

/**
 * Returns whether the `actual` type matches the constraints of the `expected` type.
 */
bool type_check(const Type &expected, const Type &actual) {
    return actual->type() == expected->type();
}

/**
 * Returns the number of elements of the given type.
 */
primitives::Int size_of(const Type &type) {
    return type->as_type_base()->size;
}

/**
 * Stream << overload for a single type.
 */
std::ostream &operator<<(std::ostream &os, const Type &type) {
    if (type.empty()) {
        os << "!EMPTY";
    } else if (type->as_qubit()) {
        os << types::qubit_type_name;
    } else if (type->as_bit()) {
        os << types::bit_type_name;
    } else if (type->as_bool()) {
        os << types::bool_type_name;
    } else if (type->as_int()) {
        os << types::integer_type_name;
    } else if (type->as_float()) {
        os << types::float_type_name;
    } else if (type->as_qubit_array()) {
        os << types::qubit_array_type_name;
    } else if (type->as_bit_array()) {
        os << types::bit_array_type_name;
    } else {
        // Fallback when no friendly repr is known
        os << *type;
    }
    return os;
}

/**
 * Stream << overload for zero or more types.
 */
std::ostream &operator<<(std::ostream &os, const Types &types) {
    return os << fmt::format("{}", fmt::join(types, ", "));
}

}  // namespace cqasm::v3x::types
