#include "cqasm-result.hpp"
#include "v3x/cqasm.hpp"  // default_analyzer

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;
using namespace cqasm::result;


TEST(to_json, v3x_parser_errors) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_3.14"/"input.cq";
    cqasm::v3x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":3,"character":5},"end":{"line":3,"character":9}},"message":"mismatched input '3.14' expecting INTEGER_LITERAL","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fbit_array_definition%2Fbit_array_of_3.14%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_parser_ast) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto ast_result = cqasm::v3x::parser::parse_file(input_file_path.generic_string(), std::nullopt);
    auto json_result = to_json(ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"version":{"Version":{"items":"3","source_location":"<unknown file name>:1:9..10"}},"block":{"GlobalBlock":{"statements":[{"Variable":{"name":{"Identifier":{"name":"b"}},"typ":{"Type":{"name":{"Keyword":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"0"}},"source_location":"<unknown file name>:3:1..7"}},"annotations":"[]","source_location":"<unknown file name>:3:8..9"}}]}}}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_errors) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":3,"character":8},"end":{"line":3,"character":9}},"message":"found bit array of size <= 0","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fbit_array_definition%2Fbit_array_of_0_b%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_ast) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_17_b"/"input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"block":{"Block":{"statements":"[]"}},"functions":"[]","variables":[{"Variable":{"name":"b","typ":{"BitArray":{"size":"17"}},"annotations":"[]"}}]}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
