#include "qasm_semantic.hpp"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


TEST(v1_0, example_str) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    std::ifstream ifs("res/v1.0/example_str.qasm");
    std::string file_str{ std::istreambuf_iterator(ifs), {} };

    const char* qasm_str = file_str.c_str();

    compiler::QasmSemanticChecker sm(qasm_str);

    EXPECT_EQ(sm.parseResult(), 0);
}
