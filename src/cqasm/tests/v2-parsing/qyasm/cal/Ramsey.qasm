/*
    File:       Ramsey.qasm
    Purpose:    Ramsey
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
                PycQED_py3::pycqed.instrument_drivers.meta_instrument.qubit_objects.CCL_Transmon.p#measure_ramsey()
*/

QYASM 1.0;
qubit q;
int meas[REP_CNT][T_CNT];
int repetition;
ufixed<a,b> time;   // FIXME: define range and resolution
int t;

for(repetition=0; repetition<REP_CNT; repetition++) {
    for(time=TIME_MIN,t=0; t<TIME_CNT; time+=TIME_STEP,i++) {
        x90 q
        wait q,time;
        x90 q;
        meas[repetition][t] = measure q,INT_AVG;
        prepz q;
    }
}
