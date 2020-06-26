
add_executable(test-parsing parsing.cpp)
target_link_libraries(test-parsing gtest cqasm)
add_test(
    NAME test-parsing
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test-parsing
)
