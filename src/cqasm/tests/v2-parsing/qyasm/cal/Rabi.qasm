/*
    File:       Rabi.qasm
    Purpose:    Rabi
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
                PycQED_py3::pycqed.instrument_drivers.meta_instrument.qubit_objects.CCL_Transmon.p#measure_rabi()
*/

QYASM 1.0;

int[][] Rabi(qubit q, int REP_CNT, int AMPL_CNT, ufixed<1,31> AMPL_STEP)
{
    int meas[REP_CNT][AMPL_CNT];
    int repetition;
    ufixed<1,31> amplitude;      // FIXME: define unit of amplitude, and range
    int a;

    for(repetition=0; repetition<REP_CNT; repetition++) {
        for(amplitude=AMPL_MIN,a=0; a<AMPL_CNT; amplitude+=AMPL_STEP,a++) {
            prepz q;
            rx q,amplitude;
            meas[repetition][a] = measure q,INT_AVG;
        }
    }
    return meas;
}