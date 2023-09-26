parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start.
program: version qubits? statement* EOF;

version: VERSION INTEGER_LITERAL (DOT INTEGER_LITERAL)?;

qubits: QUBITS expression;

statement:
    MAP IDENTIFIER EQUAL expression  # mapping
    | VAR IDENTIFIER COLON IDENTIFIER  # variable
    | IDENTIFIER expressionList  # instruction
    ;

expressionList: expression (COMMA expression)?;

expression:
    INTEGER_LITERAL  # integerLiteral
    | FLOAT_LITERAL  # floatLiteral
    | IDENTIFIER  # identifier
    | IDENTIFIER OPEN_BRACKET expression CLOSE_BRACKET  # index
    ;

/*
* Things we are leaving out at the moment:
* - Function calls.
* - Matrix literals, string literals.
* - Multiple variable declarations in the same line.
* - Operators: unary, binary, ternary.
* - Semicolons don't start a new statement.
* - Array elements cannot be neither a list of indices nor a range of indices.
*/
