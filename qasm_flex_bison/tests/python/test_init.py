import os
import unittest

import libQasm as qasminterface

class LibQasmTest(unittest.TestCase):
    def _generic_test_steps(self, file_name):
        BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        data_file = os.path.join(BASE_DIR, 'test_data', file_name + '.qasm')
        lib_qasm = qasminterface.libQasm()
        lib_qasm.parse_file(data_file)
        return lib_qasm
