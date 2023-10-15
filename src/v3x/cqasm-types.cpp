/** \file
 * Implementation for \ref include/v3x/cqasm-types.hpp "v3x/cqasm-types.hpp".
 */

#include <cctype>
#include "v3x/cqasm-types.hpp"

namespace cqasm::v3x::types {

/**
 * Constructs a set of types from a shorthand string representation.
 * In it, each character represents one type. The supported characters are as follows:
 *
 *  - i = integer
 *  - r = real
 *  - b = bit
 *  - w = bit array
 *  - q = qubit
 *  - v = qubit array
 */
Types from_spec(const std::string &spec) {
    Types types;
    for (auto c : spec) {
        switch (std::tolower(c)) {
            case 'i': types.add_raw(new types::Int()); break;
            case 'r': types.add_raw(new types::Real()); break;
            case 'b': types.add_raw(new types::Bit()); break;
            case 'w': types.add_raw(new types::BitArray()); break;
            case 'q': types.add_raw(new types::Qubit()); break;
            case 'v': types.add_raw(new types::QubitArray()); break;
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
    os << "(";
    bool first = true;
    for (const auto &type : types) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        os << type;
    }
    os << ")";
    return os;
}

} // namespace cqasm::v3x::types
