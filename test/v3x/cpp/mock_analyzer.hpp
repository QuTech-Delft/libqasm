#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-values.hpp"

#include <gmock/gmock.h>
#include <string>


namespace cqasm::v3x::analyzer {

class MockAnalyzer : public Analyzer {
public:
    MOCK_METHOD((values::Value), resolve_function, (const std::string &name, const values::Values &args), (const));
};

} // namespace cqasm::v3x::analyzer
