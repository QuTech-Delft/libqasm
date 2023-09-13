parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start.
program: version QUBITS expression statement*;

version: VERSION INTEGER_LITERAL (DOT INTEGER_LITERAL)?;

statement: mapStatement | varStatement | instruction;

mapStatement: MAP IDENTIFIER EQUAL expression;

varStatement: VAR IDENTIFIER COLON IDENTIFIER;

instruction: IDENTIFIER expressionList;

expressionList: expression (COMMA expression)?;

expression:
    INTEGER_LITERAL
    | FLOAT_LITERAL
    | IDENTIFIER
    | arrayElements;

arrayElements: IDENTIFIER OPEN_BRACKET arrayIndex CLOSE_BRACKET;
arrayIndex: expression;

/*
* Things we are leaving out at the moment:
* - Function calls.
* - Matrix literals, string literals.
* - Multiple variable declarations in the same line.
* - Operators: unary, binary, ternary.
* - Semicolons don't start a new statement.
* - Array elements cannot be neither a list of indices nor a range of indices.
*/
