## Gates
### Plain gate
QYASM:
```
x q;
```

CC backend output fragment:
```
[{awg}]	    seq_out     0xABCDEF12,1            # note 1,2
```

Notes referring to example above:
1.	the bracket expression `[{awg}]` selects the destination module 
within the CC. The syntax shown here is based on Python's f-string,
an actual expression would look like `[0]` or `[11]`
2.  codeword 0xABCDEF12 determined by active qubits, and instrument 
configuration, see the current OpenQL backend for the Central 
Controller for details

###	Simple conditional gate
Use cases:
- ancilla reset

QYASM:
```
r0 = MEAS q0
COND expression X q0
```

CC backend output fragment:
```
.CODE
.DEF        uhfLatency  10                      # constant derived from JSON config file
.DEF        smWait      3                       # constant derived from JSON config file
.DEF        byte        0                       # alias for byte size transfers for seq_in_sm

[{uhf}]     seq_out     0xABCD0000,$uhfLatency  # note 1,2
[{awg}]	    seq_wait    $uhfLatency             # balance UHF duration

[{uhf}]	    seq_in_sm	Sxx,muxXX,$byte         # note 3
[{awg}]	    seq_inv_sm	Sxx,N                   # note 4

            seq_wait    $smWait                 # wait for DSM data distribution

[{awg}]	    seq_out_sm	Sxx,plYY,1              # note 5
[{uhf}]	    seq_wait	1
.END

.DATAPATH
.MUX muxXX                                      # note 6
SM[0] := I17                                    # assuming qubit q0 connects to DIO bit 17

.PL plYY                                        # note 7
O[31] := 1                                      # HDAWG trigger
O[7] := SM[0]
.END	
```


Notes referring to example above:
1.	the bracket expression `[{awg}]` selects the destination module 
within the CC. The syntax shown here is based on Python's f-string,
an actual expression would look like `[0]` or `[11]`
2.	codeword ABCD determined by active qubits, and instrument configuration
3.	Shared memory address Sxx and MUX number muxXX determined by compiler, based on access patterns 
of classic variables and qubits
4.	Instruction seq_inv_sm invalidates N bytes of shared memory on the module involved. If the 
location is accessed by the sequencer program before the value is updated through the distributed 
shared memory protocol, an error is raised.
5.	PL (Programmable Logic) number plYY determined by compiler, based on expression, and codeword 
mode of instrument and codeword value.
6.	512 different MUX mappings are available. Input bits Ixx depend on how the qubits are connected 
to the instrument, output bits SMxx can be freely assigned, but keep in mind that it is optimal to 
minimize DSM transfer size (0: SM[7:0], 1: SM[15:0], 2: SM[31:0]), see note 2
7.	512 different PL mappings are available. SM bits depend on assignment by compiler (note that 
the scope on read is 128 bits). Output bits depend on expression, codeword mode and value, in a 
similar way to the static codeword assignments already present in the backend.


###	Conditional gate with state
Use cases:
- adaptive parity measurement

QYASM:
```
creg sel0 = 0                                   # initialize gate selector
...
for:
...
creg meas0 = measure(q0)                        # measurement
sel0 = sel0 ^ meas0                             # toggle selector on measurement==1
...
# perform gate A or B, depending on selector
COND sel0==0?A:B q0 
```

CC backend output fragment:
```
.CODE
        seq_state   0                           # note 1
        ...
        (see previous example)
.END

.DATAPATH
.PL plYY                                        # note 2
O[31] := 1                                      # HDAWG trigger
O[7] := ST[0] ^ SM[0]                           # state dependent output
O[8] := 1
ST[0] := ST[0] ^ SM[0]                          # update state
.END	
```

Notes referring to example above:
1.	classic variable sel0 is maintained in the PL state, which is initialized here
2.	in this example we use state to change the codeword output, and change state depending on shared memory input

### Control path feedback
Use cases:
- repeat until success

QYASM:
```
repeat {
    ...
    c = measure q;
    ...
until(c==1)
```


CC backend output fragment:
```
loop:       # measure

            seq_cl_sm   S<address>          ; pass 32 bit SM-data to Q1 (address depends on mapping of variable c) ...
            move_sm     R0                  ; ... and move to register
            and         R0,<mask>,R1        ; mask also depends on mapping of c
            nop															; register dependency R1
            jlt         R1,1,@loop
```
