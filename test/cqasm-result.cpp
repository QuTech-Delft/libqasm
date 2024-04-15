#include "cqasm-result.hpp"

#include <gtest/gtest.h>

#include <filesystem>

#include "v3x/cqasm.hpp"  // default_analyzer

namespace fs = std::filesystem;
using namespace cqasm::result;

TEST(to_json, v3x_parser_errors) {
    auto input_file_path =
        fs::path{ "res" } / "v3x" / "parsing" / "qubit_array_definition" / "qubit_array_of_17" / "input.cq";
    cqasm::v3x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":3,"character":10},"end":{"line":3,"character":11}},"message":"missing IDENTIFIER at '\u005Cn'","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fqubit_array_definition%2Fqubit_array_of_17%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_parser_ast) {
    auto input_file_path =
        fs::path{ "res" } / "v3x" / "parsing" / "qubit_array_definition" / "qubit_array_of_0_q" / "input.cq";
    auto ast_result = cqasm::v3x::parser::parse_file(input_file_path.generic_string(), std::nullopt);
    auto json_result = to_json(ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"version":{"Version":{"items":"3","source_location":"<unknown file name>:1:9..10"}},"block":{"GlobalBlock":{"qubit_variable_declaration":{"Variable":{"name":{"Identifier":{"name":"q"}},"typ":{"Type":{"name":{"Keyword":{"name":"qubit"}},"size":{"IntegerLiteral":{"value":"0"}},"source_location":"<unknown file name>:3:1..9"}},"annotations":"[]","source_location":"<unknown file name>:3:10..11"}},"gates":"[]","measure_instruction":"-"}}}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_errors) {
    auto input_file_path =
        fs::path{ "res" } / "v3x" / "parsing" / "qubit_array_definition" / "qubit_array_of_0_q" / "input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":3,"character":10},"end":{"line":3,"character":11}},"message":"found qubit array of size <= 0","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fqubit_array_definition%2Fqubit_array_of_0_q%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_ast) {
    auto input_file_path =
        fs::path{ "res" } / "v3x" / "parsing" / "qubit_array_definition" / "qubit_array_of_17_q" / "input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"qubit_variable_declaration":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"17"}},"annotations":"[]"}},"block":{"Block":{"statements":"[]"}}}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
