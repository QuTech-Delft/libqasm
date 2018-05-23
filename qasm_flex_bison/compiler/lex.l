%option noyywrap
%{
    #include "grammar.tab.h"
    #define TOKEN(t) {printf("Lex: " #t "\n"); return(t);}
    #define TOKENI(t) {printf("Lex: " #t "\n"); yylval.ival = atoi(yytext); return(t);}
    #define TOKEND(t) {printf("Lex: " #t "\n"); yylval.dval = atof(yytext); return(t);}
    #define TOKENSTR(t) {printf("Lex: " #t "\n"); yylval.sval = yytext; return(t);}
%}

letter             [a-zA-Z]
identifier         {letter}([0-9]*{letter}*)+
whitespace         (\t|" "|\r)+
newline            {whitespace}?\n
integer            [0-9]+
float              ({integer}\.{integer})[eE]?{integer}?
axis               ([xX]|[yY]|[zZ])
single_qubits      ((?i:i)|(?i:h)|(?i:x90)|(?i:y90)|(?i:mx90)|(?i:my90)|(?i:s)|(?i:sdag)|(?i:t)|(?i:tdag))
two_qubit_gates    ((?i:cnot)|(?i:cz)|(?i:swap))
toffoli            (?i:toffoli)
rotations          ([rR]){axis}
cr                 (?i:cr)
cdash              (?i:c\-)
not_token          (?i:not)
comma_sep          {whitespace}?,{whitespace}?
parallel_sep       {whitespace}?\|{whitespace}?
qubits             (?i:qubits)
mapkey             (?i:map)
wait               (?i:wait)
resetavg           (?i:reset-averaging)
display            (?i:display)
qubithead          [qQ]
bithead            [bB]
prep               (?i:prep)(_{axis})?
measurehead        (?i:measure)
measure            {measurehead}(_{axis})?
measureall         {measurehead}(_(?i:all))
measureparity      {measurehead}(_(?i:parity))

%%
{qubits} TOKEN(QUBITS)

{qubithead} TOKEN(QBITHEAD)

{bithead} TOKEN(BITHEAD)

{axis}  TOKEN(AXIS)

{rotations} TOKEN(ROTATIONS)

{cr} TOKEN(CR)

{cdash} TOKEN(CDASH)

{not_token} TOKEN(NOT_TOKEN)

{single_qubits} TOKEN(SINGLE_QUBIT_GATES)

{two_qubit_gates} TOKEN(TWO_QUBIT_GATES)

{toffoli} TOKEN(TOFFOLI)

{mapkey} TOKEN(MAPKEY)

{prep} TOKEN(PREP)

{measure} TOKEN(MEASURE)

{measureall} TOKEN(MEASUREALL)

{measureparity} TOKEN(MEASUREPARITY)

{display} TOKEN(DISPLAY)

{wait} TOKEN(WAIT)

{resetavg} TOKEN(RESET_AVERAGING)

":" TOKEN(COLON)

"." TOKEN(DOT)

"(" TOKEN(BRA)

"[" TOKEN(SBRA)

"{" TOKEN(CBRA)

")" TOKEN(KET)

"]" TOKEN(SKET)

"}" TOKEN(CKET)

{comma_sep} TOKEN(COMMA_SEPARATOR)

{parallel_sep} TOKEN(PARALLEL_SEPARATOR)

{integer} TOKENI(INTEGER)

{float} TOKEND(FLOAT)

{identifier} TOKENSTR(NAME)

{whitespace} TOKEN(WS)

{newline} {
    ++yylineno;
    TOKEN(NEWLINE)
}

%%
#include <stdio.h>