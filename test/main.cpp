#include <gmock/gmock.h>
#include <iostream>

#include "v3x/cpp/integration_test.hpp"

int main_impl(int argc, char **argv, std::ostream &) {
    ::testing::InitGoogleMock(&argc, argv);

    cqasm::v3x::test::register_tests();

    return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
    return main_impl(argc, argv, std::cout);
}
