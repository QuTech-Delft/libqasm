# Packages
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "60c3602aaa3f27e1dbd691549b1d8da88c14c947"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Subdirectories
if(LIBQASM_COMPAT)
    add_subdirectory(v1.0)
endif()
add_subdirectory(v1)
add_subdirectory(v3)

# Test sources
set(test_sources
    ${v1_0_test_sources}
    ${v1_test_sources}
    ${v3_test_sources}
)
set(app_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
list(APPEND app_sources ${test_sources})

# Test executable
add_executable(${PROJECT_NAME}_test ${app_sources})
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    cqasm
    gmock
    gtest
)
if(MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        /W3 /WX /w34996
        /D_CONSOLE /DCONSOLE
        /D_CRT_NONSTDC_NO_DEPRECATE
        /D_CRT_SECURE_NO_WARNINGS
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -pedantic-errors -Werror -Wall -Wextra
        -Wl,-z,defs
        -Wno-deprecated
        -Wconversion -Wsign-conversion -Wno-sign-conversion  # do not sort flags; order matters
    )
endif()

gtest_discover_tests(${PROJECT_NAME}_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
