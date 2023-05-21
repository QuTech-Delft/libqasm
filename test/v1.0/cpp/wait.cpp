/** This test is for the wait qasm file.
 *
 * Note that this is not an example from the paper,
 * but one added specifically to check wait are working for cQASM v1. **/

#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, wait) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v1.0/wait.qasm", "r");
    ASSERT_NE(fp, nullptr);

    compiler::QasmSemanticChecker sm(fp);

    auto qasm_representation = sm.getQasmRepresentation();

    int result = sm.parseResult();

    EXPECT_EQ(result, 0);
}
