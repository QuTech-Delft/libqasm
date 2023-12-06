find_package(emsdk)

add_executable(${PROJECT_NAME}_emscripten
    "${CMAKE_CURRENT_SOURCE_DIR}/emscripten_wrapper.cpp"
)

target_link_libraries(${PROJECT_NAME}_emscripten
    cqasm
)

set_target_properties(${PROJECT_NAME}_emscripten
    PROPERTIES LINK_FLAGS "-lembind -sMODULARIZE=1"
)
