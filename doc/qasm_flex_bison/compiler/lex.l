%option noyywrap
%{
    #include "grammar.tab.h"
%}

alpha       [a-zA-Z]
name        {alpha}([0-9]*{alpha}*)+
whitespace  [ \r\t\v\f]
linefeed    \n
float       [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
integer     ^\d+$
rotations   [r][xX|yY|zZ]
comma_sep   [ ]*[,][ ]*

%%
{name} {
    printf("Lex:name = \n");
    return NAME;
}

":" {
    printf("Lex: :\n");
    return COLON;
}

"qubits" {
    printf("Lex: qubits\n");
    return QUBITS;
}

{comma_sep} {
    printf("Lex: ,\n");
    return COMMA_SEPARATOR;
}

{rotations} {
    printf("Lex: r[x|y|z]\n");
    return ROTATIONS;
}

{float} {
    printf("Lex: Encountered float\n");
    return FLOAT;
}

{integer} {
    printf("Lex: Encountered integer\n");
    return INTEGER;
}

{alpha} {
    return LETTER;
}

{whitespace} {
    printf("Lex: Encountered whitespace\n");
    return WS;
}

{linefeed} {
    ++yylineno;
    return NEWLINE;
}

%%
#include <stdio.h>