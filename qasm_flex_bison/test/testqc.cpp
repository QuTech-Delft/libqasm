/** This test is for an integrated file: It is not a particualr algorithm **/

#include <iostream>
#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>
#include "qasm_semantic.hpp"

int main ()
{

    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("testqc.qasm", "r");
    // make sure it's valid:
    if (!myfile) {
        std::cout << "File 'testqc.qasm' not found!" << std::endl;
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