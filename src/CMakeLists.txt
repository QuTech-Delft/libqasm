cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(libcqasm CXX)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

# Library type option. Default is a static library.
option(
    BUILD_SHARED_LIBS
    "whether the cqasm library should be built as a shared object or as a static library"
    OFF
)

option(
    LIBQASM_BUILD_TESTS
    "whether the tests should be built and added to `make test`"
    OFF
)

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Require C++23 compiler support.
if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
    endif()
else()
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

#-------------------------------------------------------------------------------
# Code generation tool inclusion and build
#-------------------------------------------------------------------------------

# Require flex/bison; if not installed, this will try to build from source.
set(FLEX_VERSION_REQUIRED 2.6.1)
set(BISON_VERSION_REQUIRED 3.0)
set(BISON_VERSION_TO_BUILD 3.8)
include(../cmake/flex-bison.cmake)
if(WIN32)
    set(flex_win_compat --wincompat)
else()
    set(flex_win_compat)
endif()

# Include tree-gen, a custom utility to generate classes for typed tree-like
# structures and enums with variants containing data. This exposes the
# generate_tree() function.
add_subdirectory(tree-gen)

# Include func-gen, a simple program that generates the function that registers
# all default functions (operators, trigonometry, etc) with a
# resolver::FunctionTable object. This file would otherwise be extremely
# repetitive and annoying to maintain due to the amount of overloads. This
# exposes the generate_funcs() function, which should be called once with the
# header and source file paths.
add_subdirectory(func-gen)

#-------------------------------------------------------------------------------
# cQASM common code generation and inclusion
#-------------------------------------------------------------------------------

# List of non-generated sources.
set(CQASM_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-annotations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-py.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-string-builder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version.cpp"
)

# Generate the version lexer.
flex_target(cqasm-version-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version-lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-version-lexer.cpp"
    COMPILE_FLAGS
        "--header-file=${CMAKE_CURRENT_BINARY_DIR}/cqasm-version-lexer.hpp ${flex_win_compat}"
)
list(APPEND CQASM_COMMON_SOURCES ${FLEX_cqasm-version-lexer_OUTPUTS})

# Generate the version parser.
bison_target(cqasm-version-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version-parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/cqasm-version-parser.cpp"
    COMPILE_FLAGS
        "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/cqasm-version-parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
)
list(APPEND CQASM_COMMON_SOURCES ${BISON_cqasm-version-parser_OUTPUTS})

# Dependency between lexer and parser.
add_flex_bison_dependency(
    cqasm-version-lexer
    cqasm-version-parser
)

#-------------------------------------------------------------------------------
# cQASM v1.x code generation and inclusion
#-------------------------------------------------------------------------------
add_subdirectory(v1)

# Generate the lexer.
flex_target(cqasm-v1-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-lexer.cpp"
    COMPILE_FLAGS
        "--header-file=${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-lexer.hpp ${flex_win_compat}"
)
list(APPEND CQASM_V1_SOURCES ${FLEX_cqasm-v1-lexer_OUTPUTS})

# Generate the parser.
bison_target(cqasm-v1-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-parser.cpp"
    COMPILE_FLAGS
        "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
)
list(APPEND CQASM_V1_SOURCES ${BISON_cqasm-v1-parser_OUTPUTS})

# Dependency between lexer and parser.
add_flex_bison_dependency(
    cqasm-v1-lexer
    cqasm-v1-parser
)

