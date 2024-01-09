import unittest

import cqasm.v1x as cq


class TestAnalysisResult(unittest.TestCase):
    def test_to_json_with_analyzer_errors(self):
        # res/v1x/parsing/misc/wait_not_ok_1
        program_str = "version 1.0; qubits 2; wait 1"
        v1x_analyzer = cq.Analyzer()
        actual_errors_json = v1x_analyzer.analyze_string_to_json(program_str)
        expected_errors_json = '''{"errors":["Error at <unknown>:1:24..30: failed to resolve overload for wait with argument pack (int)"]}'''
        self.assertEqual(actual_errors_json, expected_errors_json)

    def test_to_json_with_analyzer_ast(self):
        # res/v1x/parsing/grammar/map
        program_str = "version 1.0; qubits 10; map 3, three @first.annot; map also_three = three @second.annot @third.annot"
        v1x_analyzer = cq.Analyzer()
        actual_ast_json = v1x_analyzer.analyze_string_to_json(program_str)
        expected_ast_json = '''{"Program":{"api_version":"1.0","version":{"Version":{"items":"1.0","source_location":"<unknown>:1:9..12"}},"num_qubits":"10","error_model":"-","subcircuits":"[]","mappings":[{"Mapping":{"name":"three","value":{"ConstInt":{"value":"3","source_location":"<unknown>:1:29..30"}},"annotations":[{"AnnotationData":{"interface":"first","operation":"annot","operands":"[]","source_location":"<unknown>:1:39..50"}}],"source_location":"<unknown>:1:25..50"}},{"Mapping":{"name":"also_three","value":{"ConstInt":{"value":"3","source_location":"<unknown>:1:69..74"}},"annotations":[{"AnnotationData":{"interface":"second","operation":"annot","operands":"[]","source_location":"<unknown>:1:76..88"}},{"AnnotationData":{"interface":"third","operation":"annot","operands":"[]","source_location":"<unknown>:1:90..101"}}],"source_location":"<unknown>:1:52..101"}}],"variables":"[]","source_location":"<unknown>:1:1..101"}}'''
        self.assertEqual(actual_ast_json, expected_ast_json)
