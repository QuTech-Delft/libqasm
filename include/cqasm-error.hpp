/** \file
 * Contains custom exception objects used by libqasm.
 */

#pragma once

#include <fmt/ostream.h>

#include <optional>
#include <string>
#include <vector>

#include "cqasm-annotations.hpp"
#include "cqasm-tree.hpp"

/**
 * Namespace for exceptions used by libqasm.
 */
namespace cqasm::error {

class Error;

using ParseError = Error;
using AnalysisError = Error;
using ParseErrors = std::vector<ParseError>;
using AnalysisErrors = std::vector<AnalysisError>;

static constexpr const char *unknown_error_message = "<unknown error message>";

/**
 * Exception used for analysis errors.
 */
class Error : public std::runtime_error {
    /**
     * The error message itself.
     */
    std::string message_;

    /**
     * The error message, decorated with a header and location information.
     * This is the string returned by the what() method.
     */
    mutable std::string what_message_;

    /**
     * Attached location in the source file, if any.
     */
    std::shared_ptr<annotations::SourceLocation> location_;

public:
    /**
     * Constructs a new error.
     * If node is a non-null annotatable with a location node, its location information is attached.
     */
    explicit Error(const std::string &message, const tree::Annotatable *node = nullptr);

    /**
     * Constructs a new error from a message and a source location.
     */
    Error(const std::string &message, std::shared_ptr<annotations::SourceLocation> location);

    /**
     * Constructs a new error from a message and all the fields of a source location.
     */
    Error(const std::string &message, const std::optional<std::string> &file_name,
        const annotations::SourceLocation::Range &range);

    /**
     * Sets the context of this error to the SourceLocation annotation of the given node,
     * if the error doesn't already have such a context.
     * If it does, this is no-op.
     */
    void context(const tree::Annotatable &node);

    /**
     * Returns the message exception-style.
     */
    const char *what() const noexcept override;

    /**
     * Returns a string with a JSON representation of an Error.
     * The JSON representation follows the Language Server Protocol (LSP) specification.
     * Every error is mapped to an LSP Diagnostic structure:
     * Severity is hardcoded to 1 at the moment (value corresponding to an Error level)
     */
    std::string to_json() const;
};

/**
 * Stream << overload for Error.
 */
std::ostream &operator<<(std::ostream &os, const Error &error);

/**
 * Defines a new analysis error class.
 */
#define CQASM_ANALYSIS_ERROR(Name)                                                         \
    class Name : public ::cqasm::error::AnalysisError {                                    \
    public:                                                                                \
        explicit Name(std::string &&message = "", const tree::Annotatable *node = nullptr) \
        : ::cqasm::error::AnalysisError(std::move(message), node) {}                       \
    }

}  // namespace cqasm::error

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <>
struct fmt::formatter<cqasm::error::Error> : ostream_formatter {};
