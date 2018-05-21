qubiTs 1024

display
display b[0], b[10], b[100,10,99], myOwn0bit
display b[100:1, 7:56, 6,8,9] , bleh

reset-AveraGing
reset-Averaging q[0], Qkl, Q[8,9, 0:100, 6, 8, 9]

map q[0,0:8,5,6,9:1000], q[6], gm2, bleh2

measure_all

x bleh3, q[6:999]

rX q[99,56:4], 3.1416E4