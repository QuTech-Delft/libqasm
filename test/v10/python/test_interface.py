from .test_init import TestLibQasm


class TestInterface(TestLibQasm):
    def test_version_number(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.versionNumber(), 1.0)

    def test_number_of_qubits(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.numQubits(), 8)

    def test_default_circuit(self):
        expected = [{'qubit': 0, 'ops': 'x'}, {'qubit': 1, 'ops': 'y'}]
        actual = []

        lib_qasm = self._generic_test_steps('example_str')
        qasm_representation = lib_qasm.getQasmRepresentation()
        for subCircuit in qasm_representation.getSubCircuits().getAllSubCircuits():

            self.assertEqual(subCircuit.nameSubCircuit(), 'default')
            self.assertEqual(subCircuit.numberIterations(), 1)

            for ops_cluster in subCircuit.getOperationsCluster():
                for ops in ops_cluster.getOperations():
                    actual.append({
                        'qubit': ops.getQubitsInvolved().getSelectedQubits().getIndices()[0],
                        'ops': ops.getType()
                    })

        self.assertListEqual(expected, actual)

    def test_parameterized_operation(self):
        expected = [{'qubit': 0, 'ops': 'rx', 'rotation': 3.14}]
        actual = []

        lib_qasm = self._generic_test_steps('parameterized_ops')
        qasm_representation = lib_qasm.getQasmRepresentation()
        for subCircuit in qasm_representation.getSubCircuits().getAllSubCircuits():

            self.assertEqual(subCircuit.nameSubCircuit(), 'default')
            self.assertEqual(subCircuit.numberIterations(), 1)

            for ops_cluster in subCircuit.getOperationsCluster():
                for ops in ops_cluster.getOperations():
                    actual.append({
                        'qubit': ops.getQubitsInvolved().getSelectedQubits().getIndices()[0],
                        'ops': ops.getType(),
                        'rotation': ops.getRotationAngle()
                    })

        self.assertListEqual(expected, actual)

    def test_parallel_operation(self):
        number_of_parallel_operations = 0
        expected = [{'circuit': 'average', 'iterations': 1000}, {'circuit': 'result', 'iterations': 1}]
        actual = []

        lib_qasm = self._generic_test_steps('example7')
        qasm_representation = lib_qasm.getQasmRepresentation()
        for subCircuit in qasm_representation.getSubCircuits().getAllSubCircuits():
            for ops_cluster in subCircuit.getOperationsCluster():

                actual.append({
                    'circuit': subCircuit.nameSubCircuit(),
                    'iterations': subCircuit.numberIterations(),
                })

                for ops in ops_cluster.getOperations():
                    selected_qubits_vector = ops.getQubitsInvolved().getSelectedQubits().getIndices()
                    if len(selected_qubits_vector) > 1:
                        number_of_parallel_operations += 1

        self.assertEqual(number_of_parallel_operations, 1)

    def test_get_error_model(self):
        lib_qasm = self._generic_test_steps('qc')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.getErrorModelType(), 'depolarizing_channel')
        self.assertEqual(qasm_representation.getErrorModelParameters(), (0.001, 0.1, 3.4))

    def test_set_qubit_register(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.numQubits(), 8)
        qasm_representation.qubitRegister(7)
        self.assertEqual(qasm_representation.numQubits(), 7)

    def test_binary_controlled_gates(self):
        expected = [{'qubit': 2, 'ops': 'x', 'bit': 1}]
        actual = []

        lib_qasm = self._generic_test_steps('example2')
        qasm_representation = lib_qasm.getQasmRepresentation()
        for subCircuit in qasm_representation.getSubCircuits().getAllSubCircuits():
            for ops_cluster in subCircuit.getOperationsCluster():
                for ops in ops_cluster.getOperations():
                    control_bits_vector = ops.getControlBits().getSelectedBits().getIndices()
                    if len(control_bits_vector) > 0:
                        actual.append({
                            'qubit': ops.getQubitsInvolved().getSelectedQubits().getIndices()[0],
                            'ops': ops.getType(),
                            'bit': control_bits_vector[0]
                        })

        self.assertListEqual(expected, actual)

    def test_get_mapped_qubit(self):
        lib_qasm = self._generic_test_steps('example2')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.getMappedIndices('ancilla', True, 7).getIndices()[0], 1)

    def test_mapped_qubit_incorrect_mapping_name(self):
        lib_qasm = self._generic_test_steps('example2')
        qasm_representation = lib_qasm.getQasmRepresentation()
        with self.assertRaisesRegex(RuntimeError, "Could not get wanted mapping incorrect_mapping"):
            qasm_representation.getMappedIndices('incorrect_mapping', True, 8).getIndices()[0]

    def test_set_error_model(self):
        lib_qasm = self._generic_test_steps('bare_minimum')
        qasm_representation = lib_qasm.getQasmRepresentation()
        self.assertEqual(qasm_representation.getErrorModelType(), 'None')
        qasm_representation.setErrorModel('depolarizing_channel', (0.001, 0.1, 3.4))
        self.assertEqual(qasm_representation.getErrorModelType(), 'depolarizing_channel')
        self.assertEqual(qasm_representation.getErrorModelParameters(), (0.001, 0.1, 3.4))

    def test_barriers(self):
        lib_qasm = self._generic_test_steps('barriers')
        parse_res = lib_qasm.getParseResult()
        self.assertEqual(parse_res, 0)

    def test_grover(self):
        lib_qasm = self._generic_test_steps('grover')
        parse_res = lib_qasm.getParseResult()
        self.assertEqual(parse_res, 0)
