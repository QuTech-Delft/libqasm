find_package(emsdk REQUIRED)

add_executable(${PROJECT_NAME}_emscripten
    "${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenWrapper.cpp"
)

target_link_libraries(${PROJECT_NAME}_emscripten
    cqasm
    emsdk::emsdk
)
