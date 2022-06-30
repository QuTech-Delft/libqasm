/*
    File:       AdaptiveParityMeasurement.qasm
    Purpose:    Adaptive Parity Measurement
    Author:     Wouter Vlothuizen (wouter.vlothuizen@tno.nl)
    Copyright:	QuTech/TNO
    Notes:      in this example, a bit ('sel') is assigned from an expression, not a measurement
    Based on:   QuantumUseCases.doc
*/

QYASM 1.0;
qubit q;
boolean meas;
boolean sel;            // state variable

sel = 0;                // initial gate selector
//...
for(;;) {
    //...
    meas = measure q;

    sel = sel ^ meas;   // toggle selector if meas==1

    sel ? [a,b] q;      // perform gate 'a' or 'b', depending on 'sel'. FIXME: syntax
}
