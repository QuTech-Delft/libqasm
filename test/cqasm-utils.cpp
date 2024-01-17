#include "cqasm-utils.hpp"
#include "v1x/cqasm.hpp"  // default_analyzer
#include "v3x/cqasm.hpp"  // default_analyzer

#include <filesystem>
#include <gmock/gmock.h>

namespace fs = std::filesystem;
using namespace cqasm::utils;


TEST(to_lowercase, empty_string) { EXPECT_TRUE(to_lowercase("").empty()); }
TEST(to_lowercase, all_lowercase) { EXPECT_EQ(to_lowercase("abcd"), "abcd"); }
TEST(to_lowercase, all_upercase) { EXPECT_EQ(to_lowercase("ABCD"), "abcd"); }
TEST(to_lowercase, contains_lowercase_and_uppercase) { EXPECT_EQ(to_lowercase("abCD"), "abcd"); }
TEST(to_lowercase, contains_numbers) { EXPECT_EQ(to_lowercase("abCD123"), "abcd123"); }
TEST(to_lowercase, contains_punctuation_signs) { EXPECT_EQ(to_lowercase("[abCD!]"), "[abcd!]"); }
TEST(to_lowercase, does_not_contain_letters) { EXPECT_EQ(to_lowercase("123"), "123"); }

TEST(equal_case_insensitive, empty_strings) { EXPECT_TRUE(equal_case_insensitive("", "")); }
TEST(equal_case_insensitive, both_all_lowercase) { EXPECT_TRUE(equal_case_insensitive("abcd", "abcd")); }
TEST(equal_case_insensitive, both_all_uppercase) { EXPECT_TRUE(equal_case_insensitive("ABCD", "ABCD")); }
TEST(equal_case_insensitive, both_contain_lowercase_and_uppercase) {
    EXPECT_TRUE(equal_case_insensitive("abCD", "ABcd")); }
TEST(equal_case_insensitive, both_contain_numbers) { EXPECT_TRUE(equal_case_insensitive("[abCD!]", "[ABcd!]")); }
TEST(equal_case_insensitive, non_contain_letters) { EXPECT_TRUE(equal_case_insensitive("123", "123")); }
TEST(equal_case_insensitive, different_strings) { EXPECT_FALSE(equal_case_insensitive("123", "ABC")); }


TEST(to_json, v1x_parser_errors) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"grammar"/"expression_recovery"/"input.cq";
    cqasm::v1x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":["res/v1x/parsing/grammar/expression_recovery/input.cq:4:21: syntax error, unexpected ','","res/v1x/parsing/grammar/expression_recovery/input.cq:4:31: syntax error, unexpected ','","res/v1x/parsing/grammar/expression_recovery/input.cq:4:37: syntax error, unexpected ')', expecting ']'","res/v1x/parsing/grammar/expression_recovery/input.cq:4:61: syntax error, unexpected ','","res/v1x/parsing/grammar/expression_recovery/input.cq:4:67: syntax error, unexpected ')', expecting ',' or ']'","Failed to parse res/v1x/parsing/grammar/expression_recovery/input.cq"]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v1x_parser_ast) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"misc"/"wait_not_ok_1"/"input.cq";
    auto ast_result = cqasm::v1x::parser::parse_file(input_file_path.generic_string());
    auto json_result = to_json(ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"version":{"Version":{"items":"1.0","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:1:9..12"}},"num_qubits":{"IntegerLiteral":{"value":"2","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:2:8..9"}},"statements":{"StatementList":{"items":[{"Bundle":{"items":[{"Instruction":{"name":{"Identifier":{"name":"wait","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..5"}},"condition":"-","operands":{"ExpressionList":{"items":[{"IntegerLiteral":{"value":"1","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:6..7"}}],"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:6..7"}},"annotations":"[]","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7"}}],"annotations":"[]","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7"}}],"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:2:1..5:7"}},"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:1:1..5:1"}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v1x_analyzer_errors) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"misc"/"wait_not_ok_1"/"input.cq";
    auto semantic_ast_result = cqasm::v1x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":["Error at res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7: failed to resolve overload for wait with argument pack (int)"]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v1x_analyzer_ast) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"grammar"/"map"/"input.cq";
    auto semantic_ast_result = cqasm::v1x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"api_version":"1.0","version":{"Version":{"items":"1.0","source_location":"res/v1x/parsing/grammar/map/input.cq:1:9..12"}},"num_qubits":"10","error_model":"-","subcircuits":"[]","mappings":[{"Mapping":{"name":"three","value":{"ConstInt":{"value":"3","source_location":"res/v1x/parsing/grammar/map/input.cq:4:5..6"}},"annotations":[{"AnnotationData":{"interface":"first","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:4:15..26"}}],"source_location":"res/v1x/parsing/grammar/map/input.cq:4:1..26"}},{"Mapping":{"name":"also_three","value":{"ConstInt":{"value":"3","source_location":"res/v1x/parsing/grammar/map/input.cq:5:18..23"}},"annotations":[{"AnnotationData":{"interface":"second","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:5:25..37"}},{"AnnotationData":{"interface":"third","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:5:39..50"}}],"source_location":"res/v1x/parsing/grammar/map/input.cq:5:1..50"}}],"variables":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:1:1..6:1"}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}

TEST(to_json, v3x_parser_errors) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_3.14"/"input.cq";
    cqasm::v3x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":["<unknown>:3:5: mismatched input '3.14' expecting INTEGER_LITERAL"]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_parser_ast) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto ast_result = cqasm::v3x::parser::parse_file(input_file_path.generic_string());
    auto json_result = to_json(ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"version":{"Version":{"items":"3","source_location":"<unknown>:1:9..10"}},"statements":{"StatementList":{"items":[{"Variable":{"name":{"Identifier":{"name":"b"}},"typ":{"Keyword":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"0"}},"annotations":"[]","source_location":"<unknown>:3:8..9"}}]}}}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_errors) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":["Error at <unknown>:3:8..9: declaring bit array of size <= 0"]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_ast) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_17_b"/"input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"statements":"[]","variables":[{"Variable":{"name":"b","typ":{"BitArray":{"size":"17"}},"annotations":"[]"}}]}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
