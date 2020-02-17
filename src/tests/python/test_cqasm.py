from .test_init import LibQasmTest


class TestQasm(LibQasmTest):
    def test_bare_minimum(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        parse_res = lib_qasm.getParseResult()
        self.assertEqual(parse_res, 0)

    def test_bare_minimum_as_string(self):
        lib_qasm = self._generic_test_steps_as_string('bare_minimum')
        parse_res = lib_qasm.getParseResult()
        self.assertEqual(parse_res, 0)

    def test_invalid(self):
        with self.assertRaisesRegex(RuntimeError, r"syntax error, unexpected AXIS, expecting NAME | Line: 6"):
            lib_qasm = self._generic_test_steps('invalid')

    def test_invalid_as_string(self):
        with self.assertRaisesRegex(RuntimeError, r"syntax error, unexpected AXIS, expecting NAME | Line: 6"):
            lib_qasm = self._generic_test_steps_as_string('invalid')

    def test_reentrant_invalid(self):
        with self.assertRaisesRegex(RuntimeError, "Qubit indices exceed"):
            lib_qasm = self._generic_test_steps('reentrant_invalid')

    def test_reentrant_invalid_as_string(self):
        with self.assertRaisesRegex(RuntimeError, "Qubit indices exceed"):
            lib_qasm = self._generic_test_steps_as_string('reentrant_invalid')
