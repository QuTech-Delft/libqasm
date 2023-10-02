#include "parsing.hpp"
#include "v1x/cqasm.hpp"
#include "v1x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-parse-helper.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <streambuf>
#include <string>


namespace cq1x = cqasm::v1x;
namespace cq3x = cqasm::v3x;
namespace fs = std::filesystem;


/**
 * Reads the given file into the given string buffer and returns true if it
 * exists, otherwise do nothing with the buffer and return false.
 */
bool read_file(const fs::path &file_path, std::string &output) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        return false;
    }
    output.clear();
    ifs.seekg(0, std::ios::end);
    output.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    output.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    return true;
}

/**
 * Overwrites or creates the given file with the given string.
 */
void write_file(const fs::path &file_path, const std::string &input) {
    std::ofstream stream(file_path, std::ios::binary | std::ios::out);  // always write LF, i.e., avoid CR+LF in Windows
    stream << input;
}

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
        cq1x::parser::ParseResult parse_result{};
        auto version = cqasm::version::parse_string(input, "input.cq");

        if (auto compare_result = version.compare("1.2"); compare_result <= 0) {
            parse_result = cq1x::parser::parse_string(input, "input.cq");
        } else if (compare_result = version.compare("3.0"); compare_result == 0) {
            parse_result = cq3x::parser::parse_string(input, "input.cq");
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
        write_file(ast_actual_file_path, ast_actual_file_contents);
        std::string ast_golden_file_contents{};
        auto ast_golden_file_path = path_ / "ast.golden.txt";
        EXPECT_TRUE(read_file(ast_golden_file_path, ast_golden_file_contents));
        EXPECT_TRUE(ast_actual_file_contents == ast_golden_file_contents);

        // Stop if parsing failed
        if (!parse_result.errors.empty()) {
            return;
        }

        // Try different API levels
        for (const auto &api_version : std::vector<std::string>({"1.0", "1.1", "1.2"})) {
            // If there were no errors, try semantic analysis.
            // We analyze using the functions, error models, and instruction set available in the compatibility layer,
            // though this is copy-pasted in here
            auto analyzer = cq1x::analyzer::Analyzer{api_version};
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

void register_v1x_tests(const fs::path& subdir) {
    // Discover the tests.
    // They should live in a directory tree with the following structure:
    //
    // <CWD>
    // |-  res/v1x/<subdir>
    //     |- <suite-name>                   test suite directory
    //     |   |- <test-name>                test case directory
    //     |   |   |- input.cq               the input file
    //     |   |   |- ast.golden             the golden AST or parse error dump
    //     |   |   |- [ast.actual.txt]       output file with the actual data
    //     |   |   |- [semantic.golden.txt]  the golden semantic tree or analysis error dump,
    //     |   |   |                         if parsing should succeed
    //     |   |   '- [semantic.actual.txt]  output file with the actual data, if parsing actually succeeded
    //     |   |- ...                        other test case directories
    //     |   :
    //     |- ...                            other test suite directories
    //     :
    auto subdir_path = fs::path{ "res" } / "v1x" / subdir;
    if (!fs::exists(subdir_path)) {
        throw std::runtime_error(fmt::format("failed to open v1x tests subdir '{}'", subdir_path.generic_string()));
    } else if (!fs::is_directory(subdir_path)) {
        throw std::runtime_error(fmt::format("'{}' is not a directory", subdir_path.generic_string()));
    }
    for (const fs::directory_entry& suite: fs::directory_iterator(subdir_path)) {
        if (fs::is_directory(suite)) {
            auto suite_name = suite.path().filename().string();
            for (const fs::directory_entry& test: fs::directory_iterator(suite.path())) {
                auto test_name = test.path().filename().string();
                if (fs::is_directory(test)) {
                    auto input_cq_path = test.path() / "input.cq";
                    if (fs::exists(input_cq_path)) {
                        ::testing::RegisterTest(
                            suite_name.c_str(), test_name.c_str(),
                            nullptr, nullptr,
                            __FILE__, __LINE__,
                            [=]() -> ParsingTest* { return new ParsingTest(test.path()); });
                    }
                }
            }
        }
    }
}

void register_v1x_tests() {
    register_v1x_tests("parsing");
    //register_v1x_tests("toy-v1x-parsing");
}
