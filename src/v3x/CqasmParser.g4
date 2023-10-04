parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start
program:
    version (NEW_LINE | SEMICOLON | EOF)
    (statement (NEW_LINE | SEMICOLON | EOF))*
    ;

version: VERSION VERSION_NUMBER;

statement:
    (QUBIT_TYPE | BIT_TYPE) (OPEN_BRACKET indexList CLOSE_BRACKET)? ID  # typeDefinition
    | expressionList EQUAL MEASURE expressionList  # measureInstruction
    | ID expressionList  # instruction
    ;

expressionList: expression (COMMA expression)*;

indexList: indexEntry (COMMA indexEntry)*;

indexEntry: expression (COLON expression)?;

expression:
    INT  # int
    | FLOAT  # float
    | ID  # id
    | ID OPEN_BRACKET indexList CLOSE_BRACKET  # index
    ;
