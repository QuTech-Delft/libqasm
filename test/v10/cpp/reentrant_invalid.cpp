#include "qasm_semantic.hpp"

#include <cstdio>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <stdexcept>  // runtime_error


TEST(v10, reentrant_invalid) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v10/reentrant_invalid.qasm", "r");
    ASSERT_NE(fp, nullptr);

    EXPECT_THAT([&]() { compiler::QasmSemanticChecker sm(fp); },
        ::testing::ThrowsMessage<std::runtime_error>(
            "Error at <unknown file name>:6:14..15: index 2 out of range (size 2)"));

    rewind(fp);

    EXPECT_THAT([&]() { compiler::QasmSemanticChecker sm(fp); },
        ::testing::ThrowsMessage<std::runtime_error>(
            "Error at <unknown file name>:6:14..15: index 2 out of range (size 2)"));
}
