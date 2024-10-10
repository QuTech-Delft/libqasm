#pragma once

#include <gmock/gmock.h>

#include "libqasm/v3x/analysis_result.hpp"
#include "libqasm/v3x/analyzer.hpp"
#include "libqasm/v3x/semantic_analyzer.hpp"

namespace cqasm::v3x::analyzer {

struct MockSemanticAnalyzer : public SemanticAnalyzer {
    explicit MockSemanticAnalyzer(Analyzer &analyzer)
    : SemanticAnalyzer{ analyzer } {}

    [[nodiscard]] AnalysisResult &result() { return result_; }
};

}  // namespace cqasm::v3x::analyzer
