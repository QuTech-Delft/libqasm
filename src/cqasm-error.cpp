/** \file
 * Implementation for \ref include/cqasm-error.hpp "cqasm-error.hpp".
 */

#include "cqasm-error.hpp"

#include <fmt/format.h>
#include <regex>


namespace cqasm::error {

/**
 * Constructs a new error.
 * If node is a non-null annotatable with a location node, its location information is attached.
 */
Error::Error(const std::string &message, const tree::Annotatable *node)
: std::runtime_error{ message.c_str() }, message_{ message } {
    if (node) {
        context(*node);
    }
}

/**
 * Constructs a new error from a message and a source location.
 */
Error::Error(const std::string &message, std::shared_ptr<annotations::SourceLocation> location)
: std::runtime_error{ message.c_str() }, message_{ message }, location_ { location }
{}

/**
 * Sets the context of this error to the SourceLocation annotation of the given node,
 * if the error doesn't already have such a context.
 * If it does, this is no-op.
 */
void Error::context(const tree::Annotatable &node) {
    if (!location_) {
        if (auto loc = node.get_annotation_ptr<annotations::SourceLocation>()) {
            location_ = std::make_unique<annotations::SourceLocation>(*loc);
        }
    }
}

/**
 * Returns the exception-style message.
 */
const char *Error::what() const noexcept {
    what_message_ = fmt::format("Error{}: {}",
        (location_) ? fmt::format(" at {}", *location_) : std::string{},
        message_);
    return what_message_.c_str();
}

/**
 * Stream << overload for Error.
 */
std::ostream &operator<<(std::ostream &os, const Error &error) {
    return os << error.what();
}

/**
 * Returns a string with a JSON representation of an Error.
 * The JSON representation follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * Severity is hardcoded to 1 at the moment (value corresponding to an Error level)
 */
std::string Error::to_json() const {
    return fmt::format(R"(
        {{ "filename" : "{0}",
          "range": {{
            "start": {{ "line" : {1}, "character" : {2} }},
            "end" : {{ "line" : {3}, "character" : {4} }}
          }},
          "message" : "{5}",
          "severity" : {6}
        }})",
        (location_ ? location_->filename : std::string{}),
        (location_ ? location_->first_line : 0),
        (location_ ? location_->first_column : 0),
        (location_ ? location_->last_line : 0),
        (location_ ? location_->last_column : 0),
        message_,
        1
    );
}

} // namespace cqasm::error
