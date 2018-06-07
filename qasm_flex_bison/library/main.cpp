/** This main file is an example of how to use the AST class for the qasm parser **/

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "qasm_ast.hpp"

extern int yyparse();
extern int yylex();
extern FILE* yyin;
extern compiler::QasmRepresentation qasm_representation;

void printOperation(compiler::Operation& op);

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

void printOperation(compiler::Operation& op)
{
    std::string type_ = op.getType();
    std::cout << "Operation " << type_ << ": ";
    if ( type_ == "rx" || type_ == "ry" || type_ == "rz" )
    {
        op.getQubitsInvolved().printMembers();
        std::cout << "Rotations = " << op.getRotationAngle() << std::endl;
    }
    else if (type_ == "measure_parity")
    {
        std::cout << std::endl;
        auto measureParityProperties = op.getMeasureParityQubitsAndAxis();
        measureParityProperties.first.first.printMembers();
        std::cout << "With axis " << measureParityProperties.second.first << std::endl;
        measureParityProperties.first.second.printMembers();
        std::cout << "With axis " << measureParityProperties.second.second << std::endl;
    }
    else if (type_ == "cnot" || type_ == "cz" || type_ == "swap")
    {
        std::cout << std::endl;
        std::cout << "Qubit Pair 1: ";
        op.getTwoQubitPairs().first.printMembers();
        std::cout << "Qubit Pair 2: "; 
        op.getTwoQubitPairs().second.printMembers();
    }
    else if (type_ == "cr")
    {
        std::cout << std::endl;
        std::cout << "Qubit Pair 1: ";
        op.getTwoQubitPairs().first.printMembers();
        std::cout << "Qubit Pair 2: "; 
        op.getTwoQubitPairs().second.printMembers();
        std::cout << "Rotation = " << op.getRotationAngle() << std::endl; 
    }
    else if (type_ == "toffoli")
    {
        std::cout << std::endl;
        std::cout << "Qubit Pair 1: ";
        op.getToffoliQubitPairs().first.printMembers();
        std::cout << "Qubit Pair 2: "; 
        op.getToffoliQubitPairs().second.first.printMembers();
        std::cout << "Qubit Pair 3: "; 
        op.getToffoliQubitPairs().second.second.printMembers();
    }
    else if (type_ == "wait")
    {
        std::cout << std::endl;
        std::cout << "Wait time (integer) = " << op.getWaitTime() << std::endl;
    }
    else if (type_ == "display")
    {
        std::cout << "Display bits: ";
        op.getDisplayBits().printMembers();
    }
    else op.getQubitsInvolved().printMembers();

    if (op.isBitControlled()){
        std::cout << "Bit controlled with bits: ";
        op.getControlBits().printMembers();
    }
}