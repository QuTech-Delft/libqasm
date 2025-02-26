#include <fmt/format.h>
#include <gmock/gmock.h>

#include "libqasm/error.hpp"
#include "libqasm/v3x/semantic_analyzer.hpp"
#include "libqasm/v3x/syntactic_generated.hpp"
#include "libqasm/v3x/values.hpp"
#include "mock_analyzer.hpp"

namespace error = cqasm::error;
namespace syntactic = cqasm::v3x::syntactic;
namespace values = cqasm::v3x::values;

namespace cqasm::v3x::analyzer {

class VisitFunctionResolveTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void expect_analyzer_resolve_function_ok(const values::Value& function_return_value) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillOnce(::testing::Return(function_return_value));
    }
    void expect_analyzer_resolve_function_throw(const std::string& error_message) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Throw(std::runtime_error{ error_message.c_str() }));
    }
    MockAnalyzer analyzer;
    SemanticAnalyzer visitor{ analyzer };
};

/**
 * visit_function_call(name, arguments) is a private method, but we can test it through visit_function_call(node)
 */
TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_returns_a_non_empty_value) {
    const auto& function_return_value = values::Value{ cqasm::tree::make<values::ConstFloat>(0) };
    expect_analyzer_resolve_function_ok(function_return_value);

    auto name = cqasm::tree::make<syntactic::Identifier>("function_that_returns_a_non_empty_value");
    auto arguments = cqasm::tree::make<syntactic::ExpressionList>(cqasm::tree::Any<syntactic::Expression>{});
    auto function_call = syntactic::FunctionCall{ name, arguments };
    auto ret = std::any_cast<values::Value>(visitor.visit_function_call(function_call));
    EXPECT_TRUE(ret.equals(function_return_value));
}

TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_returns_an_empty_value) {
    const auto& function_return_value = values::Value{};
    expect_analyzer_resolve_function_ok(function_return_value);

    auto name = cqasm::tree::make<syntactic::Identifier>("function_that_returns_an_empty_value");
    auto arguments = cqasm::tree::make<syntactic::ExpressionList>(cqasm::tree::Any<syntactic::Expression>{});
    auto function_call = syntactic::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<error::AnalysisError>(::testing::HasSubstr("function implementation returned empty value")));
}

TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_throws) {
    const auto& function_name = std::string{ "function_that_is_not_registered" };
    const auto& error_message = fmt::format("failed to resolve '{}'", function_name);
    expect_analyzer_resolve_function_throw(error_message);

    auto name = cqasm::tree::make<syntactic::Identifier>(function_name);
    auto arguments = cqasm::tree::make<syntactic::ExpressionList>(cqasm::tree::Any<syntactic::Expression>{});
    auto function_call = syntactic::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<std::runtime_error>(::testing::HasSubstr(error_message.c_str())));
}

}  // namespace cqasm::v3x::analyzer
