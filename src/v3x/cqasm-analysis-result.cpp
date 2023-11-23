#include "cqasm-result-to-json.hpp"
#include "v3x/cqasm-analysis-result.hpp"


namespace cqasm::v3x::analyzer {

std::string AnalysisResult::to_json() const {
    return cqasm::parser::to_json(*this);
}

} // namespace cqasm::v3x::analyzer
