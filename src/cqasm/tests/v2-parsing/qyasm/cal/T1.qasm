/*
    File:       T1.qasm
    Purpose:    T1
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
int meas[REP_CNT][T_CNT];
int repetition;
frac time;      // FIXME: define unit of time, and range
int t;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(time=TIME_MIN,t=0; t<TIME_CNT; time+=TIME_STEP,t++) {
        wait q,time;
        x180 q;
        wait q,T-time;
        meas[repetition][t] = measure q,INT_AVG;
        prepz q;
    }
}
