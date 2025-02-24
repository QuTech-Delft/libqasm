lexer grammar CqasmLexer;

// White spaces and comments are skipped, i.e. not passed to the parser
WHITE_SPACE: [ \t]+ -> skip;
SINGLE_LINE_COMMENT: '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*' .*? '*/' -> skip;

// Signs
NEW_LINE: '\r'?'\n';
SEMICOLON: ';';
COLON: ':';
COMMA: ',';
DOT: '.';
EQUALS: '=';
OPEN_BRACKET: '[';
CLOSE_BRACKET: ']';
OPEN_PARENS: '(';
CLOSE_PARENS: ')';
PLUS: '+';  // this token is shared by UNARY_PLUS_OP and PLUS_OP
MINUS: '-';  // this token is shared by UNARY_MINUS_OP and MINUS_OP
TRIPLE_QUOTE: '\'\'\'';

// Operators
// UNARY_PLUS_OP: '+';
// UNARY_MINUS_OP: '-';
BITWISE_NOT_OP: '~';
LOGICAL_NOT_OP: '!';
POWER_OP: '**';
PRODUCT_OP: '*';
DIVISION_OP: '/';
MODULO_OP: '%';
// PLUS_OP: '+';
// MINUS_OP: '-';
SHL_OP: '<<';
SHR_OP: '>>';
CMP_GT_OP: '>';
CMP_LT_OP: '<';
CMP_GE_OP: '>=';
CMP_LE_OP: '<=';
CMP_EQ_OP: '==';
CMP_NE_OP: '!=';
BITWISE_AND_OP: '&';
BITWISE_XOR_OP: '^';
BITWISE_OR_OP: '|';
LOGICAL_AND_OP: '&&';
LOGICAL_XOR_OP: '^^';
LOGICAL_OR_OP: '||';
TERNARY_CONDITIONAL_OP: '?';

// Keywords
VERSION: 'version' -> pushMode(VERSION_STATEMENT);  // version
MEASURE: 'measure';  // non-unitary instructions
RESET: 'reset';
INIT: 'init';
BARRIER: 'barrier';
WAIT: 'wait';
INV: 'inv';  // gate modifiers
POW: 'pow';
CTRL: 'ctrl';
QUBIT_TYPE: 'qubit';  // types
BIT_TYPE: 'bit';
ASM: 'asm';  // assembly declaration

// Numeric literals
BOOLEAN_LITERAL: 'true' | 'false';
INTEGER_LITERAL: Digit+;
FLOAT_LITERAL:
    Digit+ DOT Digit+ Exponent?
    | Digit+ DOT Exponent?  // float literals can end with a dot
    | DOT Digit+ Exponent?;  // or just start with a dot
fragment Digit: [0-9];
fragment Exponent: [eE][-+]?Digit+;

// Identifier
IDENTIFIER: Letter (Letter | Digit)*;
fragment Letter: [a-zA-Z_];

// Raw text string
RAW_TEXT_STRING: TRIPLE_QUOTE RawText TRIPLE_QUOTE;
fragment RawText: (.)*?;

// Version mode
//
// Whenever we encounter a 'version' token, we enter the Version mode
// Within the Version mode, a sequence such as '3.0' will be treated as a version number, and not as a float literal
mode VERSION_STATEMENT;
VERSION_WHITE_SPACE: [ \t]+ -> skip;
VERSION_NUMBER: Digit+ ('.' Digit+)? -> popMode;
