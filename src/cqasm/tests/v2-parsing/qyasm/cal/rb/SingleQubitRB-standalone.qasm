/*
    File:       SingleQubitRB-standalone.qasm
    Purpose:    (Restless) Randomized Benchmarking: Single qubit randomized benchmarking
                Standalone version that performs all computation in run time
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      This examples supposes a number of builtin functions are available:
                - void random_seed(int seed);
                - int random(int min, int max);                                 // time budget: 1 Clifford
                - int calcNetCliffordSingleQubit(int netClifford, int rnd);     // maybe implement via Q1 sw_req
                - int calcRecoverCliffordSingleQubit(int netClifford);          // maybe implement via Q1 sw_req
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

    bit meas[REP_CNT][SEED_CNT][AMPL_CNT];  // FIXME having REP_CNT/SEED_CNT/LEN_CNT as runtime parameter may be difficult
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
                // first calculate final clifford, so we can do experiment full speed
                // FIXME: could also make single function: calcFinalCliffordSingleQubit(len)
                netClifford = 1;
                // FIXME: save RNG state
                for(g=0; g<len; g++) {
                    rnd = random(0,23);
                    netClifford = calcNetCliffordSingleQubit(netClifford, rnd);
                }
                finalClifford = calcRecoverCliffordSingleQubit(netClifford);

                // now do the experiment
                // FIXME: restore RNG state
                // FIXME: #pragma realtime on (NB: requires seq_bar if run time of preceding part is not constant)
                prepz q;                                        // skip in case of RRB
                for(g=0; g<len; g++) {                          // NB: all cliffords must be output back to back
                    rnd = random(0,23);
                    cliffordEpstein(q, rnd);
                }
                cliffordEpstein(q, finalClifford);
                meas[repetition][seed][l] = measure q;
                // FIXME: #pragma realtime off
            }
        }
    }
    return meas;
}
