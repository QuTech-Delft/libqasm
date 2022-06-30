






/*  Comment copied from PycQED_py3::pycqed.measurement.randomized_benchmarking.two_qubit_clifford_group.py

This file contains Clifford decompositions for the two qubit Clifford group.

The Clifford decomposition closely follows two papers:
Corcoles et al. Process verification ... Phys. Rev. A. 2013
    http://journals.aps.org/pra/pdf/10.1103/PhysRevA.87.030301
for the different classes of two-qubit Cliffords.

and
Barends et al. Superconducting quantum circuits at the ... Nature 2014
    https://www.nature.com/articles/nature13171?lang=en
for writing the cliffords in terms of CZ gates.


###########################################################################
2-qubit clifford decompositions

The two qubit clifford group (C2) consists of 11520 two-qubit cliffords
These gates can be subdivided into four classes.
    1. The Single-qubit like class  | 576 elements  (24^2)
    2. The CNOT-like class          | 5184 elements (24^2 * 3^2)
    3. The iSWAP-like class         | 5184 elements (24^2 * 3^2)
    4. The SWAP-like class          | 576  elements (24^2)
    --------------------------------|------------- +
    Two-qubit Clifford group C2     | 11520 elements


1. The Single-qubit like class
    -- C1 --
    -- C1 --

2. The CNOT-like class
    --C1--•--S1--      --C1--•--S1------
          |        ->        |
    --C1--⊕--S1--      --C1--•--S1^Y90--

3. The iSWAP-like class
    --C1--*--S1--     --C1--•---Y90--•--S1^Y90--
          |       ->        |        |
    --C1--*--S1--     --C1--•--mY90--•--S1^X90--

4. The SWAP-like class
    --C1--x--     --C1--•-mY90--•--Y90--•-------
          |   ->        |       |       |
    --C1--x--     --C1--•--Y90--•-mY90--•--Y90--

C1: element of the single qubit Clifford group
    N.B. we use the decomposition defined in Epstein et al. here

S1: element of the S1 group, a subgroup of the single qubit Clifford group

S1[0] = I                   aka C1[0]
S1[1] = rY90, rX90          aka C1[1]
S1[2] = rXm90, rYm90        aka C1[2]

Important clifford indices:

        I    : Cl 0
        X90  : Cl 16
        Y90  : Cl 21
        X180 : Cl 3
        Y180 : Cl 6
        Z180 : Cl 9
        CZ   : 4368
*/





/*
NB: instead of the cliffordEpstein() program flow decomposition, we could also perform data flow decomposition
using conditional gates. This would probably work better for MultiQubitRB

static gate gateSet[7] = {I,X180,Y180,X90,Y90,mX90,mY90};    // order form MW LutMan
// 'column' slices through decomposition
const int cliffordEpsteinColumn0[24] = {
    C_I,    C_Y90,  C_mX90, C_X180, C_mY90,
    C_X90,  C_Y180, C_mY90, C_X90,  C_X180,
    C_Y90,  C_mx90, C_Y90,  C_mX90, C_X90,
    C_mY90, C_X90,  C_X90,  C_mY90, C_X90,
    C_X90,  C_Y90,  C_mX90, C_X90};
// etc

int gate0 = cliffordEpsteinColumn0[idx];
int gate1 = cliffordEpsteinColumn1[idx];
int gate2 = cliffordEpsteinColumn2[idx];
condGate gateSet[gate0] q;
condGate gateSet[gate1] q;
condGate gateSet[gate2] q;

// or, save a lookup:
condGate gatesCliffordEpsteinColumn0[idx] q;
condGate gatesCliffordEpsteinColumn1[idx] q;
condGate gatesCliffordEpsteinColumn2[idx] q;

*/

