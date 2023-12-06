import unittest

import cqasm.v3x as cq


class TestParseResult(unittest.TestCase):
    def test_to_json_with_parser_errors(self):
        # res/v3x/parsing/bit_array_definition/bit_array_of_3_14
        program_str = "version 3; bit[3.14]"
        v3x_analyzer = cq.Analyzer()
        actual_errors_json = v3x_analyzer.parse_string_to_json(program_str)
        expected_errors_json = '''{"errors":["Error at <unknown>:1:19..20: declaring bit array of size <= 0"]}'''
        self.assertEqual(expected_errors_json, expected_errors_json)

    def test_to_json_with_parser_ast(self):
        # res/v3x/parsing/bit_array_definition/bit_array_of_0_b
        program_str = "version 1.0; bit[0] b"
        v3x_analyzer = cq.Analyzer()
        actual_ast_json = v3x_analyzer.parse_string_to_json(program_str)
        expected_ast_json = '''{"Program":{"version":{"Version":{"items":"1.0","source_location":"<unknown>:1:9..12"}},"statements":{"StatementList":{"items":[{"Variables":{"names":[{"Identifier":{"name":"b"}}],"typ":{"Identifier":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"0"}},"annotations":"[]","source_location":"<unknown>:1:21..22"}}]}}}}'''
        self.assertEqual(actual_ast_json, expected_ast_json)
