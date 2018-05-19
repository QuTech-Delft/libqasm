#include <iostream>

extern "C"
{
    int yyparse();
    void yyerror(const char *);
    int yylex();
}

int main (int argc, const char* argv)
{
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