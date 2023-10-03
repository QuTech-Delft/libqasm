lexer grammar CqasmLexer;

// Rules
WS: [ \t]+ -> skip;
NEW_LINE: '\r'?'\n' -> skip;
COMMENT: '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*' .*? '*/' -> skip;

// Operators
VERSION: 'version' -> pushMode(VERSION_STATEMENT);
COLON: ':';
COMMA: ',';
DOT: '.';
EQUAL: '=';
OPEN_BRACKET: '[';
CLOSE_BRACKET: ']';

// Keywords
QUBIT: 'qubit';
MAP: 'map';
VAR: 'var';

// Numeric literals
fragment Digit: [0-9];
INT: Digit+;
FLOAT: Digit* '.' Digit+([eE][-+]Digit+)?;

// Identifiers
fragment Letter : [a-zA-Z_];
ID: Letter (Letter | Digit)*;

mode VERSION_STATEMENT;
VS_WS: [ \t]+ -> skip;
VERSION_NUMBER: Digit '.' Digit -> popMode;
