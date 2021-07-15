/** \file
 * This file contains the \ref cqasm::v1::error_model::ErrorModel "ErrorModel" class
 * and support types, each instance representing an error model supported by the
 * user.
 */

#pragma once

#include "cqasm-v1-types.hpp"
#include "cqasm-v1-values.hpp"

namespace cqasm {
namespace v1 {

/**
 * Namespace for the \ref error_model::ErrorModel "ErrorModel" class and helper
 * types.
 */
namespace error_model {

/**
 * Representation of an error model. A number of these can be registered into
 * libqasm by the program or library using it through
 * \ref cqasm::analyzer::Analyzer::register_error_model(const error_model::ErrorModel&)
 * "register_error_model()", to inform libqasm about which
 * error models are supported. libqasm needs to know its name and which
 * parameters it expects in order to be able to resolve the error model
 * information specified in the cQASM file. The resolved error model (if any
 * is specified in the file) can then be read from the parse result.
 *
 * Note that it is legal to have multiple error models with the same name, as
 * long as they can be distinguished through their parameter types (i.e. the
 * available error models can be overloaded).
 *
 * You can add any data you like to these through the
 * \ref cqasm::annotatable::Annotatable "Annotatable" interface
 * for your own bookkeeping, so you don't have to maintain an additional map
 * from this error model structure to your own internal structure if you're
 * okay with using this one.
 */
class ErrorModel : public tree::Base {
public:

    /**
     * The name of the error model. Names are matched case insensitively.
     */
    std::string name;

    /**
     * The vector of parameter types that this error model expects.
     */
    types::Types param_types;

    /**
     * Creates a new error model. param_types is a shorthand type specification
     * string as parsed by cqasm::types::from_spec(). If you need more control,
     * you can also manipulate param_types directly.
     */
    ErrorModel(const std::string &name, const std::string &param_types = "");

    /**
     * Equality operator.
     */
    bool operator==(const ErrorModel& rhs) const;

    /**
     * Inequality operator.
     */
    inline bool operator!=(const ErrorModel& rhs) const {
        return !(*this == rhs);
    }

};

/**
 * Optional reference to an error model, used within the semantic tree.
 */
using ErrorModelRef = tree::Maybe<ErrorModel>;

/**
 * Stream << overload for error models.
 */
std::ostream &operator<<(std::ostream &os, const ErrorModel &model);

/**
 * Stream << overload for error model references.
 */
std::ostream &operator<<(std::ostream &os, const ErrorModelRef &model);

} // namespace error_model

namespace primitives {

template <>
void serialize(const error_model::ErrorModelRef &obj, ::tree::cbor::MapWriter &map);
template <>
error_model::ErrorModelRef deserialize(const ::tree::cbor::MapReader &map);

} // namespace primitives
} // namespace v1
} // namespace cqasm
