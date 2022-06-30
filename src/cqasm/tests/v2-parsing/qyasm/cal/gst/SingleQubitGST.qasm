/*
    File:       SingleQubitGST.qasm
    Purpose:    SingleQubitGST
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
bit meas[REP_CNT][T_CNT];



// define 6 Fiducials:
fiducial(qubit q, int idx) {
    switch(idx) {
        case 0:
            I q;    // FIXME: or no gate
            break;

        case 1:
            x90 q;
            break;

        case 2:
            y90 q;
            break;

        case 3:
            x90 q;
            x90 q;
            break;

        case 4:
            x90 q;
            x90 q;
            x90 q;
            break;

        case 5:
            y90 q;
            y90 q;
            y90 q;
            break;

        default:
            trap();
    }
}

// define 11 Germs:
germ(qubit q, int idx, int cnt)
{
    int i;
    for(i=0; i<cnt; i++) {
        switch(idx) {
            case 0:
                x90 q;
                break;

            case 1:
                y90 q;
                break;

            case 2:
                I q;    // FIXME: or no gate, i.e. zero duration?
                break;

            case 3:
                x90 q;
                y90 q;
                break;

            case 4:
                x90 q;
                y90 q;
                I q;
                break;

            case 5:
                x90 q;
                I q;
                y90 q;
                break;

            case 6:
                x90 q;
                I q;
                I q;
                break;

            case 7:
                y90 q;
                I q;
                I q;
                break;

            case 8:
                x90 q;
                x90 q;
                I q;
                y90 q;
                break;

            case 9:
                x90 q;
                y90 q;
                y90 q;
                I q;
                break;

            case 10:
                x90 q;
                x90 q;
                y90 q;
                x90 q;
                y90 q;
                y90 q;
                break;

            default:
                trap();
        } // switch
    } // for
}


bit gstStep(qubit q, int len)
{
    bit rslt;
    int prepFid;
    int g;
    int measFid;

    prepz q;

    for(prepFid=0; prepFid<6; prepFid++) {
        fiducial(q, prepFid);
    }
    for(g=0; g<11; g++) {
        germ(q, g, len);
    }
    for(measFid=0; measFid<6; measFid++) {
        fiducial(q, measFid);
    }

    rslt = measure q;
    return rslt;
}


// FIXME: does the analysis have an assumption on ordering?
gst(qubit q, int LEN_CNT, int LEN_STEP)
{
    bit meas[LEN_CNT];
    int len;
    int l;

    for(len=0,l=0; l<LEN_CNT; len+=LEN_STEP,l++) {  // FIXME: proper evolution of len
        meas[l] = gstStep(q, len);
    }
}