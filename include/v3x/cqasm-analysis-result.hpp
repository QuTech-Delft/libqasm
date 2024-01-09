#pragma once

#include "cqasm-ast.hpp"
#include "cqasm-semantic.hpp"

#include <stdexcept>  // runtime_error
#include <iosfwd>  // ostream
#include <string>
#include <vector>


/**
 * Namespace for the \ref cqasm::analyzer::Analyzer "Analyzer" class and support classes.
 */
namespace cqasm::v3x::analyzer {

/**
 * Exception thrown by AnalysisResult::unwrap() when the cQASM file fails to parse.
 */
class AnalysisFailed: public std::runtime_error {
public:
    AnalysisFailed() : std::runtime_error("cQASM analysis failed") {};
};

/**
 * Analysis result class.
 *
 * An object of this type is returned by the various `analyze*()` methods of the Analyzer class.
 * There are three possibilities:
 *
 *  - Analysis was successful.
 *    In this case, \ref errors is empty, and \ref root contains a valid \ref semantic::Program "Program" tree.
 *  - Analysis failed.
 *    In this case, there is at least one string in \ref errors.
 *    \ref root may be an empty reference, a partial tree, or even a complete but somehow invalid tree.
 *  - Analysis failed spectacularly with an internal error,
 *    in which case an exception is thrown instead of this object being returned.
 *
 * If you don't want to manage printing error messages yourself and would like an exception upon failure in all cases,
 * you can call unwrap() on the returned object.
 */
class AnalysisResult {
public:
    /**
     * Root node of the semantic tree, if analysis was successful.
     * The node may be empty or incomplete if errors is non-empty.
     * Use unwrap() to ensure it's complete if you just want to use it and
     * don't care about things like pretty-printing error messages for your users
     * in a different way than what unwrap() does for you.
     */
    ast::One<semantic::Program> root;

    /**
     * List of accumulated errors.
     * Analysis was successful if and only if `errors.empty()`.
     */
    std::vector<std::string> errors;

    /**
     * "Unwraps" the result (as you would in Rust) to get the program node or an exception.
     * The exception is always an AnalysisFailed, deriving from std::runtime_error.
     * The actual error messages are in this case first written to the given output stream, defaulting to stderr.
     */
    ast::One<semantic::Program> unwrap(std::ostream &out = std::cerr) const;

    /**
     * Returns a string with a JSON representation of the AnalysisResult.
     */
    std::string to_json() const;
};

} // namespace cqasm::v3x::analyzer
