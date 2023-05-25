cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0078)
  cmake_policy(SET CMP0078 OLD)
endif()

if(POLICY CMP0086)
  cmake_policy(SET CMP0086 OLD)
endif()

enable_testing()

project("libQasm")

if(${CMAKE_VERSION} VERSION_LESS "3.12.0")
    find_package(PythonInterp)
    if (${PYTHONINTERP_FOUND})
        find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
    endif()
    include_directories(${PYTHON_INCLUDE_DIRS})
else()
    find_package(Python COMPONENTS Interpreter Development)
    include_directories(${Python_INCLUDE_DIRS})
endif()

set(CMAKE_REQUIRED_FLAGS "-std=c++23")

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../../../library)

# Include the new library.
add_subdirectory(../../cqasm cqasm)

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# NOTE: for compatibility purposes, the cqasm library is essentially renamed to
# lexgram/lexgramstatic here.
add_library(lexgram SHARED $<TARGET_OBJECTS:cqasm-lib-obj>)
target_include_directories(lexgram PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(lexgram PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,LINK_LIBRARIES>)
set_property(TARGET lexgram PROPERTY CXX_STANDARD 23)

add_library(lexgramstatic STATIC $<TARGET_OBJECTS:cqasm-lib-obj>)
target_include_directories(lexgramstatic PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(lexgramstatic PUBLIC $<TARGET_PROPERTY:cqasm-lib-obj,LINK_LIBRARIES>)
set_target_properties(lexgramstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_property(TARGET lexgramstatic PROPERTY CXX_STANDARD 23)

## Start of testing
include_directories(../../test/v1.0/cpp/doctest)
if(MSVC)
    add_compile_options(/std=c++23)
else()
    add_compile_options(-std=c++23 -Wall -Wextra)
endif()
# Keep test files in a separate source directory called test
file(GLOB TEST_SRCS RELATIVE ../../../library ../tests/cpp/*.cpp)

# Run through each source
foreach(testSrc ${TEST_SRCS})
    # Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)

    # Add compile target
    add_executable(${testName} ${testSrc})

    add_dependencies(${testName} lexgram)

    set_property(TARGET ${testName} PROPERTY CXX_STANDARD 23)

    # Link to Boost libraries AND targets and dependencies
    target_link_libraries(${testName} lexgram)

    # Move testing binaries into a testBin directory
    set_target_properties(${testName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/testBin)

    # Copy shared lib to the testBin directory
    add_custom_command(TARGET ${testName} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:lexgram> ${CMAKE_CURRENT_BINARY_DIR}/testBin
    )
    # Finally add it to test execution
    # Notice the WORKING_DIRECTORY and COMMAND
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../tests/test_data/${testName}.qasm
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/testBin)
    add_test(NAME ${testName}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName})
endforeach(testSrc)
