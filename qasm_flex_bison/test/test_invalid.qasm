# This is an example of an invalid file

version 1.0
qubits 8

map q[0,0:7,5,6], x
map b[5:7], bleh


prep_z q[5]
reset-AveraGing
reset-Averaging Q[1,4,6]


measure_all
measure_parity     q[2:5], x,     q[0], z

x q[4:7]

rX q[1,2:5], 3.1416E-01


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

.waits
   wait 2

.first_circuit(50)
   H q0 
   cnot q0,q1
   x  q1
   Y  q2
   z  q6
   {x q3 | h q7| c-x bitcontroller, bleh2| c-toffoli b[4], q3, q4, q5}

.second_circuit
   rx q4,     -0.453e1
   ry q5,0.244
   rz q6,0.555
   c-x bitcontroller, q[2:5]
   c-swap b[7], q1 , q[5,7,0:2]
   toffoli q3, q4, q[0:1024]

.result
# measurement
   measure q1
   {measure q0 |measure q2 | measure q3}

.display_results
display
display b[1]
display bleh