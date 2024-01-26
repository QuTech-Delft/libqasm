#include "cqasm-error.hpp"
#include "matcher_values.hpp"
#include "mock_analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/AnalyzeTreeGenAstVisitor.hpp"

#include <gmock/gmock.h>
#include <numbers>  // pi

namespace analyzer = cqasm::v3x::analyzer;
namespace ast = cqasm::v3x::ast;
namespace error = cqasm::error;
namespace values = cqasm::v3x::values;


/*
 * visit_function_call(name, arguments) is a private method, but we can test it through visit_function_call(node)
 * This test mocks the call to analyzer_.call_function, and tests the case when that call returns a non-empty value
 */
TEST(visit_function_call, analyzer_call_function_returns_a_non_empty_value) {
    const auto &function_name = std::string{ "sin" };
    const auto &function_arguments = values::Values{ { cqasm::tree::make<values::ConstReal>(std::numbers::pi) } };
    const auto &function_return_value = values::Value{ cqasm::tree::make<values::ConstReal>(0) };
    auto analyzer = analyzer::MockAnalyzer();
    EXPECT_CALL(analyzer, call_function(function_name, values::ValuesEq(function_arguments)))
        .WillOnce(::testing::Return(function_return_value));
    auto visitor = analyzer::AnalyzeTreeGenAstVisitor(analyzer);

    auto name = cqasm::tree::make<ast::Identifier>("sin");
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{
        cqasm::tree::make<ast::FloatLiteral>(std::numbers::pi) });
    auto function_call = ast::FunctionCall{ name, arguments };
    auto ret = std::any_cast<values::Value>(visitor.visit_function_call(function_call));
    EXPECT_EQ(ret->as_const_real()->value, function_return_value->as_const_real()->value);
}

/*
 * visit_function_call(name, arguments) is a private method, but we can test it through visit_function_call(node)
 * This test mocks the call to analyzer_.call_function, and tests the case when that call returns an empty value
 */
TEST(visit_function_call, analyzer_call_function_returns_an_empty_value) {
    const auto &function_name = std::string{};
    const auto &function_arguments = values::Values{};
    const auto &function_return_value = values::Value{};
    auto analyzer = analyzer::MockAnalyzer();
    EXPECT_CALL(analyzer, call_function(function_name, values::ValuesEq(function_arguments)))
        .WillOnce(::testing::Return(function_return_value));
    auto visitor = analyzer::AnalyzeTreeGenAstVisitor(analyzer);

    auto name = cqasm::tree::make<ast::Identifier>();
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<error::AnalysisError>(::testing::HasSubstr("function implementation returned empty value")));
}
