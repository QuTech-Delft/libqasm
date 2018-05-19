%option noyywrap
%{
    #include "grammar.tab.h"
%}

alpha       [a-zA-Z]
name        {alpha}([0-9]*{alpha}*)+
whitespace  [ \r\t\v\f]
linefeed    \n

%%
{name} {
    printf("Lex:name\n");
    return NAME;
}

":" {
    printf("Lex: :\n");
    return COLON;
}

"->" {
    printf("Lex: ->\n");
    return RIGHT_ARROW;
}

"{" {
    printf("Lex: {\n");
    return LEFT_BRACE;
}

"}" {
    printf("Lex: }\n");
    return RIGHT_BRACE;
}

{whitespace} {
    printf("Lex: Encountered whitespace\n");
}

{linefeed} {
    ++yylineno;
}

%%
#include <stdio.h>