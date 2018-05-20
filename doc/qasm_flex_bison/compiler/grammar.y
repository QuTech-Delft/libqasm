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
%token COMMA_SEPARATOR

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
subcircuit : "." NAME NEWLINE 
                | "." NAME "(" INTEGER ")" NEWLINE
	;
statements : qasm-line NEWLINE 
                 | qasm-line NEWLINE statements
	;
qasm-line : special-operations
	;

//# We define the convenience strings, texts, numbers here....
line-separator : NEWLINE | NEWLINE line-separator
	;
indices : "[" numerical-identifiers "]"
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
qubit : qubit-nomap | mapped-qubit
	;
qubit-nomap : "q" "[" INTEGER "]"
	;
qubit-arguments : "q" indices | mapped-qubit
	;
qubit-selection : qubit-arguments | qubit-arguments COMMA_SEPARATOR qubit-selection
	;
mapped-qubit : NAME
	;
//## Next we define a classical bit, which is required to perform control gate operations
bit :  bit-nomap | mapped-bit
	;
bit-nomap : "b" indices
	;
bit-selection : bit
                    | bit COMMA_SEPARATOR bit-selection
	;
mapped-bit : NAME
	;


//# Special operations
special-operations : display-operation | wait-operation | reset-averaging-operation
	;
display-operation : DISPLAY | DISPLAY WS bit
	;
wait-operation : WAIT WS INTEGER
	;
reset-averaging-operation : RESET_AVERAGING | RESET_AVERAGING WS qubit
	;

%%

void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}