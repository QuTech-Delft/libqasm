/*
    File:       cliffordEpstein.qasm
    Purpose:    perform clifford, using gate decomposition of the clifford group as per Epstein et al. Phys. Rev. A 89, 062321 (2014)
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      must meet real-time constraints set by caller: fast switch execution, maybe inline function
    Based on:   PycQED_py3::pycqed.measurement.randomized_benchmarking.clifford_decompositions.py#epstein_efficient_decomposition
*/

// FIXME: split off to separate file for reuse?
void cliffordEpstein(qubit q, int idx) {
    switch(idx) {
        case 0:     I q;                    break;
        case 1:     Y90 q;  X90 q;          break;
        case 2:     mX90 q; mY90 q;         break;
        case 3:     X180 q;                 break;
        case 4:     mY90 q; mX90 q;         break;
        case 5:     X90 q;  mY90 q;         break;
        case 6:     Y180 q;                 break;
        case 7:     mY90 q; X90 q;          break;
        case 8:     X90 q;  Y90 q;          break;
        case 9:     X180 q; Y180 q;         break;
        case 10:    Y90 q;  mX90 q;         break;
        case 11:    mX90 q; Y90 q;          break;
        case 12:    Y90 q;  X180 q;         break;
        case 13:    mX90 q;                 break;
        case 14:    X90 q;  mY90 q; mX90 q; break;
        case 15:    mY90 q;                 break;
        case 16:    X90 q;                  break;
        case 17:    X90 q;  Y90 q;  X90 q;  break;
        case 18:    mY90 q; X180 q;         break;
        case 19:    X90 q;  Y180 q;         break;
        case 20:    X90 q;  mY90 q; X90 q;  break;
        case 21:    Y90 q;                  break;
        case 22:    mX90 q; Y180 q;         break;
        case 23:    X90 q;  Y90 q;  mX90;   break;
        default:    // trap();  // FIXME: add SEQ_TRAP (and TRAP?) instruction to Q1 to allow user generated faults
    }
}



/*
    Purpose:    perform clifford, using 5 primitives decomposition of the single qubit clifford group as per Asaad et al. arXiv:1508.06676
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      must meet real-time constraints set by caller: fast switch execution, maybe inline function
    Based on:   PycQED_py::pycqed.measurement.randomized_benchmarking.clifford_decompositions.py#Five_primitives_decomposition
*/

void clifford5primitives(qreg q, int idx) {
    // FIXME: add, and move to separate file
}
