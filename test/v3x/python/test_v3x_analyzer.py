from ...test_init import TestLibQasm


class TestV3xAnalyzer(TestLibQasm):
    def test_parse_string(self):
        program_str = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        v3x_analyzer = self._get_v3x_analyzer()
        ret = v3x_analyzer.parse_string(program_str)
        self.assertEqual(ret, "")