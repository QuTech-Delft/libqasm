cmake_minimum_required(VERSION 2.8)

find_package(BISON 3.0)
find_package(FLEX 2.6)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_REQUIRED_FLAGS "-std=c++11")
endif()

BISON_TARGET(
                MyParser 
                grammar.y ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c
                COMPILE_FLAGS "-Wall -d -t -g -r all"
            )
FLEX_TARGET(MyScanner lex.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c PROPERTIES LANGUAGE CXX )
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c PROPERTIES LANGUAGE CXX )
ADD_LIBRARY( lexgram STATIC
             ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

set_property(TARGET lexgram PROPERTY CXX_STANDARD 11)

add_compile_options(-std=c++11 -Wall -Wextra -Werror -DYYDEBUG=0)
add_executable(
                  testCompiler main.cpp
              )
add_dependencies(testCompiler lexgram)
target_link_libraries(testCompiler lexgram)
set_property(
                TARGET testCompiler 
                PROPERTY CXX_STANDARD 11
            )