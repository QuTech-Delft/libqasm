import unittest

import cqasm.v1x as cq


class TestParseResult(unittest.TestCase):
    def test_to_json_with_parser_errors(self):
        # res/v1x/parsing/grammar/expression_recovery
        program_str = "version 1.0; qubits 10; display @test.test([, 1, 2, 3+, 4, 5) @test.test([, 1, 2, 3+, 4, 5)"
        v1x_analyzer = cq.Analyzer()
        actual_errors_json = v1x_analyzer.parse_string_to_json(program_str)
        print(actual_errors_json)
        expected_errors_json = '''{"errors":["<unknown>:1:45: syntax error, unexpected ','","<unknown>:1:55: syntax error, unexpected ','","<unknown>:1:61: syntax error, unexpected ')', expecting ']'","<unknown>:1:85: syntax error, unexpected ','","<unknown>:1:91: syntax error, unexpected ')', expecting ',' or ']'","Failed to parse <unknown>"]}'''
        self.assertEqual(actual_errors_json, expected_errors_json)

    def test_to_json_with_parser_ast(self):
        # res/v1x/parsing/misc/wait_not_ok_1
        program_str = "version 1.0; qubits 2; wait 1"
        v1x_analyzer = cq.Analyzer()
        actual_ast_json = v1x_analyzer.parse_string_to_json(program_str)
        expected_ast_json = '''{"Program":{"version":{"Version":{"items":"1.0","source_location":"<unknown>:1:9..12"}},"num_qubits":{"IntegerLiteral":{"value":"2","source_location":"<unknown>:1:21..22"}},"statements":{"StatementList":{"items":[{"Bundle":{"items":[{"Instruction":{"name":{"Identifier":{"name":"wait","source_location":"<unknown>:1:24..28"}},"condition":"-","operands":{"ExpressionList":{"items":[{"IntegerLiteral":{"value":"1","source_location":"<unknown>:1:29..30"}}],"source_location":"<unknown>:1:29..30"}},"annotations":"[]","source_location":"<unknown>:1:24..30"}}],"annotations":"[]","source_location":"<unknown>:1:24..30"}}],"source_location":"<unknown>:1:22..30"}},"source_location":"<unknown>:1:1..30"}}'''
        self.assertEqual(actual_ast_json, expected_ast_json)
