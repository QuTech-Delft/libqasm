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
program: statementSeparator* version statements statementSeparator* EOF;

version: VERSION VERSION_NUMBER;

statements: (statementSeparator+ statement)*;

statementSeparator: NEW_LINE | SEMICOLON;

statement:
    QUBIT_TYPE arraySizeDeclaration? IDENTIFIER  # qubitTypeDeclaration
    | BIT_TYPE arraySizeDeclaration? IDENTIFIER  # bitTypeDeclaration
    | AXIS_TYPE IDENTIFIER (EQUALS expression)?  # axisTypeDeclaration
    | BOOL_TYPE arraySizeDeclaration? IDENTIFIER (EQUALS expression)?  # boolTypeDeclaration
    | INT_TYPE arraySizeDeclaration? IDENTIFIER (EQUALS expression)?  # intTypeDeclaration
    | FLOAT_TYPE arraySizeDeclaration? IDENTIFIER (EQUALS expression)?  # floatTypeDeclaration
    | expression EQUALS MEASURE expression  # measureInstruction
    | IDENTIFIER expressionList  # instruction
    ;

arraySizeDeclaration: OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET;

expressionList: expression (COMMA expression)*;

indexList: indexEntry (COMMA indexEntry)*;

// Current implementation of the semantic parser will expect constant integers
// both for the expression in indexItem and the two expressions in indexRange
indexEntry:
    expression  # indexItem
    | expression COLON expression  # indexRange
    ;

expression:
    OPEN_PARENS expression CLOSE_PARENS  # parensExpression
    | <assoc=right> (PLUS | MINUS) expression  # unaryPlusMinusExpression
    | <assoc=right> BITWISE_NOT_OP expression  # bitwiseNotExpression
    | <assoc=right> LOGICAL_NOT_OP expression  # logicalNotExpression
    | <assoc=right> POWER_OP expression  # powerExpression
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
    | OPEN_BRACKET expression COMMA expression COMMA expression CLOSE_BRACKET  # axisInitializationList
    | OPEN_BRACE expressionList CLOSE_BRACE  # initializationList
    | BOOLEAN_LITERAL  # booleanLiteral
    | INTEGER_LITERAL  # integerLiteral
    | FLOAT_LITERAL  # floatLiteral
    ;
