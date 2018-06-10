cmake_minimum_required(VERSION 2.8)

find_package(BISON 3.0)
find_package(FLEX 2.6)

# BISON_TARGET(MyParser cppcalc.yy ${CMAKE_CURRENT_BINARY_DIR}/cppcalc.tab.cpp)
# FLEX_TARGET(MyScanner cppcalc.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
# ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

# include_directories(${CMAKE_CURRENT_BINARY_DIR})
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# add_executable(Calc
#   ${BISON_MyParser_OUTPUTS}
#   ${FLEX_MyScanner_OUTPUTS}
# )