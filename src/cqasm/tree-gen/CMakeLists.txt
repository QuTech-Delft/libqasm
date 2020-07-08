cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# Generate the lexer.
if(WIN32)
    set(flex_win_compat --wincompat)
else()
    set(flex_win_compat)
endif()
flex_target(
    tree-gen-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp"
    COMPILE_FLAGS
        "--header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.hpp ${flex_win_compat}"
)

# Generate the parser.
bison_target(
    tree-gen-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/parser.cpp"
    COMPILE_FLAGS
        "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
)

# Dependency between lexer and parser.
add_flex_bison_dependency(
    tree-gen-lexer
    tree-gen-parser
)

# Generate the AST node class definitions.
add_executable(
    tree-gen
    ${BISON_tree-gen-parser_OUTPUTS}
    ${FLEX_tree-gen-lexer_OUTPUTS}
    "${CMAKE_CURRENT_SOURCE_DIR}/tree-gen.cpp"
)

target_include_directories(
    tree-gen
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
)

# Utility function for generating a tree with tree-gen.
function(generate_tree TREE HDR SRC)

    # Get the directory for the header file and make sure it exists.
    get_filename_component(HDR_DIR "${HDR}" PATH)
    file(MAKE_DIRECTORY "${HDR_DIR}")

    # Get the directory for the source file and make sure it exists.
    get_filename_component(SRC_DIR "${SRC}" PATH)
    file(MAKE_DIRECTORY "${SRC_DIR}")

    # Add a command to do the generation.
    add_custom_command(
        COMMAND tree-gen "${TREE}" "${HDR_DIR}" "${SRC_DIR}"
        OUTPUT "${HDR}" "${SRC}"
        DEPENDS "${TREE}" tree-gen
    )

endfunction()
