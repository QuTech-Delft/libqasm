/** This main file is an example of how to use the AST class for the qasm parser **/

#include <iostream>
#include <vector>
#include <string>
#include "qasm_semantic.hpp"

int main (int argc, const char** argv)
{

    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    if (argc > 1)
        std::cout << "Path to QASM file = " << argv[1] << std::endl;

    // open a file handle to a particular file:
    FILE *myfile = fopen(argv[1], "r");
    // make sure it's valid:
    if (!myfile) {
        std::cout << "File " << argv[1] << " not found!" << std::endl;
        return -1;
    }
    
    compiler::QasmSemanticChecker sm(myfile);

    auto qasm_representation = sm.getQasmRepresentation();
    
    int result = sm.parseResult();

    if (!result)
    {
        std::cout << "Input is valid.\n" << std::endl;
    }
    else
    {
        std::cout << "Input is invalid!" << std::endl;
    }

    return result;
}