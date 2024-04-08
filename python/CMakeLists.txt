cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(POLICY CMP0078)
    cmake_policy(SET CMP0078 OLD)
endif()

if(POLICY CMP0086)
    cmake_policy(SET CMP0086 OLD)
endif()

#=============================================================================#
# Configure, build, and link dependencies                                     #
#=============================================================================#

# Python development headers --------------------------------------------------

# CMake's Python-finding logic is dodgy at best at dealing with venvs, and
# venvs themselves are dodgy at best at dealing with the development headers and so on.
# "If you want something done, do it yourself," I guess.

# If there is no override, look for the Python executable first.
# If there's a virtualenv, look in it first and foremost.
# PYTHON_EXECUTABLE can also be overridden with -DPYTHON_EXECUTABLE= outright.
if(DEFINED ENV{VIRTUAL_ENV})
    find_program(PYTHON_EXECUTABLE python3 HINTS "$ENV{VIRTUAL_ENV}/bin")
else()
    find_program(PYTHON_EXECUTABLE python3)
endif()

# If we found it, we can ask it where its include directory is, using the sysconfig module.
if(PYTHON_EXECUTABLE)
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_paths()[\"include\"])"
        OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # On Windows, we need to explicitly link against the Python library as well.
    # Unfortunately we can't ask Python directly where this is, but we can figure it out
    if(WIN32)
        execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/python_lib.py" "${CMAKE_BUILD_TYPE}"
            OUTPUT_VARIABLE PYTHON_LIBRARIES
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
endif()

# If we failed to find the headers ourselves, fall back to CMake's internal logic.
if(NOT PYTHON_INCLUDE_DIRS)
    message(WARNING "Could NOT find python3 include directory with custom logic, falling back to CMake internal")
    unset(PYTHON_INCLUDE_DIRS)
    if("${CMAKE_VERSION}" VERSION_LESS "3.12.0")
        find_package(PythonInterp 3 REQUIRED)
        if(${PYTHONINTERP_FOUND})
            find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
        endif()
    else()
        find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
        set(PYTHON_INCLUDE_DIRS "${Python3_INCLUDE_DIRS}")
        set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}")
    endif()
endif()

# Print the directory we found for reference.
message(STATUS "Found python3 include directory at ${PYTHON_INCLUDE_DIRS}")


# SWIG ------------------------------------------------------------------------

# Look for SWIG.
find_package(SWIG REQUIRED)

include(${SWIG_USE_FILE})


#=============================================================================#
# CMake configuration                                                         #
#=============================================================================#

# Windows weirdness: need a .lib file to link against a DLL at compile-time (I think),
# but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Require C++20 compiler support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#=============================================================================#
# Build the SWIG module                                                       #
#=============================================================================#

# Configure SWIG.
set(SWIG_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/libQasm.i"
)

set_source_files_properties("${SWIG_FILE}" PROPERTIES
    CPLUSPLUS ON
    INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>;${PYTHON_INCLUDE_DIRS}"
    SWIG_FLAGS "-castmode"
)

set(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")

# Run SWIG to generated the C++ source file and Python wrapper module.
swig_add_library(
    libQasm
    TYPE MODULE
    LANGUAGE python
    SOURCES "${SWIG_FILE}"
)

# Compile the C++ source file and link it against the cqasm library.
if(WIN32)
    swig_link_libraries(libQasm "${PYTHON_LIBRARIES}" cqasm)
else()
    swig_link_libraries(libQasm cqasm)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(_libQasm PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()
target_include_directories(_libQasm PRIVATE "${PYTHON_INCLUDE_DIRS}")


#=============================================================================#
# Construct & install the complete Python module                              #
#=============================================================================#

# Install the shared object (*.pyd on Windows).
if("${LIBQASM_PYTHON_EXT}" STREQUAL "")
    install(
        TARGETS _libQasm
        LIBRARY DESTINATION "${LIBQASM_PYTHON_DIR}"
    )
else()
    install(
        FILES "$<TARGET_FILE:_libQasm>"
        DESTINATION "${LIBQASM_PYTHON_DIR}"
        RENAME "${LIBQASM_PYTHON_EXT}"
    )
endif()

# Install the generated files.
install(
    FILES "${CMAKE_SWIG_OUTDIR}/libQasm.py"
    DESTINATION "${LIBQASM_PYTHON_DIR}"
)
install(
    FILES "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/cqasm-ast-gen.py"
    DESTINATION "${LIBQASM_CQASM_PYTHON_DIR}/v3x"
    RENAME "ast.py"
)
install(
    FILES "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/cqasm-semantic-gen.py"
    DESTINATION "${LIBQASM_CQASM_PYTHON_DIR}/v3x"
    RENAME "semantic.py"
)
install(
    FILES "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/cqasm-types-gen.py"
    DESTINATION "${LIBQASM_CQASM_PYTHON_DIR}/v3x"
    RENAME "types.py"
)
install(
    FILES "${LIBQASM_GENERATED_PYTHON_FILES}/v3x/cqasm-values-gen.py"
    DESTINATION "${LIBQASM_CQASM_PYTHON_DIR}/v3x"
    RENAME "values.py"
)
