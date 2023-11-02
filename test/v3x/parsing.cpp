#include "parsing.hpp"
#include "test-register.hpp"
#include "utils.hpp"
#include "v3x/cqasm.hpp"
#include "v3x/cqasm-parse-helper.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <string>


namespace cqasm::test {

namespace cq3x = cqasm::v3x;
namespace fs = std::filesystem;

/**
 * Tests the default parsing pipeline, varying only the input files, and
 * matching debug dumps of the output against golden versions.
 */
class ParsingTest : public ::testing::Test {
private:
    fs::path path_{};

public:
    explicit ParsingTest(fs::path path) : path_{ std::move(path) } {}

    void TestBody() override {
        // Parse the test input file.
        std::string input{};
        ASSERT_TRUE(read_file(path_ / "input.cq", input));
        cq3x::parser::ParseResult parse_result{};
        parse_result = cq3x::parser::parse_string(input, "input.cq");

        // Check the parse result
        std::string ast_actual_file_contents{};
        if (parse_result.errors.empty()) {
            ast_actual_file_contents = fmt::format("SUCCESS\n{}\n", *parse_result.root);
        } else {
            ast_actual_file_contents = fmt::format("ERROR\n{}\n", fmt::join(parse_result.errors, "\n"));
        }

        auto ast_actual_file_path = path_ / "ast.actual.txt";
        write_file(ast_actual_file_path, ast_actual_file_contents);
        std::string ast_golden_file_contents{};
        auto ast_golden_file_path = path_ / "ast.golden.txt";
        EXPECT_TRUE(read_file(ast_golden_file_path, ast_golden_file_contents));
        EXPECT_TRUE(ast_actual_file_contents == ast_golden_file_contents);

        // Stop if parsing failed
        if (!parse_result.errors.empty()) {
            return;
        }

        // If there were no errors, try semantic analysis
        for (const auto &api_version : std::vector<std::string>( { "3.0" } )) {
            auto analyzer = cq3x::analyzer::Analyzer{ api_version };

            analyzer.register_default_mappings();

            analyzer.register_instruction("cnot", "QQ");
            analyzer.register_instruction("cnot", "VV");
            analyzer.register_instruction("cr", "QQr");
            analyzer.register_instruction("crk", "QQi");
            analyzer.register_instruction("cz", "QQ");
            analyzer.register_instruction("h", "Q");
            analyzer.register_instruction("h", "V");
            analyzer.register_instruction("i", "Q");
            analyzer.register_instruction("measure", "QB");
            analyzer.register_instruction("measure", "VW");
            analyzer.register_instruction("mx90", "Q");
            analyzer.register_instruction("my90", "Q");
            analyzer.register_instruction("rx", "Qr");
            analyzer.register_instruction("ry", "Qr");
            analyzer.register_instruction("rz", "Qr");
            analyzer.register_instruction("s", "Q");
            analyzer.register_instruction("sdag", "Q");
            analyzer.register_instruction("x", "Q");
            analyzer.register_instruction("x90", "Q");
            analyzer.register_instruction("y", "Q");
            analyzer.register_instruction("y90", "Q");
            analyzer.register_instruction("z", "Q");

            // Run the actual semantic analysis
            auto analysis_result = analyzer.analyze(*parse_result.root->as_program());

            // Check the analysis results
            std::string semantic_actual_file_contents{};
            if (analysis_result.errors.empty()) {
                semantic_actual_file_contents = fmt::format("SUCCESS\n{}\n", *analysis_result.root);
            } else {
                semantic_actual_file_contents = fmt::format("ERROR\n{}\n", fmt::join(analysis_result.errors, "\n"));
            }
            auto semantic_actual_file_path = path_ / fmt::format("semantic.{}.actual.txt", api_version);
            write_file(semantic_actual_file_path, semantic_actual_file_contents);
            std::string semantic_golden_file_contents{};
            auto semantic_golden_file_path = path_ / fmt::format("semantic.{}.golden.txt", api_version);
            EXPECT_TRUE(read_file(semantic_golden_file_path, semantic_golden_file_contents));
            EXPECT_TRUE(semantic_actual_file_contents == semantic_golden_file_contents);

            if (analysis_result.errors.empty()) {
                ::tree::base::serialize(analysis_result.root);
            }
        }
    }
};


void register_v3x_tests() {
    register_tests(
        fs::path{ "res" } / "v3x" / "parsing",
        [=](fs::path test_path) -> ParsingTest* { return new ParsingTest(std::move(test_path)); }
    );
}

}  // namespace cqasm::test
