#include "cqasm-error.hpp"
#include "mock_analyze_tree_gen_ast_visitor.hpp"
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


namespace cqasm::v3x::analyzer {

class VisitFunctionResolveTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void ExpectAnalyzerResolveFunctionOK(const values::Value &function_return_value) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillOnce(::testing::Return(function_return_value));
    }
    void ExpectAnalyzerResolveFunctionThrow(const std::string& error_message) {
        EXPECT_CALL(analyzer, resolve_function(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Throw(std::runtime_error{ error_message.c_str() }));
    }
    analyzer::MockAnalyzer analyzer;
    analyzer::AnalyzeTreeGenAstVisitor visitor{ analyzer };
};

/**
 * visit_function_call(name, arguments) is a private method, but we can test it through visit_function_call(node)
 */
TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_returns_a_non_empty_value) {
    const auto &function_return_value = values::Value{ cqasm::tree::make<values::ConstFloat>(0) };
    ExpectAnalyzerResolveFunctionOK(function_return_value);

    auto name = cqasm::tree::make<ast::Identifier>("function_that_returns_a_non_empty_value");
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    auto ret = std::any_cast<values::Value>(visitor.visit_function_call(function_call));
    EXPECT_TRUE(ret.equals(function_return_value));
}

TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_returns_an_empty_value) {
    const auto &function_return_value = values::Value{};
    ExpectAnalyzerResolveFunctionOK(function_return_value);

    auto name = cqasm::tree::make<ast::Identifier>("function_that_returns_an_empty_value");
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<error::AnalysisError>(::testing::HasSubstr("function implementation returned empty value")));
}

TEST_F(VisitFunctionResolveTest, analyzer_resolve_function_throws) {
    const auto &function_name = std::string{ "function_that_is_not_registered" };
    const auto &error_message = fmt::format("failed to resolve '{}'", function_name);
    ExpectAnalyzerResolveFunctionThrow(error_message);

    auto name = cqasm::tree::make<ast::Identifier>(function_name);
    auto arguments = cqasm::tree::make<ast::ExpressionList>(cqasm::tree::Any<ast::Expression>{});
    auto function_call = ast::FunctionCall{ name, arguments };
    EXPECT_THAT([&]() { visitor.visit_function_call(function_call); },
        ThrowsMessage<std::runtime_error>(::testing::HasSubstr(error_message.c_str())));
}


class VisitFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {
        EXPECT_EQ(analyzer.scope_stack().size(), 1);
    }

    std::string function_name = "f";
    tree::One<ast::Identifier> name = cqasm::tree::make<ast::Identifier>(function_name);
    tree::One<ast::LocalBlock> parameters = tree::make<ast::LocalBlock>();
    tree::Maybe<ast::Type> return_type_empty;
    tree::Maybe<ast::Type> return_type_bool =
        tree::make<ast::Type>(tree::make<ast::Keyword>("bool"), tree::Maybe<ast::IntegerLiteral>{});
    tree::Maybe<ast::Type> return_type_int =
        tree::make<ast::Type>(tree::make<ast::Keyword>("int"), tree::Maybe<ast::IntegerLiteral>{});
    tree::One<ast::LocalBlock> block = tree::make<ast::LocalBlock>();
    tree::One<ast::ReturnStatement> return_statement =
        tree::make<ast::ReturnStatement>(tree::make<ast::IntegerLiteral>(42));

    analyzer::MockAnalyzer analyzer;
    analyzer::MockAnalyzeTreeGenAstVisitor visitor{ analyzer };
};

TEST_F(VisitFunctionTest, function__no_return_type__yes_return_statement) {
    block->statements.add(return_statement);
    auto function = ast::Function{ name, parameters, return_type_empty, block };
    auto result = std::any_cast<tree::One<semantic::Function>>(visitor.visit_function(function));
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(analyzer.global_functions().empty());
    EXPECT_EQ(visitor.result().errors.size(), 1);
    EXPECT_THAT(visitor.result().errors[0].what(), ::testing::HasSubstr(
        "found return statement but function does not have a return type"));
}

TEST_F(VisitFunctionTest, function__yes_return_type__no_return_statement) {
    auto function = ast::Function{ name, parameters, return_type_int, block };
    auto result = std::any_cast<tree::One<semantic::Function>>(visitor.visit_function(function));
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(analyzer.global_functions().empty());
    EXPECT_EQ(visitor.result().errors.size(), 1);
    EXPECT_THAT(visitor.result().errors[0].what(), ::testing::HasSubstr(
        "function has a return type but return statement was not found"));
}

TEST_F(VisitFunctionTest, function__no_return_type__no_return_statement) {
    auto function = ast::Function{ name, parameters, return_type_empty, block };
    auto result = std::any_cast<tree::One<semantic::Function>>(visitor.visit_function(function));
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(analyzer.global_functions().size(), 1);
    EXPECT_TRUE(visitor.result().errors.empty());
}

TEST_F(VisitFunctionTest, function__yes_return_type__yes_return_statement) {
    block->statements.add(return_statement);
    auto function = ast::Function{ name, parameters, return_type_int, block };
    auto result = std::any_cast<tree::One<semantic::Function>>(visitor.visit_function(function));
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(analyzer.global_functions().size(), 1);
    EXPECT_TRUE(visitor.result().errors.empty());
}

TEST_F(VisitFunctionTest, return_value_cannot_be_promoted_to_return_type) {
    block->statements.add(return_statement);
    auto function = ast::Function{ name, parameters, return_type_bool, block };
    auto result = std::any_cast<tree::One<semantic::Function>>(visitor.visit_function(function));
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(analyzer.global_functions().empty());
    EXPECT_EQ(visitor.result().errors.size(), 1);
    EXPECT_THAT(visitor.result().errors[0].what(), ::testing::HasSubstr(
        "type of right-hand side (int) could not be coerced to left-hand side (bool)"));
}

}  // namespace cqasm::v3x::analyzer
