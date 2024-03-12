/** \file
 * Implementation for \ref include/v3x/cqasm-values.hpp "v3x/cqasm-values.hpp".
 */

#include "cqasm-error.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-semantic.hpp"

#include <fmt/format.h>
#include <ostream>
#include <stdexcept>  // runtime_error


namespace cqasm::v3x::values {

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Returns null if the check/promotion fails,
 * otherwise returns the constructed value by way of a smart pointer.
 *
 * If the type was an exact match, this may return the given value without modification or a clone thereof.
 *
 * For a variable or a return value, a promotion is just a check.
 * If the check is successful, the variable or return value is returned
*/
Value promote(const Value &value, const types::Type &type) {
    // If the types match exactly, just return the original value
    if (types::type_check(type, type_of(value))) {
        return value;
    }

    Value ret{};

    // Booleans promote to integer
    if (type->as_int()) {
        if (const auto &const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstInt>(static_cast<ConstInt>(const_bool->value));
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::Bool>())) {
                ret = value;
            }
        }
    }
    // Boolean arrays promote to integer arrays
    if (type->as_int_array()) {
        if (const auto &const_bool_array = value->as_const_bool_array()) {
            ret = promote_array_value_to_array_type<ConstBoolArray, ConstIntArray, types::Int>(const_bool_array);
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::BoolArray>())) {
                ret = value;
            }
        }
    }

    // Booleans and integers promote to float
    if (type->as_float()) {
        if (const auto &const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstFloat>(static_cast<ConstFloat>(const_bool->value));
        } else if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstFloat>(static_cast<ConstFloat>(static_cast<double>(const_int->value)));
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::Bool>()) ||
                types::type_check(type_of(value), tree::make<types::Int>())) {
                ret = value;
            }
        }
    }
    // Boolean and integer arrays promote to float arrays
    if (type->as_float_array()) {
        if (const auto &const_bool_array = value->as_const_bool_array()) {
            ret = promote_array_value_to_array_type<ConstBoolArray, ConstFloatArray, types::Float>(const_bool_array);
        } else if (const auto &const_int_array = value->as_const_int_array()) {
            ret = promote_array_value_to_array_type<ConstIntArray, ConstFloatArray, types::Float>(const_int_array);
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::BoolArray>()) ||
                types::type_check(type_of(value), tree::make<types::IntArray>())) {
                ret = value;
            }
        }
    }

    // Booleans, integers and floats promote to complex
    if (type->as_complex()) {
        if (const auto &const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_bool->value));
        } else if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(static_cast<double>(const_int->value)));
        } else if (const auto &const_float = value->as_const_float()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_float->value));
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::Bool>()) ||
                types::type_check(type_of(value), tree::make<types::Int>()) ||
                types::type_check(type_of(value), tree::make<types::Float>())) {
                ret = value;
            }
        }
    }

    // Boolean, integer, and float arrays promote to axis
    // But the returned value is a float array
    // TODO: why cannot we just return an axis?
    if (type->as_axis()) {
        if (const auto &const_bool_array = value->as_const_bool_array()) {
            if (const_bool_array->value.size() == 3) {
                ret = promote_array_value_to_array_type<ConstBoolArray, ConstFloatArray, types::Float>(const_bool_array);
            }
        } else if(const auto &const_int_array = value->as_const_int_array()) {
            if (const_int_array->value.size() == 3) {
                ret = promote_array_value_to_array_type<ConstIntArray, ConstFloatArray, types::Float>(const_int_array);
            }
        } else if(const auto &const_float_array = value->as_const_float_array()) {
            if (const_float_array->value.size() == 3) {
                ret = tree::make<values::ConstFloatArray>(const_float_array->value);
            }
        } else if (value->as_variable_ref() || value->as_function_call()) {
            if (types::type_check(type_of(value), tree::make<types::BoolArray>()) ||
                types::type_check(type_of(value), tree::make<types::IntArray>()) ||
                types::type_check(type_of(value), tree::make<types::FloatArray>())) {
                if (types::size_of(type_of(value)) == 3) {
                    ret = value;
                }
            }
        }
    }

    // If a promotion rule was successful, copy the source location annotation from the old value to the new one
    if (!ret.empty()) {
        ret->copy_annotation<parser::SourceLocation>(*value);
    }

    return ret;
}

/**
 * Checks if a from_type can be promoted to a to_type.
 */
bool check_promote(const types::Type &from_type, const types::Type &to_type) {
    if (types::type_check(from_type, to_type)) {
        return true;
    } else if (from_type->as_bool()) {
        return to_type->as_int() || to_type->as_float() || to_type->as_complex();
    } else if  (from_type->as_int()) {
        return to_type->as_float() || to_type->as_complex();
    } else if (from_type->as_float()) {
        return to_type->as_complex();
    }
    return false;
}

