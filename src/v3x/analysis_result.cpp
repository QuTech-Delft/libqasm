#include "libqasm/v3x/analysis_result.hpp"

#include <fmt/format.h>

#include "libqasm/result.hpp"

namespace cqasm::v3x::analyzer {

/**
 * "Unwraps" the result (as you would in Rust) to get the program node or an exception.
 * The exception is always an AnalysisFailed, deriving from std::runtime_error.
 * The actual error messages are in this case first written to the given output stream, defaulting to stderr.
 */
Root AnalysisResult::unwrap(std::ostream& out) const {
    if (errors.empty()) {
        return root;
    }
    out << fmt::format("{}", fmt::join(errors, "\n"));
    throw AnalysisFailed();
}

/**
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x semantic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> AnalysisResult::to_strings() const {
    return cqasm::result::to_strings(*this);
}

/**
 * Returns a string with a JSON representation of an AnalysisResult.
 */
std::string AnalysisResult::to_json() const {
    return cqasm::result::to_json(*this);
}

}  // namespace cqasm::v3x::analyzer
