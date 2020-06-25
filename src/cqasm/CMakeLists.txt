cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
if(CMAKE_VERSION VERSION_GREATER 3.0)
    CMAKE_POLICY(SET CMP0048 NEW)
endif()

project(libcqasm CXX)

# Library type option. Default is a static library.
option(
    BUILD_SHARED_LIBS
    "whether the cqasm library should be built as a shared object or as a static library"
    OFF
)

option(
    BUILD_TESTS
    "whether the tests should be built and added to `make test`"
    OFF
)

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Require C++11 compiler support.
if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif()
else()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

# Require flex/bison; if not installed, this will try to build from source.
set(FLEX_VERSION_REQUIRED 2.6.1)
set(BISON_VERSION_REQUIRED 3.0)
include(cmake/flex-bison.cmake)

# Generate the lexer.
if(WIN32)
    set(flex_win_compat --wincompat)
else()
    set(flex_win_compat)
endif()
flex_target(
    cqasm-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-lexer.cpp"
    COMPILE_FLAGS
        "--header-file=${CMAKE_CURRENT_BINARY_DIR}/cqasm-lexer.hpp ${flex_win_compat}"
)

# Generate the parser.
bison_target(
    cqasm-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-parser.cpp"
    COMPILE_FLAGS
        "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/cqasm-parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
)

# Dependency between lexer and parser.
add_flex_bison_dependency(
    cqasm-lexer
    cqasm-parser
)

# Include tree-gen, a custom utility to generate classes for typed tree-like
# structures and enums with variants containing data. This exposes the
# generate_tree() function.
add_subdirectory(tree-gen)

# Generate the abstract syntax tree classes.
generate_tree(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-ast.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/include/cqasm-ast-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-ast-gen.cpp"
)

# Generate the type tree classes.
generate_tree(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-types.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/include/cqasm-types-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-types-gen.cpp"
)

# Generate the semantic value classes. Values can be either known at
# compile-time or only at runtime, but their type is resolved to one of the
# type tree nodes at compile-time.
generate_tree(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-values.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/include/cqasm-values-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-values-gen.cpp"
)

# Generate the semantic tree classes.
generate_tree(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-semantic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/include/cqasm-semantic-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-semantic-gen.cpp"
)

# Include func-gen, a simple program that generates the function that registers
# all default functions (operators, trigonometry, etc) with a
# resolver::FunctionTable object. This file would otherwise be extremely
# repetitive and annoying to maintain due to the amount of overloads. This
# exposes the generate_funcs() function, which should be called once with the
# header and source file paths.
add_subdirectory(func-gen)

# Generate the cQASM default function set.
generate_funcs(
    "${CMAKE_CURRENT_BINARY_DIR}/include/cqasm-functions-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-functions-gen.cpp"
)

# Main cQASM library as an object library.
add_library(
    cqasm_objlib OBJECT
    ${BISON_cqasm-parser_OUTPUTS}
    ${FLEX_cqasm-lexer_OUTPUTS}
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-ast-gen.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-types-gen.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-values-gen.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-semantic-gen.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-functions-gen.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-primitives.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-ast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-types.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-values.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-error-model.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-instruction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-resolver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-parse-helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-analyzer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cqasm-utils.cpp"
)

# The src directory and binary directory (for generated header files) are
# private. The include directory is public. That is, the files in include
# must not try to include generated files and files from the src directory,
# but the other direction is fine.
target_include_directories(
    cqasm_objlib
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include"
)

# Enable all warnings and treat them as errors when compiling libcqasm.
if(MSVC)
    #set_target_properties(cqasm_objlib PROPERTIES COMPILE_FLAGS "/Wall")
else()
    set_target_properties(cqasm_objlib PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
endif()

# Always compiled with -fPIC, otherwise some weirdness happens with pthreads or
# something when linking statically.
set_target_properties(cqasm_objlib PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Main cQASM library in shared or static form as managed by cmake's
# internal BUILD_SHARED_LIBS variable.
add_library(cqasm $<TARGET_OBJECTS:cqasm_objlib>)
target_include_directories(cqasm PUBLIC $<TARGET_PROPERTY:cqasm_objlib,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(cqasm PUBLIC $<TARGET_PROPERTY:cqasm_objlib,LINK_LIBRARIES>)

# Add the test directory.
if(BUILD_TESTS)
    include(cmake/googletest.cmake)
    enable_testing()
    add_subdirectory(tests)
endif()
