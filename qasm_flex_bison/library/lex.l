%option noyywrap
%{
    #include "grammar.tab.h"
    #include <cstdlib>
    #include <string>
    #define TOKEN(t) {printf("Lex: " #t ": %s\n",yytext); return(t);}
    #define TOKENI(t) {printf("Lex: " #t "\n"); yylval.ival = static_cast<int> ( std::strtol(yytext,NULL,10) ); return(t);}
    #define TOKEND(t) {printf("Lex: " #t "\n"); yylval.dval = std::strtod(yytext,NULL); return(t);}
    #define TOKENSTR(t) {printf("Lex: " #t ": %s\n",yytext); yylval.sval = yytext; return(t);}
%}

letter             [a-zA-Z\_]
identifier         {letter}([0-9]*{letter}*)+
whitespace         (\t|" "|\r)+
newline            {whitespace}?(\n|\r\n)
singlenewline      (\n|\r\n)
comment            \#(.*)
integer            [0-9]+
float              (\-)?({integer}\.{integer})[eE]?{integer}?
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

{axis}  TOKENSTR(AXIS)

{rotations} TOKENSTR(ROTATIONS)

{cr} TOKENSTR(CR)

{cdash} TOKEN(CDASH)

{not_token} TOKENSTR(NOT_TOKEN)

{single_qubits} TOKENSTR(SINGLE_QUBIT_GATES)

{two_qubit_gates} TOKENSTR(TWO_QUBIT_GATES)

{toffoli} TOKENSTR(TOFFOLI)

{mapkey} TOKEN(MAPKEY)

{prep} TOKENSTR(PREP)

{measure} TOKENSTR(MEASURE)

{measureall} TOKENSTR(MEASUREALL)

{measureparity} TOKENSTR(MEASUREPARITY)

{display} TOKENSTR(DISPLAY)

{wait} TOKENSTR(WAIT)

{resetavg} TOKENSTR(RESET_AVERAGING)

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

{comment} {
    ++yylineno;
    TOKENSTR(COMMENT)
}

%%
#include <stdio.h>