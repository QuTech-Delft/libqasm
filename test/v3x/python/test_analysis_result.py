import libQasm as cq
import unittest


class TestAnalysisResult(unittest.TestCase):
    def setUp(self):
        self.analyzer = cq.V3xAnalyzer()

    def analyze(self, s: str):
        return self.analyzer.analyze_string(s)

    def test_to_json_with_analyzer_errors(self):
        result = self.analyzer("version 3; bit[0] b")
        json_result = result.to_json()
        expected_json_result = '''{"errors":["Error at <unknown>:3:8..9: declaring bit array of size <= 0"]}'''
        self.assertEqual(json_result, expected_json_result)

    def test_to_json_with_analyzer_ast(self):
        result = self.analyzer("version 3; bit[17] b")
        json_result = result.to_json()
        expected_json_result = '''{"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"statements":"[]","variables":[{"Variable":{"name":"b","typ":{"BitArray":{"size":"17"}},"annotations":"[]"}}]}}'''
        self.assertEqual(json_result, expected_json_result)


if __name__ == '__main__':
    unittest.main()
