/** \file
 * Contains the ParseResult class.
 */

#pragma once

#include <string>
#include <vector>

#include "libqasm/annotations.hpp"
#include "libqasm/error.hpp"
#include "libqasm/v3x/syntactic.hpp"

/**
 * Namespace for the parser functions and classes.
 */
namespace cqasm::v3x::parser {

using Root = syntactic::One<syntactic::Root>;

/**
 * Parse result information.
 */
class ParseResult {
public:
    /**
     * Root node of the AST, if analysis was sufficiently successful.
     * This may be set even if parsing was not ENTIRELY successful,
     * in which case it will contain one or more error nodes.
     */
    Root root;

    /**
     * List of accumulated errors.
     * Analysis was successful if and only if errors.empty().
     */
    error::ParseErrors errors;

    /**
     * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x syntactic AST.
     * Any additional strings represent error messages.
     * Notice that the AST and error messages won't be available at the same time.
     */
    [[nodiscard]] std::vector<std::string> to_strings() const;

    /**
     * Returns a string with a JSON representation of a ParseResult.
     */
    [[nodiscard]] std::string to_json() const;
};

}  // namespace cqasm::v3x::parser
