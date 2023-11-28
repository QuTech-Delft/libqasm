import libQasm as cq
import unittest


class TestParseResult(unittest.TestCase):
    def setUp(self):
        self.analyzer = cq.V1xAnalyzer()

    def analyze(self, s: str):
        return self.analyzer.analyze_string(s)

    def test_to_json_with_parser_errors(self):
        result = self.analyze("version 1.0; qubits 10; display @test.test([, 1, 2, 3+, 4, 5) @test.test([, 1, 2, 3+, 4, 5)")
        json_result = result.to_json()
        expected_json_result = '''{"errors":["res/v1x/parsing/grammar/expression_recovery/input.cq:4:21: syntax error, unexpected ','","res/v1x/parsing/grammar/expression_recovery/input.cq:4:31: syntax error, unexpected ','","res/v1x/parsing/grammar/expression_recovery/input.cq:4:37: syntax error, unexpected ')', expecting ']'","res/v1x/parsing/grammar/expression_recovery/input.cq:4:61: syntax error, unexpected ','""res/v1x/parsing/grammar/expression_recovery/input.cq:4:67: syntax error, unexpected ')', expecting ',' or ']'","Failed to parse res/v1x/parsing/grammar/expression_recovery/input.cq"]}'''
        self.assertEqual(json_result, expected_json_result)

    def test_to_json_with_parser_ast(self):
        result = self.analyze("version 1.0; qubits 2; wait 1")
        json_result = result.to_json()
        expected_json_result = '''{"Program":{"version":{"Version":{"items":"1.0","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:1:9..12"}},"num_qubits":{"IntegerLiteral":{"value":"2","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:2:8..9"}},"statements":{"StatementList":{"items":[{"Bundle":{"items":[{"Instruction":{"name":{"Identifier":{"name":"wait","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..5"}},"condition":"-","operands":{"ExpressionList":{"items":[{"IntegerLiteral":{"value":"1","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:6..7"}}],"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:6..7"}},"annotations":"[]","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7"}}],"annotations":"[]","source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7"}}],"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:2:1..5:7"}},"source_location":"res/v1x/parsing/misc/wait_not_ok_1/input.cq:1:1..5:1"}}'''
        self.assertEqual(json_result, expected_json_result)


if __name__ == '__main__':
    unittest.main()
