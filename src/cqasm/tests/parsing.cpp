#include <gtest/gtest.h> // googletest header file
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "dirent-compat.h"

#include <cqasm.hpp>

/**
 * Reads the given file into the given string buffer and returns true if it
 * exists, otherwise do nothing with the buffer and return false.
 */
bool read_file(const std::string &filename, std::string &output) {
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        return false;
    }
    output.clear();
    stream.seekg(0, std::ios::end);
    output.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);
    output.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return true;
}

/**
 * Overwrites or creates the given file with the given string.
 */
void write_file(const std::string &filename, const std::string &input) {
    std::ofstream stream(filename);
    stream << input;
}

/**
 * Tests the default parsing pipeline, varying only the input files, and
 * matching debug dumps of the output against golden versions.
 */
class ParsingTest : public ::testing::Test {
private:
    std::string path;

public:
    explicit ParsingTest(const std::string &path) : path(path) {}

    void TestBody() override {

        // Parse the test input file.
        std::string input;
        ASSERT_TRUE(read_file(path + "/input.cq", input));
        auto parse_result = cqasm::parser::parse_string(input, "input.cq");

        // Check the parse result.
        std::ostringstream ss;
        if (parse_result.errors.empty()) {
            ss << "SUCCESS" << std::endl;
            ss << *parse_result.root << std::endl;
        } else {
            ss << "ERROR" << std::endl;
            for (const auto &error : parse_result.errors) {
                ss << error << std::endl;
            }
        }
        std::string ast_result = ss.str();
        write_file(path + "/ast.actual.txt", ast_result);
        std::string ast_golden;
        EXPECT_TRUE(read_file(path + "/ast.golden.txt", ast_golden));
        EXPECT_TRUE(ast_result == ast_golden);

        // Stop if parsing failed.
        if (!parse_result.errors.empty()) {
            return;
        }

        // If there were no errors, try semantic analysis. We analyze using the
        // functions, error models, and instruction set available in the
        // compatibility layer, though this is copy-pasted in here.
        auto analyzer = cqasm::analyzer::Analyzer{"1.1"};
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
        analyzer.register_instruction("wait", "i", false, false);
        analyzer.register_instruction("reset-averaging", "", false, false);
        analyzer.register_instruction("reset-averaging", "Q", false, false);
        analyzer.register_instruction("load_state", "s", false, false);

        // Add a dynamic function in order to test the behavior of dynamic
        // function nodes.
        analyzer.register_function("or", "bb", [](const cqasm::values::Values &v) -> cqasm::values::Value {
            auto lhs = v[0];
            auto rhs = v[1];
            if (auto lhs_const = lhs->as_const_bool()) {
                if (lhs_const->value) {
                    return cqasm::tree::make<cqasm::values::ConstBool>(true);
                } else {
                    return rhs;
                }
            }
            if (auto rhs_const = lhs->as_const_bool()) {
                if (rhs_const->value) {
                    return cqasm::tree::make<cqasm::values::ConstBool>(true);
                } else {
                    return lhs;
                }
            }
            return cqasm::tree::make<cqasm::values::Function>("operator||", v, cqasm::tree::make<cqasm::types::Bool>());
        });

        // Run the actual semantic analysis.
        auto analysis_result = analyzer.analyze(*parse_result.root->as_program());

        // Check the analysis results.
        ss.str("");
        if (analysis_result.errors.empty()) {
            ss << "SUCCESS" << std::endl;
            ss << *analysis_result.root << std::endl;
        } else {
            ss << "ERROR" << std::endl;
            for (const auto &error : analysis_result.errors) {
                ss << error << std::endl;
            }
        }
        std::string semantic_result = ss.str();
        write_file(path + "/semantic.actual.txt", semantic_result);

        std::string semantic_golden;
        EXPECT_TRUE(read_file(path + "/semantic.golden.txt", semantic_golden));
        EXPECT_TRUE(semantic_result == semantic_golden);

        // If analysis was successful, serialize and deserialize to check if
        // result is the same.
        if (analysis_result.errors.empty()) {
            auto serdes_root = tree::base::deserialize<cqasm::semantic::Program>(
                tree::base::serialize(analysis_result.root));
            ss.str("");
            ss << "SUCCESS" << std::endl;
            ss << *serdes_root << std::endl;
            std::string serdes_result = ss.str();
            write_file(path + "/serdes.actual.txt", serdes_result);
            EXPECT_TRUE(serdes_result == semantic_golden);
        }
    }

};

/**
 * Entry point.
 */
int main(int argc, char** argv) {
    testing::InitGoogleTest();

    // Discover the tests. They should live in a directory tree with the
    // following structure:
    //
    // <CWD>
    //  '- parsing
    //      |- <suite-name>                   test suite directory
    //      |   |- <test-name>                test case directory
    //      |   |   |- input.cq               the input file
    //      |   |   |- ast.golden             the golden AST or parse error dump
    //      |   |   |- [ast.actual.txt]       output file with the actual data
    //      |   |   |- [semantic.golden.txt]  the golden semantic tree or
    //      |   |   |                           analysis error dump, if parsing
    //      |   |   |                           should succeed
    //      |   |   '- [semantic.actual.txt]  output file with the actual data,
    //      |   |                               if parsing actually succeeded
    //      |   |- ...                        other test case directories
    //      |   :
    //      |- ...                            other test suite directories
    //      :
    DIR *parsing_dir = opendir("parsing");
    if (!parsing_dir) {
        throw std::runtime_error("failed to open dir for parsing tests");
    }
    while (dirent *parsing_dir_ent = readdir(parsing_dir)) {
        if (parsing_dir_ent->d_name[0] == '.') {
            continue;
        }
        auto suite_name = std::string(parsing_dir_ent->d_name);
        auto suite_path = "parsing/" + suite_name;
        DIR *suite_dir = opendir(suite_path.c_str());
        if (!suite_dir) {
            continue;
        }
        while (dirent *suite_dir_ent = readdir(suite_dir)) {
            if (suite_dir_ent->d_name[0] == '.') {
                continue;
            }
            auto test_name = std::string(suite_dir_ent->d_name);
            auto test_path = suite_path + "/";
            test_path += test_name;
            {
                std::ifstream stream(test_path + "/input.cq");
                if (!stream.is_open()) {
                    continue;
                }
            }
            ::testing::RegisterTest(
                suite_name.c_str(), test_name.c_str(),
                nullptr, nullptr,
                __FILE__, __LINE__,
                [=]() -> ParsingTest* { return new ParsingTest(test_path); });
        }
        closedir(suite_dir);
    }
    closedir(parsing_dir);

    // FIXME: this should be in an initialization function or something.
    tree::annotatable::serdes_registry.add<cqasm::parser::SourceLocation>("location");

    return RUN_ALL_TESTS();
}
