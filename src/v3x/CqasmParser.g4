parser grammar CqasmParser;

options {
	tokenVocab = CqasmLexer;
}

// Actual grammar start
//
// Note that texts such as '# integerLiteral' are not comments but alternative labels
// The use of alternative labels simplifies the visitor classes by removing the need to implement some methods,
// that would otherwise contain boilerplate code (e.g. 'statement' and 'expression')
program: statementSeparator* version statements statementSeparator* EOF;

version: VERSION VERSION_NUMBER;

statements: (statementSeparator+ statement)*;

statementSeparator: NEW_LINE | SEMICOLON;

statement:
    QUBIT_TYPE arraySizeDefinition? IDENTIFIER  # qubitTypeDefinition
    | BIT_TYPE arraySizeDefinition? IDENTIFIER  # bitTypeDefinition
    | AXIS_TYPE IDENTIFIER (EQUALS expression)?  # axisTypeDefinition
    | BOOL_TYPE arraySizeDefinition? IDENTIFIER (EQUALS expression)?  # boolTypeDefinition
    | INT_TYPE arraySizeDefinition? IDENTIFIER (EQUALS expression)?  # intTypeDefinition
    | FLOAT_TYPE arraySizeDefinition? IDENTIFIER (EQUALS expression)?  # floatTypeDefinition
    | expression EQUALS MEASURE expression  # measureInstruction
    | IDENTIFIER expressionList  # instruction
    ;

arraySizeDefinition: OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET;

expressionList: expression (COMMA expression)*;

indexList: indexEntry (COMMA indexEntry)*;

indexEntry:
    expression  # indexItem
    | expression COLON expression  # indexRange
    ;

expression:
    INTEGER_LITERAL  # integerLiteral
    | FLOAT_LITERAL  # floatLiteral
    | IDENTIFIER  # identifier
    | IDENTIFIER OPEN_BRACKET indexList CLOSE_BRACKET  # index
    | OPEN_BRACKET expression COMMA expression COMMA expression CLOSE_BRACKET  # axisInitializationList
    | OPEN_BRACE expressionList CLOSE_BRACE  # initializationList
    ;
