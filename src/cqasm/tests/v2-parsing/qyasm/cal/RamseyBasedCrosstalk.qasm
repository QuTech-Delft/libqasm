/*
    File:       RamseyBasedCrosstalk.qasm
    Purpose:    Ramsey Based Crosstalk
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q[2];
bit meas[REP_CNT][N];
int repetition;
int n;
int i;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(n=0; n<N; n++) {    // FIXME: n>=1, or maybe 2
        prepz q;            // prepare both qubits
        x90 q[0];
        for(i=0; i<n; i++) {
            x180 q[1];
        }
        for(; i<N; i++) {   // count onwards to N to ensure fixed timing between x90's
            I q[1];         // do nothing, assumes gate time of 'x180' and 'I' are equal
        }
        x90 q[0];

        meas[repetition][n] = measure q[0];
    }
}
