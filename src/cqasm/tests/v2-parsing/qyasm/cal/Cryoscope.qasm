/*
    File:       Cryoscope.qasm
    Purpose:    Cryoscope
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
bit meas[REP_CNT][LEN_CNT][4];
int repetition;
frac len;
int l;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(len=0,l=0; l<LEN_CNT; len+=LEN_STEP,l++) {
        prepz q;
        rx90 q;
        wait(bufferTime1) q;    // buffer time
        flux(len) q;            // variable high time, fixed total duration
        wait(bufferTime2) q;    // buffer time
        rx90 q;
        meas[repetition][l][0] = measure q,INT_AVG

        FIXME: repeat with ry90, rxm90, rym90 as last gate
    }
}
