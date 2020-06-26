
add_executable(test-parsing parsing.cpp)
target_link_libraries(test-parsing gtest cqasm)
add_test(
    NAME test-parsing
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test-parsing
)

add_executable(tutorial tutorial.cpp)
target_link_libraries(tutorial gtest_main cqasm)
add_test(
    NAME tutorial
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND tutorial
)
