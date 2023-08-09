parser grammar cqasm_parser;

options {
	tokenVocab = cqasm_lexer;
}

// These are all supported parser sections:

// Parser file header. Appears at the top in all parser related files. Use e.g. for copyrights.
@parser::header {}

// Appears before any #include in h + cpp files.
@parser::preinclude {}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
}

// Directly precedes the parser class declaration in the h file (e.g. for additional types etc.).
@parser::context {}

// Appears in the private part of the parser in the h file.
@parser::members {}

// Appears in the public part of the parser in the h file.
@parser::declarations {}

// Appears in line with the other class member definitions in the cpp file.
@parser::definitions {}

// Additionally there are similar sections for (base)listener and (base)visitor files.
@parser::listenerpreinclude {/* listener preinclude section */}
@parser::listenerpostinclude {/* listener postinclude section */}
@parser::listenerdeclarations {/* listener public declarations/members section */}
@parser::listenermembers {/* listener private declarations/members section */}
@parser::listenerdefinitions {/* listener definitions section */}

@parser::baselistenerpreinclude {/* base listener preinclude section */}
@parser::baselistenerpostinclude {/* base listener postinclude section */}
@parser::baselistenerdeclarations {/* base listener public declarations/members section */}
@parser::baselistenermembers {/* base listener private declarations/members section */}
@parser::baselistenerdefinitions {/* base listener definitions section */}

@parser::visitorpreinclude {/* visitor preinclude section */}
@parser::visitorpostinclude {/* visitor postinclude section */}
@parser::visitordeclarations {/* visitor public declarations/members section */}
@parser::visitormembers {/* visitor private declarations/members section */}
@parser::visitordefinitions {/* visitor definitions section */}

@parser::basevisitorpreinclude {/* base visitor preinclude section */}
@parser::basevisitorpostinclude {/* base visitor postinclude section */}
@parser::basevisitordeclarations {/* base visitor public declarations/members section */}
@parser::basevisitormembers {/* base visitor private declarations/members section */}
@parser::basevisitordefinitions {/* base visitor definitions section */}

/*
* Things we are leaving out at the moment:
* - Function calls.
* - Matrix literals.
* - Multiple variable declarations in the same line.
* - Semicolons don't start a new statement.
* - Strings containing escaped characters.
*/

// Actual grammar start.
program: version statements;

version: 'version' IntegerLiteral (Dot IntegerLiteral)*;

statement: map_statement | var_statement | instruction;

map_statement: KwMap Identifier Equal expression;

var_statement: KwVar Identifier Colon Identifier;

instruction:
    Identifier
    | Identifier expression_list

expression_list:
    expression_list Comma expression
    | expression

expression:
    IntegerLiteral
    | FloatLiteral
    | StringLiteral
    | Identifier
    | index
    | unary_op
    | binary_op
    | ternary_op
    | OpenParen expression CloseParen

unary_op:
    Minus expression
    | Tilde expression
    | ExclamationMark expression

binary_op:
    expression OpPower expression
    | expression Times expression
    | expression Slash expression
    | expression OpIntegerDivision expression
    | expression Percent expression
    | expression Plus expression
    | expression Minus expression
    | expression OpShL expression
    | expression OpArithmeticShR expression
    | expression OpLogicShR expression
    | expression OpEq expression
    | expression OpNE expression
    | expression CloseAngle expression
    | expression OpGE expression
    | expression OpenAngle expression
    | expression OpLE expression
    | expression Ampersand expression
    | expression Circumflex expression
    | expression Pipe expression
    | expression OpLogicAnd expression
    | expression OpLogicXor expression
    | expression OpLogicOr expression

ternary_op: expression QuestionMark expression Colon expression

index: expression OpenBracket index_list CloseBracket
index_list: index_entry (Comma index_list)?
index_entry : index_item | index_range
index_item: expression
index_range: expression Colon expression
