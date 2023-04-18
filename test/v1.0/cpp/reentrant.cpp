#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// [[noreturn]] completely breaks MSVC 2015, and is basically unnecessary
#ifdef _MSC_VER
#define DOCTEST_NORETURN
#endif

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include "qasm_semantic.hpp"
#include "doctest/doctest.h"

TEST_CASE("Test for the reentrant.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("reentrant.qasm", "r");

    compiler::QasmSemanticChecker sm1(myfile);
    CHECK(sm1.parseResult() == 0);

    rewind(myfile);

    compiler::QasmSemanticChecker sm2(myfile);
    CHECK(sm2.parseResult() == 0);
}
