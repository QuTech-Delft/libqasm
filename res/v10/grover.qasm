version 1.0
# define a quantum register of 9 qubits
qubits 9

map q[4],oracle

# sub-circuit for state initialization
.init
    x oracle
    { h q[0] | h q[1] | h q[2] | h q[3] | h oracle }

# core step of Grover’s algorithm
# loop with 3 iterations
.grover(3)

    # search for |x> = |0100>

    # oracle implementation

    x q[2]
    toffoli q[0],q[1],q[5]
    toffoli q[1],q[5],q[6] #test of multiline comments
    #blabla
    toffoli q[2],q[6],q[7]
    toffoli q[3],q[7],q[8]
    cnot q[8],oracle
    toffoli q[3],q[7],q[8]
    toffoli q[2],q[6],q[7]
    toffoli q[1],q[5],q[6]
    toffoli q[0],q[1],q[5]
    x q[2]

    # Grover diffusion operator
    { h q[0] | h q[1] | h q[2] | h q[3] }
    { x q[0] | x q[1] | x q[2] | x q[3] }
    h q[3]
    toffoli q[0],q[1],q[5]
    toffoli q[1],q[5],q[6]
    toffoli q[2],q[6],q[7]
    cnot q[7],q[3]
    toffoli q[2],q[6],q[7]
    toffoli q[1],q[5],q[6]
    toffoli q[0],q[1],q[5]
    h q[3]
    { x q[0] | x q[1] | x q[2] | x q[3] }
    { h q[0] | h q[1] | h q[2] | h q[3] }
    display

# final measurement
.final_measurement
    h oracle
    measure oracle
    display 

#error_model depolarizing_channel ,0.001