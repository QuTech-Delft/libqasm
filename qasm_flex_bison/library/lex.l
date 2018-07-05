%option noyywrap
%option yylineno
%{
    #include "grammar.tab.h"
    #include <cstdlib>
    #include <string>
    #define PRINTLEX(t) { printf("Lex: " #t ": %s\n",t); }
    #define TOKEN(t) { return(t); }
    #define TOKENI(t) {  yylval.ival = static_cast<int> ( std::strtol(yytext,NULL,10) ); return(t); }
    #define TOKEND(t) {  yylval.dval = std::strtod(yytext,NULL); return(t); }
    #define TOKENSTR(t) { yylval.sval = strdup(yytext); return(t); }
%}


letter             [a-zA-Z\_]
identifier         {letter}([0-9]|{letter})*
whitespace         (\t|" ")*
newline            {whitespace}?(\n|\r\n)*
singlenewline      (\n|\r\n)
comment            \#(.*){singlenewline}+
nonneginteger      [0-9]+
integer            (\-)?{nonneginteger}
float              (\-)?({nonneginteger}\.{nonneginteger})[eE]?((\-)?{nonneginteger})?
axis               ([xX]|[yY]|[zZ])
single_qubits      ((?i:i)|(?i:h)|(?i:x90)|(?i:y90)|(?i:mx90)|(?i:my90)|(?i:s)|(?i:sdag)|(?i:t)|(?i:tdag))
two_qubit_gates    ((?i:cnot)|(?i:cz)|(?i:swap))
toffoli            (?i:toffoli)
rotations          ([rR]){axis}
cr                 (?i:cr)
crk                (?i:crk)
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
qasm_version       (?i:version)

%%
{qasm_version} TOKENSTR(QASM_VERSION)

{qubits} TOKEN(QUBITS)

{qubithead} TOKEN(QBITHEAD)

{bithead} TOKEN(BITHEAD)

{axis}  TOKENSTR(AXIS)

{rotations} TOKENSTR(ROTATIONS)

{cr} TOKENSTR(CR)

{crk} TOKENSTR(CRK)

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

{whitespace} TOKEN(WS)

{newline} {
    TOKEN(NEWLINE)
}

{identifier} TOKENSTR(NAME)

{comment} {
    TOKENSTR(COMMENT)
}

%%
#include <stdio.h>