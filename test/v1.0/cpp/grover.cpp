/** This test is for grover qasm file in the paper **/

#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, grover) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v1.0/grover.qasm", "r");
    ASSERT_NE(fp, nullptr);

    compiler::QasmSemanticChecker sm(fp);

    auto qasm_representation = sm.getQasmRepresentation();

    int result = sm.parseResult();

    EXPECT_EQ(result, 0);
}
