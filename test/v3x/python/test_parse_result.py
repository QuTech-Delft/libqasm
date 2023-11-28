import libQasm as cq
import unittest


class TestParseResult(unittest.TestCase):
    def setUp(self):
        self.analyzer = cq.V3xAnalyzer()

    def analyze(self, s: str):
        return self.analyzer.analyze_string(s)

    def test_to_json_with_parser_errors(self):
        result = self.analyzer("version 3; bit[3.14]")
        json_result = result.to_json()
        expected_json_result = '''{"errors":["<unknown>:3:5: mismatched input '3.14' expecting INTEGER_LITERAL"]}'''
        self.assertEqual(json_result, expected_json_result)

    def test_to_json_with_parser_ast(self):
        result = self.analyzer("version 1.0; bit[0] b")
        json_result = result.to_json()
        expected_json_result = '''{"Program":{"version":{"Version":{"items":"3","source_location":"<unknown>:1:9..10"}},"statements":{"StatementList":{"items":[{"Variables":{"names":[{"Identifier":{"name":"b"}}],"typ":{"Identifier":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"0"}},"annotations":"[]","source_location":"<unknown>:3:8..9"}}]}}}}'''
        self.assertEqual(json_result, expected_json_result)


if __name__ == '__main__':
    unittest.main()

