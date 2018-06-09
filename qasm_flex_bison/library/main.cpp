/** This main file is an example of how to use the AST class for the qasm parser **/

#include <iostream>
#include <vector>
#include <string>
#include "qasm_ast.hpp"
#include "qasm_semantic.hpp"


void printOperationProperties(compiler::Operation& op);
void printIfValid(compiler::QasmRepresentation& qasm_representation);
void printSubCircuitProperties(compiler::SubCircuit& subs);
void printSerialParallelOperations(compiler::OperationsCluster& ops_cluster);

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
    
    compiler::SemanticChecker sm(myfile);

    compiler::QasmRepresentation qasm_representation = sm.getQasmRepresentation();
    
    int result = sm.parseResult();

    if (!result)
    {
        std::cout << "Input is valid.\n" << std::endl;
        printIfValid(qasm_representation);
    }
    else
    {
        std::cout << "Input is invalid!" << std::endl;
    }

    return result;
}

void printIfValid(compiler::QasmRepresentation& qasm_representation)
{
    std::cout << "QASM file asks for " << qasm_representation.numQubits() << " qubits" << std::endl;
    std::cout << "With the following mappings: " << std::endl;
    qasm_representation.printMappings();
    std::cout << "Number of subcircuits found = " << qasm_representation.getSubCircuits().numberOfSubCircuits() << std::endl;
    const std::vector<compiler::SubCircuit>& found_subcircuits = qasm_representation.getSubCircuits().getAllSubCircuits();
    for (compiler::SubCircuit subcircuit : found_subcircuits){
        printSubCircuitProperties(subcircuit);
    }
}

void printSubCircuitProperties(compiler::SubCircuit& subs)
{
    std::cout << "Subcircuit Name = " << subs.nameSubCircuit() <<  " , Rank = " << subs.rankSubCircuit() << std::endl;
    std::cout << subs.nameSubCircuit() << " has " << subs.numberIterations() << " iterations." << std::endl;
    std::cout << "Contains these operations clusters:" << std::endl;
    for (auto elem : subs.getOperationsCluster())
        printSerialParallelOperations(*elem);
    std::cout << "End of subcircuit " << subs.nameSubCircuit() << std::endl << std::endl;
}

void printSerialParallelOperations(compiler::OperationsCluster& ops_cluster)
{
    if (ops_cluster.isParallel())
    {
        std::cout << "Parallel operations cluster: " << std::endl;
        for (auto elem : ops_cluster.getOperations())
            printOperationProperties(*elem);
        std::cout << "End Parallel operations \n" << std::endl;
    }
    else
    {
        std::cout << "Serial operation: " << std::endl;
        for (auto elem : ops_cluster.getOperations())
            printOperationProperties(*elem);
        std::cout << "End Serial operation \n" << std::endl;
    }
}

void printOperationProperties(compiler::Operation& op)
// Potentially a function example where one can create an interface with whatever simulator/eQASM generator class.
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