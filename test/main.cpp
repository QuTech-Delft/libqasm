#include "v1x/parsing.hpp"
#include "v3x/parsing.hpp"

#include <gmock/gmock.h>
#include <iostream>


int main_impl(int argc, char** argv, std::ostream&) {
    ::testing::InitGoogleMock(&argc, argv);

    cqasm::v1x::test::register_tests();
    cqasm::v3x::test::register_tests();

    return RUN_ALL_TESTS();
}

int main(int argc, char** argv) {
    return main_impl(argc, argv, std::cout);
}
