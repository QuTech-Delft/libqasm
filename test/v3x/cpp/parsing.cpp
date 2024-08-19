#include "parsing.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>
#include <string>

#include "test-register.hpp"
#include "utils.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm.hpp"

namespace cqasm::v3x::test {

namespace cq3x = cqasm::v3x;
namespace fs = std::filesystem;

/**
 * Tests the default parsing pipeline, varying only the input files, and
 * matching debug dumps of the output against golden versions.
 */
class ParsingTest : public ::testing::Test {
    fs::path path_{};

public:
    explicit ParsingTest(fs::path path)
    : path_{ std::move(path) } {}

    void TestBody() override {
        // Parse the test input file
        std::string input{};
        ASSERT_TRUE(cqasm::test::read_file(path_ / "input.cq", input));
        cq3x::parser::ParseResult parse_result{};
        parse_result = cq3x::parser::parse_string(input, "input.cq");

        // Check the debug dump of the parse result
        std::string ast_actual_file_contents = parse_result.errors.empty()
            ? fmt::format("SUCCESS\n{}\n", *parse_result.root)
            : fmt::format("ERROR\n{}\n", fmt::join(parse_result.errors, "\n"));
        std::string ast_golden_file_contents{};
        auto ast_actual_file_path = path_ / "ast.actual.txt";
        auto ast_golden_file_path = path_ / "ast.golden.txt";
        cqasm::test::write_file(ast_actual_file_path, ast_actual_file_contents);
        EXPECT_TRUE(cqasm::test::read_file(ast_golden_file_path, ast_golden_file_contents));
        EXPECT_TRUE(ast_actual_file_contents == ast_golden_file_contents);

        // Check the JSON dump of the parse result
        if (parse_result.errors.empty()) {
            if (auto json_golden_file_path = path_ / "ast.golden.json"; fs::exists(json_golden_file_path)) {
                auto json_actual_file_path = path_ / "ast.actual.json";
                std::string json_actual_file_contents{};
                std::string json_golden_file_contents{};
                {
                    std::ofstream json_actual_ofs{ json_actual_file_path };
                    parse_result.root->dump_json(json_actual_ofs);
                }
                EXPECT_TRUE(cqasm::test::read_file(json_actual_file_path, json_actual_file_contents));
                EXPECT_TRUE(cqasm::test::read_file(json_golden_file_path, json_golden_file_contents));
                EXPECT_TRUE(json_actual_file_contents == json_golden_file_contents);
            }
        }

        // Stop if parsing failed
        if (!parse_result.errors.empty()) {
            return;
        }

        // If there were no errors, try semantic analysis
        for (const auto &api_version : std::vector<std::string>({ "3.0" })) {
            auto analyzer = cq3x::analyzer::Analyzer{ api_version };

            analyzer.register_default_constants();
            analyzer.register_default_functions();
            analyzer.register_default_instructions();

            // Run the actual semantic analysis
            auto analysis_result = analyzer.analyze(*parse_result.root->as_program());

            // Check the debug dump of the analysis result
            std::string semantic_actual_file_contents = analysis_result.errors.empty()
                ? fmt::format("SUCCESS\n{}\n", *analysis_result.root)
                : fmt::format("ERROR\n{}\n", fmt::join(analysis_result.errors, "\n"));
            auto semantic_actual_file_path = path_ / fmt::format("semantic.{}.actual.txt", api_version);
            auto semantic_golden_file_path = path_ / fmt::format("semantic.{}.golden.txt", api_version);
            cqasm::test::write_file(semantic_actual_file_path, semantic_actual_file_contents);
            std::string semantic_golden_file_contents{};
            EXPECT_TRUE(cqasm::test::read_file(semantic_golden_file_path, semantic_golden_file_contents));
            EXPECT_TRUE(semantic_actual_file_contents == semantic_golden_file_contents);

            // Check the JSON dump of the analysis result
            if (analysis_result.errors.empty()) {
                if (auto semantic_json_golden_file_path = path_ / fmt::format("semantic.{}.golden.json", api_version);
                    fs::exists(semantic_json_golden_file_path)) {
                    auto semantic_json_actual_file_path = path_ / fmt::format("semantic.{}.actual.json", api_version);
                    std::string semantic_json_actual_file_contents{};
                    std::string semantic_json_golden_file_contents{};
                    {
                        std::ofstream semantic_json_actual_ofs{ semantic_json_actual_file_path };
                        analysis_result.root->dump_json(semantic_json_actual_ofs);
                    }
                    EXPECT_TRUE(
                        cqasm::test::read_file(semantic_json_actual_file_path, semantic_json_actual_file_contents));
                    EXPECT_TRUE(
                        cqasm::test::read_file(semantic_json_golden_file_path, semantic_json_golden_file_contents));
                    EXPECT_TRUE(semantic_json_actual_file_contents == semantic_json_golden_file_contents);
                }
            }

            if (analysis_result.errors.empty()) {
                ::tree::base::serialize(analysis_result.root);
            }
        }
    }
};

void register_tests() {
    cqasm::test::register_tests(fs::path{ "res" } / "v3x" / "parsing",
        [=](fs::path test_path) -> ParsingTest * { return new ParsingTest(std::move(test_path)); });
}

}  // namespace cqasm::v3x::test
