target_sources(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/AnalyzeTreeGenAstVisitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-functions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-parse-helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/matcher_values.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/parsing.cpp"
)