const gate gatesCliffordEpsteinColumn0[24] = {
    I,      Y90,    mX90,   X180,   mY90,
    X90,    Y180,   mY90,   X90,    X180,
    Y90,    mx90,   Y90,    mX90,   X90,
    mY90,   X90,    X90,    mY90,   X90,
    X90,    Y90,    mX90,   X90};
// etc


/* FIXME:
- to reach the last if 4 compares are required, use 'BSP'? Or use 2 RNGs, one per qubit
- PycQED performs (variable length) decomposition per qubit and only aligns timelines on 2 qubit gates and measurement. That
 runs faster and may have other consequences
- the '/' and '%' operators may require too much time to keep up with the gates, unless we add hardware support
- if we allow the timing of the qubits involved to diverge (during single qubit gates), we need some run-time mechanism
to align it to perform 2-qubit gates or measurements (counting gates, seq_bar, ...)
*/

// NB: try to minimize classic calculation preceding gates
void clifford2qxxx(qreg q, int idx) {
    if(idx < 24*24) {                           // 24^2=576 single_qubit_like_gates
        idx0 = idx%24;
        idx1 = idx/24;
        cliffordEpstein(q[0], idx0);
        cliffordEpstein(q[1], idx1);
        // FIXME: we want those two scheduled in the same time slot (?can the CC-light do that). See condGate below

    } else if(idx < 24*24+72*72 ) {             // 24^2*3^2=5184 CNOT_like_gates
        /* gates for Cliffords of the cnot like class
            (q0)  --C1--•--S1--      --C1--•--S1------
                        |        ->        |
            (q1)  --C1--⊕--S1--      --C1--•--S1^Y90--
        */

        idx -= 24*24;

        // decompose Cliffords
        idx_q0_c1 = idx % 24;
        idx_q1_c1 = (idx / 24) % 24;
        condGate gatesCliffordEpsteinColumn0[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn0[idx_q1_c1] q[1];
        condGate gatesCliffordEpsteinColumn1[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn1[idx_q1_c1] q[1];
        condGate gatesCliffordEpsteinColumn2[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn2[idx_q1_c1] q[1];

        CZ q;

        // decompose S1 and S1^Y90
        idx_q0_s1 = (idx / 576) % 3;
        idx_q1_s1 = (idx / 1728);
        condgate gatesS1Column0[idx_q0_s1] q[0] | condgate gatesS1DotY90Column0[idx_q1_s1] q[1];
        condgate gatesS1Column1[idx_q0_s1] q[0] | condgate gatesS1DotY90Column1[idx_q1_s1] q[1];
        condgate gatesS1Column2[idx_q0_s1] q[0] | condgate gatesS1DotY90Column2[idx_q1_s1] q[1];

    } else if(idx < 24*24+72*72+72*72) {        // 24^2*3^2=5184 iSWAP_like_gates
        /* gates for Cliffords of the iSWAP like class
            (q0)  --C1--*--S1--     --C1--•---Y90--•--S1^Y90--
                        |       ->        |        |
            (q1)  --C1--*--S1--     --C1--•--mY90--•--S1^X90--
        */

        idx -= 24*24+72*72;

        // FIXME: copy of above
        // decompose Cliffords
        idx_q0_c1 = idx % 24;
        idx_q1_c1 = (idx / 24) % 24;
        condGate gatesCliffordEpsteinColumn0[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn0[idx_q1_c1] q[1];
        condGate gatesCliffordEpsteinColumn1[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn1[idx_q1_c1] q[1];
        condGate gatesCliffordEpsteinColumn2[idx_q0_c1] q[0] | condGate gatesCliffordEpsteinColumn2[idx_q1_c1] q[1];

        CZ q;
        // FIXME: end copy

        // decompose Y90/mY90 (FIXME: why decompose? So we could use 5 primitives? Still, be could manually unroll and use plain gates)
        idx_Y90 = 21;   // FIXME
        idx_mY90 = 15;  // FIXME
        condGate gatesCliffordEpsteinColumn0[idx_Y90] q[0] | condGate gatesCliffordEpsteinColumn0[idx_mY90] q[1];
        condGate gatesCliffordEpsteinColumn1[idx_Y90] q[0] | condGate gatesCliffordEpsteinColumn1[idx_mY90] q[1];
        condGate gatesCliffordEpsteinColumn2[idx_Y90] q[0] | condGate gatesCliffordEpsteinColumn2[idx_mY90] q[1];

        CZ q;

        // decompose S1^Y90 and S1^X90
        idx_q0_s1 = (idx / 576) % 3;
        idx_q1_s1 = (idx / 1728);
        condgate gatesS1DotY90Column0[idx_q0_s1] q[0] | condgate gatesS1DotX90Column0[idx_q1_s1] q[1];
        condgate gatesS1DotY90Column1[idx_q0_s1] q[0] | condgate gatesS1DotX90Column1[idx_q1_s1] q[1];
        condgate gatesS1DotY90Column2[idx_q0_s1] q[0] | condgate gatesS1DotX90Column2[idx_q1_s1] q[1];

    } else if(idx < 24*24+72*72+72*72+24*24) {  // 24^2=576 SWAP_like_gates
    } else  {
        // trap()
    }
}


// FIXME: does the RB analysis need to know the RB sequence?




/* From: PycQED_py3::pycqed.measurement.randomized_benchmarking.two_qubit_clifford_group

#TwoQubitClifford
    def gate_decomposition(self):
        """
        Returns the gate decomposition of the two qubit Clifford group.

        Single qubit Cliffords are decompesed according to Epstein et al.

        Using the method to get this avoids expensive function calls
        whenever the Clifford is instantiated
        """
        if not hasattr(self, '_gate_decomposition'):
            if self.idx < 576:
                self._gate_decomposition = single_qubit_like_gates(self.idx)
            elif self.idx < 576 + 5184:
                self._gate_decomposition = CNOT_like_gates(self.idx-576)
            elif self.idx < 576 + 2*5184:
                self._gate_decomposition = iSWAP_like_gates(
                    self.idx-(576+5184))
            elif self.idx < 11520:
                self._gate_decomposition = SWAP_like_gates(
                    self.idx-(576+2*5184))

        return self._gate_decomposition



def single_qubit_like_gates(idx):
    """
    Returns the gates for Cliffords of the single qubit like class
        (q0)  -- C1 --
        (q1)  -- C1 --
    """
    assert(idx < 24**2)
    idx_q0 = idx % 24
    idx_q1 = idx//24

    g_q0 = [(g, 'q0') for g in gate_decomposition[idx_q0]]
    g_q1 = [(g, 'q1') for g in gate_decomposition[idx_q1]]
    gates = g_q0 + g_q1
    return gates

def CNOT_like_gates(idx):
    """
    Returns the gates for Cliffords of the cnot like class
        (q0)  --C1--•--S1--      --C1--•--S1------
                    |        ->        |
        (q1)  --C1--⊕--S1--      --C1--•--S1^Y90--
    """
    assert(idx < 5184)
    idx_0 = idx % 24
    idx_1 = (idx // 24) % 24
    idx_2 = (idx // 576) % 3
    idx_3 = (idx // 1728)

    C1_q0 = [(g, 'q0') for g in gate_decomposition[idx_0]]
    C1_q1 = [(g, 'q1') for g in gate_decomposition[idx_1]]
    CZ = [('CZ', ['q0', 'q1'])]

    idx_2s = get_clifford_id(S1[idx_2])
    S1_q0 = [(g, 'q0') for g in gate_decomposition[idx_2s]]
    idx_3s = get_clifford_id(np.dot(C1[idx_3], Y90))
    S1_yq1 = [(g, 'q1') for g in gate_decomposition[idx_3s]]

    gates = C1_q0 + C1_q1 + CZ + S1_q0 + S1_yq1
    return gates


def iSWAP_like_gates(idx):
    """
    Returns the gates for Cliffords of the iSWAP like class
        (q0)  --C1--*--S1--     --C1--•---Y90--•--S1^Y90--
                    |       ->        |        |
        (q1)  --C1--*--S1--     --C1--•--mY90--•--S1^X90--
    """
    assert(idx < 5184)
    idx_0 = idx % 24
    idx_1 = (idx // 24) % 24
    idx_2 = (idx // 576) % 3
    idx_3 = (idx // 1728)

    C1_q0 = [(g, 'q0') for g in gate_decomposition[idx_0]]
    C1_q1 = [(g, 'q1') for g in gate_decomposition[idx_1]]
    CZ = [('CZ', ['q0', 'q1'])]

    sqs_idx_q0 = get_clifford_id(Y90)
    sqs_idx_q1 = get_clifford_id(mY90)
    sq_swap_gates_q0 = [(g, 'q0') for g in gate_decomposition[sqs_idx_q0]]
    sq_swap_gates_q1 = [(g, 'q1') for g in gate_decomposition[sqs_idx_q1]]

    # S1_q0 = np.kron(np.eye(4), np.dot(S1[idx_2], Y90))
    # S1y_q1 = np.kron(np.dot(C1[idx_3], X90), np.eye(4))

    idx_2s = get_clifford_id(np.dot(S1[idx_2], Y90))
    S1_q0 = [(g, 'q0') for g in gate_decomposition[idx_2s]]
    idx_3s = get_clifford_id(np.dot(C1[idx_3], X90))
    S1y_q1 = [(g, 'q1') for g in gate_decomposition[idx_3s]]

    gates = (C1_q0 + C1_q1 + CZ +
             sq_swap_gates_q0 + sq_swap_gates_q1 + CZ +
             S1_q0 + S1y_q1)
    return gates


def SWAP_like_gates(idx):
    """
    Returns the gates for Cliffords of the SWAP like class

    (q0)  --C1--x--     --C1--•-mY90--•--Y90--•-------
                |   ->        |       |       |
    (q1)  --C1--x--     --C1--•--Y90--•-mY90--•--Y90--
    """
    assert(idx < 24**2)
    idx_q0 = idx % 24
    idx_q1 = idx//24
    C1_q0 = [(g, 'q0') for g in gate_decomposition[idx_q0]]
    C1_q1 = [(g, 'q1') for g in gate_decomposition[idx_q1]]
    CZ = [('CZ', ['q0', 'q1'])]

    sq_swap_gates_0 = np.kron(Y90, mY90)

    sqs_idx_q0 = get_clifford_id(mY90)
    sqs_idx_q1 = get_clifford_id(Y90)
    sq_swap_gates_0_q0 = [(g, 'q0') for g in gate_decomposition[sqs_idx_q0]]
    sq_swap_gates_0_q1 = [(g, 'q1') for g in gate_decomposition[sqs_idx_q1]]

    sqs_idx_q0 = get_clifford_id(Y90)
    sqs_idx_q1 = get_clifford_id(mY90)
    sq_swap_gates_1_q0 = [(g, 'q0') for g in gate_decomposition[sqs_idx_q0]]
    sq_swap_gates_1_q1 = [(g, 'q1') for g in gate_decomposition[sqs_idx_q1]]

    sqs_idx_q1 = get_clifford_id(Y90)
    sq_swap_gates_2_q0 = [(g, 'q0') for g in gate_decomposition[0]]
    sq_swap_gates_2_q1 = [(g, 'q1') for g in gate_decomposition[sqs_idx_q1]]

    gates = (C1_q0 + C1_q1 + CZ +
             sq_swap_gates_0_q0 + sq_swap_gates_0_q1 + CZ +
             sq_swap_gates_1_q0 + sq_swap_gates_1_q1 + CZ +
             sq_swap_gates_2_q0 + sq_swap_gates_2_q1)
    return gates

*/
