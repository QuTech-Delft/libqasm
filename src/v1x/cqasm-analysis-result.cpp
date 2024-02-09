#include "cqasm-result.hpp"
#include "v1x/cqasm-analysis-result.hpp"

#include <fmt/format.h>


namespace cqasm::v1x::analyzer {

/**
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v1.x semantic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> AnalysisResult::to_strings() const {
    return cqasm::result::to_strings(*this);
}

std::string AnalysisResult::to_json() const {
    return cqasm::result::to_json(*this);
}

} // namespace cqasm::v1x::analyzer
