#include <filesystem>
#include <fmt/format.h>
#include <gtest/gtest.h>

namespace cqasm::test {

namespace fs = std::filesystem;

template <typename TestFactory>
void register_tests(const fs::path &tests_root_path, TestFactory test_factory) {
    // Discover the tests.
    // They should live in a directory tree with the following structure:
    //
    // <CWD>
    // '-  tests_root_path
    //     |- <suite-name>                   test suite directory
    //     |   |- <test-name>                test case directory
    //     |   |   |- input.cq               the input file
    //     |   |   |- ast.golden             the golden AST or parse error dump
    //     |   |   |- [ast.actual.txt]       output file with the actual data
    //     |   |   |- [semantic.golden.txt]  the golden semantic tree or analysis error dump,
    //     |   |   |                         if parsing should succeed
    //     |   |   '- [semantic.actual.txt]  output file with the actual data, if parsing actually succeeded
    //     |   |- ...                        other test case directories
    //     |   :
    //     |- ...                            other test suite directories
    //     :
    if (!fs::exists(tests_root_path)) {
        throw std::runtime_error(fmt::format("failed to open tests root path '{}'", tests_root_path.generic_string()));
    } else if (!fs::is_directory(tests_root_path)) {
        throw std::runtime_error(fmt::format("'{}' is not a directory", tests_root_path.generic_string()));
    }
    for (const fs::directory_entry &suite : fs::directory_iterator(tests_root_path)) {
        if (fs::is_directory(suite)) {
            auto suite_name = suite.path().filename().string();
            for (const fs::directory_entry &test : fs::directory_iterator(suite.path())) {
                auto test_name = test.path().filename().string();
                if (fs::is_directory(test)) {
                    auto input_cq_path = test.path() / "input.cq";
                    if (fs::exists(input_cq_path)) {
                        ::testing::RegisterTest(suite_name.c_str(),
                            test_name.c_str(),
                            nullptr,
                            nullptr,
                            __FILE__,
                            __LINE__,
                            [=]() -> ::testing::Test * { return test_factory(test.path()); });
                    }
                }
            }
        }
    }
}

}  // namespace cqasm::test
