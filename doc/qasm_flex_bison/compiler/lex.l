%option noyywrap
%{
    #include "grammar.tab.h"
%}

alpha       [a-zA-Z]
name        {alpha}([0-9]*{alpha}*)+
whitespace  [ \r\t\v\f]
linefeed    \n
float       [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
digit       [0-9]
nzdigit     [1-9]

%%
{name} {
    printf("Lex:name = \n");
    return NAME;
}

":" {
    printf("Lex: :\n");
    return COLON;
}

{float} {
    printf("Lex: Encountered float\n");
    return FLOAT;
}

{digit} {
    printf("Lex: Encountered digit\n");
    return DIGIT;
}

{alpha} {
    return LETTER;
}

{nzdigit} {
    printf("Lex: Encountered non-zero digit\n");
    return NONZERODIGIT;
}

{whitespace} {
    printf("Lex: Encountered whitespace\n");
}

{linefeed} {
    ++yylineno;
    return NEWLINE;
}

%%
#include <stdio.h>