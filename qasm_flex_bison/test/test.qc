# qcode test file

# qubit definition
qubits 8

.mapping
   map b[0:11], bitcontroller
   map q[0] , q0
   map q[1] , q1
   map q[2] , q2
   map q[3] , q3
   map q[4] , q4
   map q[5] , q5
   map q[6] , q6
   map q[7] , q7

.first_circuit(50)
   H q0
   cnot q0,q1
   x  q1
   Y  q2
   z  q6

.second_circuit
   rx q4,0.453
   ry q5,0.244
   rz q6,0.555
   c-x bitcontroller, q[12:14,56]

.result
# measurement
   measure q1

