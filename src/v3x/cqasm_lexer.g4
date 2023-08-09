lexer grammar cqasm_lexer;

// Rules
NEW_LINE: '\r'?'\n' -> skip;
END_OF_LINE: (NEW_LINE | EOF) -> skip;
SINGLE_LINE_WHITE_SPACE: [ \t]+ -> skip;
MULTI_LINE_WHITE_SPACE: (SINGLE_LINE_WHITE_SPACE | NEW_LINE)+;
SINGLE_LINE_COMMENT: '#' ~[\r\n]* END_OF_LINE -> skip;
MULTI_LINE_COMMENT_START: '/*' -> pushMode(MULTI_LINE_COMMENT);

// Operators
VERSION: 'version';
COLON: ':';
COMMA: ',';
DOT: '.';
EQUAL: '=';
OPEN_BRACKET: '[';
CLOSE_BRACKET: ']';

// Keywords
QUBITS: 'qubits';
MAP: 'map';
VAR: 'var';

// Numeric literals
fragment Digit: [0-9];
INTEGER_LITERAL: Digit+;
FLOAT_LITERAL: Digit* '.' Digit+([eE][-+]Digit+)?;

// Identifiers
fragment Letter : [a-zA-Z_];
IDENTIFIER: Letter (Letter | Digit)*;

// Modes
mode MULTI_LINE_COMMENT;
MULTI_LINE_COMMENT_END: '*/' -> popMode;
ANY: . -> skip;

/*
* Things we are leaving out at the moment:
* - Annotations.
* - Bundles.
* - JSON literals.
* - String literals.
* - Multi-line strings.
* - Statements:
*   - c-dash, cond.
*   - cQasm v1.2: if, else, for, foreach, while, repeat, until, continue, break, set, reset-averaging.
* - Subcircuits.
*/
