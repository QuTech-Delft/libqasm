find_package(emsdk)

add_executable(${PROJECT_NAME}_emscripten
    "${CMAKE_CURRENT_SOURCE_DIR}/emscripten_wrapper.cpp"
)

target_link_libraries(${PROJECT_NAME}_emscripten
    PRIVATE cqasm
)

target_compile_options(${PROJECT_NAME}_emscripten
    PUBLIC -fwasm-exceptions
)

target_link_options(${PROJECT_NAME}_emscripten
    PUBLIC -lembind -fwasm-exceptions --profiling-funcs -sASSERTIONS=1 -sMODULARIZE=1
)
