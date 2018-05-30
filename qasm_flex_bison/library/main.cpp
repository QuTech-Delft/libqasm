#include <iostream>
#include <vector>
#include <string>
#include "pass_class.hpp"

extern int yyparse();
extern void yyerror(const char *);
extern int yylex();
extern compiler::NumericalIdentifiers test_indices;
extern compiler::SubCircuits test_subcircuits;

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
        std::cout << "Number of subcircuits found = " << test_subcircuits.numberOfSubCircuits() << std::endl;
        const std::vector<compiler::SubCircuit>& found_subcircuits = test_subcircuits.getAllSubCircuits();
        for (compiler::SubCircuit elem : found_subcircuits)
            elem.printMembers();
    }
    else
    {
        std::cout << "Input is invalid!" << std::endl;
    }

    return result;
}