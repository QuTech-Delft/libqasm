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
//         Examples:
//             int a              Variable declaration
//             int a = 1          Variable declaration and assignment
//             a = 1              Variable assignment (preceded by variable declaration of ‘a’)
//             int[2] a           Array of integers declaration
//             int[2] a = {0, 1}  Array of integers declaration and assignment
//             a = {0, 1}         Array of integers assignment (if 'a' has been declared as an array of size 2)
//
// REQ013: cQASM shall support the optional assignment of the measurement outcome(s) of the measurement instruction:
//         measure(\([x-z<id>]+\))? to (a) bit variable(s).
//         Examples:
//             b[0], b[1] = measure q[0], q[1]  The measurement outcome of q[0] is stored in b[0], and of q[1] in b[1]
//             b2, b3 = measure q[3], q[4]      The measurement outcome of q[3] is stored in b2, and of q[4] in b3
//             b[2:4] = measure q[3:5]          The measurement outcome of q[3:5] is stored in b[2:4]
//             b0, b1, b2 = measure q[0:1]      No value is assigned to b2
//
// REQ014: cQASM shall support the explicit declaration of qubit registers through a declaration statement:
//         qubit(\[[0-9]+\])* <id>(, <id>)*
//         where the number enclosed in square brackets denotes the size of the register.
//         Examples:
//             qubit q0       Declaration of a single qubit reference
//             qubit[3] q     Declaration of a qubit register/array of size 3
//
// REQ015: cQASM shall support the explicit declaration of bit registers through a declaration statement:
//         bit(\[[0-9]+\])* <id>(, <id>)*
//         where the number enclosed in square brackets denotes the size of the register.
//         Examples:
//             bit b0         Declaration of a single bit
//             bit[2] b2      Declaration of a bit register/array of size 2
//             bit[3][2] b32  Declaration of a 2D bit array of dimensions 3 x 2
//
// REQ016: cQASM shall support the declaration of separate qubit and bit registers of equal or distinct lengths.
//
// REQ024: when Single Gate Multiple Qubit (SGMQ) notation is used in multi-qubit gates,
//         cQASM shall preserve the order of the qubit arguments.
//         E.g. the following CNOT q[0,1], q[3,2] is equivalent to CNOT q[0], q[3]; CNOT q[1], q[2]
//         Examples of SGMQ:
//             X q[0:2] is functionally equivalent to X q[0]; X q[1]; X q[2]
//             X q[1,3,6] is functionally equivalent to X q[1]; X q[3]; X[6]

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