# Generate the abstract syntax tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-ast.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1/cqasm-ast-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-ast-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1/cqasm-ast-gen.py"
)
list(APPEND CQASM_V1_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-ast-gen.cpp")

# Generate the type tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-types.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1/cqasm-types-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-types-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1/cqasm-types-gen.py"
)
list(APPEND CQASM_V1_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-types-gen.cpp")

# Generate the semantic value classes. Values can be either known at
# compile-time or only at runtime, but their type is resolved to one of the
# type tree nodes at compile-time.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-values.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1/cqasm-values-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-values-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1/cqasm-values-gen.py"
)
list(APPEND CQASM_V1_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-values-gen.cpp")

# Generate the semantic tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1/cqasm-semantic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1/cqasm-semantic-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-semantic-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1/cqasm-semantic-gen.py"
)
list(APPEND CQASM_V1_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-semantic-gen.cpp")

# Generate the cQASM default function set.
generate_funcs(
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1/cqasm-functions-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-functions-gen.cpp"
    "v1"
)
list(APPEND CQASM_V1_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1/cqasm-functions-gen.cpp")

#-------------------------------------------------------------------------------
# cQASM v3 parser generation and inclusion
#-------------------------------------------------------------------------------
add_subdirectory(v3)

# Generate the lexer.
flex_target(cqasm-v3-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-lexer.cpp"
    COMPILE_FLAGS
    "--header-file=${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-lexer.hpp ${flex_win_compat}"
    )
list(APPEND CQASM_V3_SOURCES ${FLEX_cqasm-v3-lexer_OUTPUTS})

# Generate the parser.
bison_target(cqasm-v3-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-parser.cpp"
    COMPILE_FLAGS
    "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
    )
list(APPEND CQASM_V3_SOURCES ${BISON_cqasm-v3-parser_OUTPUTS})

# Dependency between lexer and parser.
add_flex_bison_dependency(
    cqasm-v3-lexer
    cqasm-v3-parser
)

# Generate the abstract syntax tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-ast.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v3/cqasm-ast-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-ast-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3/cqasm-ast-gen.py"
)
list(APPEND CQASM_V3_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-ast-gen.cpp")

# Generate the type tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-types.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v3/cqasm-types-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-types-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3/cqasm-types-gen.py"
)
list(APPEND CQASM_V3_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-types-gen.cpp")

# Generate the semantic value classes. Values can be either known at
# compile-time or only at runtime, but their type is resolved to one of the
# type tree nodes at compile-time.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-values.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v3/cqasm-values-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-values-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3/cqasm-values-gen.py"
)
list(APPEND CQASM_V3_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-values-gen.cpp")

# Generate the semantic tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v3/cqasm-semantic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v3/cqasm-semantic-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-semantic-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3/cqasm-semantic-gen.py"
)
list(APPEND CQASM_V3_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-semantic-gen.cpp")

# Generate the cQASM default function set.
generate_funcs(
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v3/cqasm-functions-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-functions-gen.cpp"
    "v3"
)
list(APPEND CQASM_V3_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3/cqasm-functions-gen.cpp")

#-------------------------------------------------------------------------------
# Compilation instructions
#-------------------------------------------------------------------------------

# Main cQASM library as an object library.
# NOTE: tree-gen's support library is included here directly. It'd be great to
# just be able to use target_link_libraries(cqasm_objlib tree-lib-obj), but
# that only works from CMake 3.12 onwards. So for compatibility, we're forced
# to do it the old-fashioned way.
add_library(
    cqasm_objlib OBJECT
    ${TREE_LIB_SRCS}
    ${CQASM_COMMON_SOURCES}
    ${CQASM_V1_SOURCES}
    ${CQASM_V3_SOURCES}
)

target_compile_definitions(cqasm_objlib PRIVATE ${TREE_LIB_PRIVATE_DEFS})

# The src directory and binary directory (for generated header files) are
# private. The include directory is public. That is, the files in include
# must not try to include generated files and files from the src directory,
# but the other direction is fine.
target_include_directories(
    cqasm_objlib
    PRIVATE ${TREE_LIB_PRIVATE_INCLUDE}
    PUBLIC ${TREE_LIB_PUBLIC_INCLUDE}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/../include"
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
set_target_properties(tree-lib-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Main cQASM library in shared or static form as managed by cmake's
# internal BUILD_SHARED_LIBS variable.
add_library(cqasm $<TARGET_OBJECTS:cqasm_objlib>)
target_include_directories(cqasm PUBLIC $<TARGET_PROPERTY:cqasm_objlib,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(cqasm PUBLIC $<TARGET_PROPERTY:cqasm_objlib,LINK_LIBRARIES>)

#-------------------------------------------------------------------------------
# Install instructions
#-------------------------------------------------------------------------------

# Install target.
include(GNUInstallDirs)
install(
    TARGETS cqasm
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
get_target_property(INCLUDE_DIRS cqasm_objlib INTERFACE_INCLUDE_DIRECTORIES)
install(
    DIRECTORY ${INCLUDE_DIRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)
