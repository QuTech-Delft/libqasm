/** \file
 * Implementation for \ref include/v1x/cqasm-values.hpp "v1x/cqasm-values.hpp".
 */

#include "cqasm-error.hpp"
#include "v1x/cqasm-parse-helper.hpp"
#include "v1x/cqasm-values.hpp"
#include "v1x/cqasm-types.hpp"
#include "v1x/cqasm-semantic.hpp"

#include <fmt/format.h>


namespace cqasm::v1x::values {

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Also checks assignability of the value if the type says the value must be
 * assignable. Returns null if the check/promotion fails, otherwise returns the
 * constructed value by way of a smart pointer. If the type was an exact match,
 * this may return the given value without modification or a clone thereof.
 */
Value promote(const Value &value, const types::Type &type) {
    // If the types match exactly, just return the original value
    if (types::type_check(type, type_of(value))) {
        return value;
    }

    // Handle promotion rules. If a promotion succeeds, the promoted value is put in retval
    Value retval{};

    // Integers promote to real
    if (type->as_real()) {
        if (const auto &const_int = value->as_const_int()) {
            retval = tree::make<values::ConstReal>(static_cast<ConstReal>(static_cast<double>(const_int->value)));
        }
    }

    // Integers and reals promote to complex
    if (type->as_complex()) {
        if (const auto &const_int = value->as_const_int()) {
            retval = tree::make<values::ConstComplex>(static_cast<ConstComplex>(static_cast<double>(const_int->value)));
        } else if (auto const_real = value->as_const_real()) {
            retval = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_real->value));
        }
    }

    // Real matrix promotes to complex matrix
    if (auto mat_type = type->as_complex_matrix()) {
        if (auto const_real_matrix = value->as_const_real_matrix()) {
            // Match matrix size. Negative sizes in the type mean unconstrained
            if ((tree::signed_size_t) const_real_matrix->value.size_rows() == mat_type->num_rows || mat_type->num_rows < 0) {
                if ((tree::signed_size_t) const_real_matrix->value.size_cols() == mat_type->num_cols || mat_type->num_cols < 0) {
                    // Convert double to complex
                    const size_t rows = const_real_matrix->value.size_rows();
                    const size_t cols = const_real_matrix->value.size_cols();
                    primitives::CMatrix complex_mat_value(rows, cols);
                    for (size_t row = 1; row <= rows; row++) {
                        for (size_t col = 1; col <= cols; col++) {
                            complex_mat_value.at(row, col) = const_real_matrix->value.at(row, col);
                        }
                    }
                    retval = tree::make<values::ConstComplexMatrix>(complex_mat_value);
                }
            }

            // NOTE: DEPRECATED BEHAVIOR, FOR BACKWARDS COMPATIBILITY ONLY
            // If the expected matrix has a defined size and is square, and
            // the real matrix is a vector with the 2 * 4**n entries,
            // we interpret it as an old-style cqasm unitary matrix,
            // from before cqasm knew what complex numbers (or multidimensional matrices) were.
            if (retval.empty() && mat_type->num_rows == mat_type->num_cols && mat_type->num_rows > 0) {
                const size_t size = mat_type->num_rows;
                const size_t num_elements = 2 * size * size;
                if (const_real_matrix->value.size_rows() == 1 && const_real_matrix->value.size_cols() == num_elements) {
                    primitives::CMatrix complex_mat_value(size, size);
                    size_t index = 1;
                    for (size_t row = 1; row <= size; row++) {
                        for (size_t col = 1; col <= size; col++) {
                            double re = const_real_matrix->value.at(1, index++);
                            double im = const_real_matrix->value.at(1, index++);
                            complex_mat_value.at(row, col) = std::complex<double>(re, im);
                        }
                    }
                    retval = tree::make<values::ConstComplexMatrix>(static_cast<ConstComplexMatrix>(complex_mat_value));
                }
            }
        }
    }

    // If a promotion rule was successful, copy the source location annotation from the old value to the new one
    if (!retval.empty()) {
        retval->copy_annotation<parser::SourceLocation>(*value);
    }

    return retval;
}

/**
 * Returns the type of the given value.
 */
types::Type type_of(const Value &value) {
    if (value->as_const_bool()) {
        return tree::make<types::Bool>(false);
    } else if (value->as_const_axis()) {
        return tree::make<types::Axis>(false);
    } else if (value->as_const_int()) {
        return tree::make<types::Int>(false);
    } else if (value->as_const_real()) {
        return tree::make<types::Real>(false);
    } else if (value->as_const_complex()) {
        return tree::make<types::Complex>(false);
    } else if (auto const_real_matrix = value->as_const_real_matrix()) {
        return tree::make<types::RealMatrix>(
            const_real_matrix->value.size_rows(),
            const_real_matrix->value.size_cols(),
            false);
    } else if (auto const_complex_matrix = value->as_const_complex_matrix()) {
        return tree::make<types::ComplexMatrix>(
            const_complex_matrix->value.size_rows(),
            const_complex_matrix->value.size_cols(),
            false);
    } else if (value->as_const_string()) {
        return tree::make<types::String>(false);
    } else if (value->as_const_json()) {
        return tree::make<types::Json>(false);
    } else if (value->as_qubit_refs()) {
        return tree::make<types::Qubit>(true);
    } else if (value->as_bit_refs()) {
        return tree::make<types::Bool>(true);
    } else if (auto fn = value->as_function()) {
        return fn->return_type;
    } else if (auto var = value->as_variable_ref()) {
        return var->variable->typ;
    } else {
        throw std::runtime_error("unknown type!");
    }
}

/**
 * Returns the types of the given values.
 */
types::Types types_of(const Values &values) {
    types::Types types;
    for (const auto &value : values) {
        types.add(type_of(value));
    }
    return types;
}

/**
 * Throws an AnalysisError if the given value is not a constant, i.e. if it
 * doesn't have a known value at this time.
 */
void check_const(const Value &value) {
    if (!value->as_constant()) {
        throw error::AnalysisError("dynamic values are not supported here", &*value);
    }
}

/**
 * Throws an AnalysisError if any of the given values are not a constant, i.e.
 * if it doesn't have a known value at this time.
 */
void check_const(const Values &values) {
    for (const auto &value : values) {
        check_const(value);
    }
}

/**
 * Stream << overload for a single value.
 */
std::ostream &operator<<(std::ostream &os, const Value &value) {
    return (value.empty())
        ? os << "NULL"
        : os << *value;
}

/**
 * Stream << overload for zero or more values.
 */
std::ostream &operator<<(std::ostream &os, const Values &values) {
    return os << fmt::format("[{}]", fmt::join(values, ", "));
}

} // namespace cqasm::v1x::values
