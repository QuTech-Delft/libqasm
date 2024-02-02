#include "v1x/cqasm.hpp"  // default_analyzer
#include "v1x/cqasm-result.hpp"

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;
using namespace cqasm::v1x;


TEST(to_json, v1x_parser_errors) {
    auto input_file_path = fs::path{"res"}/"v1x"/"parsing"/"grammar"/"expression_recovery"/"input.cq";
    cqasm::v1x::analyzer::Analyzer analyzer{};
    auto semantic_ast_result = analyzer.analyze_file(input_file_path.generic_string());
    auto json_result = to_json(semantic_ast_result);
    auto expected_json_result = std::string{
        R"delim({"errors":["Error at res/v1x/parsing/grammar/expression_recovery/input.cq:4:21: syntax error, unexpected ','","Error at res/v1x/parsing/grammar/expression_recovery/input.cq:4:31: syntax error, unexpected ','","Error at res/v1x/parsing/grammar/expression_recovery/input.cq:4:37: syntax error, unexpected ')', expecting ']'","Error at res/v1x/parsing/grammar/expression_recovery/input.cq:4:61: syntax error, unexpected ','","Error at res/v1x/parsing/grammar/expression_recovery/input.cq:4:67: syntax error, unexpected ')', expecting ',' or ']'","Failed to parse res/v1x/parsing/grammar/expression_recovery/input.cq"]})delim"
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
        R"delim({"errors":["Error at res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7: failed to resolve overload for 'wait' with argument pack (int)"]})delim"
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
