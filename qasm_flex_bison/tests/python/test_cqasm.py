from test_init import LibQasmTest


class TestQasm(LibQasmTest):
    def test_bare_minimum(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        parse_res = lib_qasm.getParseResult()
        self.assertEqual(parse_res, 0)

    def test_invalid(self):
        with self.assertRaisesRegex(RuntimeError, "expecting NAME"):
            lib_qasm = self._generic_test_steps('invalid')

    def test_reentrant_invalid(self):
        with self.assertRaisesRegex(RuntimeError, "Qubit indices exceed"):
            lib_qasm = self._generic_test_steps('reentrant_invalid')
