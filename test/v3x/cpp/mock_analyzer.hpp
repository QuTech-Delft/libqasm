#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-values.hpp"

#include <gmock/gmock.h>
#include <string>


namespace cqasm::v3x::analyzer {

struct MockAnalyzer : public Analyzer {
    MOCK_METHOD((AnalysisResult), analyze, (const parser::ParseResult &parse_result));
    MOCK_METHOD((values::Value), resolve_function, (const std::string &name, const values::Values &args), (const));

    [[nodiscard]] std::list<Scope> &scope_stack() { return scope_stack_; }

    [[nodiscard]] Scope &global_scope() { return Analyzer::global_scope(); }
    [[nodiscard]] Scope &current_scope() { return Analyzer::current_scope(); }
    [[nodiscard]] tree::One<semantic::Block> current_block() { return Analyzer::current_block(); }
    [[nodiscard]] tree::Any<semantic::Variable> &current_variables() { return Analyzer::current_variables(); }
    [[nodiscard]] tree::Any<semantic::Function> &global_functions() { return Analyzer::global_functions(); }

    [[nodiscard]] const Scope &global_scope() const { return Analyzer::global_scope(); }
    [[nodiscard]] const Scope &current_scope() const { return Analyzer::current_scope(); }
    [[nodiscard]] const tree::Any<semantic::Variable> &current_variables() const { return Analyzer::current_variables(); }
    [[nodiscard]] const tree::Any<semantic::Function> &global_functions() const { return Analyzer::global_functions(); }
};

} // namespace cqasm::v3x::analyzer
