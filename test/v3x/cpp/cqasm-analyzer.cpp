#include "cqasm-error.hpp"
#include "cqasm-tree.hpp"
#include "cqasm-version.hpp"
#include "mock_analyzer.hpp"
#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast.hpp"
#include "v3x/cqasm-parse-result.hpp"

#include <functional>
#include <gmock/gmock.h>

using namespace ::testing;


namespace cqasm::v3x::analyzer {

class AnalyzerAnalyzeTest : public ::testing::Test {
protected:
    void SetUp() override {}

    version::Version version_3_0 = "3.0";

    tree::One<ast::Version> version = tree::make<ast::Version>(version_3_0);
    tree::One<ast::GlobalBlock> global_block = tree::make<ast::GlobalBlock>();
    tree::One<ast::Program> program = tree::make<ast::Program>(version, global_block);
    parser::ParseResult parse_result_ok = parser::ParseResult{ program, error::ParseErrors{} };

    std::string parse_error_message{ "parse error" };
    error::ParseErrors parse_errors{ error::ParseError{ parse_error_message } };
    parser::ParseResult parse_result_errors = parser::ParseResult{ tree::make<ast::Program>(), parse_errors };
};

TEST_F(AnalyzerAnalyzeTest, parser_returns_parse_result) {
    auto analyzer = Analyzer{};
    const auto &analysis_result = analyzer.analyze(parse_result_ok);
    auto program = analysis_result.root->as_program();
    const auto &version = program->version->items;
    EXPECT_EQ(version, version_3_0);
}
TEST_F(AnalyzerAnalyzeTest, parser_throws) {
    auto analyzer = Analyzer{};
    const auto &analysis_result = analyzer.analyze(parse_result_errors);
    const auto &error = analysis_result.errors[0];
    EXPECT_THAT(error.what(), ::testing::HasSubstr(parse_error_message));
}

TEST(Analyzer, constructor) {
    MockAnalyzer analyzer{};
    EXPECT_EQ(analyzer.scope_stack().size(), 1);
    EXPECT_FALSE(analyzer.current_block().empty());
    EXPECT_TRUE(analyzer.current_variables().empty());
    EXPECT_TRUE(analyzer.global_functions().empty());
}
TEST(Analyzer, push_scope) {
    MockAnalyzer analyzer{};
    analyzer.push_scope();
    EXPECT_EQ(analyzer.scope_stack().size(), 2);
}
TEST(Analyzer, pop_scope) {
    MockAnalyzer analyzer{};
    analyzer.pop_scope();
    EXPECT_TRUE(analyzer.scope_stack().empty());
}
TEST(Analyzer, add_statement_to_current_scope) {
    MockAnalyzer analyzer{};
    auto statement = tree::make<semantic::ReturnStatement>(tree::make<values::ConstInt>(42));
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
}
TEST(Analyzer, add_statement_with_source_location_information_to_current_scope) {
    MockAnalyzer analyzer{};
    auto statement = tree::make<semantic::ReturnStatement>(tree::make<values::ConstInt>(42));
    const auto &statement_source_location = annotations::SourceLocation{ "input.cq", { { 10, 20 }, { 11, 10 } } };
    statement->set_annotation(statement_source_location);
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
    const auto &block_source_location = analyzer.current_block()->get_annotation<annotations::SourceLocation>();
    EXPECT_EQ(block_source_location.file_name, statement_source_location.file_name);
    EXPECT_EQ(block_source_location.range, statement_source_location.range);
}
TEST(Analyzer, add_statement_with_source_location_information_to_current_scope_and_block_has_source_location_information) {
    MockAnalyzer analyzer{};
    //     10 15 20 25 30
    //  5      <
    //  8               >
    const auto &block_initial_source_location = annotations::SourceLocation{ "input.cq", { { 5, 15 }, { 8, 30 } } };
    analyzer.current_block()->set_annotation(block_initial_source_location);
    auto statement = tree::make<semantic::ReturnStatement>(tree::make<values::ConstInt>(42));
    //     10 15 20 25 30
    // 10   <
    // 11         >
    const auto &statement_source_location = annotations::SourceLocation{ "input.cq", { { 10, 10 }, { 11, 20 } } };
    statement->set_annotation(statement_source_location);
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
    //     10 15 20 25 30
    //  5      <
    // 11         >
    const auto &block_final_source_location = analyzer.current_block()->get_annotation<annotations::SourceLocation>();
    EXPECT_EQ(block_final_source_location.file_name, "input.cq");
    EXPECT_EQ(block_final_source_location.range, (annotations::SourceLocation::Range{ { 5, 15 }, { 11, 20 } }));
}

}  // namespace cqasm::v3x::analyzer
