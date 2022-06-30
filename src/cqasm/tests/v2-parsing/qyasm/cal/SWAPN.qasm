/*
    File:       SWAPN.qasm
    Purpose:    SWAPN
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
bit meas[REP_CNT][AMPL_CNT][N];
int repetition;
frac amplitude;      // FIXME: define unit of amplitude, and range
int a;
int n;
int i;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(amplitude=AMPL_MIN,a=0; a<AMPL_CNT; amplitude+=AMPL_STEP,a++) {
        for(n=0; n<N; n++) {
            x180 q;
            wait q,N-n;     // ensure fixed timing. FIXME: timing units differ, perform I gates in loop?
            for(i=0; i<n; i++) {
                swap q,amplitude;
            }
            meas[repetition][a][n] = measure q;
        }
    }
}
