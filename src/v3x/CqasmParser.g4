parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start.
program: version qubitDefinition? statement* EOF;

version: VERSION VERSION_NUMBER;

qubitDefinition: QUBIT OPEN_BRACKET expression CLOSE_BRACKET ID;

statement:
    MAP ID EQUAL expression  # mapping
    | VAR ID COLON ID  # variable
    | ID expressionList  # instruction
    ;

expressionList: expression (COMMA expression)?;

indexList: expression (COMMA expression)?;

expression:
    INT  # int
    | FLOAT  # float
    | ID  # id
    | ID OPEN_BRACKET indexList CLOSE_BRACKET  # index
    ;
