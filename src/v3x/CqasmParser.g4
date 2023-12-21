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
    BOOLEAN_LITERAL  # booleanLiteral
    | INTEGER_LITERAL  # integerLiteral
    | FLOAT_LITERAL  # floatLiteral
    | IDENTIFIER  # identifier
    | IDENTIFIER OPEN_BRACKET indexList CLOSE_BRACKET  # index
    | OPEN_BRACKET expression COMMA expression COMMA expression CLOSE_BRACKET  # axisInitializationList
    | OPEN_BRACE expressionList CLOSE_BRACE  # initializationList
    ;
