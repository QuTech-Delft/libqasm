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
    QUBIT_TYPE (OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET)? IDENTIFIER  # qubitTypeDefinition
    | BIT_TYPE (OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET)? IDENTIFIER  # bitTypeDefinition
    | AXIS_TYPE IDENTIFIER (EQUALS axisTypeInitializer)?  # axisTypeDefinition
    | BOOL_TYPE IDENTIFIER (EQUALS expression)?  # boolTypeDefinition
    | BOOL_TYPE arraySizeDefinition IDENTIFIER (EQUALS arrayTypeInitializer)?  # boolArrayTypeDefinition
    | INT_TYPE IDENTIFIER (EQUALS expression)?  # intTypeDefinition
    | INT_TYPE arraySizeDefinition IDENTIFIER (EQUALS arrayTypeInitializer)?  # intArrayTypeDefinition
    | FLOAT_TYPE IDENTIFIER (EQUALS expression)?  # floatTypeDefinition
    | FLOAT_TYPE arraySizeDefinition IDENTIFIER (EQUALS arrayTypeInitializer)?  # floatArrayTypeDefinition
    | expression EQUALS MEASURE expression  # measureInstruction
    | IDENTIFIER expressionList  # instruction
    ;

arraySizeDefinition: OPEN_BRACKET INTEGER_LITERAL CLOSE_BRACKET;

axisTypeInitializer: OPEN_BRACKET expression COMMA expression COMMA expression CLOSE_BRACKET;

arrayTypeInitializer: OPEN_BRACE expressionList CLOSE_BRACE;

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
    ;
