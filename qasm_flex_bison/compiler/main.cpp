#include <iostream>

extern "C"
{
    int yyparse();
    void yyerror(const char *);
    int yylex();
}

int main (int argc, const char** argv)
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif
    int result = yyparse();
    if (!result)
    {
        std::cout << "Input is valid." << std::endl;
    }
    else
    {
        std::cout << "Input is invalid!" << std::endl;
    }
    return result;
}