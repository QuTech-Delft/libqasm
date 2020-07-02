/** \file
 * Implementation for \ref include/cqasm-error-model.hpp "cqasm-error-model.hpp".
 */

#include "cqasm-error-model.hpp"
#include "cqasm-utils.hpp"

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
