cmake_minimum_required(VERSION 2.8.2)

project(bison-download NONE)

if("${BISON_VERSION_TO_BUILD}" VERSION_LESS "${BISON_VERSION_REQUIRED}")
    message(FATAL_ERROR "Bison version to build should be greater or equal to the minimum required Bison version.")
endif()

include(ExternalProject)
ExternalProject_Add(
    bison
    URL "https://ftp.gnu.org/gnu/bison/bison-${BISON_VERSION_TO_BUILD}.tar.gz"
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/bison-build"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/bison-install"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
)
