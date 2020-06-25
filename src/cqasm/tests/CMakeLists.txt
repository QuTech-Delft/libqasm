
add_executable(parsing parsing.cpp)
target_link_libraries(parsing gtest cqasm)
add_test(
    NAME parsing_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND parsing
)
