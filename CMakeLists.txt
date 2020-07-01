cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
if(CMAKE_VERSION VERSION_GREATER 3.0)
    CMAKE_POLICY(SET CMP0048 NEW)
endif()

project(cqasm CXX)

# Library type option. Default is a static library.
option(
    BUILD_SHARED_LIBS
    "whether the cqasm library should be built as a shared object or as a static library"
    OFF
)

# Whether tests should be built.
option(
    LIBQASM_BUILD_TESTS
    "whether the tests should be built and added to `make test`"
    OFF
)
if (LIBQASM_BUILD_TESTS)
    enable_testing()
endif()

# Compatibility mode. When enabled, the legacy API headers in src/library are
# added to the public headers of the cqasm target. To enable this in your CMake
# project, add `option(LIBQASM_COMPAT "" ON)` before the `add_subdirectory`
# command.
option(
    LIBQASM_COMPAT
    "whether the legacy API headers should be added to the cqasm library"
    OFF
)

# Jump to where the new API and the core of libqasm lives.
add_subdirectory(src/cqasm)

# Compatibility mode: provide/test the original API in addition to the new one.
if (LIBQASM_COMPAT)

    # Add the compatibility headers to the include path.
    target_include_directories(cqasm_objlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/library/)

    # Make sure to install the headers as well.
    include(GNUInstallDirs)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/library/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )

    # If LIBQASM_BUILD_TESTS is also enabled, also add the tests for the
    # original API.
    if (LIBQASM_BUILD_TESTS)
        file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/tests/cpp/*.cpp)
        foreach(TEST_SRC ${TEST_SRCS})
            get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
            add_executable(${TEST_NAME} ${TEST_SRC})
            target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/cpp/doctest)
            target_link_libraries(${TEST_NAME} cqasm)
            add_test(
                NAME ${TEST_NAME}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_data
                COMMAND ${TEST_NAME}
            )
        endforeach()
    endif()

endif()
