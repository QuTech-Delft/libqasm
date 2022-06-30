/*
    File:       RepeatUntilSuccess.qasm
    Purpose:    Repeat Until Success
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
bit meas;

do {
    // ...
    meas = measure q;
} while(meas==1);
