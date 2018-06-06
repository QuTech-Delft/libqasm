#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "qasm_ast.hpp"

extern int yyparse();
extern int yylex();
extern FILE* yyin;
extern compiler::QasmRepresentation qasm_representation;

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
        std::cout << "I can't open a qc file!" << std::endl;
        return -1;
    }
    // set lex to read from it instead of defaulting to STDIN:
    yyin = myfile;

    // parse through the input until there is no more:
    int result = 0;
    
    do {
        result += yyparse();
    } while (!feof(yyin));

    if (!result)
    {
        std::cout << "Input is valid.\n" << std::endl;
        std::cout << "QASM file asks for " << qasm_representation.numQubits() << " qubits" << std::endl;
        std::cout << "With the following mappings: " << std::endl;
        qasm_representation.printMappings();
        std::cout << "Number of subcircuits found = " << qasm_representation.getSubCircuits().numberOfSubCircuits() << std::endl;
        const std::vector<compiler::SubCircuit>& found_subcircuits = qasm_representation.getSubCircuits().getAllSubCircuits();
        for (compiler::SubCircuit elem : found_subcircuits)
            elem.printMembers();
    }
    else
    {
        std::cout << "Input is invalid!" << std::endl;
    }

    return result;
}