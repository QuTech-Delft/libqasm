#include "v1/parsing.hpp"

#include <gtest/gtest.h>
#include <iostream>

int main_impl(int argc, char** argv, std::ostream&) {
    ::testing::InitGoogleTest(&argc, argv);

    register_v1_parsing_tests();

    return RUN_ALL_TESTS();
}

int main(int argc, char** argv) {
    return main_impl(argc, argv, std::cout);
}
