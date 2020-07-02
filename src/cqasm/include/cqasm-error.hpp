/** \file
 * Contains custom exception objects used by libqasm.
 */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "cqasm-annotatable.hpp"
#include "cqasm-parse-helper.hpp"

namespace cqasm {

/**
 * Namespace for exceptions used by libqasm.
 */
namespace error {

/**
 * Exception used for analysis errors.
 *
 * Contains a message in the form of an `std::ostringstream` that can be
 * appended to or otherwise modified even after the initial throw to add
 * information, as well as an optional source location object.
 */
class AnalysisError : public std::runtime_error {
private:

    /**
     * The fully constructed message, as returned by get_message().
     */
    mutable std::string msg;

public:

    /**
     * The stringstream used to construct the message. Additional information
     * can be appended after construction.
     */
    std::ostringstream message;

    /**
     * Attached location in the source file, if any.
     */
    std::unique_ptr<parser::SourceLocation> location;

    /**
     * Constructs a new error. If node is a non-null annotatable with a
     * location node, its location information is attached.
     */
    explicit AnalysisError(std::string &&message = "", const annotatable::Annotatable *node = nullptr);

    /**
     * Sets the context of this error to the SourceLocation annotation of the
     * given node, if the error doesn't already have such a context. If it
     * does, this is no-op.
     */
    void context(const annotatable::Annotatable &node);

    /**
     * Constructs the message string.
     */
    const std::string &get_message() const;

    /**
     * Returns the message exception-style.
     */
    const char *what() const noexcept override;

};

/**
 * Defines a new analysis error class.
 */
#define CQASM_ANALYSIS_ERROR(Name)                                  \
    class Name : public ::cqasm::error::AnalysisError {             \
    public:                                                         \
        explicit Name(                                              \
            std::string &&message = "",                             \
            const annotatable::Annotatable *node = nullptr          \
        ) :                                                         \
            ::cqasm::error::AnalysisError(std::move(message), node) \
        {}                                                          \
    }

} // namespace error
} // namespace cqasm
