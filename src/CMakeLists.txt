cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(libcqasm C CXX)

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

# Windows weirdness: need a .lib file to link against a DLL at compile-time (I think),
# but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#-------------------------------------------------------------------------------
# Packages
#-------------------------------------------------------------------------------

include(FetchContent)
# tree-gen, a custom utility to generate classes for typed tree-like structures and enums with variants containing data.
# This exposes the generate_tree() function.
FetchContent_Declare(tree-gen
    GIT_REPOSITORY https://github.com/QuTech-Delft/tree-gen.git
    GIT_TAG "d708ea629bad014011d3f1efc352077414dd86f7"
)
FetchContent_MakeAvailable(tree-gen)

# Include func-gen, a simple program that generates the function that registers all default functions
# (operators, trigonometry, etc) with a resolver::FunctionTable object.
# This file would otherwise be extremely repetitive and annoying to maintain due to the amount of overloads.
# This exposes the generate_funcs() function, which should be called once with the header and source file paths.
add_subdirectory(func-gen)

# Require flex/bison; if not installed, this will try to build from source.
find_package(BISON 3.0)
find_package(FLEX 2.6.4)
if(WIN32)
    set(flex_win_compat --wincompat)
else()
    set(flex_win_compat)
endif()

find_package(antlr4-runtime REQUIRED)

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
add_subdirectory(v1x)

# Generate the lexer.
flex_target(cqasm-v1x-lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-lexer.l"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-lexer.cpp"
    COMPILE_FLAGS
        "--header-file=${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-lexer.hpp ${flex_win_compat}"
)
list(APPEND CQASM_V1X_SOURCES ${FLEX_cqasm-v1x-lexer_OUTPUTS})

# Generate the parser.
bison_target(cqasm-v1x-parser
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-parser.y"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-parser.cpp"
    COMPILE_FLAGS
        "-rall --report-file=${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-parser.txt -Werror=conflicts-sr -Werror=conflicts-rr"
)
list(APPEND CQASM_V1X_SOURCES ${BISON_cqasm-v1x-parser_OUTPUTS})

# Dependency between lexer and parser.
add_flex_bison_dependency(
    cqasm-v1x-lexer
    cqasm-v1x-parser
)

# Generate the abstract syntax tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-ast.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1x/cqasm-ast-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-ast-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1x/cqasm-ast-gen.py"
)
list(APPEND CQASM_V1X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-ast-gen.cpp")

# Generate the type tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-types.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1x/cqasm-types-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-types-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1x/cqasm-types-gen.py"
)
list(APPEND CQASM_V1X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-types-gen.cpp")

# Generate the semantic value classes. Values can be either known at
# compile-time or only at runtime, but their type is resolved to one of the
# type tree nodes at compile-time.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-values.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1x/cqasm-values-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-values-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1x/cqasm-values-gen.py"
)
list(APPEND CQASM_V1X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-values-gen.cpp")

# Generate the semantic tree classes.
generate_tree_py(
    "${CMAKE_CURRENT_SOURCE_DIR}/v1x/cqasm-semantic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1x/cqasm-semantic-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-semantic-gen.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v1x/cqasm-semantic-gen.py"
)
list(APPEND CQASM_V1X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-semantic-gen.cpp")

# Generate the cQASM default function set.
generate_funcs(
    "${CMAKE_CURRENT_BINARY_DIR}/../include/v1x/cqasm-functions-gen.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-functions-gen.cpp"
    "v1x"
)
list(APPEND CQASM_V1X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v1x/cqasm-functions-gen.cpp")

#-------------------------------------------------------------------------------
# cQASM v3.x parser generation and inclusion
#-------------------------------------------------------------------------------
add_subdirectory(v3x)

# Generate the lexer and the parser.
function(antlr_target SCRIPT INPUT_DIR OUTPUT_DIR ANTLR_OUTPUTS)
    add_custom_command(
        OUTPUT ${ANTLR_OUTPUTS}
        COMMAND ${PYTHON_EXECUTABLE} ${SCRIPT} ${INPUT_DIR} ${OUTPUT_DIR}
    )
endfunction()

set(ANTLR_OUTPUTS
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/cqasm_lexer.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/cqasm_parser.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/cqasm_parserBaseListener.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/cqasm_parserListener.cpp"
)
antlr_target(
    "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/generate_antlr_parser.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x"  # Input dir (grammar files)
    "${CMAKE_CURRENT_BINARY_DIR}/v3x"  # Output dir (lexer and parser cpp files)
    "${ANTLR_OUTPUTS}"  # List of generated files
)
list(APPEND CQASM_V3X_SOURCES ${ANTLR_OUTPUTS})

#-------------------------------------------------------------------------------
# Compilation instructions
#-------------------------------------------------------------------------------

# Main cQASM library as an object library.
add_library(cqasm-lib-obj OBJECT
    ${CQASM_COMMON_SOURCES}
    ${CQASM_V1X_SOURCES}
    ${CQASM_V3X_SOURCES}
)

target_compile_definitions(cqasm-lib-obj
    PRIVATE ${TREE_LIB_PRIVATE_DEFS}
)

# The src directory and binary directory (for generated header files) are private.
# The include directory is public.
# That is, the files in include must not try to include generated files and files from the src directory,
# but the other direction is fine.
target_include_directories(cqasm-lib-obj
    PRIVATE ${TREE_LIB_PRIVATE_INCLUDE}
    PUBLIC ${TREE_LIB_PUBLIC_INCLUDE}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/"
    # Do not remove the ending '/' since
    # it avoids the whole 'include' directory to be copied to the installation folder.
    # Instead, just its contents are copied.
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/../include/"
    PUBLIC "${antlr4-runtime_INCLUDE_DIR}"
)

target_compile_features(cqasm-lib-obj PUBLIC
    cxx_std_20
)

target_link_libraries(cqasm-lib-obj PUBLIC
    antlr4_static
    tree-lib-obj
)

# fPIC: otherwise some weirdness happens with pthreads or something when linking statically.
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(cqasm-lib-obj PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=deprecated-declarations
        -Wno-error=restrict
        -Wno-error=sign-compare
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(cqasm-lib-obj PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=sign-compare
        -Wno-error=unused-private-field
        -Wno-error=unused-but-set-variable
    )
elseif(MSVC)
    target_compile_options(cqasm-lib-obj PRIVATE
        /W3 /w34996  # not using /WX at the moment because the flex generated code has some warnings
        /D_CRT_NONSTDC_NO_DEPRECATE
        /D_CRT_SECURE_NO_WARNINGS
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

# Main cQASM library in shared or static form as managed by cmake's
# internal BUILD_SHARED_LIBS variable.
add_library(cqasm
    $<TARGET_OBJECTS:cqasm-lib-obj>
)
target_include_directories(cqasm PUBLIC
    $<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(cqasm PUBLIC
    $<TARGET_PROPERTY:cqasm-lib-obj,LINK_LIBRARIES>
)

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
get_target_property(INCLUDE_DIRS cqasm-lib-obj INTERFACE_INCLUDE_DIRECTORIES)
install(
    DIRECTORY ${INCLUDE_DIRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN "*.hpp" PATTERN "*.inc"
)
