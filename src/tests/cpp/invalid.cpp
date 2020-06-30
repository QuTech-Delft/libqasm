/** This test is for a test invalid qasm file **/
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

TEST_CASE("Test for the test_invalid.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("invalid.qasm", "r");
    CHECK_THROWS(compiler::QasmSemanticChecker{myfile});
}
