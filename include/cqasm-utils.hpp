/** \file
 * Defines various utility functions.
 */

#pragma once

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

/**
 * Encodes a string in URL format.
 */
std::string url_encode(const std::string &str);

/**
 * Encodes a string in JSON format.
 */
std::string json_encode(const std::string &str);

}  // namespace cqasm::utils
