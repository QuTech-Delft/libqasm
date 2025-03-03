cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(libqasm C CXX)

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

find_package(Python3 REQUIRED)

# antlr4-runtime
message(STATUS "Fetching antlr4-runtime")
# We point SOURCE_SUBDIR to a non existent directory, because we want FetchContent to download the sources,
# but we do not want CMake to process any subdirectory.
# Instead we will just build all the antlr4-runtime source files ourselves.
# This is needed when compiling to emscripten, because exception support is disabled by default.
# So if you compile C++ code using exceptions, as it is the case of the antlr4-runtime, to emscripten,
# and you want it to work as expected, you have to build the sources with the -fwasm-exceptions option.
FetchContent_Declare(antlr4-runtime
    GIT_REPOSITORY https://github.com/antlr/antlr4.git
    GIT_TAG "df4d68c09cdef73e023b8838a8bc7ca4dff1d1de"
    SOURCE_SUBDIR this-directory-does-not-exist
)
FetchContent_MakeAvailable(antlr4-runtime)

# fmt
find_package(fmt)
if(NOT fmt_FOUND OR LIBQASM_BUILD_EMSCRIPTEN)
    message(STATUS "Fetching fmt")
    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG "e69e5f977d458f2650bb346dadf2ad30c5320281"
        SOURCE_SUBDIR this-directory-does-not-exist
    )
    FetchContent_MakeAvailable(fmt)
endif()

# range-v3
find_package(range-v3)
if(NOT range-v3_FOUND)
    message(STATUS "Fetching range-v3")
    FetchContent_Declare(range-v3
        GIT_REPOSITORY https://github.com/ericniebler/range-v3
        GIT_TAG "a81477931a8aa2ad025c6bda0609f38e09e4d7ec"
    )
    FetchContent_MakeAvailable(range-v3)
    add_library(range-v3::range-v3 ALIAS range-v3)
endif()

# tree-gen library
find_package(tree-gen REQUIRED)
if(NOT tree-gen_FOUND OR LIBQASM_BUILD_EMSCRIPTEN)
    message(STATUS "Fetching tree-gen")
    FetchContent_Declare(tree-gen
        GIT_REPOSITORY https://github.com/QuTech-Delft/tree-gen
        GIT_TAG "4e50a87d05be88ad190e0a8be2d711faf9d1bce6"
        SOURCE_SUBDIR this-directory-does-not-exist
    )
    FetchContent_MakeAvailable(tree-gen)
endif()

# tree-gen executable
find_program(TREE_GEN_EXECUTABLE tree-gen REQUIRED)
message(STATUS "TREE_GEN_EXECUTABLE: ${TREE_GEN_EXECUTABLE}")

#-------------------------------------------------------------------------------
# cQASM common code generation and inclusion
#-------------------------------------------------------------------------------

# List of non-generated sources.
set(CQASM_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/annotations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/string_builder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/version.cpp"
)

#-------------------------------------------------------------------------------
# cQASM v3.x parser generation and inclusion
#-------------------------------------------------------------------------------
add_subdirectory(v3x)

# Generate the lexer and the parser.
function(antlr_target SCRIPT INPUT_DIR OUTPUT_SRC_DIR OUTPUT_INC_DIR ANTLR_OUTPUTS)
    add_custom_command(
        OUTPUT ${ANTLR_OUTPUTS}
        COMMAND ${Python3_EXECUTABLE} ${SCRIPT} ${INPUT_DIR} ${OUTPUT_SRC_DIR} ${OUTPUT_INC_DIR}
    )
endfunction()

set(ANTLR_OUTPUTS
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/CqasmLexer.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/CqasmParser.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/CqasmParserBaseVisitor.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/CqasmParserVisitor.cpp"
)
antlr_target(
    "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/generate_antlr_parser.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x"  # Input dir (grammar files)
    "${CMAKE_CURRENT_BINARY_DIR}/v3x"  # Sources output dir (lexer and parser cpp files)
    "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x"  # Include output dir (lexer and parser h files)
    "${ANTLR_OUTPUTS}"  # List of generated files
)
list(APPEND CQASM_V3X_SOURCES ${ANTLR_OUTPUTS})

