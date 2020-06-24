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
        with self.assertRaisesRegex(RuntimeError, r"Error at <unknown>:24:7\.\.8: index 11 out of range \(size 8\)"):
            lib_qasm = self._generic_test_steps('invalid')

    def test_invalid_as_string(self):
        with self.assertRaisesRegex(RuntimeError, r"Error at <unknown>:24:7\.\.8: index 11 out of range \(size 8\)"):
            lib_qasm = self._generic_test_steps_as_string('invalid')

    def test_reentrant_invalid(self):
        with self.assertRaisesRegex(RuntimeError, "Error at <unknown>:6:14\.\.15: index 2 out of range \(size 2\)"):
            lib_qasm = self._generic_test_steps('reentrant_invalid')

    def test_reentrant_invalid_as_string(self):
        with self.assertRaisesRegex(RuntimeError, "Error at <unknown>:6:14\.\.15: index 2 out of range \(size 2\)"):
            lib_qasm = self._generic_test_steps_as_string('reentrant_invalid')
