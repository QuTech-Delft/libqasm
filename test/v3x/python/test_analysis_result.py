import unittest

import cqasm.v3x as cq


class TestAnalysisResult(unittest.TestCase):
    def test_to_json_with_analyzer_errors(self):
        # res/v3x/parsing/bit_array_definition/bit_array_of_0_b
        program_str = "version 3; bit[0] b"
        v3x_analyzer = cq.Analyzer()
        actual_errors_json = v3x_analyzer.analyze_string_to_json(program_str)
        expected_errors_json = '''{"errors":[{"range":{"start":{"line":1,"character":19},"end":{"line":1,"character":20}},"message":"found bit array of size <= 0","severity":1}]}'''
        self.assertEqual(actual_errors_json, expected_errors_json)

    def test_to_json_with_analyzer_ast(self):
        # res/v3x/parsing/bit_array_definition/bit_array_of_17_b
        program_str = "version 3; bit[17] b"
        v3x_analyzer = cq.Analyzer()
        actual_ast_json = v3x_analyzer.analyze_string_to_json(program_str)
        expected_ast_json = '''{"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"block":{"Block":{"statements":"[]"}},"functions":"[]","variables":[{"Variable":{"name":"b","typ":{"BitArray":{"size":"17"}},"annotations":"[]"}}]}}'''
        self.assertEqual(actual_ast_json, expected_ast_json)
