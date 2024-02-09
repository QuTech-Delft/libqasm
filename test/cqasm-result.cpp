#include "cqasm-result.hpp"
#include "v1x/cqasm.hpp"  // default_analyzer
#include "v3x/cqasm.hpp"  // default_analyzer

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;
using namespace cqasm::result;


TEST(to_json, v1x_parser_errors) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"grammar"/"expression_recovery"/"input.cq";
    cqasm::v1x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":4,"character":21},"end":{"line":4,"character":22}},"message":"syntax error, unexpected ','","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fgrammar%2Fexpression_recovery%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]},{"range":{"start":{"line":4,"character":31},"end":{"line":4,"character":32}},"message":"syntax error, unexpected ','","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fgrammar%2Fexpression_recovery%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]},{"range":{"start":{"line":4,"character":37},"end":{"line":4,"character":38}},"message":"syntax error, unexpected ')', expecting ']'","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fgrammar%2Fexpression_recovery%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]},{"range":{"start":{"line":4,"character":61},"end":{"line":4,"character":62}},"message":"syntax error, unexpected ','","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fgrammar%2Fexpression_recovery%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]},{"range":{"start":{"line":4,"character":67},"end":{"line":4,"character":68}},"message":"syntax error, unexpected ')', expecting ',' or ']'","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fgrammar%2Fexpression_recovery%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]},{"range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}},"message":"failed to parse 'res/v1x/parsing/grammar/expression_recovery/input.cq'","severity":1}]})delim"
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
        R"delim({"errors":[{"range":{"start":{"line":4,"character":1},"end":{"line":4,"character":7}},"message":"failed to resolve overload for 'wait' with argument pack (int)","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv1x%2Fparsing%2Fmisc%2Fwait_not_ok_1%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
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
        R"delim({"errors":[{"range":{"start":{"line":3,"character":5},"end":{"line":3,"character":9}},"message":"mismatched input '3.14' expecting INTEGER_LITERAL","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fbit_array_definition%2Fbit_array_of_3.14%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_parser_ast) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto ast_result = cqasm::v3x::parser::parse_file(input_file_path.generic_string(), std::nullopt);
    auto json_result = to_json(ast_result);
    auto expected_json_result = std::string{
        R"delim({"Program":{"version":{"Version":{"items":"3","source_location":"<unknown file name>:1:9..10"}},"statements":{"StatementList":{"items":[{"Variable":{"name":{"Identifier":{"name":"b"}},"typ":{"Keyword":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"0"}},"annotations":"[]","source_location":"<unknown file name>:3:8..9"}}]}}}})delim"
    };
    EXPECT_EQ(json_result, expected_json_result);
}
TEST(to_json, v3x_analyzer_errors) {
    auto input_file_path = fs::path{"res"}/"v3x"/"parsing"/"bit_array_definition"/"bit_array_of_0_b"/"input.cq";
    auto semantic_ast_result = cqasm::v3x::default_analyzer().analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":[{"range":{"start":{"line":3,"character":8},"end":{"line":3,"character":9}},"message":"declaring bit array of size <= 0","severity":1,"relatedInformation":[{"location":{"uri":"file:///res%2Fv3x%2Fparsing%2Fbit_array_definition%2Fbit_array_of_0_b%2Finput.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]})delim"
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
