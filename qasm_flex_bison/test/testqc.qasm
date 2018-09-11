version 1.0

#foo
#bar


qubits 8 #foo


map q[0,0:7,5,6], bleh2
map b[5:7], bleh


prep_z q[5]
reset-AveraGing
reset-Averaging Q[1,4,6]


measure_all
measure_parity     q[2:5], x,     q[0], z

x q[4:7]

rX q[1,2:5], 3.1416E-01


.mapping # mappin
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
   c-swap b[7], q1 , q[5]
   toffoli q3, q4, q[0]
   crk q1, q6, 4
   crk q2, q3, -10
   cr q2, q5, 3.1416
   U q1, [1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0]

.result
# measurement
   measure q1
   {measure q0 |measure q2 | measure q3}

.display_results
display
display b[1]
display_binary b[6]
display_binary

load_state "test.qc"


display bleh
error_model depolarizing_channel ,0.001, 0.1, 3.4
