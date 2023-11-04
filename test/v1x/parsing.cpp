#include "parsing.hpp"
#include "test-register.hpp"
#include "utils.hpp"
#include "v1x/cqasm.hpp"
#include "v1x/cqasm-parse-helper.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <sstream>
#include <string>


namespace cqasm::v1x::test {

namespace cq1x = cqasm::v1x;
namespace fs = std::filesystem;

/**
 * Tests the default parsing pipeline, varying only the input files, and
 * matching debug dumps of the output against golden versions.
 */
class ParsingTest : public ::testing::Test {
    fs::path path_{};

public:
    explicit ParsingTest(fs::path path) : path_{ std::move(path) } {}

    void TestBody() override {
        // Parse the test input file
        std::string input{};
        ASSERT_TRUE(cqasm::test::read_file(path_ / "input.cq", input));
        cq1x::parser::ParseResult parse_result{};
        auto version = cqasm::version::parse_string(input, "input.cq");

        if (version.less_than_or_equal("1.2")) {
            parse_result = cq1x::parser::parse_string(input, "input.cq");
        } else {
            parse_result.errors.push_back(fmt::format("detected version {}", version));
        }

        // Check the parse result
        std::string ast_actual_file_contents{};
        if (parse_result.errors.empty()) {
            ast_actual_file_contents = fmt::format("SUCCESS\n{}\n", *parse_result.root);
        } else {
            ast_actual_file_contents = fmt::format("ERROR\n{}\n", fmt::join(parse_result.errors, "\n"));
        }

        auto ast_actual_file_path = path_ / "ast.actual.txt";
        cqasm::test::write_file(ast_actual_file_path, ast_actual_file_contents);
        std::string ast_golden_file_contents{};
        auto ast_golden_file_path = path_ / "ast.golden.txt";
        EXPECT_TRUE(cqasm::test::read_file(ast_golden_file_path, ast_golden_file_contents));
        EXPECT_TRUE(ast_actual_file_contents == ast_golden_file_contents);

        // Stop if parsing failed
        if (!parse_result.errors.empty()) {
            return;
        }

        // Try different API levels
        for (const auto &api_version : std::vector<std::string>( { "1.0", "1.1", "1.2" } )) {
            // If there were no errors, try semantic analysis.
            // We analyze using the functions, error models, and instruction set available in the compatibility layer,
            // though this is copy-pasted in here
            auto analyzer = cq1x::analyzer::Analyzer{ api_version };
            analyzer.register_default_functions_and_mappings();
            std::ostringstream args;
            for (int i = 0; i <= 50; i++) {
                analyzer.register_error_model("depolarizing_channel", args.str());
                args << "r";
            }
            analyzer.register_instruction("measure_all", "", false, false);
            analyzer.register_instruction("measure_parity", "QaQa", false, false, false, true);
            analyzer.register_instruction("x", "Q");
            analyzer.register_instruction("y", "Q");
            analyzer.register_instruction("z", "Q");
            analyzer.register_instruction("i", "Q");
            analyzer.register_instruction("h", "Q");
            analyzer.register_instruction("x90", "Q");
            analyzer.register_instruction("y90", "Q");
            analyzer.register_instruction("mx90", "Q");
            analyzer.register_instruction("my90", "Q");
            analyzer.register_instruction("s", "Q");
            analyzer.register_instruction("sdag", "Q");
            analyzer.register_instruction("t", "Q");
            analyzer.register_instruction("tdag", "Q");
            analyzer.register_instruction("u", "Qu");
            analyzer.register_instruction("prep", "Q", false);
            analyzer.register_instruction("prep_x", "Q", false);
            analyzer.register_instruction("prep_y", "Q", false);
            analyzer.register_instruction("prep_z", "Q", false);
            analyzer.register_instruction("measure", "Q", false);
            analyzer.register_instruction("measure_x", "Q", false);
            analyzer.register_instruction("measure_y", "Q", false);
            analyzer.register_instruction("measure_z", "Q", false);
            analyzer.register_instruction("rx", "Qr");
            analyzer.register_instruction("ry", "Qr");
            analyzer.register_instruction("rz", "Qr");
            analyzer.register_instruction("cnot", "QQ");
            analyzer.register_instruction("cz", "QQ");
            analyzer.register_instruction("swap", "QQ");
            analyzer.register_instruction("cr", "QQr");
            analyzer.register_instruction("crk", "QQi");
            analyzer.register_instruction("toffoli", "QQQ");
            analyzer.register_instruction("not", "B");
            analyzer.register_instruction("display", "", false, false);
            analyzer.register_instruction("display", "B", false, false);
            analyzer.register_instruction("display_binary", "", false, false);
            analyzer.register_instruction("display_binary", "B", false, false);
            analyzer.register_instruction("skip", "i", false, false);
            analyzer.register_instruction("wait", "Qi", false, false);
            analyzer.register_instruction("barrier", "Q", false, false);
            analyzer.register_instruction("reset-averaging", "", false, false);
            analyzer.register_instruction("reset-averaging", "Q", false, false);
            analyzer.register_instruction("load_state", "s", false, false);

            // Add a dynamic function in order to test the behavior of dynamic function nodes.
            if (api_version != "1.0") {
                analyzer.register_function("or", "bb",
                    [](const cq1x::values::Values &v) -> cq1x::values::Value {
                        auto lhs = v[0];
                        auto rhs = v[1];
                        if (auto lhs_const = lhs->as_const_bool()) {
                            if (lhs_const->value) {
                                return cqasm::tree::make<cq1x::values::ConstBool>(true);
                            } else {
                                return rhs;
                            }
                        }
                        if (auto rhs_const = lhs->as_const_bool()) {
                            if (rhs_const->value) {
                                return cqasm::tree::make<cq1x::values::ConstBool>(true);
                            } else {
                                return lhs;
                            }
                        }
                        return cqasm::tree::make<cq1x::values::Function>(
                            "operator||", v, cqasm::tree::make<cq1x::types::Bool>());
                    }
                );
                analyzer.register_function("operator<", "ii",
                    [](const cq1x::values::Values &v) -> cq1x::values::Value {
                        const auto& lhs = v[0];
                        const auto& rhs = v[1];
                        if (auto lhs_const = lhs->as_const_int()) {
                            if (auto rhs_const = rhs->as_const_int()) {
                                return cqasm::tree::make<cq1x::values::ConstBool>(
                                    lhs_const->value < rhs_const->value
                                );
                            }
                        }
                        return cqasm::tree::make<cq1x::values::Function>(
                            "operator<", v, cqasm::tree::make<cq1x::types::Bool>());
                    }
                );
                analyzer.register_function("operator+", "ii",
                    [](const cq1x::values::Values &v) -> cq1x::values::Value {
                        const auto& lhs = v[0];
                        const auto& rhs = v[1];
                        if (auto lhs_const = lhs->as_const_int()) {
                            if (auto rhs_const = rhs->as_const_int()) {
                                return cqasm::tree::make<cq1x::values::ConstInt>(
                                    lhs_const->value + rhs_const->value
                                );
                            }
                        }
                        return cqasm::tree::make<cq1x::values::Function>(
                            "operator+", v, cqasm::tree::make<cq1x::types::Int>());
                    }
                );
            }

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
            cqasm::test::write_file(semantic_actual_file_path, semantic_actual_file_contents);
            std::string semantic_golden_file_contents{};
            auto semantic_golden_file_path = path_ / fmt::format("semantic.{}.golden.txt", api_version);
            EXPECT_TRUE(cqasm::test::read_file(semantic_golden_file_path, semantic_golden_file_contents));
            EXPECT_TRUE(semantic_actual_file_contents == semantic_golden_file_contents);

            if (analysis_result.errors.empty()) {
                ::tree::base::serialize(analysis_result.root);
            }
        }
    }
};


void register_tests() {
    cqasm::test::register_tests(
        fs::path{ "res" } / "v1x" / "parsing",
        [=](fs::path test_path) -> ParsingTest* { return new ParsingTest(std::move(test_path)); }
    );
}

}  // namespace cqasm::v1x::test
