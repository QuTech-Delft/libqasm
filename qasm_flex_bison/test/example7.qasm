version 1.0
# define a quantum register of 3 qubits
qubits 3

# reset the counters for the average procedure
reset-averaging

# prepare and measure the quantum state 1000 times
# to accumulate a large outcome statistics

.average(1000)

# state preparation
prep_z q[0:2]
{ rx q[0], 3.14 | ry q[1] ,0.23 | h q[2]}
rx q[2], 3.14
cnot q[2],q[1]
{ z q[1] | rx q[2], 1.57 }

# measure of $Z_1$
measure_z q[1]

# the corresponding average is automatically updated
# measure of $X_0 X_2$
measure_parity q[0],x,q[2],x

# the corresponding average is automatically updated
# estimate the observable A
.result

# show the average of $X_0 X_2$ together with its test outcome
display b[0]

# show the average of $Z_1$ together with its latest outcome
display b[1]

# the expectation value of $\hat{A}$ follows
# from a straightforward postprocess