/**
 * Returns the element type of the given type.
 * Throws an error if the given type is not of array type.
 */
types::Type element_type_of(const types::Type &type) {
    if (types::type_check(type, tree::make<types::QubitArray>())) {
        return tree::make<types::Qubit>();
    } else if (types::type_check(type, tree::make<types::BitArray>())) {
        return tree::make<types::Bit>();
    } else if (types::type_check(type, tree::make<types::BoolArray>())) {
        return tree::make<types::Bool>();
    } else if (types::type_check(type, tree::make<types::IntArray>())) {
        return tree::make<types::Int>();
    } else if (types::type_check(type, tree::make<types::FloatArray>())) {
        return tree::make<types::Float>();
    } else {
        throw std::runtime_error{ fmt::format("type ({}) is not of array type", type) };
    }
}

types::Type get_function_ref_return_type(auto function_ref_ptr) {
    auto semantic_function = function_ref_ptr->function;
    if (auto semantic_core_function_ptr = semantic_function->as_core_function()) {
        return semantic_core_function_ptr->core_function_ref->return_type;
    } else if (auto semantic_function_ptr = semantic_function->as_function()) {
        return semantic_function_ptr->return_type;
    } else {
        throw std::runtime_error("unknown type!");
    }
}

/**
 * Returns the type of the given value.
 */
types::Type type_of(const Value &value) {
    if (value->as_const_axis()) {
        return tree::make<types::Axis>();
    } else if (value->as_const_bool()) {
        return tree::make<types::Bool>();
    } else if (value->as_const_int()) {
        return tree::make<types::Int>();
    } else if (value->as_const_float()) {
        return tree::make<types::Float>();
    } else if (value->as_const_complex()) {
        return tree::make<types::Complex>();
    } else if (value->as_const_bool_array()) {
        return tree::make<types::BoolArray>();
    } else if (value->as_const_int_array()) {
        return tree::make<types::IntArray>();
    } else if (value->as_const_float_array()) {
        return tree::make<types::FloatArray>();
    } else if (auto index = value->as_index_ref()) {
        // If the size of the index is 1, return the type of the element (qubit, bit, bool...)
        // Otherwise, return the type of the variable it refers to (qubit array, bit array, bool array...)
        if (size_of(value) == 1) {
            return element_type_of(index->variable->typ);
        } else {
            return index->variable->typ;
        }
    } else if (auto var = value->as_variable_ref()) {
        return var->variable->typ;
    } else if (auto value_function_ref_ptr = value->as_function_ref()) {
        return get_function_ref_return_type(value_function_ref_ptr);
    } else if (auto function_call = value->as_function_call()) {
        return get_function_ref_return_type(function_call->function);
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
 * Returns the number of elements of the given value.
 */
primitives::Int size_of(const Value &value) {
    if (value->as_const_axis()) {
        return 3;
    } else if (value->as_const_bool() || value->as_const_int() || value->as_const_float() || value->as_const_complex()) {
        return 1;
    } else if (auto bool_array = value->as_const_bool_array()) {
        return static_cast<primitives::Int>(bool_array->value.size());
    } else if (auto int_array = value->as_const_int_array()) {
        return static_cast<primitives::Int>(int_array->value.size());
    } else if (auto float_array = value->as_const_float_array()) {
        return static_cast<primitives::Int>(float_array->value.size());
    } else if (auto index = value->as_index_ref()) {
        return static_cast<primitives::Int>(index->indices.size());
    } else if (auto var = value->as_variable_ref()) {
        return types::size_of(var->variable->typ);
    } else if (auto function_ref_ptr = value->as_function_ref()) {
        auto return_type = get_function_ref_return_type(function_ref_ptr);
        return !return_type.empty()
            ? types::size_of(return_type)
            : primitives::Int{ 0 };
    } else if (auto function_call = value->as_function_call()) {
        auto return_type = get_function_ref_return_type(function_call->function);
        return !return_type.empty()
            ? types::size_of(return_type)
            : primitives::Int{ 0 };
    } else {
        throw std::runtime_error("unknown type!");
    }
}

/**
 * Throws an AnalysisError if the given value is not a constant,
 * i.e. if it doesn't have a known value at this time.
 */
void check_const(const Value &value) {
    if (!value->as_constant()) {
        throw error::AnalysisError("dynamic values are not supported here", &*value);
    }
}

/**
 * Throws an AnalysisError if any of the given values are not a constant,
 * i.e. if it doesn't have a known value at this time.
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

} // namespace cqasm::v3x::values
