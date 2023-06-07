cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

if(POLICY CMP0058)
    cmake_policy(SET CMP0058 NEW)
endif()

project(cqasm C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library type option. Default is a static library.
option(BUILD_SHARED_LIBS
    "whether the cqasm library should be built as a shared object or as a static library"
    OFF
)

# Whether tests should be built.
option(LIBQASM_BUILD_TESTS
    "whether the tests should be built and added to `make test`"
    OFF
)

# Compatibility mode. When enabled, the legacy API headers in src/library are
# added to the public headers of the cqasm target. To enable this in your CMake
# project, add `option(LIBQASM_COMPAT "" ON)` before the `add_subdirectory`
# command.
option(LIBQASM_COMPAT
    "whether the legacy API headers should be added to the cqasm library"
    OFF
)

# Whether the Python module should be built. This should only be enabled for
# setup.py's builds.
option(LIBQASM_BUILD_PYTHON
    "Whether the Python module should be built"
    OFF
)
mark_as_advanced(LIBQASM_BUILD_PYTHON)

# Where the "libQasm" Python module should be built.
set(LIBQASM_PYTHON_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/python/libQasm"
    CACHE STRING "Where to install the libQasm Python module"
)
mark_as_advanced(LIBQASM_PYTHON_DIR)

# Where the "cqasm" Python module should be built.
set(LIBQASM_CQASM_PYTHON_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/python/cqasm"
    CACHE STRING "Where to install the cqasm Python module"
)
mark_as_advanced(LIBQASM_CQASM_PYTHON_DIR)

# Used to override the (base)name of the Python extension.
set(LIBQASM_PYTHON_EXT ""
    CACHE STRING "Basename for the Python extension, or \"\" to let CMake's SWIG implementation handle it"
)
mark_as_advanced(LIBQASM_PYTHON_EXT)

# The Python library requires the compatibility headers, so make sure those are
# enabled.
if(LIBQASM_BUILD_PYTHON AND NOT LIBQASM_COMPAT)
    message(SEND_ERROR "The Python library requires the compatibility headers! Please add -DLIBQASM_COMPAT=ON")
endif()

# Jump to where the new API and the core of libqasm lives.
add_subdirectory(src)

# Add the test directory.
if(LIBQASM_BUILD_TESTS)
    enable_testing()
    include(CTest)
    include(GoogleTest)
    set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
    add_subdirectory(test)
endif()

# Compatibility mode: provide/test the original API in addition to the new one.
if(LIBQASM_COMPAT)
    # Add the compatibility headers to the include path.
    target_include_directories(cqasm-lib-obj INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/v1.0)

    # Make sure to install the headers as well.
    include(GNUInstallDirs)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/v1.0/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )
endif()

# Include the tests directory if requested.
if(LIBQASM_BUILD_PYTHON)
    add_subdirectory(python)
endif()
