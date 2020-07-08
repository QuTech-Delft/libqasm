cmake_minimum_required(VERSION 2.8.2)

project(bison-download NONE)

include(ExternalProject)
ExternalProject_Add(
    bison
    URL "https://ftp.gnu.org/gnu/bison/bison-${BISON_VERSION_REQUIRED}.tar.gz"
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/bison-build"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/bison-install"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
)
