#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, reentrant) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v1.0/reentrant.qasm", "r");
    ASSERT_NE(fp, nullptr);

    compiler::QasmSemanticChecker sm1(fp);
    EXPECT_EQ(sm1.parseResult(), 0);

    rewind(fp);

    compiler::QasmSemanticChecker sm2(fp);
    EXPECT_EQ(sm2.parseResult(), 0);
}
