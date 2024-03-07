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
    void ExpectVersionParserReturnsVersionBiggerThanApiVersion() {
        version_parser = [this]() { return version_4_0; };
    }
    void ExpectVersionParserThrows() {
        version_parser = [this]() -> version::Version { throw error::ParseError{ version_parser_error_message }; };
    }
    void ExpectParserReturnsParseResult() {
        version_parser = [this]() { return version_3_0; };
        parser = [this]() {
            auto one_version = tree::make<ast::Version>( version_3_0 );
            auto one_global_block = tree::make<ast::GlobalBlock>();
            auto one_program = tree::make<ast::Program>(one_version, one_global_block);
            return parser::ParseResult{ one_program, error::ParseErrors{} };
        };
    }
    void ExpectParserThrows() {
        version_parser = [this]() { return version_3_0; };
        parser = [this]() -> parser::ParseResult { throw error::ParseError{ parser_error_message }; };
    }

    std::function<version::Version()> version_parser;
    std::function<parser::ParseResult()> parser;

    version::Version version_3_0 = "3.0";
    version::Version version_4_0 = "4.0";

    std::string version_parser_error_message = "version parser error";
    std::string parser_error_message = "parser error";
};

TEST_F(AnalyzerAnalyzeTest, version_parser_returns_version_bigger_than_api_version) {
    ExpectVersionParserReturnsVersionBiggerThanApiVersion();
    auto analyzer = Analyzer{};
    auto analysis_result = analyzer.analyze(version_parser, parser);
    const auto &errors = analysis_result.errors;
    EXPECT_TRUE(errors.size() == 1);
    EXPECT_EQ(
        fmt::format("{}", errors[0]),
        fmt::format("Error: cQASM file version is {}, but at most {} is supported here",
            version_4_0, analyzer.api_version)
    );
}
TEST_F(AnalyzerAnalyzeTest, version_parser_throws) {
    ExpectVersionParserThrows();
    auto analyzer = Analyzer{};
    auto analysis_result = analyzer.analyze(version_parser, parser);
    const auto &errors = analysis_result.errors;
    EXPECT_TRUE(errors.size() == 1);
    EXPECT_EQ(fmt::format("{}", errors[0]), fmt::format("Error: {}", version_parser_error_message));
}
TEST_F(AnalyzerAnalyzeTest, parser_returns_parse_result) {
    ExpectParserReturnsParseResult();
    auto analyzer = Analyzer{};
    auto analysis_result = analyzer.analyze(version_parser, parser);
    auto program = analysis_result.root->as_program();
    const auto &version = program->version->items;
    EXPECT_EQ(version, version_3_0);
}
TEST_F(AnalyzerAnalyzeTest, parser_throws) {
    ExpectParserThrows();
    auto analyzer = Analyzer{};
    EXPECT_THAT([&]() { (void) analyzer.analyze(version_parser, parser); },
        ThrowsMessage<error::ParseError>(::testing::HasSubstr(parser_error_message)));
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
