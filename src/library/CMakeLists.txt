CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
  CMAKE_POLICY(SET CMP0078 OLD)
ENDIF()
IF (CMAKE_VERSION VERSION_GREATER_EQUAL 3.14)
  CMAKE_POLICY(SET CMP0086 OLD)
ENDIF()

ENABLE_TESTING()

PROJECT("libQasm")

FIND_PACKAGE(BISON 3.0)
FIND_PACKAGE(FLEX 2.6)

if(${CMAKE_VERSION} VERSION_LESS "3.12.0") 
    FIND_PACKAGE(PythonInterp)
    if (${PYTHONINTERP_FOUND})
        FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
    endif()
    INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_DIRS} )
else()
    find_package (Python COMPONENTS Interpreter Development)
    INCLUDE_DIRECTORIES( ${Python_INCLUDE_DIRS} )
endif()


IF(COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_REQUIRED_FLAGS "-std=c++11")
ENDIF()

BISON_TARGET(
    MyParser
    grammar.y ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c
    COMPILE_FLAGS "-Wall -d -t -g -r all"
)
FLEX_TARGET(
    MyScanner
    lex.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c PROPERTIES LANGUAGE CXX)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c PROPERTIES LANGUAGE CXX)

ADD_LIBRARY(lexgram SHARED ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
SET_PROPERTY(TARGET lexgram PROPERTY CXX_STANDARD 11)

ADD_LIBRARY(lexgramstatic STATIC ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
SET_TARGET_PROPERTIES(lexgramstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
SET_PROPERTY(TARGET lexgramstatic PROPERTY CXX_STANDARD 11)

## Start of testing
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/../tests/cpp/doctest)
ADD_COMPILE_OPTIONS(-std=c++11 -Wall -Wextra)
# Keep test files in a separate source directory called test
FILE(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../tests/cpp/*.cpp)

# Run through each source
FOREACH(testSrc ${TEST_SRCS})
    # Extract the filename without an extension (NAME_WE)
    GET_FILENAME_COMPONENT(testName ${testSrc} NAME_WE)

    # Add compile target
    ADD_EXECUTABLE(${testName} ${testSrc})

    ADD_DEPENDENCIES(${testName} lexgram)

    SET_PROPERTY(TARGET ${testName} PROPERTY CXX_STANDARD 11)

    # Link to Boost libraries AND targets and dependencies
    TARGET_LINK_LIBRARIES(${testName} lexgram)

    # Move testing binaries into a testBin directory
    SET_TARGET_PROPERTIES(${testName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/testBin)

    # Copy shared lib to the testBin directory
    ADD_CUSTOM_COMMAND(TARGET ${testName} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:lexgram> ${CMAKE_CURRENT_BINARY_DIR}/testBin
    )
    # Finally add it to test execution
    # Notice the WORKING_DIRECTORY and COMMAND
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../tests/test_data/${testName}.qasm
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/testBin)
    ADD_TEST(NAME ${testName}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testBin
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBin/${testName})
ENDFOREACH(testSrc)

## Start of SWIG module for the python interface
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

SET_SOURCE_FILES_PROPERTIES(libQasm_swig.i PROPERTIES CPLUSPLUS ON)
SET_PROPERTY(SOURCE libQasm_swig.i PROPERTY SWIG_FLAGS -castmode -modern -keyword -w511)

IF (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    SWIG_ADD_MODULE(libQasm python libQasm_swig.i)
ELSE()
    SWIG_ADD_LIBRARY(libQasm LANGUAGE python SOURCES libQasm_swig.i)
ENDIF()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SWIG_LINK_LIBRARIES(libQasm lexgramstatic)
    SET_TARGET_PROPERTIES(_libQasm PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE ()
    SWIG_LINK_LIBRARIES(libQasm ${PYTHON_LIBRARIES} lexgramstatic)
ENDIF()
