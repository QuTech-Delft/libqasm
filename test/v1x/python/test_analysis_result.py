import libQasm as cq
import unittest


class TestAnalysisResult(unittest.TestCase):
    def setUp(self):
        self.analyzer = cq.V1xAnalyzer()

    def analyze(self, s: str):
        return self.analyzer.analyze_string(s)

    def test_to_json_with_analyzer_errors(self):
        result = self.analyzer("version 1.0; qubits 2; wait 1")
        json_result = result.to_json()
        expected_json_result = '''{"errors":["Error at res/v1x/parsing/misc/wait_not_ok_1/input.cq:4:1..7: failed to resolve overload for wait with argument pack (int)"]}'''
        self.assertEqual(json_result, expected_json_result)

    def test_to_json_with_analyzer_ast(self):
        result = self.analyzer("version 1.0; qubits 2; wait 1")
        json_result = result.to_json()
        expected_json_result = '''{"Program":{"api_version":"1.0","version":{"Version":{"items":"1.0","source_location":"res/v1x/parsing/grammar/map/input.cq:1:9..12"}},"num_qubits":"10","error_model":"-","subcircuits":"[]","mappings":[{"Mapping":{"name":"three","value":{"ConstInt":{"value":"3","source_location":"res/v1x/parsing/grammar/map/input.cq:4:5..6"}},"annotations":[{"AnnotationData":{"interface":"first","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:4:15..26"}}],"source_location":"res/v1x/parsing/grammar/map/input.cq:4:1..26"}},{"Mapping":{"name":"also_three","value":{"ConstInt":{"value":"3","source_location":"res/v1x/parsing/grammar/map/input.cq:5:18..23"}},"annotations":[{"AnnotationData":{"interface":"second","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:5:25..37"}},{"AnnotationData":{"interface":"third","operation":"annot","operands":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:5:39..50"}}],"source_location":"res/v1x/parsing/grammar/map/input.cq:5:1..50"}}],"variables":"[]","source_location":"res/v1x/parsing/grammar/map/input.cq:1:1..6:1"}}'''
        self.assertEqual(json_result, expected_json_result)


if __name__ == '__main__':
    unittest.main()
