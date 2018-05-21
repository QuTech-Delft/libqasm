%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    int yylex(void);
    void yyerror (char const *);
%}

%define api.value.type {double}

%token NAME 
%token INTEGER FLOAT
%token COMMA_SEPARATOR PARALLEL_SEPARATOR BRA KET DOT SBRA SKET LS_SEP NEWLINE WS COLON
%token ROTATIONS AXIS
%token QUBITS
%token SINGLE_QUBIT_GATES
%token MAPKEY PREP MEASURE MEASUREPARITY MEASUREALL
%token WAIT DISPLAY RESET_AVERAGING
%token QBITHEAD BITHEAD

%start qasm-file

%%

//# Describe the general structure of a qasm file
qasm-file : qubit-register line-separator circuits
    ;
circuits : circuit 
               | circuit circuits
    ;
circuit : subcircuit statements 
              | statements
    ;
subcircuit : DOT NAME line-separator 
                | DOT NAME BRA INTEGER KET line-separator
    ;
statements : qasm-line
                 | statements line-separator qasm-line
    ;
qasm-line : map-operation
          | measureall-operation
          | measure-parity-operation
          | regular-operations
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
numerical-identifier-range : INTEGER COLON INTEGER | numerical-identifier-range COMMA_SEPARATOR numerical-identifiers
    ;


qubit-register : QUBITS WS INTEGER

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
single-qubit-operation : single-qubit-gate WS qubit-selection 
                             | prep_measure-ops WS qubit-selection
    ;
single-qubit-operation-args : parameterized-single-qubit-gate WS qubit-selection COMMA_SEPARATOR FLOAT
    ;
map-operation : MAPKEY WS qubit-selection COMMA_SEPARATOR NAME | MAPKEY WS bit-selection COMMA_SEPARATOR NAME
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

//## Define a superset of all general "normal" operations
regular-operations : single-qubit-operation 
                   | single-qubit-operation-args 
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