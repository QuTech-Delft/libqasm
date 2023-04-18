cmake_minimum_required(VERSION 2.8.2)

project(m4-download NONE)

include(ExternalProject)
ExternalProject_Add(
    m4
    URL "https://ftp.gnu.org/gnu/m4/m4-${M4_VERSION_REQUIRED}.tar.gz"
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/m4-build"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/m4-install"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
)
