%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    int yylex(void);
    void yyerror (char const *);
%}

%define api.value.type {double}

%token NAME COLON RIGHT_ARROW LEFT_BRACE RIGHT_BRACE

%start input

%%

input:
    NAME COLON RIGHT_ARROW LEFT_BRACE RIGHT_BRACE
    | %empty

%%

void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}