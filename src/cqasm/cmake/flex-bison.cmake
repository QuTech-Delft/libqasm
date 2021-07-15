cmake_minimum_required(VERSION 2.8.2)

find_package(BISON ${BISON_VERSION_REQUIRED})
find_package(FLEX ${FLEX_VERSION_REQUIRED})
find_program(M4 m4)

if(NOT M4_FOUND AND (NOT BISON_FOUND OR NOT FLEX_FOUND))

    message(WARNING "m4 not found on your system. trying to build from source...")
    set(M4_VERSION_REQUIRED 1.4.6)

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/m4-download.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/m4-download/CMakeLists.txt"
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/m4-download"
    )
    if(result)
        message(FATAL_ERROR "download step for m4 ${M4_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/m4-download"
    )
    if(result)
        message(FATAL_ERROR "download step for m4 ${M4_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ./configure --prefix "${CMAKE_CURRENT_BINARY_DIR}/m4-install"
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/m4-build"
    )
    if(result)
        message(FATAL_ERROR "configure step for m4 ${M4_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make -j8
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/m4-build"
    )
    if(result)
        message(FATAL_ERROR "build step for m4 ${M4_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make install
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/m4-build"
    )
    if(result)
        message(FATAL_ERROR "install step for m4 ${M4_VERSION_REQUIRED} failed: ${result}")
    endif()

    # Add the new m4 binary in front of the system path, so the configure
    # script of flex/bison should find it.
    set(ENV{PATH} ${CMAKE_CURRENT_BINARY_DIR}/m4-install/bin:$ENV{PATH})

endif()

if(NOT BISON_FOUND)

    message(WARNING "bison ${BISON_VERSION_REQUIRED} not found on your system. trying to build from source...")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bison-download.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/bison-download/CMakeLists.txt"
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bison-download"
    )
    if(result)
        message(FATAL_ERROR "download step for bison ${BISON_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bison-download"
    )
    if(result)
        message(FATAL_ERROR "download step for bison ${BISON_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ./configure --prefix "${CMAKE_CURRENT_BINARY_DIR}/bison-install"
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bison-build"
    )
    if(result)
        message(FATAL_ERROR "configure step for bison ${BISON_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make -j8
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bison-build"
    )
    if(result)
        message(FATAL_ERROR "build step for bison ${BISON_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make install
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bison-build"
    )
    if(result)
        message(FATAL_ERROR "install step for bison ${BISON_VERSION_REQUIRED} failed: ${result}")
    endif()

    # Add the binary directory to the system environment path. Not sure if this
    # is necessary.
    set(ENV{PATH} ${CMAKE_CURRENT_BINARY_DIR}/bison-install/bin:$ENV{PATH})

    # Unset cached variables which may have been set by the previous invocation
    # of FindBISON.cmake, preventing it from searching again.
    unset(BISON_EXECUTABLE CACHE)

    # Set the path for FIND_*() so FindBISON.cmake hopefully finds the new one,
    # even if an older version of bison is already installed on the system.
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/bison-install")

    # Find again.
    find_package(
        BISON ${BISON_VERSION_REQUIRED} EXACT
        REQUIRED
    )

    # Clear the prefix path.
    unset(CMAKE_PREFIX_PATH)

endif()

if(NOT FLEX_FOUND)

    message(WARNING "flex ${FLEX_VERSION_REQUIRED} not found on your system. trying to build from source...")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/flex-download.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/flex-download/CMakeLists.txt"
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/flex-download"
    )
    if(result)
        message(FATAL_ERROR "download step for flex ${FLEX_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/flex-download"
    )
    if(result)
        message(FATAL_ERROR "download step for flex ${FLEX_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND ./configure --prefix "${CMAKE_CURRENT_BINARY_DIR}/flex-install"
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/flex-build"
    )
    if(result)
        message(FATAL_ERROR "configure step for flex ${FLEX_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make -j8
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/flex-build"
    )
    if(result)
        message(FATAL_ERROR "build step for flex ${FLEX_VERSION_REQUIRED} failed: ${result}")
    endif()

    execute_process(
        COMMAND make install
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/flex-build"
    )
    if(result)
        message(FATAL_ERROR "install step for flex ${FLEX_VERSION_REQUIRED} failed: ${result}")
    endif()

    # Add the binary directory to the system environment path. Not sure if this
    # is necessary.
    set(ENV{PATH} ${CMAKE_CURRENT_BINARY_DIR}/flex-install/bin:$ENV{PATH})

    # Unset cached variables which may have been set by the previous invocation
    # of FindFLEX.cmake, preventing it from searching again.
    unset(FLEX_EXECUTABLE CACHE)
    unset(FL_LIBRARY CACHE)
    unset(FLEX_INCLUDE_DIR CACHE)

    # Set the path for FIND_*() so FindFLEX.cmake hopefully finds the new one,
    # even if an older version of bison is already installed on the system.
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/flex-install")

    # Find again.
    find_package(
        FLEX ${FLEX_VERSION_REQUIRED} EXACT
        REQUIRED
    )

    # Clear the prefix path.
    unset(CMAKE_PREFIX_PATH)

endif()
