/** This test is for a test invalid qasm file **/

#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, invalid) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v1.0/invalid.qasm", "r");
    EXPECT_THROW(compiler::QasmSemanticChecker{fp}, std::runtime_error);
}
