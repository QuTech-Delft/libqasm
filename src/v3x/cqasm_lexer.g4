lexer grammar cqasm_lexer;

// These are all supported lexer sections:

// Lexer file header. Appears at the top of h + cpp files. Use e.g. for copyrights.
@lexer::header {}

// Appears before any #include in h + cpp files.
@lexer::preinclude {}

// Follows directly after the standard #includes in h + cpp files.
@lexer::postinclude {}

// Directly precedes the lexer class declaration in the h file (e.g. for additional types etc.).
@lexer::context {}

// Appears in the public part of the lexer in the h file.
@lexer::members {}

// Appears in the private part of the lexer in the h file.
@lexer::declarations {}

// Appears in line with the other class member definitions in the cpp file.
@lexer::definitions {}

/*
* Things we are leaving out at the moment:
* - Annotations.
* - Bundles.
* - JSON literals.
* - Multi-line strings.
* - Statements:
*   - c-dash, cond.
*   - cQasm v1.2: if, else, for, foreach, while, repeat, until, continue, break, set, reset-averaging.
* - Subcircuits.
*
* Things we are treating different than cQasm v1.x lexer/grammar:
* - Unexpected EOF.
* - Version. We don't have a Version mode. We just plan to parse the version in the parser.
*/

channels {
    COMMENTS_CHANNEL
}

// Rules
NewLine: \r?\n -> skip;
EndOfLine: NewLine | EOF -> skip;
SingleLineWhiteSpace: [ \t]+ -> skip;
MultiLineWhiteSpace: (SingleLineWhiteSpace | NewLine)+;
SingleLineComment: '#' ~[\r\n]* EndOfLine -> channel(COMMENTS_CHANNEL);
MultiLineCommentStart: '/*' -> pushMode(MULTI_LINE_COMMENT);
StringStart: '"' -> pushMode(STRING);

// Single character tokens
OpenParen: '(';
CloseParen: ')';
OpenBracket: '[';
CloseBracket: ']';
OpenBrace: '{';
CloseBrace: '}';
OpenAngle: '<';
CloseAngle: '>';

Colon: ':';
Semicolon: ';';
Comma: ',';
Dot: '.';
QuestionMark: '?';
ExclamationMark: '!';
Tilde: '~';
Ampersand: '&';
Pipe: '|';
Circumflex: '^';
Backslash: '\\';

SingleQuote: '\'';
DoubleQuote: '"';

Plus: '+';
Minus: '-';
Star: '*';
Slash: '/';
Percent: '%';
Equal: '=';

// Operators
OpPower: '**';
OpIntegerDivision: '//';
OpLogicOr: '||';
OpLogicAnd: '&&';
OpLogicXor: '^^';
OpGE: '>=';
OpLE: '<=';
OpEq: '==';
OpNE: '!=';
OpShL: '<<';
OpArithmeticShR: '>>';
OpLogicShR: '>>>';
OpEllipsis: '..';

// Keywords
KwQubits: 'qubits';
KwMap: 'map';
KwVar: 'var';

// Numeric literals
Digit: [0-9];
IntegerLiteral: Digit+;
FloatLiteral: Digit* Dot Digit+([eE][-+]Digit+)?;

// Identifiers
Letter : [a-zA-Z_];
Identifier: Letter (Letter | Digit)*;

// Modes
mode MULTI_LINE_COMMENT;
MultiLineCommentEnd: '*/' -> popMode(MULTI_LINE_COMMENT);
ANY: . -> channel(COMMENTS_CHANNEL);

mode STRING;
StringEnd: '"' -> popMode(STRING);
EscapedCharacter: [\\][tn'"\\] -> stringPushBack();
NotNewLine: ~NewLine+ -> stringPushBack();
ErrorChar: .;
