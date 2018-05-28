%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    int yylex(void);
    void yyerror (char const *);
%}

//%define api.value.type {double}
%union {
  int ival;
  double dval;
  char* sval;
}

%token <sval> NAME 
%token <ival> INTEGER
%token <dval> FLOAT
%token COMMA_SEPARATOR PARALLEL_SEPARATOR BRA KET DOT SBRA SKET CBRA CKET LS_SEP NEWLINE WS COLON COMMENT
%token ROTATIONS AXIS
%token QUBITS
%token SINGLE_QUBIT_GATES TWO_QUBIT_GATES CR TOFFOLI
%token CDASH NOT_TOKEN
%token MAPKEY PREP MEASURE MEASUREPARITY MEASUREALL
%token WAIT DISPLAY RESET_AVERAGING
%token QBITHEAD BITHEAD


%start qasm-file

%%

//# Describe the general structure of a qasm file
qasm-file : qubit-register line-separator circuits
          | comments line-separator qasm-file
    ;
circuits : circuit 
           | circuits circuit
           | comments
           | circuits comments

    ;
circuit : subcircuit statements 
              | statements
    ;
subcircuit : DOT NAME {printf("Found subcircuit: %s\n", $2);}
                | subcircuit BRA INTEGER KET {printf("With: %d iterations\n", $3);}
    ;
statements : qasm-line | subcircuit
             |qasm-line line-separator
             | subcircuit line-separator
             | statements line-separator qasm-line
             | statements line-separator subcircuit
    ;
comments : COMMENT | comments COMMENT
    ;
qasm-line : map-operation
          | measureall-operation
          | measure-parity-operation
          | regular-operations
          | binary-controlled-operations
          | parallel-operations
          | special-operations
    ;

//# We define the convenience strings, texts, numbers here....
line-separator : NEWLINE | line-separator NEWLINE
    ;
indices : SBRA numerical-identifiers SKET
    ;
numerical-identifiers : numerical-identifier-list 
                        | numerical-identifier-range
    ;
numerical-identifier-list : INTEGER | numerical-identifier-list COMMA_SEPARATOR numerical-identifiers
    ;
numerical-identifier-range : INTEGER COLON INTEGER {printf("Num: {%d,%d}\n", $1, $3);}| numerical-identifier-range COMMA_SEPARATOR numerical-identifiers
    ;


qubit-register : QUBITS WS INTEGER | line-separator QUBITS WS INTEGER

//# We define the syntax for selecting the qubits/bits, either by a range or a list
qubit : qubit-nomap | NAME
    ;
qubit-nomap : QBITHEAD indices
    ;
qubit-selection : qubit | qubit-selection COMMA_SEPARATOR qubit 
    ;
//## Next we define a classical bit, which is required to perform control gate operations
bit :  bit-nomap | NAME
    ;
bit-nomap : BITHEAD indices
    ;
bit-selection : bit
                | bit-selection COMMA_SEPARATOR bit
    ;

//# Define the single qubit operation line
single-qubit-operation : single-qubit-gate WS qubit 
                             | prep_measure-ops WS qubit
    ;
single-qubit-operation-args : parameterized-single-qubit-gate WS qubit COMMA_SEPARATOR FLOAT {printf("Found float = %lf\n",$5);}
    ;
map-operation : MAPKEY WS qubit-selection COMMA_SEPARATOR NAME {printf("Mapped qubit to %s\n",$5);}| MAPKEY WS bit-selection COMMA_SEPARATOR NAME {printf("Mapped bit to %s\n",$5);}
    ;
//## Define the single qubit operations/gates
single-qubit-gate : AXIS | SINGLE_QUBIT_GATES
    ;
parameterized-single-qubit-gate : ROTATIONS
    ;
//# This is to define the state preparation/measurement
prep_measure-ops : PREP | MEASURE
    ;
measure-parity-operation : MEASUREPARITY WS qubit COMMA_SEPARATOR AXIS | measure-parity-operation COMMA_SEPARATOR qubit COMMA_SEPARATOR AXIS
    ;
measureall-operation : MEASUREALL
    ;

//# Qubit-controlled operations
two-qubit-operation : two-qubit-gates WS qubit COMMA_SEPARATOR qubit
    ;
two-qubit-operation-args : two-qubit-gate-args WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
    ;
//## Define the two qubit gates
two-qubit-gates : TWO_QUBIT_GATES
    ;
two-qubit-gate-args : CR
    ;
//## Define the toffoli gate
toffoli-operation : toffoli-gate WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
    ;
toffoli-gate : TOFFOLI
    ;
//## Define a superset of all general "normal" operations
regular-operations : single-qubit-operation 
                   | single-qubit-operation-args
                   | two-qubit-operation 
                   | two-qubit-operation-args
                   | toffoli-operation 
    ;
//# Binary-controlled operations
qubit-gates : single-qubit-gate 
                  | parameterized-single-qubit-gate
                  | two-qubit-gates 
                  | two-qubit-gate-args 
                  | toffoli-gate
    ;

binary-controlled-operations : bit-single-qubit-operation 
                                   | bit-single-qubit-operation-args 
                                   | bit-two-qubit-operation 
                                   | bit-two-qubit-operation-args 
                                   | bit-toffoli-operation 
                                   | negate-binary-operation
    ;
bit-single-qubit-operation : CDASH single-qubit-gate WS bit-selection COMMA_SEPARATOR qubit-selection
    ;
bit-single-qubit-operation-args : CDASH parameterized-single-qubit-gate WS bit-selection COMMA_SEPARATOR qubit-selection COMMA_SEPARATOR FLOAT
    ;
bit-two-qubit-operation : CDASH two-qubit-gates WS bit-selection COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
    ;
bit-two-qubit-operation-args : CDASH two-qubit-gate-args WS bit-selection COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
    ;
bit-toffoli-operation : CDASH toffoli-gate WS bit-selection COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
    ;
negate-binary-operation : NOT_TOKEN WS bit-selection
    ;

//# Parallel execution
parallel-operations : CBRA parallelizable-ops CKET
    ;
all-valid-operations : regular-operations | binary-controlled-operations 
    ;
parallelizable-ops : all-valid-operations | parallelizable-ops PARALLEL_SEPARATOR all-valid-operations
    ;

//# Special operations
special-operations : display-operation | wait-operation | reset-averaging-operation
    ;
display-operation : DISPLAY | display-operation WS bit-selection
    ;
wait-operation : WAIT WS INTEGER
    ;
reset-averaging-operation : RESET_AVERAGING | reset-averaging-operation WS qubit-selection
    ;

%%

void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}