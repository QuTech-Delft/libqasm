qubiTs 1024

display
display b[0], b[10], b[100,10,99], myOwn0bit
display b[100:1, 7:56, 6,8,9] , bleh

reset-AveraGing
reset-Averaging q[0], Qkl, Q[8,9, 0:100, 6, 8, 9]

map q[0,0:8,5,6,9:1000], q[6], gm2, bleh2

measure_all
measure_parity     q[90:45], x,     mapqubits, z
measure_parity q[9122:56,6,2], z

x bleh3, q[6:999]

rX q[99,56:4], 3.1416E4

prep_x q[5:6], gs
measure_z fgs, q[6], q[7:90]

cNot q[4,6:90] , bleh45
CR          ghjkls,    q[90:456], 40
NOT b[90,120], ghsbits
c-x bitcontroller, q[12:289,56]
c-swap bitcontbleh, b[1234:789], q[567:452,890,23], qswaps

toffoli q[45:23], mappedqbit, q[67,23,89:23]

c-toffoli b[45:89,234] ,q[45:23], mappedqbit, q[67,23,89:23]

{c-toffoli b[45:89,234] ,q[45:23], mappedqbit, q[67,23,89:23] | rX q[99,56:4], 3.1416E4}

.singleparallel
{c-toffoli b[45:89,234] ,q[45:23], mappedqbit, q[67,23,89:23]}

.bleh2circuit(50)
measure_all