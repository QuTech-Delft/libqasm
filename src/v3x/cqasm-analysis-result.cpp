#include "cqasm-utils.hpp"
#include "v3x/cqasm-analysis-result.hpp"


namespace cqasm::v3x::analyzer {

std::string AnalysisResult::to_json() const {
    return cqasm::utils::to_json(*this);
}

} // namespace cqasm::v3x::analyzer
