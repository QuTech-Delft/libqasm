# qcode test file

# qubit definition
qubits 8

map q[0,0:7,5,6], bleh2
map b[5], bleh


prep_z q[5]
reset-AveraGing
reset-Averaging Q[1,4,6]


measure_all
measure_parity     q[2:5], x,     q[0], z
measure_parity q[1:3,4,6,7], z

x q[4:7]

rX q[1,2:5], 3.1416E4


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
   {x q3 | h q7}

.second_circuit
   rx q4,     -0.453e1
   ry q5,0.244
   rz q6,0.555
   c-x bitcontroller, q[12:14,56]

.result
# measurement
   measure q1

.display_results
display
display b[100,10,99]
display bleh
