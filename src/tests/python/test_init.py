import os
import unittest

import libQasm as qasminterface


class LibQasmTest(unittest.TestCase):
    @staticmethod
    def _generic_test_steps(file_name):
        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        data_file = os.path.join(base_dir, 'test_data', file_name + '.qasm')
        lib_qasm = qasminterface.libQasm()
        lib_qasm.parse_file(data_file)
        return lib_qasm

    @staticmethod
    def _generic_test_steps_as_string(file_name):
        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        data_file = os.path.join(base_dir, 'test_data', file_name + '.qasm')
        with open(data_file, 'r') as qasm_file:
            qasm_string = qasm_file.read()
        lib_qasm = qasminterface.libQasm()
        lib_qasm.parse_string(qasm_string)
        return lib_qasm
