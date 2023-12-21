/** \file
 * Implementation for \ref include/v3x/cqasm-values.hpp "v3x/cqasm-values.hpp".
 */

#include "cqasm-error.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-semantic.hpp"

#include <cassert>
#include  <fmt/format.h>


namespace cqasm::v3x::values {

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Returns null if the check/promotion fails,
 * otherwise returns the constructed value by way of a smart pointer.
 * If the type was an exact match, this may return the given value without modification or a clone thereof.
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
        }
    }

    // Booleans and integers promote to real
    if (type->as_real()) {
        if (const auto &const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstReal>(static_cast<ConstReal>(const_bool->value));
        } else if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstReal>(static_cast<ConstReal>(static_cast<double>(const_int->value)));
        }
    }

    // Booleans, integers and reals promote to complex
    if (type->as_complex()) {
        if (const auto &const_bool = value->as_const_bool()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_bool->value));
        } else if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(static_cast<double>(const_int->value)));
        } else if (const auto &const_real = value->as_const_real()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_real->value));
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
    return types::type_check(from_type, to_type) ||
        (from_type->as_bool() && (to_type->as_int() || to_type->as_real() || to_type->as_complex())) ||
        (from_type->as_int() && (to_type->as_real() || to_type->as_complex())) ||
        (from_type->as_real() && to_type->as_complex());
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
    } else if (value->as_const_real()) {
        return tree::make<types::Real>();
    } else if (value->as_const_complex()) {
        return tree::make<types::Complex>();
    } else if (value->as_const_bool_array()) {
        return tree::make<types::BoolArray>();
    } else if (value->as_const_int_array()) {
        return tree::make<types::IntArray>();
    } else if (value->as_const_real_array()) {
        return tree::make<types::RealArray>();
    } else if (auto index = value->as_index_ref()) {
        return index->variable->typ;
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
 * Returns the number of elements of the given value.
 */
primitives::Int range_of(const Value &value) {
    if (value->as_const_axis() ||
        value->as_const_bool() ||
        value->as_const_int() ||
        value->as_const_real() ||
        value->as_const_complex()) {
        return 1;
    } else if (auto bool_array = value->as_const_bool_array()) {
        return static_cast<primitives::Bool>(bool_array->value.size());
    } else if (auto int_array = value->as_const_int_array()) {
        return static_cast<primitives::Int>(int_array->value.size());
    } else if (auto real_array = value->as_const_real_array()) {
        return static_cast<primitives::Real>(real_array->value.size());
    } else if (auto index = value->as_index_ref()) {
        return static_cast<primitives::Int>(index->indices.size());
    } else if (auto var = value->as_variable_ref()) {
        assert(var->variable->typ->as_type_base());
        return var->variable->typ->as_type_base()->size;
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
