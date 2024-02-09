#include "cqasm-error.hpp"
#include "cqasm-tree.hpp"
#include "cqasm-version.hpp"
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
            auto one_statement_list = tree::make<ast::StatementList>();
            auto one_program = tree::make<ast::Program>(one_version, one_statement_list);
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

}  // namespace cqasm::v3x::analyzer
