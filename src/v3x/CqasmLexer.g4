lexer grammar CqasmLexer;

@members {
    int unmatched_inner_open_braces = 0;
}

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
ASM: 'asm'  -> pushMode(ASSEMBLY_DECLARATION);  // assembly declaration

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

// Version mode
//
// Whenever we encounter a 'version' token, we enter the Version mode
// Within the Version mode, a sequence such as '3.0' will be treated as a version number, and not as a float literal
mode VERSION_STATEMENT;
VERSION_NUMBER: Digit+ ('.' Digit+)? -> popMode;

// Assembly declaration mode
//
// Whenever we encounter an 'asm' token, we enter the Assembly declaration mode
mode ASSEMBLY_DECLARATION;
BE_CODE_OPEN_BRACE: '{' -> pushMode(BACKEND_CODE);

// Backend code
//
// Whenever we are in Assembly declaration mode and encounter a '{' token, we enter the Backend code mode
mode BACKEND_CODE;
BE_CODE: (BE_CODE_CHAR | BE_CODE_INNER_OPEN_BRACE | BE_CODE_INNER_CLOSE_BRACE)+;
BE_CODE_CHAR: ~[{}];
BE_CODE_INNER_OPEN_BRACE: '{' { unmatched_inner_open_braces++; };
BE_CODE_INNER_CLOSE_BRACE: '}' { if (unmatched_inner_open_braces) { unmatched_inner_open_braces--; } };
BE_CODE_CLOSE_BRACE: '}' { popMode(); popMode(); };
