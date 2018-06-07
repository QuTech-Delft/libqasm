version 1.0
#define a quantum register of 2 qubits
qubits 2

# create a Bell pair via a Hadamard rotation
h q[0]
# followed by a CNOT gate
# q[0]: control qubit, q[1]: target qubit

cnot q[0],q[1]

# measure both qubits to test correlations
measure q[0]
measure q[1]