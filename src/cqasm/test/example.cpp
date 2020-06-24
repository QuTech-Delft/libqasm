#include <gtest/gtest.h> // googletest header file

#include <cqasm.hpp>

TEST(example, grover) {
    auto r = cqasm::parser::parse_file("grover.cq");
    EXPECT_TRUE(r.root.is_complete());
    for (auto err : r.errors) {
        EXPECT_EQ(err, "");
    }

    auto a = cqasm::analyzer::Analyzer();
    a.register_default_functions_and_mappings();
    auto r2 = a.analyze(*r.root->as_program());
    //EXPECT_TRUE(r2.root.is_complete());
    for (auto err : r2.errors) {
        EXPECT_EQ(err, "");
    }

    //std::cout << *r.root << std::endl;
    std::cout << *r2.root << std::endl;
    //EXPECT_TRUE(false);
}
