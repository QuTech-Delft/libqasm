/** This test is for a bare minimum qasm file **/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TEST_BARE_MINIMUM
#include <iostream>
#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>
#include "qasm_semantic.hpp"


struct bare_minimum
{
    bare_minimum()
    {
    }
    
    void TestMapping(compiler::QasmRepresentation& qasm)
    {
        std::string first_key = "bleh2";
        auto indices = qasm.getMappedIndices(first_key, true);
        std::vector<size_t> qubit_indices = indices.getIndices();
        std::vector<size_t> correct_result = {0,1,2,3,4,5,6,7};
        BOOST_CHECK(qubit_indices == correct_result);
    }

};

BOOST_FIXTURE_TEST_SUITE(Test_Valid, bare_minimum);

BOOST_AUTO_TEST_CASE(testValidity)
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("bare_minimum.qasm", "r");
    
    compiler::QasmSemanticChecker sm(myfile);

    auto qasm_representation = sm.getQasmRepresentation();
    
    int result = sm.parseResult();

    BOOST_REQUIRE(result == 0);   // Stop here if it fails.
}

BOOST_AUTO_TEST_SUITE_END();