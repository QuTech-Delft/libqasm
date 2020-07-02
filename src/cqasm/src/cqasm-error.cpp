#include "cqasm-error.hpp"

namespace cqasm {
namespace error {

/**
 * Constructs a new error. If node is a non-null annotatable with a
 * location node, its location information is attached.
 */
AnalysisError::AnalysisError(
    std::string &&message,
    const annotatable::Annotatable *node
) : std::runtime_error("") {
    this->message << message;
    if (node) {
        context(*node);
    }
}

/**
 * Sets the context of this error to the SourceLocation annotation of the
 * given node, if the error doesn't already have such a context. If it
 * does, this is no-op.
 */
void AnalysisError::context(const annotatable::Annotatable &node) {
    if (!location) {
        if (auto loc = node.get_annotation_ptr<parser::SourceLocation>()) {
            location = std::unique_ptr<parser::SourceLocation>(new parser::SourceLocation(*loc));
        }
    }
}

/**
 * Constructs the message string.
 */
const std::string &AnalysisError::get_message() const {
    std::ostringstream ss;
    ss << "Error";
    if (location) {
        ss << " at " << *location;
    }
    ss << ": " << message.str();
    msg = ss.str();
    return msg;
}

/**
 * Returns the message exception-style.
 */
const char *AnalysisError::what() const noexcept {
    return get_message().c_str();
}

} // namespace error
} // namespace cqasm
