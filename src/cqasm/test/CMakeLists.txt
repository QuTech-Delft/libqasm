
add_executable(example example.cpp)
target_link_libraries(example gtest_main cqasm)
add_test(
    NAME example_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND example
)
