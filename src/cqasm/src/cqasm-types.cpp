#include "cqasm-types.hpp"

namespace cqasm {
namespace types {

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
                    1ull << num_qubits, 1ull << num_qubits, assignable));
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

} // namespace types
} // namespace cqasm

static void mat_size(std::ostream& os, ssize_t nrows, ssize_t ncols) {
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
std::ostream& operator<<(std::ostream& os, const ::cqasm::types::Type& type) {
    if (type.empty()) {
        os << "!EMPTY";
    } else if (type->as_bool()) {
        os << "bool/bit";
    } else if (type->as_axis()) {
        os << "axis";
    } else if (type->as_int()) {
        os << "int";
    } else if (type->as_real()) {
        os << "real";
    } else if (type->as_complex()) {
        os << "complex";
    } else if (auto real_mat = type->as_real_matrix()) {
        os << "real ";
        mat_size(os, real_mat->num_rows, real_mat->num_cols);
    } else if (auto complex_mat = type->as_complex_matrix()) {
        os << "complex ";
        mat_size(os, complex_mat->num_rows, complex_mat->num_cols);
    } else if (type->as_string()) {
        os << "string";
    } else if (type->as_json()) {
        os << "json";
    } else if (type->as_qubit()) {
        os << "qubit";
    } else {
        // Fallback when no friendly repr is known.
        os << *type;
        return os;
    }
    if (type->assignable) {
        os << " reference";
    }
    return os;
}

/**
 * Stream << overload for zero or more types.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::types::Types& types) {
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
