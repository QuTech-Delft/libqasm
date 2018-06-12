version 1.0
qubits 5
h q[0]
# measurement outcome in b0
measure_z q[0] 
# simple binary-contolled gate
# apply Pauli-X to q[1] if b[0]=1
c-x b[0],q[1] 
measure_z q[2]
measure_z q[3]
measure_z q[4]
# multi-binary controlled gate
# apply pauli-x to q4 if b2=1 and b3=1 and b4=1
c-x b[2,3,4],q[4]
# binary controlled gate using an arbitrary mask :
# we want to apply a Pauli-X to q[4] if b[0]=0 and b[1]=1
# negate b0
not b[0] 
# multi-bits controlled X gate
c-x b[0,1],q[4] 
# restore the measurement register
not b[0] 
display 
