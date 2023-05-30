cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Generator for functions usable within cQASM.
add_executable(
    func-gen
    "func-gen.cpp"
)

target_include_directories(
    func-gen
    PRIVATE "."
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
)

set(func-gen-path "${CMAKE_CURRENT_BINARY_DIR}/func-gen" PARENT_SCOPE)

function(generate_funcs HDR SRC VER)

    # Get the directory for the header file and make sure it exists.
    get_filename_component(HDR_DIR "${HDR}" PATH)
    file(MAKE_DIRECTORY "${HDR_DIR}")

    # Get the directory for the source file and make sure it exists.
    get_filename_component(SRC_DIR "${SRC}" PATH)
    file(MAKE_DIRECTORY "${SRC_DIR}")

    # Add a command to do the generation. Note the $<TARGET_FILE:func-gen>
    # dependency; this makes cmake correctly recompile the generator and
    # regenerate the files with it if its source files change.
    add_custom_command(
        COMMAND "${func-gen-path}" "${HDR}" "${SRC}" "${VER}"
        OUTPUT "${HDR}" "${SRC}"
        DEPENDS func-gen
    )

endfunction()
