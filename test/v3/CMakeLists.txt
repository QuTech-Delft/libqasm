add_subdirectory(parsing)

set(v3_test_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version.cpp"
    ${parsing_test_sources}
    PARENT_SCOPE
)
