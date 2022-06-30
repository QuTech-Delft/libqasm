/*
    File:       QEC.qasm
    Purpose:    pipelined Quantum Error Correction cycle
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      see: R. Versluis et al., Phys. Rev. A 8, 034021 (2017)
                - we rotate grid by 45 deg for convenience: [nw, ne, sw, se] -> [n, e, w, s]
                - H -> rym90, ry90, see Fig 2 of reference
    Based on:   QuantumUseCases.doc
*/

// FIXME:
// - qubits participating in CZ need phase correction, which may be part of gate, or separate
// - similar for qubits not participating
// - phase corrections performed using flux lines:
//      + duration?
//      + possible in parallel without doing 2 qubits gate?


// NB: all parameters can also be qubit *vectors* (FIXME: but then we must support non-existing qubits at the edge of the grid)
void QEC(qubit x, qubit xN, qubit xE, qubit xS, qubit xW, qubit z, qubit zN, qubit zE, qubit zS, qubit zW) {
    // X stabilizers
    { rym90 [x,xN,xE,xW,xS] };
    { cz x,xE | cz x,xN | cz x,xS | cz x,xW };
    { ry90 [x,xN,xE,xW,xS] };

    { measure x };

    // Z stabilizers
    { rym90 z };
    { cz z,zE | cz z,zS | cz z,zN | cz z,zW };
    { ry90 z };

    { measure z };
}
