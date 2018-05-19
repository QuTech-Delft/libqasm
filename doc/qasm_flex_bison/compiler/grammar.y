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
%token DIGIT 
%token NONZERODIGIT 
%token NUMBER 
%token FLOAT 
%token NEWLINE 
%token WS_SEPARATOR 
%token COLON

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
subcircuit : "." identifier NEWLINE 
                | "." identifier "(" NUMBER ")" NEWLINE
	;
statements : qasm-line NEWLINE 
                 | qasm-line NEWLINE statements
	;
qasm-line : map-operation 
	;

//# This defines the number of qubits taking part in this qasm file
qubit-register : "qubits " NUMBER
	;


//# We define the convenience strings, texts, numbers here....
line-separator : NEWLINE | NEWLINE line-separator
	;
par-separator : "|" 
                 | " |" 
                 | "| " 
                 | " | "
	;
identifier : NAME
	;
indices : "[" numerical-identifiers "]"
	;
numerical-identifiers : numerical-identifier-list 
                           | numerical-identifier-range
                           | numerical-identifier-list WS_SEPARATOR numerical-identifiers
                           | numerical-identifier-range WS_SEPARATOR numerical-identifiers
	;
numerical-identifier-list : NUMBER | NUMBER WS_SEPARATOR numerical-identifier-list
	;
numerical-identifier-range : NUMBER COLON NUMBER
	;
axis : "x" | "y" | "z"

//# We define the syntax for selecting the qubits/bits, either by a range or a list
qubit : qubit-nomap | mapped-qubit
	;
qubit-nomap : "q[" NUMBER "]"
	;
qubit-arguments : "q" indices | mapped-qubit
	;
qubit-selection : qubit-arguments | qubit-arguments WS_SEPARATOR qubit-selection
	;
mapped-qubit : identifier
	;
//## Next we define a classical bit, which is required to perform control gate operations
bit :  bit-nomap | mapped-bit
	;
bit-nomap : "b" indices
	;
bit-selection : bit
                    | bit WS_SEPARATOR bit-selection
	;
mapped-bit : identifier
	;


//# Define the single qubit operation line
map-operation : "map" " " qubit-selection WS_SEPARATOR mapped-qubit | "map" " " bit-selection WS_SEPARATOR mapped-bit
%%

void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}