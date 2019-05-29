#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "qasm_semantic.hpp"
#include "doctest/doctest.h"

TEST_CASE("Test for the example_str.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    std::string file_str;
    std::ifstream myfile("example_str.qasm");
    while (!myfile.eof())
    {
        std::string line_str;
        std::getline(myfile, line_str);
        file_str = file_str + line_str + std::string("\n");
    }

    const char* qasm_str = file_str.c_str();

    compiler::QasmSemanticChecker sm(qasm_str);

    CHECK(sm.parseResult() == 0);
}
