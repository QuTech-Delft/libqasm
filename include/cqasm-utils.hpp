/** \file
 * Defines various utility functions.
 */

#pragma once

#include <string>

namespace cqasm {

/**
 * Namespace for various utility functions.
 */
namespace utils {

/**
 * Makes a string lowercase.
 */
std::string to_lowercase(const std::string &name);

/**
 * Case-insensitive string compare.
 */
bool equal_case_insensitive(const std::string &lhs, const std::string &rhs);

} // namespace utils
} // namespace cqasm
