/** \file
 * Implementation for \ref include/v3x/cqasm-types.hpp "v3x/cqasm-types.hpp".
 */

#include "v3x/cqasm-types.hpp"

#include <cctype>
#include <fmt/ostream.h>


namespace cqasm::v3x::types {

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
Types from_spec(const std::string &spec) {
    Types types;
    for (auto c : spec) {
        switch (std::tolower(c)) {
            case 'q': types.add_raw(new types::Qubit()); break;
            case 'b': types.add_raw(new types::Bit()); break;
            case 'v': types.add_raw(new types::QubitArray()); break;
            case 'w': types.add_raw(new types::BitArray()); break;
            case 'i': types.add_raw(new types::Int()); break;
            case 'r': types.add_raw(new types::Real()); break;
            case 'c': types.add_raw(new types::Complex()); break;
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
 * Stream << overload for a single type.
 */
std::ostream &operator<<(std::ostream &os, const Type &type) {
    if (type.empty()) { os << "!EMPTY"; }
    else if (type->as_int()) { os << "int"; }
    else if (type->as_real()) { os << "real"; }
    else if (type->as_complex()) { os << "complex"; }
    else if (type->as_bit()) { os << "bit"; }
    else if (type->as_bit_array()) { os << "bit array"; }
    else if (type->as_qubit()) { os << "qubit"; }
    else if (type->as_qubit_array()) { os << "qubit array"; }
    else {
        // Fallback when no friendly repr is known
        os << *type;
        return os;
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
