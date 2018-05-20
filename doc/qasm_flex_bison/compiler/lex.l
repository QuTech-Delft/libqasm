%option noyywrap
%{
    #include "grammar.tab.h"
%}

letter       [a-zA-Z]
identifier   {letter}([0-9]*{letter}*)+
whitespace   (\t|" "|\r)+
newline      {whitespace}?\n
integer      [0-9]+
axis         ([xX]|[yY]|[zZ])+
rotations    r{axis}
comma_sep    {whitespace}?,{whitespace}?
qubits       [qQ][uU][bB][iI][tT][sS]
wait         [wW][aA][iI][tT]
resetavg     [rR][eE][sS][eE][tT]-[aA][vV][eE][rR][aA][gG][iI][nN][gG]
display      [dD][iI][sS][pP][lL][aA][yY]
qubithead    [qQ]
bithead      [bB]

%%
{qubits} {
    printf("Lex: qubits\n");
    return QUBITS;
}

{qubithead} {
    return QBITHEAD;
}

{bithead} {
    return BITHEAD;
}

{display} {
    printf("Lex: display\n");
    return DISPLAY;
}

{wait} {
    return WAIT;
}

{resetavg} {
    return RESET_AVERAGING;
}

":" {
    printf("Lex: :\n");
    return COLON;
}

"." {
    return DOT;
}

"(" {
    return BRA;
}

"[" {
    return SBRA;
}

")" {
    return KET;
}

"]" {
    return SKET;
}

{comma_sep} {
    printf("Lex: ,\n");
    return COMMA_SEPARATOR;
}

{rotations} {
    printf("Lex: r[x|y|z]\n");
    return ROTATIONS;
}

{integer} {
    printf("Lex: Encountered integer\n");
    return INTEGER;
}

{identifier} {
    printf("Lex:name = \n");
    return NAME;
}

{whitespace} {
    printf("Lex: Encountered whitespace\n");
    return WS;
}

{newline} {
    ++yylineno;
    printf("Lex: Encountered newline\n");
    return NEWLINE;
}

%%
#include <stdio.h>