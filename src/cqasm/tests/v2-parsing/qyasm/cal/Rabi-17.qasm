/*
    File:       Rabi-17.qasm
    Purpose:    Rabi on 17 qubits
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   softwareStack.doc
*/

QYASM 1.0;

include "Rabi.qasm"

// int[][][] Rabi-17(int REP_CNT, int AMPL_CNT, ufixed<1,31> AMPL_STEP)

qubit q[17];
int meas[17][REP_CNT][AMPL_CNT];
int i;

for(i=0; i<17; i++) {
    // int[][] Rabi(qubit q, int REP_CNT, int AMPL_CNT, ufixed<1,31> AMPL_STEP)
    meas[i] = Rabi(q[i], REP_CNT, AMPL_CNT, AMPL_STEP);
}
