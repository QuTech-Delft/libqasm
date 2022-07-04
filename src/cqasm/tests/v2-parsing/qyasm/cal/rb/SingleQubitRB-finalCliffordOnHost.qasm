/*
    File:       SingleQubitRB-finalCliffordOnHost.qasm
    Purpose:    (Restless) Randomized Benchmarking: Single qubit randomized benchmarking
                Version that computes final Clifford on host
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      requires capability to pass data array from host
    Based on:   QuantumUseCases.doc
                PycQED_py3::pycqed.instrument_drivers.meta_instrument.qubit_objects.CCL_Transmon.p#measure_single_qubit_randomized_benchmarking()
                PycQED_py3::pycqed.measurement.openql_experiments.clifford_rb_oql.py#randomized_benchmarking
                PycQED_py3::pycqed.measurement.randomized_benchmarking.randomized_benchmarking.py#randomized_benchmarking_sequence
*/

// FIXME: Single qubit interleaved randomized benchmarking

QYASM 1.0;

bit[][][] SingleQubitRB(qreg q, int REP_CNT, int SEED_CNT, int LEN_CNT) {
// FIXME: add parameters net_cliffords, max_clifford_idx, initialize, interleaving_cliffords, sim_cz_qubits
// FIXME: support simultaneous_single_qubit_RB, which may only need allowing q to be a vector

    bit meas[REP_CNT][SEED_CNT][AMPL_CNT];
    int repetition;
    int seed;
    int len;                        // nr of Cliffords
    int l;
    int g;
    int rnd;
    int netClifford;
    int finalClifford;

    for(repetition=0; repetition<REP_CNT; repetition++) {
        for(seed=0; seed<SEED_CNT; seed++) {
            random_seed(seed);                                  // initialize random number generator
            for(len=1,l=0; l<LEN_CNT; len<<=1,l++) {
                finalClifford = XXX;                            // FIXME: get from host

                // do the experiment
                prepz q;                                        // skip in case of RRB
                for(g=0; g<len; g++) {                          // NB: all cliffords must be output back to back
                    rnd = random(0,23);
                    cliffordEpstein(q, rnd);
                }
                cliffordEpstein(q, finalClifford);
                meas[repetition][seed][l] = measure q;
            }
        }
    }
    return meas;
}
