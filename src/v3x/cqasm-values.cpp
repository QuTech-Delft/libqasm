/** \file
 * Implementation for \ref include/v3x/cqasm-values.hpp "v3x/cqasm-values.hpp".
 */

#include "cqasm-error.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-semantic.hpp"

#include  <fmt/format.h>


namespace cqasm::v3x::values {

/**
 * Type-checks and (if necessary) promotes the given value to the given type.
 * Also checks assignability of the value if the type says the value must be assignable.
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

    // Integers promote to real
    if (type->as_real()) {
        if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstReal>(static_cast<ConstReal>(const_int->value));
        }
    }

    // Integers and reals promote to complex
    if (type->as_complex()) {
        if (const auto &const_int = value->as_const_int()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_int->value));
        } else if (const auto &const_real = value->as_const_real()) {
            ret = tree::make<values::ConstComplex>(static_cast<ConstComplex>(const_real->value));
        }
    }

    return ret;
}

/**
 * Returns the type of the given value.
 */
types::Type type_of(const Value &value) {
    if (value->as_const_int()) {
        return tree::make<types::Int>();
    } else if (value->as_const_real()) {
        return tree::make<types::Real>();
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
