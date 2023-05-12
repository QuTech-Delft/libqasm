include(FetchContent)
FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest.git
    GIT_TAG "ae7a13539fb71f270b87eb2e874fbac80bc8dda2"
)
FetchContent_MakeAvailable(doctest)


#=============================================================================#
# v1 tests                                                                    #
#=============================================================================#
add_subdirectory(v1)


#=============================================================================#
# v3 tests                                                                    #
#=============================================================================#

# Subdirectories
add_subdirectory(v3)


# Test sources
set(test_sources
    ${v1_test_sources}
    ${v3_test_sources}
)
set(app_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
list(APPEND app_sources ${test_sources})


# Test executable
add_executable(${PROJECT_NAME}_test ${app_sources})
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    cqasm
    gmock
    gtest
)
if(MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        /W3 /WX /w34996
        /D_CONSOLE /DCONSOLE
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        #-pedantic-errors -Werror -Wall -Wextra
        -Wl,-z,defs
        -Wno-deprecated
        # -Wconversion -Wsign-conversion -Wno-sign-conversion  # do not sort flags; order matters
    )
endif()


# Shared libraries
if(WIN32)
    # Copy DLLs the target depends on
    add_custom_command(
        TARGET ${PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}_test> $<TARGET_FILE_DIR:${PROJECT_NAME}_test>
        COMMAND_EXPAND_LISTS
    )
elseif(UNIX)
    if(APPLE)
        set(CMAKE_MACOSX_RPATH 1)
    endif()
    set_target_properties(${PROJECT_NAME}_test PROPERTIES
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
        INSTALL_RPATH ""
        INSTALL_RPATH_USE_LINK_PATH FALSE
        LINK_FLAGS "-Wl,-rpath,./"
    )
endif()


include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
