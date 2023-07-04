/** This test is for the barriers qasm file.
 *
 * Note that this is not an example from the paper,
 * but one added specifically to check barriers are working for cQASM v1. **/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// [[noreturn]] completely breaks MSVC 2015, and is basically unnecessary
#ifdef _MSC_VER
#define DOCTEST_NORETURN
#endif

#include <iostream>
#include <vector>
#include <string>
#include "qasm_semantic.hpp"
#include "doctest/doctest.h"

TEST_CASE("Test for the barriers.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("barriers.qasm", "r");

    compiler::QasmSemanticChecker sm(myfile);

    auto &qasm_representation = sm.getQasmRepresentation();

    int result = sm.parseResult();

    CHECK(result == 0);   // Stop here if it fails.
}
