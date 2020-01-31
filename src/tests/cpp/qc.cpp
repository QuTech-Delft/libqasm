/** This test is for an integrated file: It is not a particular algorithm **/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <string>
#include "qasm_semantic.hpp"
#include "doctest/doctest.h"

TEST_CASE("Test for the testqc.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("qc.qasm", "r");

    compiler::QasmSemanticChecker sm(myfile);

    auto qasm_representation = sm.getQasmRepresentation();

    auto error_model_params = qasm_representation.getErrorModelParameters();

    std::vector<double> true_results_ = {0.001, 0.1, 3.4};

    CHECK(true_results_.size() == error_model_params.size());

    for (size_t i = 0; i < error_model_params.size(); ++i)
    {
        CHECK(error_model_params.at(i) == true_results_.at(i));
    }

    int result = sm.parseResult();

    CHECK(result == 0);   // Stop here if it fails.
}
