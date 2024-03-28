import unittest

import cqasm.v3x as cq


class TestV3xAnalyzer(unittest.TestCase):
    def test_parse_string_returning_ast(self):
        program_str = "version 3;qubit[5] q;H q[0:4];measure q"
        v3x_analyzer = cq.Analyzer()
        ast = v3x_analyzer.parse_string(program_str)

        self.assertEqual(ast.version.items[0], 3)
        
        qubit_array = ast.block.qubit_variable_declaration
        self.assertEqual(qubit_array.name.name, "b'q'")
        self.assertEqual(qubit_array.typ.name.name, "b'qubit'")
        self.assertEqual(qubit_array.typ.size.value, 5)
        
        h_instruction = ast.block.gates[0]
        self.assertEqual(h_instruction.name.name, "b'H'")
        h_operand = h_instruction.operands.items[0]
        self.assertEqual(h_operand.expr.name, "b'q'")
        self.assertEqual(h_operand.indices.items[0].first.value, 0)
        self.assertEqual(h_operand.indices.items[0].last.value, 4)

        measure_instruction = ast.block.measure_instruction
        self.assertEqual(measure_instruction.name.name, "b'measure'")
        self.assertEqual(measure_instruction.operands.items[0].name, "b'q'")

    def test_parse_string_returning_errors(self):
        program_str = "version 3;qubit[5] q;H q[0:4];measure"
        v3x_analyzer = cq.Analyzer()
        errors = v3x_analyzer.parse_string(program_str)
        expected_errors = ["Error at <unknown file name>:1:38: mismatched input '<EOF>' expecting {'(', '+', '-', '~', '!', BOOLEAN_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, IDENTIFIER}"]
        self.assertEqual(errors, expected_errors)

    def test_analyze_string_returning_ast(self):
        program_str = "version 3;qubit[5] q;H q[0:4];measure q"
        v3x_analyzer = cq.Analyzer()
        ast = v3x_analyzer.analyze_string(program_str)

        self.assertEqual(ast.version.items[0], 3)

        h_instruction = ast.block.statements[0]
        self.assertEqual(h_instruction.name, "b'H'")
        h_operand = h_instruction.operands[0]
        self.assertEqual(h_operand.variable.name, "b'q'")
        self.assertIsInstance(h_operand.variable.typ, cq.types.QubitArray)
        self.assertEqual(h_operand.variable.typ.size, 5)
        self.assertEqual(h_operand.indices[0].value, 0)
        self.assertEqual(h_operand.indices[1].value, 1)
        self.assertEqual(h_operand.indices[2].value, 2)
        self.assertEqual(h_operand.indices[3].value, 3)
        self.assertEqual(h_operand.indices[4].value, 4)

        measure_instruction = ast.block.statements[1]
        self.assertEqual(measure_instruction.name, "b'measure'")
        measure_qubit_operand = measure_instruction.operands[0]
        self.assertEqual(measure_qubit_operand.variable.name, "b'q'")
        self.assertIsInstance(measure_qubit_operand.variable.typ, cq.types.QubitArray)
        self.assertEqual(measure_qubit_operand.variable.typ.size, 5)

        qubit_array = ast.qubit_variable_declaration
        self.assertEqual(qubit_array.name, "b'q'")
        self.assertIsInstance(qubit_array.typ, cq.types.QubitArray)

    def test_analyze_string_returning_errors(self):
        program_str = "version 3;qubit[3] q;X q[3]"
        v3x_analyzer = cq.Analyzer()
        errors = v3x_analyzer.analyze_string(program_str)
        expected_errors = ["Error at <unknown file name>:1:24..25: index 3 out of range (size 3)"]
        self.assertEqual(errors, expected_errors)
