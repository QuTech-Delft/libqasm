/*
    File:       Chevron.qasm
    Purpose:    Chevron
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
bit meas[REP_CNT][AMPL_CNT][LEN_CNT];
int repetition;
frac amplitude;      // FIXME: define unit of amplitude, and range
int a;
frac len;
int l;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(amplitude=AMPL_MIN,a=0; a<AMPL_CNT; amplitude+=AMPL_STEP,a++) {
        for(len=0,l=0; l<LEN_CNT; len+=LEN_STEP,l++) {
            flux(amplitude,len) q,;     // FIXME: len as parameter to flux?
            meas[repetition][a][l] = measure q; // FIXME: INT_AVG?
        }
    }
}
