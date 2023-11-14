import unittest

import libQasm


class TestV3xAnalyzer(unittest.TestCase):
    def parse_string(self):
        program_str = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        ret = libQasm.V3xAnalyzer.parse_string(program_str)
        self.assertEqual(ret, "")


if __name__ == '__main__':
    unittest.main()
