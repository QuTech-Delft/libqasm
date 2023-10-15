find_package(GTest REQUIRED)

# Test executable
add_executable(${PROJECT_NAME}_test)

# Subdirectories
if(LIBQASM_COMPAT)
    add_subdirectory(v10)
endif()
add_subdirectory(v1x)
add_subdirectory(v3x)

target_sources(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cqasm-version.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
)

target_include_directories(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(${PROJECT_NAME}_test PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE
    cqasm
    GTest::gmock
    GTest::gtest
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=deprecated-declarations
        -Wno-error=restrict
        -Wno-error=sign-compare
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -fPIC
        -Wno-error=sign-compare
        -Wno-error=unused-private-field
        -Wno-error=unused-but-set-variable
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        /W3 /WX /w34996
        /D_CONSOLE /DCONSOLE
        /D_CRT_NONSTDC_NO_DEPRECATE
        /D_CRT_SECURE_NO_WARNINGS
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

gtest_discover_tests(${PROJECT_NAME}_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
