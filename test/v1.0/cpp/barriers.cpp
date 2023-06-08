/** This test is for the barriers qasm file.
 *
 * Note that this is not an example from the paper,
 * but one added specifically to check barriers are working for cQASM v1. **/

#include "v1.0/qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, barriers) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *fp = fopen("res/v1.0/barriers.qasm", "r");

    compiler::QasmSemanticChecker sm(fp);

    auto qasm_representation = sm.getQasmRepresentation();

    int result = sm.parseResult();

    EXPECT_EQ(result, 0);
}
