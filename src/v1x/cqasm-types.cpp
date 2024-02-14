/** \file
 * Implementation for \ref include/v1x/cqasm-types.hpp "v1x/cqasm-types.hpp".
 */

#include "v1x/cqasm-types.hpp"

#include <cctype>
#include <cstdint>
#include <fmt/format.h>


namespace cqasm::v1x::types {

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
Types from_spec(const std::string &spec) {
    // Count the number of qubits, in case we find a unitary parameter.
    size_t num_qubits = 0;
    for (auto c : spec) {
        if (c == 'Q') {
            num_qubits += 1;
        }
    }

    // Now resolve the types.
    Types types;
    for (auto c : spec) {
        bool assignable = std::tolower(c) != (int)c;
        switch (std::tolower(c)) {
            case 'q':
                if (!assignable) {
                    throw std::invalid_argument("use uppercase Q for qubits");
                }
                types.add_raw(new types::Qubit(true));
                break;
            case 'a':
                types.add_raw(new types::Axis(assignable));
                break;
            case 'b':
                types.add_raw(new types::Bool(assignable));
                break;
            case 'i':
                types.add_raw(new types::Int(assignable));
                break;
            case 'r':
                types.add_raw(new types::Real(assignable));
                break;
            case 'c':
                types.add_raw(new types::Complex(assignable));
                break;
            case 'u':
                types.add_raw(new types::ComplexMatrix(
                    static_cast<primitives::Int>(static_cast<std::uint64_t>(1) << num_qubits),
                    static_cast<primitives::Int>(static_cast<std::uint64_t>(1) << num_qubits),
                    assignable));
                break;
            case 'm':
                types.add_raw(new types::RealMatrix(-1, -1, assignable));
                break;
            case 'n':
                types.add_raw(new types::ComplexMatrix(-1, -1, assignable));
                break;
            case 's':
                types.add_raw(new types::String(assignable));
                break;
            case 'j':
                types.add_raw(new types::Json(assignable));
                break;
            default:
                throw std::invalid_argument("unknown type code encountered");
        }
    }
    return types;
}

/**
 * Returns whether the `actual` type matches the constraints of the `expected`
 * type.
 */
bool type_check(const Type &expected, const Type &actual) {

    // Check assignability constraint.
    if (expected->assignable && !actual->assignable) {
        return false;
    }

    // Check the type itself.
    if (actual->type() != expected->type()) {
        return false;
    }

    // Check matrix constraints if applicable.
    if (auto expected_real_mat = expected->as_real_matrix()) {
        auto actual_mat = actual->as_real_matrix();
        if (expected_real_mat->num_cols >= 0) {
            if (actual_mat->num_cols != expected_real_mat->num_cols) {
                return false;
            }
        }
        if (expected_real_mat->num_rows >= 0) {
            if (actual_mat->num_rows != expected_real_mat->num_rows) {
                return false;
            }
        }
    } else if (auto expected_complex_mat = expected->as_complex_matrix()) {
        auto actual_mat = actual->as_complex_matrix();
        if (expected_complex_mat->num_cols >= 0) {
            if (actual_mat->num_cols != expected_complex_mat->num_cols) {
                return false;
            }
        }
        if (expected_complex_mat->num_rows >= 0) {
            if (actual_mat->num_rows != expected_complex_mat->num_rows) {
                return false;
            }
        }
    }

    // All constraints seem to match.
    return true;
}

static void mat_size(std::ostream &os, tree::signed_size_t nrows, tree::signed_size_t ncols) {
    if (ncols == 0) {
        os << "empty matrix";
    } else if (nrows == 1) {
        os << ncols << "-dimensional vector";
    } else {
        if (nrows < 0) {
            os << "*";
        } else {
            os << nrows;
        }
        os << "-by-";
        if (ncols < 0) {
            os << "*";
        } else {
            os << ncols;
        }
        os << " matrix";
    }
}

/**
 * Stream << overload for a single type.
 */
std::ostream &operator<<(std::ostream &os, const Type &type) {
    if (type.empty()) {
        os << "!EMPTY";
    } else if (type->as_qubit()) {
        os << types::qubit_type_name;
    } else if (type->as_bool()) {
        os << types::bool_type_name << "/" << types::bit_type_name;
    } else if (type->as_axis()) {
        os << types::axis_type_name;
    } else if (type->as_int()) {
        os << types::integer_type_name;
    } else if (type->as_real()) {
        os << types::real_type_name;
    } else if (type->as_complex()) {
        os << types::complex_type_name;
    } else if (auto rm = type->as_real_matrix()) {
        os << types::real_type_name << " ";
        mat_size(os, rm->num_rows, rm->num_cols);
    } else if (auto cm = type->as_complex_matrix()) {
        os << types::complex_type_name << " ";
        mat_size(os, cm->num_rows, cm->num_cols);
    } else if (type->as_string()) {
        os << types::string_type_name;
    } else if (type->as_json()) {
        os << types::json_type_name;
    } else {
        // Fallback when no friendly repr is known.
        return os << *type;
    }

    if (type->assignable) {
        os << " reference";
    }

    return os;
}

/**
 * Stream << overload for zero or more types.
 */
std::ostream &operator<<(std::ostream &os, const Types &types) {
    return os << fmt::format("({})", fmt::join(types, ", "));
}

} // namespace cqasm::v1x::types
