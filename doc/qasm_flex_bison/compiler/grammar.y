%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    int yylex(void);
    void yyerror (char const *);
%}

%define api.value.type {double}

%token NAME 
%token LETTER 
%token INTEGER 
%token FLOAT 
%token NEWLINE 
%token WS 
%token COLON
%token ROTATIONS
%token QUBITS
%token WAIT
%token DISPLAY
%token RESET_AVERAGING
%token COMMA_SEPARATOR BRA KET DOT SBRA SKET QBITHEAD BITHEAD LS_SEP

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
qasm-line : special-operations
    ;

//# We define the convenience strings, texts, numbers here....
line-separator : NEWLINE | line-separator NEWLINE
    ;
indices : SBRA numerical-identifiers SKET
    ;
numerical-identifiers : numerical-identifier-list 
                           | numerical-identifier-range
                           | numerical-identifier-list COMMA_SEPARATOR numerical-identifiers
                           | numerical-identifier-range COMMA_SEPARATOR numerical-identifiers
    ;
numerical-identifier-list : INTEGER | INTEGER COMMA_SEPARATOR numerical-identifier-list
    ;
numerical-identifier-range : INTEGER COLON INTEGER
    ;


qubit-register : QUBITS WS INTEGER

//# We define the syntax for selecting the qubits/bits, either by a range or a list
qubit : qubit-nomap | NAME
    ;
qubit-nomap : QBITHEAD indices
    ;
qubit-arguments : QBITHEAD indices | NAME
    ;
qubit-selection : qubit-arguments | qubit-arguments COMMA_SEPARATOR qubit-selection
    ;
//## Next we define a classical bit, which is required to perform control gate operations
bit :  bit-nomap | NAME
    ;
bit-nomap : BITHEAD indices
    ;
bit-selection : bit
                | bit COMMA_SEPARATOR bit-selection
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