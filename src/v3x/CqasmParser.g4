parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start
//
// Note that texts such as '# integerLiteral' are not comments but alternative labels
// The use of alternative labels simplifies the visitor classes
// by removing the need to implement some methods,
// which would otherwise contain boilerplate code (e.g. 'statement' and 'expression')

program: versionSection globalBlockSection? eofSection;

versionSection: statementSeparator* version;

globalBlockSection: (statementSeparator+ globalBlockStatement)+;

eofSection: statementSeparator* EOF;

statementSeparator: NEW_LINE | SEMICOLON;

version: VERSION VERSION_NUMBER;

globalBlockStatement:
    variableDefinition
    | instruction
    ;

variableDefinition: type IDENTIFIER;

instruction:
    gateInstruction
    | nonGateInstruction
    | asmDeclaration
    ;

gateInstruction: gate expressionList;

gate:
    INV DOT gate  # invGate
    | POW OPEN_PARENS expression CLOSE_PARENS DOT gate  # powGate
    | CTRL DOT gate # ctrlGate
    | IDENTIFIER (OPEN_PARENS expression CLOSE_PARENS)?  # namedGate
    ;

// Current implementation of the semantic parser will expect a constant integer
// for the first expression in the WAIT instruction
nonGateInstruction:
    expression EQUALS MEASURE expression  # measureInstruction
    | RESET expression  # resetInstruction
    | INIT expression  # initInstruction
    | BARRIER expression  # barrierInstruction
    | WAIT OPEN_PARENS expression CLOSE_PARENS expression  # waitInstruction
    ;

asmDeclaration: ASM OPEN_PARENS IDENTIFIER CLOSE_PARENS BE_CODE_OPEN_BRACE BE_CODE? BE_CODE_CLOSE_BRACE;

type: quantumType;

quantumType:
    QUBIT_TYPE arraySizeDeclaration?  # qubitType
    | BIT_TYPE arraySizeDeclaration?  # bitType
    ;

arraySizeDeclaration: OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET;

expressionList: expression (COMMA expression)*;

// Current implementation of the semantic parser will expect constants
// for all the expressions of the arithmetic operators
expression:
    OPEN_PARENS expression CLOSE_PARENS  # parensExpression
    | <assoc=right> (PLUS | MINUS) expression  # unaryPlusMinusExpression
    | <assoc=right> BITWISE_NOT_OP expression  # bitwiseNotExpression
    | <assoc=right> LOGICAL_NOT_OP expression  # logicalNotExpression
    | <assoc=right> expression POWER_OP expression  # powerExpression
    | expression (PRODUCT_OP | DIVISION_OP | MODULO_OP) expression  # productExpression
    | expression (PLUS | MINUS) expression  # additionExpression
    | expression (SHL_OP | SHR_OP) expression  # shiftExpression
    | expression (CMP_GT_OP | CMP_LT_OP | CMP_GE_OP | CMP_LE_OP) expression  # comparisonExpression
    | expression (CMP_EQ_OP | CMP_NE_OP) expression  # equalityExpression
    | expression BITWISE_AND_OP expression  # bitwiseAndExpression
    | expression BITWISE_XOR_OP expression  # bitwiseXorExpression
    | expression BITWISE_OR_OP expression  # bitwiseOrExpression
    | expression LOGICAL_AND_OP expression  # logicalAndExpression
    | expression LOGICAL_XOR_OP expression  # logicalXorExpression
    | expression LOGICAL_OR_OP expression  # logicalOrExpression
    | <assoc=right> expression TERNARY_CONDITIONAL_OP expression COLON expression  # ternaryConditionalExpression
    | IDENTIFIER OPEN_PARENS expressionList? CLOSE_PARENS  # functionCall
    | IDENTIFIER OPEN_BRACKET indexList CLOSE_BRACKET  # index
    | IDENTIFIER  # identifier
    | BOOLEAN_LITERAL  # booleanLiteral
    | INTEGER_LITERAL  # integerLiteral
    | FLOAT_LITERAL  # floatLiteral
    ;

indexList: indexEntry (COMMA indexEntry)*;

// Current implementation of the semantic parser will expect constant integers
// both for the expression in indexItem and the two expressions in indexRange
indexEntry:
    expression  # indexItem
    | expression COLON expression  # indexRange
    ;
