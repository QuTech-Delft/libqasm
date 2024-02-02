/** \file
 * Defines various utility functions.
 */

#pragma once

#include <fmt/format.h>
#include <sstream>  // ostringstream
#include <string>

/**
 * Namespace for various utility functions.
 */
namespace cqasm::utils {

/**
 * Makes a string lowercase.
 */
std::string to_lowercase(const std::string &name);

/**
 * Case-insensitive string compare.
 */
bool equal_case_insensitive(const std::string &lhs, const std::string &rhs);

} // namespace cqasm::utils
