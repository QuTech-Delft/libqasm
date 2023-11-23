#include "cqasm-utils.hpp"
#include "v1x/cqasm-analysis-result.hpp"


namespace cqasm::v1x::analyzer {

std::string AnalysisResult::to_json() const {
    return cqasm::utils::to_json(*this);
}

} // namespace cqasm::v1x::analyzer
