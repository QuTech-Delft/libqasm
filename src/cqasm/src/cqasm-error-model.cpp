/** \file
 * Implementation for \ref include/cqasm-error-model.hpp "cqasm-error-model.hpp".
 */

#include "cqasm-error-model.hpp"
#include "cqasm-utils.hpp"
#include "cqasm-semantic.hpp"

namespace cqasm {
namespace error_model {

/**
 * Creates a new error model. param_types is a shorthand type specification
 * string as parsed by cqasm::types::from_spec(). If you need more control,
 * you can also manipulate param_types directly.
 */
ErrorModel::ErrorModel(
    const std::string &name,
    const std::string &param_types
) :
    name(name),
    param_types(types::from_spec(param_types))
{}

/**
 * Equality operator.
 */
bool ErrorModel::operator==(const ErrorModel& rhs) const {
    return utils::case_insensitive_equals(name, rhs.name) && param_types == rhs.param_types;
}

} // namespace error_model

namespace primitives {

template <>
void serialize<error_model::ErrorModelRef>(
    const error_model::ErrorModelRef &obj,
    ::tree::cbor::MapWriter &map
) {
    // Use an empty map to signal an empty instruction reference.
    if (obj.empty()) {
        return;
    }

    // Serialize the fields that are basically primitives.
    map.append_string("n", obj->name);

    // Serialize the parameter types, which behave like a subtree.
    ::tree::base::PointerMap types_id_map{};
    obj->param_types.find_reachable(types_id_map);
    auto types = map.append_map("t");
    obj->param_types.serialize(types, types_id_map);

}

template <>
error_model::ErrorModelRef deserialize<error_model::ErrorModelRef>(
    const ::tree::cbor::MapReader &map
) {
    // Empty map signals empty reference.
    if (map.empty()) {
        return error_model::ErrorModelRef{};
    }

    // Restore the fields that are basically primitives.
    auto obj = tree::make<error_model::ErrorModel>(map.at("n").as_string());

    // Restore the types subtree.
    ::tree::base::IdentifierMap types_id_map{};
    obj->param_types = tree::Any<types::TypeBase>(map.at("t").as_map(), types_id_map);
    types_id_map.restore_links();
    obj->param_types.check_well_formed();

    return std::move(obj);
}

} // namespace primitives
} // namespace cqasm

/**
 * Stream << overload for error models.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::error_model::ErrorModel& model) {
    os << model.name << model.param_types;
    return os;
}

/**
 * Stream << overload for error model references.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::error_model::ErrorModelRef& model) {
    if (model.empty()) {
        os << "unresolved";
    } else {
        os << *model;
    }
    return os;
}
