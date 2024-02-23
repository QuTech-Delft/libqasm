#include "cqasm-error.hpp"
#include "mock_analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/AnalyzeTreeGenAstVisitor.hpp"

#include <fmt/format.h>
#include <gmock/gmock.h>

namespace analyzer = cqasm::v3x::analyzer;
namespace ast = cqasm::v3x::ast;
namespace error = cqasm::error;
namespace values = cqasm::v3x::values;


class VisitFunctionCallTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void ExpectAnalyzerCallFunctionCall(const values::Value &function_return_value) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillOnce(::testing::Return(function_return_value));
    }
    void ExpectAnalyzerCallFunctionThrow(const std::string& error_message) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Throw(std::runtime_error{ error_message.c_str() }));
    }
    analyzer::MockAnalyzer analyzer;
    analyzer::AnalyzeTreeGenAstVisitor visitor{ analyzer };
};


/**
 * visit_function_call(name, arguments) is a private method, but we can test it through visit_function_call(node)
 */
TEST_F(VisitFunctionCallTest, analyzer_call_function_returns_a_non_empty_value) {
    const auto &function_return_value = values::Value{ cqasm::tree::make<values::ConstFloat>(0) };
    ExpectAnalyzerCallFunctionCall(function_return_value);

    auto name = cqasm::tree::make<ast::Identifier>("function_that_returns_a_non_empty_value");
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    auto ret = std::any_cast<values::Value>(visitor.visit_function_call(function_call));
    EXPECT_TRUE(ret.equals(function_return_value));
}

TEST_F(VisitFunctionCallTest, analyzer_call_function_returns_an_empty_value) {
    const auto &function_return_value = values::Value{};
    ExpectAnalyzerCallFunctionCall(function_return_value);

    auto name = cqasm::tree::make<ast::Identifier>("function_that_returns_an_empty_value");
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<error::AnalysisError>(::testing::HasSubstr("function implementation returned empty value")));
}

TEST_F(VisitFunctionCallTest, analyzer_call_function_throws) {
    const auto &function_name = std::string{ "function_that_is_not_registered" };
    const auto &error_message = fmt::format("failed to resolve '{}'", function_name);
    ExpectAnalyzerCallFunctionThrow(error_message);

    auto name = cqasm::tree::make<ast::Identifier>(function_name);
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<std::runtime_error>(::testing::HasSubstr(error_message.c_str())));
}
