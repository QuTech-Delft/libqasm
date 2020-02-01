version 1.0
#define a quantum register of 3 qubits
qubits 3

# rename qubits
map q[0],data
map q[1],ancilla
map q[2],output
map q[1],extra

# address qubits via their names
prep_z data
prep_z ancilla
prep_z output
cnot data,ancilla
cnot data,extra

# rename classical bit
map b[1],error_syndrome
measure ancilla

#apply binary controlled Pauli-X gate
c-x error_syndrome,q[2]
