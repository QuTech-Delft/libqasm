/** \file
 * Implementation for \ref include/cqasm-error.hpp "cqasm-error.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-utils.hpp"  // url_encode

#include <fmt/format.h>
#include <regex>


namespace cqasm::error {

/**
 * Constructs a new error.
 * If node is a non-null annotatable with a location node, its location information is attached.
 */
Error::Error(const std::string &message, const tree::Annotatable *node)
: std::runtime_error{ !message.empty() ? message : unknown_error_message }
, message_{ !message.empty() ? message : unknown_error_message } {
    if (node) {
        context(*node);
    }
}

/**
 * Constructs a new error from a message and a source location.
 */
Error::Error(const std::string &message, std::shared_ptr<annotations::SourceLocation> location)
: std::runtime_error{ !message.empty() ? message : unknown_error_message }
, message_{ !message.empty() ? message : unknown_error_message }
, location_{ std::move(location) }
{}

/**
 * Constructs a new error from a message and all the fields of a source location.
 */
Error::Error(
    const std::string &message,
    const std::string &file_name,
    std::uint32_t first_line,
    std::uint32_t first_column,
    std::uint32_t last_line,
    std::uint32_t last_column)
: std::runtime_error{ !message.empty() ? message.c_str() : unknown_error_message }
, message_{ !message.empty() ? message : unknown_error_message }
, location_{ std::make_shared<annotations::SourceLocation>(
    !file_name.empty() ? file_name : annotations::unknown_file_name,
    first_line,
    first_column,
    last_line,
    last_column) }
{}

/**
 * Sets the context of this error to the SourceLocation annotation of the given node,
 * if the error doesn't already have such a context.
 * If it does, this is no-op.
 */
void Error::context(const tree::Annotatable &node) {
    if (!location_) {
        if (auto loc = node.get_annotation_ptr<annotations::SourceLocation>()) {
            location_ = std::make_shared<annotations::SourceLocation>(*loc);
        }
    }
}

/**
 * Returns the exception-style message.
 */
const char *Error::what() const noexcept {
    what_message_ = fmt::format("Error{}: {}",
        location_ ? fmt::format(" at {}", *location_) : std::string{},
        !message_.empty() ? message_ : unknown_error_message);
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
    std::string related_information{};
    if (location_ && location_->file_name_known()) {
        related_information = fmt::format(
            R"(,"relatedInformation":[{{)"
                R"("location":{{)"
                    R"("uri":"file:///{0}")"
                    R"(,"range":{{)"
                        R"("start":{{)"
                            R"("line":0)"
                            R"(,"character":0)"
                        R"(}})"
                        R"(,"end":{{)"
                            R"("line":0)"
                            R"(,"character":0)"
                        R"(}})"
                    R"(}})"
                R"(}})"
                R"(,"message":"{1}")"
            R"(}}])",
            cqasm::utils::url_encode(location_->file_name),
            cqasm::utils::json_encode(unknown_error_message)
        );
    }
    return fmt::format(
        R"({{)"
            R"("range":{{)"
                R"("start":{{)"
                    R"("line":{0})"
                    R"(,"character":{1})"
                R"(}})"
                R"(,"end":{{)"
                    R"("line":{2})"
                    R"(,"character":{3})"
                R"(}})"
            R"(}})"
            R"(,"message":"{4}")"
            R"(,"severity":{5})"
            R"({6})"
        R"(}})",
        location_ ? location_->first_line : 0,
        location_ ? location_->first_column : 0,
        location_ ? location_->last_line : 0,
        location_ ? location_->last_column : 0,
        cqasm::utils::json_encode(message_),
        1,
        related_information
    );
}

} // namespace cqasm::error
