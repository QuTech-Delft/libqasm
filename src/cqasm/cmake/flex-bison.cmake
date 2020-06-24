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

    set(ENV{PATH} $ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/m4-install/bin)

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/m4-install")

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

    set(ENV{PATH} $ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/bison-install/bin)

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/bison-install")
    find_package(
        BISON ${BISON_VERSION_REQUIRED} EXACT
        REQUIRED
    )

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

    set(ENV{PATH} $ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/flex-install/bin)

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/flex-install")
    find_package(
        FLEX ${FLEX_VERSION_REQUIRED} EXACT
        REQUIRED
    )

endif()
