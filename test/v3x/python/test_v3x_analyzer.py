import unittest

import cqasm.v3x as cq


class TestV3xAnalyzer(unittest.TestCase):
    def test_parse_string_returning_ast(self):
        program_str = "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure q"
        v3x_analyzer = cq.Analyzer()
        ast = v3x_analyzer.parse_string(program_str)

        self.assertEqual(ast.version.items[0], 3)

        qubit_array = ast.block.statements[0]
        self.assertEqual(qubit_array.name.name, "q")
        self.assertEqual(qubit_array.typ.name.name, "qubit")
        self.assertEqual(qubit_array.typ.size.value, 5)

        bit_array = ast.block.statements[1]
        self.assertEqual(bit_array.name.name, "b")
        self.assertEqual(bit_array.typ.name.name, "bit")
        self.assertEqual(bit_array.typ.size.value, 5)

        h_instruction = ast.block.statements[2]
        self.assertEqual(h_instruction.name.name, "H")
        h_operand = h_instruction.operands.items[0]
        self.assertEqual(h_operand.expr.name, "q")
        self.assertEqual(h_operand.indices.items[0].first.value, 0)
        self.assertEqual(h_operand.indices.items[0].last.value, 4)

        measure_instruction = ast.block.statements[3]
        self.assertEqual(measure_instruction.name.name, "measure")
        self.assertEqual(measure_instruction.lhs.name, "b")
        self.assertEqual(measure_instruction.rhs.name, "q")

    def test_parse_string_returning_errors(self):
        program_str = "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure"
        v3x_analyzer = cq.Analyzer()
        errors = v3x_analyzer.parse_string(program_str)
        print(errors)
        expected_errors = ["Error at <unknown file name>:1:51: mismatched input '<EOF>' expecting {'(', '+', '-', '~', '!', BOOLEAN_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, IDENTIFIER}"]
        self.assertEqual(errors, expected_errors)

    def test_analyze_string_returning_ast(self):
        program_str = "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure q"
        v3x_analyzer = cq.Analyzer()
        ast = v3x_analyzer.analyze_string(program_str)

        self.assertEqual(ast.version.items[0], 3)

        h_instruction = ast.block.statements[0]
        self.assertEqual(h_instruction.name, "H")
        h_operand = h_instruction.operands[0]
        self.assertEqual(h_operand.variable.name, "q")
        self.assertIsInstance(h_operand.variable.typ, cq.types.QubitArray)
        self.assertEqual(h_operand.variable.typ.size, 5)
        self.assertEqual(h_operand.indices[0].value, 0)
        self.assertEqual(h_operand.indices[1].value, 1)
        self.assertEqual(h_operand.indices[2].value, 2)
        self.assertEqual(h_operand.indices[3].value, 3)
        self.assertEqual(h_operand.indices[4].value, 4)

        measure_instruction = ast.block.statements[1]
        self.assertEqual(measure_instruction.name, "measure")
        measure_bit_operand = measure_instruction.operands[0]
        self.assertEqual(measure_bit_operand.variable.name, "b")
        self.assertIsInstance(measure_bit_operand.variable.typ, cq.types.BitArray)
        self.assertEqual(measure_bit_operand.variable.typ.size, 5)
        measure_qubit_operand = measure_instruction.operands[1]
        self.assertEqual(measure_qubit_operand.variable.name, "q")
        self.assertIsInstance(measure_qubit_operand.variable.typ, cq.types.QubitArray)
        self.assertEqual(measure_qubit_operand.variable.typ.size, 5)

        qubit_array = ast.variables[0]
        self.assertEqual(qubit_array.name, "q")
        self.assertIsInstance(qubit_array.typ, cq.types.QubitArray)
        bit_array = ast.variables[1]
        self.assertEqual(bit_array.name, "b")
        self.assertIsInstance(bit_array.typ, cq.types.BitArray)

    def test_analyze_string_returning_errors(self):
        program_str = "version 3;qubit[3] q;X q[3]"
        v3x_analyzer = cq.Analyzer()
        errors = v3x_analyzer.analyze_string(program_str)
        expected_errors = ["Error at <unknown file name>:1:24..25: index 3 out of range (size 3)"]
        self.assertEqual(errors, expected_errors)

    def test_to_json_vrsion(self):
        """This test is used to check that a program with a syntactic error
        produces the same error message both when parsing and analyzing."""
        # res/v3x/parsing/version/vrsion
        program_str = "vrsion 3"
        v3x_analyzer = cq.Analyzer()
        actual_parse_json = v3x_analyzer.parse_string_to_json(program_str)
        actual_analyze_json = v3x_analyzer.analyze_string_to_json(program_str)
        self.assertEqual(actual_parse_json, actual_analyze_json)
        expected_json = '''{"errors":[{"range":{"start":{"line":1,"character":1},"end":{"line":1,"character":7}},"message":"mismatched input 'vrsion' expecting {NEW_LINE, ';', 'version'}","severity":1}]}'''
        self.assertEqual(actual_parse_json, expected_json)
