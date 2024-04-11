#pragma once

#include <gmock/gmock.h>

#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-analysis-result.hpp"
#include "v3x/cqasm-analyzer.hpp"

namespace cqasm::v3x::analyzer {

struct MockAnalyzeTreeGenAstVisitor : public AnalyzeTreeGenAstVisitor {
    explicit MockAnalyzeTreeGenAstVisitor(Analyzer &analyzer)
    : AnalyzeTreeGenAstVisitor{ analyzer } {}

    [[nodiscard]] AnalysisResult &result() { return result_; }
};

}  // namespace cqasm::v3x::analyzer
