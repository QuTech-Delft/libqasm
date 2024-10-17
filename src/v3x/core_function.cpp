/** \file
 * Implementation for \ref include/libqasm/v3x/core_function.hpp "libqasm/v3x/core_function.hpp".
 */

#include "libqasm/v3x/core_function.hpp"

#include <fmt/format.h>

#include "libqasm/v3x/semantic.hpp"

namespace cqasm::v3x::function {

/**
 * Creates a new function.
 * param_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
 * return_type is a shorthand type specification char as parsed by cqasm::types::from_spec().
 */
CoreFunction::CoreFunction(std::string name, const std::string& param_types, const char return_type)
: name{ std::move(name) }
, param_types{ types::from_spec(param_types) }
, return_type{ types::from_spec(return_type) } {}

/**
 * Equality operator.
 */
bool CoreFunction::operator==(const CoreFunction& rhs) const {
    return name == rhs.name && param_types.equals(rhs.param_types) && return_type.equals(rhs.return_type);
}

/**
 * Stream << overload for functions.
 */
std::ostream& operator<<(std::ostream& os, const CoreFunction& function) {
    return os << fmt::format("{}({}) -> {}", function.name, function.param_types, function.return_type);
}

/**
 * Stream << overload for function references.
 */
std::ostream& operator<<(std::ostream& os, const CoreFunctionRef& function) {
    return function.empty() ? os << "unresolved" : os << *function;
}

}  // namespace cqasm::v3x::function

namespace cqasm::v3x::primitives {

template <>
void serialize(const function::CoreFunctionRef& obj, ::tree::cbor::MapWriter& map) {
    if (obj.empty()) {
        return;
    }
    map.append_string("n", obj->name);
    auto aw = map.append_array("pt");
    for (const auto& t : obj->param_types) {
        aw.append_binary(::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{ t.get_ptr() }));
    }
    aw.close();
    map.append_binary(
        "rt", ::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{ obj->return_type.get_ptr() }));
}

template <>
function::CoreFunctionRef deserialize(const ::tree::cbor::MapReader& map) {
    if (!map.count("n")) {
        return {};
    }
    auto ret = tree::make<function::CoreFunction>();
    ret->name = map.at("n").as_string();
    auto ar = map.at("pt").as_array();
    for (const auto& element : ar) {
        ret->param_types.add(::tree::base::deserialize<types::Node>(element.as_binary()));
    }
    ret->return_type = ::tree::base::deserialize<types::Node>(map.at("rt").as_binary());
    return ret;
}

}  // namespace cqasm::v3x::primitives
