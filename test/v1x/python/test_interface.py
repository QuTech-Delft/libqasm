import unittest
import cqasm.v1x as cq
import typing

class PythonAPITest(unittest.TestCase):
    def setUp(self):
        self.analyzer = cq.Analyzer("1.0")

    def analyze(self, s: str):
        return self.analyzer.analyze_string(s)

    def test_simplecircuit(self):
        result = self.analyze('version 1.0; qubits 2; x q[1]; cnot q[0], q[1]')

        self.assertEqual(result.subcircuits[0].bundles[0].items[0].name, "x")
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[0].value, 1)
        self.assertEqual(result.subcircuits[0].bundles[1].items[0].name, "cnot")
        self.assertEqual(result.subcircuits[0].bundles[1].items[0].operands[0].index[0].value, 0)
        self.assertEqual(result.subcircuits[0].bundles[1].items[0].operands[1].index[0].value, 1)

    def test_range(self):
        result = self.analyze('version 1.0; qubits 20; x q[0:19]')

        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[0].value, 0)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[1].value, 1)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[12].value, 12)
        self.assertEqual(len(result.subcircuits[0].bundles[0].items[0].operands[0].index), 20)

    def test_subcircuit(self):
        result = self.analyze('version 1.0; qubits 20; .myTestSubcircuit')

        self.assertEqual(result.subcircuits[0].name, "myTestSubcircuit")
        self.assertEqual(len(result.subcircuits[0].body.statements), 0)

    def test_subcircuit(self):
        result = self.analyze('version 1.0; qubits 20; .myTestSubcircuit')

        self.assertEqual(result.subcircuits[0].name, "myTestSubcircuit")
        self.assertEqual(len(result.subcircuits[0].bundles), 0)

    def test_qubits(self):
        result = self.analyze('version 1.0; qubits 39; cnot q[0, 2], q[1, 3]')

        self.assertEqual(result.num_qubits, 39)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[0].value, 0)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[1].value, 2)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[1].index[0].value, 1)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[1].index[1].value, 3)

    def test_bundle(self):
        result = self.analyze('version 1.0; qubits 2; { rx q[0], 1.234 | ry q[1], -0.5 }')

        self.assertIsInstance(result.subcircuits[0].bundles[0].items[0].operands[0], cq.values.QubitRefs)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[0].index[0].value, 0)
        self.assertIsInstance(result.subcircuits[0].bundles[0].items[0].operands[1], cq.values.ConstReal)
        self.assertEqual(result.subcircuits[0].bundles[0].items[0].operands[1].value, 1.234)
        self.assertEqual(result.subcircuits[0].bundles[0].items[1].operands[0].index[0].value, 1)
        self.assertIsInstance(result.subcircuits[0].bundles[0].items[1].operands[1], cq.values.ConstReal)
        self.assertEqual(result.subcircuits[0].bundles[0].items[1].operands[1].value, -0.5)

    def test_errorQubitIndex(self):
        result = self.analyze('version 1.0; qubits 1; x q[1]')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "Error at <unknown>:1:28..29: index 1 out of range (size 1)")

    def test_errorVersion(self):
        result = self.analyze('version 5.2; qubits 1; x q[1]')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "cQASM file version is 5.2, but at most 1.0 is supported here")

    def test_errorMissingQubits(self):
        result = self.analyze('version 1.0; x q[1]')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "Error: missing qubits statement (required until version 1.1)")

    def test_errorSyntax(self):
        result = self.analyze('version 1.0; qubits 20; x q[1:2:3]')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "<unknown>:1:32: syntax error, unexpected ':', expecting ',' or ']'")

    def test_unknownGate(self):
        result = self.analyze('version 1.0; qubits 20; unknownGate q[1]')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "Error at <unknown>:1:25..41: failed to resolve unknownGate")

    def test_wrongArguments(self):
        result = self.analyze('version 1.0; qubits 20; x q[1], 2.2')

        self.assertIsInstance(result, list)
        self.assertEqual(result[0], "Error at <unknown>:1:25..36: failed to resolve overload for x with argument pack (qubit reference, real)")

if __name__ == '__main__':
    unittest.main()