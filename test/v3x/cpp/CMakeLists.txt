target_sources(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/integration_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/matcher_values.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_analyzer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_functions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_instruction_set.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_parse_helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_semantic_analyzer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_values.cpp"
)
