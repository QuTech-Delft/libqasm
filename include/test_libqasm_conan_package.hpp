#pragma once

#ifdef _WIN32
#define libqasm_EXPORT __declspec(dllexport)
#else
#define libqasm_EXPORT
#endif

libqasm_EXPORT void test_libqasm_conan_package();
