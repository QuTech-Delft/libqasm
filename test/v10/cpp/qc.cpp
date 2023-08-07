/** This test is for an integrated file: It is not a particular algorithm **/

#include "qasm_semantic.hpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


TEST(v10, qc) {
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    FILE *fp = fopen("res/v10/qc.qasm", "r");
    ASSERT_NE(fp, nullptr);

    compiler::QasmSemanticChecker sm(fp);

    auto qasm_representation = sm.getQasmRepresentation();

    auto error_model_params = qasm_representation.getErrorModelParameters();

    std::vector<double> true_results_ = {0.001, 0.1, 3.4};

    EXPECT_EQ(true_results_.size(), error_model_params.size());

    for (size_t i = 0; i < error_model_params.size(); ++i) {
        EXPECT_EQ(error_model_params.at(i), true_results_.at(i));
    }

    int result = sm.parseResult();

    EXPECT_EQ(result, 0);
}
