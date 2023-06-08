#include "v1.0/qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v1_0, reentrant_invalid) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v1.0/reentrant_invalid.qasm", "r");
    ASSERT_NE(fp, nullptr);

    EXPECT_THAT([&]() { compiler::QasmSemanticChecker sm(fp); },
        ::testing::ThrowsMessage<std::runtime_error>(
            "Error at <unknown>:6:14..15: index 2 out of range (size 2)"));

    rewind(fp);

    EXPECT_THAT([&]() { compiler::QasmSemanticChecker sm(fp); },
        ::testing::ThrowsMessage<std::runtime_error>(
            "Error at <unknown>:6:14..15: index 2 out of range (size 2)"));
}
