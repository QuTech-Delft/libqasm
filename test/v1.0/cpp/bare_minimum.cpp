/** This test is for a bare minimum qasm file **/

#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, bare_minimum) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *fp = fopen("res/v1.0/bare_minimum.qasm", "r");
    ASSERT_NE(fp, nullptr);

    compiler::QasmSemanticChecker sm(fp);

    auto qasm_representation = sm.getQasmRepresentation();

    int result = sm.parseResult();

    EXPECT_EQ(result, 0);
}
