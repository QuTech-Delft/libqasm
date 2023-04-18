version 1.0
#define a quantum register of 3 qubits
qubits 3

x q[0]
barrier q[0,1]
x q[1]
barrier q[0:2]
x q[0]
x q[2]
