/** \file
 * This file contains the \ref cqasm::v3x::function::CoreFunction "CoreFunction" class and support types,
 * each instance representing a function supported natively by the language.
 */

#pragma once

#include <fmt/ostream.h>

#include <string>

#include "libqasm/tree.hpp"
#include "libqasm/v3x/types.hpp"

namespace cqasm::v3x::function {

/**
 * Representation of a function supported by the language.
 *
 * A number of these can be registered into libqasm by the program or library using it through
 * \ref cqasm::analyzer::Analyzer::register_instruction(const function::CoreFunction&) "register_instruction",
 * to inform libqasm of the supported functions.
 * For each function, libqasm needs to know its name and which parameters it expects.
 * The resolved function type is part of the cqasm::values::FunctionCall node present in the semantic tree
 * returned through the parse result structure.
 *
 * Note that it is legal to have multiple functions with the same name,
 * as long as they can be distinguished through their parameter types (i.e. functions can be overloaded).
 */
class CoreFunction : public tree::Base {
public:
    /**
     * The name of the function. Names are matched case sensitively.
     */
    std::string name;

    /**
     * The vector of parameter types that this function expects.
     */
    types::Types param_types;

    /**
     * The return type for this function.
     */
    types::Type return_type;

    /**
     * Creates a new function.
     * param_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
     * return_type is a shorthand type specification char as parsed by cqasm::types::from_spec().
     */
    CoreFunction(std::string name, const std::string& param_types, const char return_type);

    CoreFunction() = default;
    ~CoreFunction() override = default;
    CoreFunction(const CoreFunction& t) = default;
    CoreFunction(CoreFunction&& t) noexcept = default;
    CoreFunction& operator=(const CoreFunction& t) = default;
    CoreFunction& operator=(CoreFunction&& t) noexcept = default;

    bool operator==(const CoreFunction& rhs) const;
    inline bool operator!=(const CoreFunction& rhs) const { return !(*this == rhs); }
};

/**
 * Optional reference to an function, used within the semantic tree.
 */
using CoreFunctionRef = tree::Maybe<CoreFunction>;

/**
 * Stream << overload for functions.
 */
std::ostream& operator<<(std::ostream& os, const CoreFunction& function);

/**
 * Stream << overload for function references.
 */
std::ostream& operator<<(std::ostream& os, const CoreFunctionRef& function);

}  // namespace cqasm::v3x::function

namespace cqasm::v3x::primitives {

template <>
void serialize(const function::CoreFunctionRef& obj, ::tree::cbor::MapWriter& map);
template <>
function::CoreFunctionRef deserialize(const ::tree::cbor::MapReader& map);

}  // namespace cqasm::v3x::primitives

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <>
struct fmt::formatter<cqasm::v3x::function::CoreFunction> : ostream_formatter {};
