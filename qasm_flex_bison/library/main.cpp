#include <iostream>
#include <vector>
#include <string>
//#include "pass_class.hpp"

extern int yyparse();
extern void yyerror(const char *);
extern int yylex();
extern std::vector<int> identities;
extern std::vector<std::string> subcircuits;

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

    for (elems:identities)
        std::cout << elems << std::endl;

    for (elems:subcircuits)
        std::cout << elems << std::endl;
    return result;
}