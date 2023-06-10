cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

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

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#-------------------------------------------------------------------------------
# cQASM common code
#-------------------------------------------------------------------------------

# List of non-generated sources.
set(CQASM_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-annotations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-py.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-string-builder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version-lexer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version-parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_libqasm_conan_package.cpp"
)

#-------------------------------------------------------------------------------
# tree-gen
#-------------------------------------------------------------------------------
add_subdirectory(tree-gen)

#-------------------------------------------------------------------------------
# cQASM v1.x parser
#-------------------------------------------------------------------------------
add_subdirectory(v1)

#-------------------------------------------------------------------------------
# cQASM v3 parser
#-------------------------------------------------------------------------------
add_subdirectory(v3)

#-------------------------------------------------------------------------------
# Compilation instructions
#-------------------------------------------------------------------------------

# Main cQASM library as an object library.
add_library(
    cqasm-lib-obj OBJECT
    ${CQASM_COMMON_SOURCES}
    ${TREE_GEN_SOURCES}
    ${CQASM_V1_SOURCES}
    ${CQASM_V3_SOURCES}
)

# The src directory and binary directory (for generated header files) are private.
# The include directory is public.
# That is, the files in include must not try to include generated files and files from the src directory,
# but the other direction is fine.
target_include_directories(
    cqasm-lib-obj
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/"
    # Do not remove the ending '/' since
    # it avoids the whole 'include' directory to be copied to the installation folder.
    # Instead, just its contents are copied.
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
)

target_compile_features(cqasm-lib-obj PUBLIC cxx_std_20)
target_compile_definitions(cqasm-lib-obj PRIVATE BUILDING_TREE_LIB)

# fPIC: otherwise some weirdness happens with pthreads or something when linking statically.
if(MSVC)
    target_compile_options(cqasm-lib-obj PRIVATE
        /W3 /w34996  # not using /WX at the moment because the flex generated code has some warnings
        /D_CRT_NONSTDC_NO_DEPRECATE
        /D_CRT_SECURE_NO_WARNINGS
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
   )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(cqasm-lib-obj PRIVATE
        -pedantic-errors -Wall -Wextra -Werror
        -fPIC
        -Wno-error=sign-compare
    )
endif()

# Main cQASM library in shared or static form as managed by cmake's
# internal BUILD_SHARED_LIBS variable.
add_library(cqasm $<TARGET_OBJECTS:cqasm-lib-obj>)
target_include_directories(cqasm PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(cqasm PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,LINK_LIBRARIES>)

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
