import unittest

import cqasm.v3x as cq


class TestParseResult(unittest.TestCase):
    def test_to_json_with_parser_errors(self):
        # res/v3x/parsing/qubit_array_definition/qubit_array_of_3.14
        program_str = "version 3; qubit[3.14]"
        v3x_analyzer = cq.Analyzer()
        actual_errors_json = v3x_analyzer.parse_string_to_json(program_str)
        expected_errors_json = '''{"errors":[{"range":{"start":{"line":1,"character":18},"end":{"line":1,"character":22}},"message":"mismatched input '3.14' expecting INTEGER_LITERAL","severity":1}]}'''
        self.assertEqual(actual_errors_json, expected_errors_json)

    def test_to_json_with_parser_ast(self):
        # res/v3x/parsing/qubit_array_definition/qubit_array_of_0_b
        program_str = "version 3; qubit[0] q"
        v3x_analyzer = cq.Analyzer()
        actual_ast_json = v3x_analyzer.parse_string_to_json(program_str)
        expected_ast_json = '''{"Program":{"version":{"Version":{"items":"3","source_location":"<unknown file name>:1:9..10"}},"block":{"GlobalBlock":{"qubit_variable_declaration":{"Variable":{"name":{"Identifier":{"name":"q"}},"typ":{"Type":{"name":{"Keyword":{"name":"qubit"}},"size":{"IntegerLiteral":{"value":"0"}},"source_location":"<unknown file name>:1:12..20"}},"annotations":"[]","source_location":"<unknown file name>:1:21..22"}},"gates":"[]","measure_instructions":"[]"}}}}'''
        self.assertEqual(actual_ast_json, expected_ast_json)
