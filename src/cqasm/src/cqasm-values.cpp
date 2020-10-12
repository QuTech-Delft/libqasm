/** \file
 * Implementation for \ref include/cqasm-values.hpp "cqasm-values.hpp".
 */

#include <cqasm-parse-helper.hpp>
#include "cqasm-values.hpp"
#include "cqasm-error.hpp"

namespace cqasm {
namespace values {

using TypeEnum = types::NodeType;
using ValueEnum = values::NodeType;

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Also checks assignability of the value if the type says the value must be
 * assignable. Returns null if the check/promotion fails, otherwise returns the
 * constructed value by way of a smart pointer.
 */
Value promote(const Value &value, const types::Type &type) {
    Value retval;
    switch (type->type()) {
        case TypeEnum::Qubit:
            if (auto qubit_refs = value->as_qubit_refs()) {
                retval = tree::make<values::QubitRefs>(*qubit_refs);
            }
            break;

        case TypeEnum::Bool:
            if (auto bit_refs = value->as_bit_refs()) {
                retval = tree::make<values::BitRefs>(*bit_refs);
            } else if (auto const_bool = value->as_const_bool()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstBool>(const_bool->value);
                }
            }
            break;

        case TypeEnum::Axis:
            if (auto const_axis = value->as_const_axis()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstAxis>(const_axis->value);
                }
            }
            break;

        case TypeEnum::Int:
            if (auto const_int = value->as_const_int()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstInt>(const_int->value);
                }
            }
            break;

        case TypeEnum::Real:
            if (auto const_int = value->as_const_int()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstReal>(const_int->value);
                }
            } else if (auto const_real = value->as_const_real()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstReal>(const_real->value);
                }
            }
            break;

        case TypeEnum::Complex:
            if (auto const_int = value->as_const_int()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstComplex>(const_int->value);
                }
            } else if (auto const_real = value->as_const_real()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstComplex>(const_real->value);
                }
            } else if (auto const_complex = value->as_const_complex()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstComplex>(const_complex->value);
                }
            }
            break;

        case TypeEnum::RealMatrix: {
            auto mat_type = type->as_real_matrix();
            if (auto const_real_matrix = value->as_const_real_matrix()) {
                if (!type->assignable) {
                    // Match matrix size. Negative sizes in the type mean unconstrained.
                    if ((tree::signed_size_t) const_real_matrix->value.size_rows() == mat_type->num_rows || mat_type->num_rows < 0) {
                        if ((tree::signed_size_t) const_real_matrix->value.size_cols() == mat_type->num_cols || mat_type->num_cols < 0) {
                            retval = tree::make<values::ConstRealMatrix>(const_real_matrix->value);
                        }
                    }
                }
            }
            break;
        }

        case TypeEnum::ComplexMatrix: {
            auto mat_type = type->as_complex_matrix();
            if (auto const_complex_matrix = value->as_const_complex_matrix()) {
                if (!type->assignable) {
                    // Match matrix size. Negative sizes in the type mean unconstrained.
                    if ((tree::signed_size_t) const_complex_matrix->value.size_rows() == mat_type->num_rows || mat_type->num_rows < 0) {
                        if ((tree::signed_size_t) const_complex_matrix->value.size_cols() == mat_type->num_cols || mat_type->num_cols < 0) {
                            retval = tree::make<values::ConstComplexMatrix>(const_complex_matrix->value);
                        }
                    }
                }
            } else if (auto const_real_matrix = value->as_const_real_matrix()) {
                if (!type->assignable) {
                    // Match matrix size. Negative sizes in the type mean unconstrained.
                    if ((tree::signed_size_t) const_real_matrix->value.size_rows() == mat_type->num_rows || mat_type->num_rows < 0) {
                        if ((tree::signed_size_t) const_real_matrix->value.size_cols() == mat_type->num_cols || mat_type->num_cols < 0) {
                            // Convert double to complex.
                            const size_t rows = const_real_matrix->value.size_rows();
                            const size_t cols = const_real_matrix->value.size_cols();
                            cqasm::primitives::CMatrix complex_mat_value(rows, cols);
                            for (size_t row = 1; row <= rows; row++) {
                                for (size_t col = 1; col <= cols; col++) {
                                    complex_mat_value.at(row, col) = const_real_matrix->value.at(row, col);
                                }
                            }
                            retval = tree::make<values::ConstComplexMatrix>(complex_mat_value);
                            break;
                        }
                    }
                    // NOTE: DEPRECATED BEHAVIOR, FOR BACKWARDS COMPATIBILITY ONLY
                    // If the expected matrix has a defined size and is square, and
                    // the real matrix is a vector with the 2 * 4**n entries, we
                    // interpret it as an old-style cqasm unitary matrix, from
                    // before cqasm knew what complex numbers (or multidimensional
                    // matrices) were.
                    if (mat_type->num_rows == mat_type->num_cols &&
                        mat_type->num_rows > 0) {
                        const size_t size = mat_type->num_rows;
                        const size_t num_elements = 2 * size * size;
                        if (const_real_matrix->value.size_rows() == 1 && const_real_matrix->value.size_cols() == num_elements) {
                            cqasm::primitives::CMatrix complex_mat_value(size, size);
                            size_t index = 1;
                            for (size_t row = 1; row <= size; row++) {
                                for (size_t col = 1; col <= size; col++) {
                                    double re = const_real_matrix->value.at(1, index++);
                                    double im = const_real_matrix->value.at(1, index++);
                                    complex_mat_value.at(row, col) = std::complex<double>(re, im);
                                }
                            }
                            retval = tree::make<values::ConstComplexMatrix>(complex_mat_value);
                        }
                    }
                }
            }
            break;
        }

        case TypeEnum::String:
            if (auto const_string = value->as_const_string()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstString>(const_string->value);
                }
            }
            break;

        case TypeEnum::Json:
            if (auto const_json = value->as_const_json()) {
                if (!type->assignable) {
                    retval = tree::make<values::ConstJson>(const_json->value);
                }
            }
            break;
    }

    // Copy source location annotations into the new object.
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
    } else {
        throw std::runtime_error("unknown type!");
    }
}

/**
 * Returns the types of the given values.
 */
types::Types types_of(const Values &values) {
    types::Types types;
    for (auto value : values) {
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
    for (auto value : values) {
        check_const(value);
    }
}


} // namespace values
} // namespace cqasm

/**
 * Stream << overload for a single value.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::values::Value& value) {
    if (value.empty()) {
        os << "NULL";
    } else {
        os << *value;
    }
    return os;
}

/**
 * Stream << overload for zero or more values.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::values::Values& values) {
    os << "[";
    bool first = true;
    for (const auto &value : values) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        if (value.empty()) {
            os << "NULL";
        } else {
            os << *value;
        }
    }
    os << "]";
    return os;
}
