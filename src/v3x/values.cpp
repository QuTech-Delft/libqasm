/** \file
 * Implementation for \ref include/libqasm/v3x/values.hpp "libqasm/v3x/values.hpp".
 */

#include "libqasm/v3x/values.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <ostream>
#include <stdexcept>  // runtime_error

#include "libqasm/error.hpp"
#include "libqasm/v3x/parse_helper.hpp"
#include "libqasm/v3x/semantic.hpp"
#include "libqasm/v3x/types.hpp"

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
Value promote(const Value& value, const types::Type& type) {
    // If the types match exactly, just return the original value
    if (types::type_check(type, type_of(value))) {
        return value;
    }

    Value ret{};

    // Booleans promote to integer
    if (type->as_int()) {
        if (const auto& const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstInt>(static_cast<ConstInt>(const_bool->value));
        } else if (value->as_variable_ref()) {
            if (types::type_check(type_of(value), tree::make<types::Bool>())) {
                ret = value;
            }
        }
    }

    // Booleans and integers promote to float
    if (type->as_float()) {
        if (const auto& const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstFloat>(static_cast<ConstFloat>(const_bool->value));
        } else if (const auto& const_int = value->as_const_int()) {
            ret = tree::make<values::ConstFloat>(static_cast<ConstFloat>(static_cast<double>(const_int->value)));
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
bool check_promote(const types::Type& from_type, const types::Type& to_type) {
    if (types::type_check(from_type, to_type)) {
        return true;
    } else if (from_type->as_bool()) {
        return to_type->as_int() || to_type->as_float();
    } else if (from_type->as_int()) {
        return to_type->as_float();
    }
    return false;
}

/**
 * Returns the element type of the given type.
 * Throws an error if the given type is not of array type.
 */
types::Type element_type_of(const types::Type& type) {
    if (types::type_check(type, tree::make<types::QubitArray>())) {
        return tree::make<types::Qubit>();
    } else if (types::type_check(type, tree::make<types::BitArray>())) {
        return tree::make<types::Bit>();
    } else {
        throw std::runtime_error{ fmt::format("type ({}) is not of array type", type) };
    }
}

/**
 * Returns the type of the given value.
 */
types::Type type_of(const Value& value) {
    if (value->as_const_bool()) {
        return tree::make<types::Bool>();
    } else if (value->as_const_int()) {
        return tree::make<types::Int>();
    } else if (value->as_const_float()) {
        return tree::make<types::Float>();
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
    } else {
        throw std::runtime_error("type_of unknown Value type!");
    }
}

/**
 * Returns the types of the given values.
 */
types::Types types_of(const Values& values) {
    types::Types types;
    for (const auto& value : values) {
        types.add(type_of(value));
    }
    return types;
}

/**
 * Returns the number of elements of the given value.
 */
primitives::Int size_of(const Value& value) {
    if (value->as_const_bool() || value->as_const_int() || value->as_const_float()) {
        return 1;
    } else if (auto index = value->as_index_ref()) {
        return static_cast<primitives::Int>(index->indices.size());
    } else if (auto var = value->as_variable_ref()) {
        return types::size_of(var->variable->typ);
    } else {
        throw std::runtime_error("size_of unknown Value type!");
    }
}

/**
 * Throws an AnalysisError if the given value is not a constant,
 * i.e. if it doesn't have a known value at this time.
 */
void check_const(const Value& value) {
    if (!value->as_constant()) {
        throw error::AnalysisError("dynamic values are not supported here", &*value);
    }
}

/**
 * Throws an AnalysisError if any of the given values are not a constant,
 * i.e. if it doesn't have a known value at this time.
 */
void check_const(const Values& values) {
    for (const auto& value : values) {
        check_const(value);
    }
}

/**
 * Stream << overload for a single value.
 */
std::ostream& operator<<(std::ostream& os, const Value& value) {
    return (value.empty()) ? os << "NULL" : os << *value;
}

/**
 * Stream << overload for zero or more values.
 */
std::ostream& operator<<(std::ostream& os, const Values& values) {
    return os << fmt::format("[{}]", fmt::join(values, ", "));
}

}  // namespace cqasm::v3x::values
