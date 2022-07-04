This document provides example code generation output for language constructs used in the QYASM files in this repository. One of the purposes of this exercise is to identify mismatches between required constructs and the capabilities of the Q1 processor.

## Measuring
### measure

see example [simple conditional gate](qasm/cond/CCCodegenCond.md#simple-conditional-gate)

### measure (mode)
Many calibration experiments require access to raw or preprocessed measurement data instead of a binary 0 or 1. In the 
typical CC setup a ZI UHFQA instrument will perform such measurements, and store them internally.
Here we propose structured access to such data using multi-dimensional arrays that are actually external
to the CC. The CC just generates data descriptors that allow retrieval of the actual data from the measurement
instrument.

QYASM:
```
int meas[REP_CNT*AMPL_CNT];

meas[i++] = measure q,INT_AVG;
```
or

```
int meas[REP_CNT][AMPL_CNT];

meas[repetition][t] = measure q,INT_AVG;
```

The mode INT_AVG should have several effects:
- measurement unit (e.g. UHF)
    - configure unit for the selected mode, and its associated parameters (e.g. the dimensions of the result)
- CC
    - store an ID of the measurement in array meas[]. ID contains:
        - a bit mask for the qubits measured, using the same encoding as the digital input interface
        - index number of measurement for this qubit. <!-- FIXME: not required if we only allow sequential access -->
<!-- we could put the index number in a register, and also the bit mask. Currently, the only
place we could store these would be in ARM memory, using the sw_req instruction to get it there. 
sw_req takes a 32 bit immediate or register value as parameter, and uses a FIFO, currently 8 deep.
-->
- at the level of the Quantum Compiler software interface: 
    - get the IDs from the CC
    - grab the raw measurements from the measurement unit 
    - associate the IDs with the measurements
    - return the result

NB: support both CC+UHF and measurement unit with integrated sequencer (e.g. QBlox)


## Timing
### wait (variable)
QYASM:
```
wait q,time;
```

FIXME:implicit assumption on time unit

CC backend output fragment:
```
            move        time,Rx             ; or reuse register 'time'
loopNN:     seq_wait    1
            loop        Rx,@loopNN                   
```

### wait (expression)
QYASM:
```
wait q,T-time;
```

FIXME:implicit assumption on time unit

CC backend output fragment:
```
            move        T,Rx
            move        time,Ry             ; or reuse register 'time'
            sub         Rx,Ry,Ry
loopNN:     seq_wait    1
            loop        Ry,@loopNN                   
```


## Program flow
### for loop
QYASM:
```
for(repetition=0; repetition<REP_CNT; repetition++) {}
```

CC backend output fragment:
```
            move        0,Rx
loopNN:     jlt         Rx,REP_CNT,@loopExitNN
            ...
            add         Rx,1,Rx
            jmp         @loopNN
loopExitNN:
```
    
FIXME: too slow on CC with single 20 ns gate inside loop

### for loop (2)
QYASM:
```
for(amplitude=AMPL_MIN,a=0; a<AMPL_CNT; amplitude+=AMPL_STEP,a++) {}
```

CC backend output fragment:
```
            move        AMPL_MIN,Rx
            move        0,Ry
loopNN:     jlt         Ry,AMPL_CNT,@loopExitNN
            ...
            add         Rx,AMPL_STEP,Rx
            add         Ry,1,Ry
            jmp         @loopNN
loopExitNN:
```

FIXME: too slow on CC with single 20 ns gate inside loop
     
### if
```
if(cond) {} else if(cond) () else ();
```

FIXME: TBW

### switch

QYASM:
```
switch(idx) {
    case 0:     I q;                    break;
    case 1:     Y90 q;  X90 q;          break;
    ...
    case 23:    X90 q;  Y90 q;  mX90;   break;
    default:    trap();
}
```
                
CC backend output fragment:
```
            move        idx,Rx              ; or reuse register 'idx'
            jge         Rx,24,@defaultNN
            asl         Rx,2,Ry             ; Rz = 4 * idx. Size depends on maximum case size
            nop                             ; because of register dependency (or move up preceding instruction) 
            add         @case0,Ry,Ry        ; computed goto
            nop                             ; register dependency
            jmp         Ry
case0:      seq_out     I,N
            jmp         switchExitNN
            nop                             ; align #instructions per case
            nop
case1:      seq_out     Y90,N
            seq_out     X90,N
            jmp         switchExitNN
            nop
...
case23:
            seq_out     X90,N
            seq_out     Y90,N
            seq_out     mX90,N
            jmp         switchExitNN
                
defaultNN:  trap
switchExitNN:
```

## Function call
### Q1 local function call
The Q1 processor features neither a stack nor a subroutine call instruction. It can support a single level of 
subroutine calling using the jump indirect mechanism. Support for local variables could be added by reserving
some registers for this purpose.

QYASM:
```
int var;

...
function(0, var);   // function call
...

// function definition
function(int par0, int par1)
{
    ...
}
```

CC backend output fragment:
```
# define registers for ABI
.DEF        regPar0     R0
.DEF        regPar1     R1
.DEF        regRts      R63

# function call
            ...
            move        0,$regPar0          ; par0
            move        var,$regPar1        ; par1
            move        @retCall,$regRts    ; return address
            jmp         @function           ; call
retCall:    ...                             ; continue after call

# function definition
function:   ...
            jmp         @regRts             ; return
```

### Q1 to ARM function call

FIXME: TBW

### Host to Q1 function call

FIXME: TBW


## Special gates (future)
### Conditional gates (vectorized, based on classic variable)
Use cases:
- randomized benchmarking

QYASM:
```
const gate gatesCliffordEpsteinColumn0[24] = {
    I,      Y90,    mX90,   X180,   mY90,
    X90,    Y180,   mY90,   X90,    X180,
    Y90,    mx90,   Y90,    mX90,   X90,
    mY90,   X90,    X90,    mY90,   X90,
    X90,    Y90,    mX90,   X90};
condGate gatesCliffordEpsteinColumn0[idx_Y90] q[0] | condGate gatesCliffordEpsteinColumn0[idx_mY90] q[1];
```

FIXME: TBW

### Parametrized gate
QYASM:
```
flux q,(amplitude,len);     // FIXME: len as parameter to flux?
```

FIXME: TBW