# Generate the type tree classes.
generate_tree_py(
    "${TREE_GEN_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x/types.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x/types_generated.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/types_generated.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/types_generated.py"
)
list(APPEND CQASM_V3X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3x/types_generated.cpp")

# Generate the semantic value classes.
# Values can be either known at compile-time or only at runtime,
# but their type is resolved to one of the type tree nodes at compile-time.
generate_tree_py(
    "${TREE_GEN_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x/values.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x/values_generated.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/values_generated.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/values_generated.py"
)
list(APPEND CQASM_V3X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3x/values_generated.cpp")

# Generate the syntactic tree classes.
generate_tree_py(
    "${TREE_GEN_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x/syntactic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x/syntactic_generated.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/syntactic_generated.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/syntactic_generated.py"
)
list(APPEND CQASM_V3X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3x/syntactic_generated.cpp")

# Generate the semantic tree classes.
generate_tree_py(
    "${TREE_GEN_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/v3x/semantic.tree"
    "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x/semantic_generated.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/v3x/semantic_generated.cpp"
    "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/semantic_generated.py"
)
list(APPEND CQASM_V3X_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/v3x/semantic_generated.cpp")

#-------------------------------------------------------------------------------
# Compilation instructions
#-------------------------------------------------------------------------------

# Main cQASM library as an object library.
add_library(cqasm-lib-obj OBJECT
    ${CQASM_COMMON_SOURCES}
    ${CQASM_V3X_SOURCES}
)

file(GLOB ANTLR4_CPP_RUNTIME_SOURCES
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/atn/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/dfa/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/internal/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/misc/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/support/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/tree/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/tree/pattern/*.cpp"
    "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src/tree/xpath/*.cpp"
)
target_sources(cqasm-lib-obj
    PRIVATE "${ANTLR4_CPP_RUNTIME_SOURCES}"
)

if(LIBQASM_BUILD_EMSCRIPTEN)
    file(GLOB FMT_SOURCES
        "${fmt_SOURCE_DIR}/src/*"
    )
    file(GLOB TREE_GEN_SOURCES
        "${tree-gen_SOURCE_DIR}/include/*"
        "${tree-gen_SOURCE_DIR}/src/*"
    )
    target_sources(cqasm-lib-obj
        PRIVATE "${FMT_SOURCES}"
        PRIVATE "${TREE_GEN_SOURCES}"
    )
endif()

set(antlr4-runtime_INCLUDE_DIRS "${antlr4-runtime_SOURCE_DIR}/runtime/Cpp/runtime/src")
if(LIBQASM_BUILD_EMSCRIPTEN)
    set(fmt_INCLUDE_DIRS "${fmt_SOURCE_DIR}/include")
    set(range-v3_INCLUDE_DIRS "${range-v3_SOURCE_DIR}/include")
    set(tree-gen_INCLUDE_DIRS "${tree-gen_SOURCE_DIR}/include")
endif()
target_include_directories(cqasm-lib-obj
    # Do not remove the ending '/' since
    # it avoids the whole 'include' directory to be copied to the installation folder.
    # Instead, just its contents are copied.
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/../include/"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/../include/libqasm/v3x"
    PUBLIC "${antlr4-runtime_INCLUDE_DIRS}/"
    PRIVATE "${fmt_INCLUDE_DIRS}/"
    PRIVATE "${range-v3_INCLUDE_DIRS}/"
    PRIVATE "${tree-gen_INCLUDE_DIRS}/"
)

target_compile_features(cqasm-lib-obj
    PUBLIC cxx_std_20
)

target_link_libraries(cqasm-lib-obj PRIVATE range-v3::range-v3)
if(NOT LIBQASM_BUILD_EMSCRIPTEN)
    target_link_libraries(cqasm-lib-obj PRIVATE fmt::fmt tree-gen::tree-gen)
endif()

# fPIC: otherwise some weirdness happens with pthreads or something when linking statically.
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(cqasm-lib-obj PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=array-bounds=
        -Wno-error=deprecated-declarations
        -Wno-error=overloaded-virtual=
        -Wno-error=sign-compare
        -Wno-error=uninitialized
        -Wno-error=unused-parameter
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # CqasmParser.cpp, expressionSempred method defines _localctx parameter but does not use it
    target_compile_options(cqasm-lib-obj PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=sign-compare
        -Wno-error=unused-parameter
        -Wno-error=unused-private-field
        -Wno-error=unused-but-set-variable
    )
elseif(MSVC)
    target_compile_options(cqasm-lib-obj PRIVATE
        /WX
        /D_CRT_NONSTDC_NO_DEPRECATE
        /D_CRT_SECURE_NO_WARNINGS
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
else()
    message(SEND_ERROR "Unknown compiler!")
endif()
if(LIBQASM_BUILD_EMSCRIPTEN)
    target_compile_options(cqasm-lib-obj PRIVATE
        -Wno-error=include-angled-in-module-purview
        -Wno-error=unused-function
        -Wno-error=unused-parameter
    )
endif()

# Main cQASM library in shared or static form as managed by CMake's internal BUILD_SHARED_LIBS variable.
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
# Debug info
#-------------------------------------------------------------------------------

message(STATUS
    "[${PROJECT_NAME}] Target include directories:\n"
    "      CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}\n"
    "      CMAKE_CURRENT_SOURCE_DIR/../include/: ${CMAKE_CURRENT_SOURCE_DIR}/../include/\n"
    "      CMAKE_CURRENT_BINARY_DIR/../include/: ${CMAKE_CURRENT_BINARY_DIR}/../include/\n"
    "      antlr4-runtime_INCLUDE_DIRS: ${antlr4-runtime_INCLUDE_DIRS}\n"
    "      fmt_INCLUDE_DIRS: ${fmt_INCLUDE_DIRS}\n"
    "      range-v3_INCLUDE_DIRS: ${range-v3_INCLUDE_DIRS}\n"
    "      tree-gen_INCLUDE_DIRS: ${tree-gen_INCLUDE_DIRS}\n"
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
install(
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
        "${CMAKE_CURRENT_BINARY_DIR}/../include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "*.inc"
)
