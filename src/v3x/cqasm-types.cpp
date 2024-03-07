/** \file
 * Implementation for \ref include/v3x/cqasm-types.hpp "v3x/cqasm-types.hpp".
 */

#include "v3x/cqasm-types.hpp"

#include <fmt/ostream.h>


namespace cqasm::v3x::types {

/**
 * Constructs a set of types from a shorthand string representation.
 * In it, each character represents one type. The supported characters are as follows:
 *
 *  - Q = qubit
 *  - B = bit (measurement register)
 *  - a = axis (x, y, and z vectors)
 *  - b = bool
 *  - i = int
 *  - f = float
 *  - c = complex
 *  - V = qubit array
 *  - W = bit array
 *  - X = bool array
 *  - Y = int array
 *  - Z = float array
 */
Types from_spec(const std::string &spec) {
    Types types;
    for (auto c : spec) {
        switch (c) {
            case 'Q': types.add_raw(new types::Qubit()); break;
            case 'B': types.add_raw(new types::Bit()); break;
            case 'a': types.add_raw(new types::Axis()); break;
            case 'b': types.add_raw(new types::Bool()); break;
            case 'i': types.add_raw(new types::Int()); break;
            case 'f': types.add_raw(new types::Float()); break;
            case 'c': types.add_raw(new types::Complex()); break;
            case 'V': types.add_raw(new types::QubitArray()); break;
            case 'W': types.add_raw(new types::BitArray()); break;
            case 'X': types.add_raw(new types::BoolArray()); break;
            case 'Y': types.add_raw(new types::IntArray()); break;
            case 'Z': types.add_raw(new types::FloatArray()); break;
            default: throw std::invalid_argument("unknown type code encountered");
        }
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
    if (type.empty()) { os << "!EMPTY"; }
    else if (type->as_qubit()) { os << types::qubit_type_name; }
    else if (type->as_bit()) { os << types::bit_type_name; }
    else if (type->as_axis()) { os << types::axis_type_name; }
    else if (type->as_bool()) { os << types::bool_type_name; }
    else if (type->as_int()) { os << types::integer_type_name; }
    else if (type->as_float()) { os << types::float_type_name; }
    else if (type->as_complex()) { os << types::complex_type_name; }
    else if (type->as_qubit_array()) { os << types::qubit_array_type_name; }
    else if (type->as_bit_array()) { os << types::bit_array_type_name; }
    else if (type->as_bool_array()) { os << types::bool_array_type_name; }
    else if (type->as_int_array()) { os << types::integer_array_type_name; }
    else if (type->as_float_array()) { os << types::float_array_type_name; }
    else {
        // Fallback when no friendly repr is known
        return os << *type;
    }
    return os;
}

/**
 * Stream << overload for zero or more types.
 */
std::ostream &operator<<(std::ostream &os, const Types &types) {
    return os << fmt::format("({})", fmt::join(types, ", "));
}

} // namespace cqasm::v3x::types
