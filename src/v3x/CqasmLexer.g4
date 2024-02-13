// Requirements
//
// REQ001: cQASM shall be case-sensitive.
//         Allows to distinguish between, e.g., an X gate and an x axis.
//
// REQ002: cQASM shall be newline-sensitive.
//         Semicolons may still be used to separate instructions on the same line (if required).
//
// REQ003: cQASM shall support a version statement that accepts the version number as an argument.
//         It must be the first statement of the program, and occur only once.
//
// REQ004: cQASM shall support the following named constants: pi, im, eu, tau.
//         They are not to be reserved as keywords.
//
// REQ005: cQASM shall support the following types: qubit, bit, bool, int, float.
//
// REQ007: cQASM shall support the axis type.
//
// REQ008: cQASM shall support the following single-qubit gate modifier: pow.
//
// REQ009: cQASM shall support the following single-qubit gate modifier: inv.
//
// REQ010: cQASM shall support the following single-qubit gate modifier: ctrl.
//
// REQ011: cQASM shall support the following two-qubit gate modifiers: pow, inv, ctrl.
//
// REQ012: cQASM shall support a measure instruction that, optionally,
//         accepts an argument of type axis denoting the axis of measurement.
//         Examples:
//             b0 = measure(x) q0
//             axis a = [0, 1, 1]; br0 = measure(a) qr0
//
// REQ013: cQASM shall support the assignment of a measurement outcome to a bit variable.
//         Examples:
//             b[0] = measure q[0]      The measurement outcome of q[0] is stored in b[0]
//             b2 = measure q[3]        The measurement outcome of q[3] is stored in b2
//             b[2:4] = measure q[3:5]  The measurement outcome of q[3:5] is stored in b[2:4]
//
// REQ014: cQASM shall support the explicit declaration of qubit registers through a declaration statement.
//         Examples:
//             qubit q0, q1       Declaration of single qubits
//             qubit[3] qr0, qr1  Declaration of qubit registers/arrays of size 3
//
// REQ015: cQASM shall support the explicit declaration of bit registers through a declaration statement.
//         Examples:
//             bit b0, b1       Declaration of single bits
//             bit[2] br0, br1  Declaration of bit registers/arrays of size 2
//
// REQ016: cQASM shall support the declaration of separate qubit and bit registers of equal or distinct lengths.
//
// REQ021: cQASM shall support the definition of composite gates.
//
// REQ023: cQASM shall expose the quantum instruction database through its API.
//
// REQ024: when Single Gate Multiple Qubit (SGMQ) notation is used in multi-qubit gates,
//         cQASM shall preserve the order of the qubit arguments.
//         E.g. the following:  CNOT q[0,1], q[3,2]  is equivalent to:  CNOT q[0], q[3]; CNOT q[1], q[2]
//         Examples of SGMQ:
//             X q[0:2]    is functionally equivalent to  X q[0]; X q[1]; X q[2]
//             X q[1,3,6]  is functionally equivalent to  X q[1]; X q[3]; X[6]

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
OPEN_BRACE: '{';
CLOSE_BRACE: '}';
OPEN_PARENS: '(';
CLOSE_PARENS: ')';
PLUS: '+';  // this token is shared by UNARY_PLUS_OP and PLUS_OP
MINUS: '-';  // this token is shared by UNARY_MINUS_OP and MINUS_OP
ARROW: '->';

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
VERSION: 'version' -> pushMode(VERSION_STATEMENT);
MEASURE: 'measure';
QUBIT_TYPE: 'qubit';
BIT_TYPE: 'bit';
AXIS_TYPE: 'axis';
BOOL_TYPE: 'bool';
INT_TYPE: 'int';
FLOAT_TYPE: 'float';
FUNCTION: 'def';
VOID: 'void';
RETURN: 'return';

// Numeric literals
BOOLEAN_LITERAL: 'true' | 'false';
INTEGER_LITERAL: Digit+;
FLOAT_LITERAL:
    Digit+ '.' Digit+ Exponent?
    | Digit+ '.' Exponent?  // float literals can end with a dot
    | '.' Digit+ Exponent?;  // or just start with a dot
fragment Digit: [0-9];
fragment Exponent: [eE][-+]?Digit+;

// Identifier
IDENTIFIER: Letter (Letter | Digit)*;
fragment Letter: [a-zA-Z_];

// Version mode
//
// Whenever we encounter a 'version' token, we enter the Version mode
// Within the version mode, a sequence such as '3.0' will be treated as a version number, and not as a float literal
mode VERSION_STATEMENT;
VERSION_WHITESPACE: [ \t]+ -> skip;
VERSION_NUMBER: Digit+ ('.' Digit+)? -> popMode;
