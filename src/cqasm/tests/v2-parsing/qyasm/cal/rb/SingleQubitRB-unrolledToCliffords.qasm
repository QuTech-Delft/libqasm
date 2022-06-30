/*
    File:       SingleQubitRB-unrolledToCliffords.qasm
    Purpose:    (Restless) Randomized Benchmarking: Single qubit randomized benchmarking
                All randomization and calculation on host, just spit out Cliffords
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      easy on scheduler, because it schedules the gates in cliffordEpstein() just once
    Based on:   QuantumUseCases.doc
                PycQED_py3::pycqed.instrument_drivers.meta_instrument.qubit_objects.CCL_Transmon.p#measure_single_qubit_randomized_benchmarking()
                PycQED_py3::pycqed.measurement.openql_experiments.clifford_rb_oql.py#randomized_benchmarking
                PycQED_py3::pycqed.measurement.randomized_benchmarking.randomized_benchmarking.py#randomized_benchmarking_sequence
*/


QYASM 1.0;

bit[][][] SingleQubitRB(qreg q, int REP_CNT, int SEED_CNT, int LEN_CNT)) {
    bit meas[REP_CNT][SEED_CNT][AMPL_CNT];

    // first step
    prepz q;                                        // skip in case of RRB
    cliffordEpstein(q, 3);      // first
    // ...
    cliffordEpstein(q, 7);      // last
    cliffordEpstein(q, 12);     // final
    meas[0][0][0] = measure q;

    // many more steps
    // ...

    return meas;
}
