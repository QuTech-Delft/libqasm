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

/**
 * Returns a string with a JSON representation of a ParseResult or an AnalysisResult.
 */
template <typename Result>
std::string to_json(const Result &result) {
    if (!result.errors.empty()) {
        return fmt::format(R"({{"errors":["{}"]}})",  // first quote of first error message, and
                                                      // last quote of last error message
            fmt::join(result.errors, R"("")"));  // last quote of any intermediate error message, and
                                                 // first quote of the following
    } else {
        std::ostringstream oss{};
        result.root->dump_json(oss);
        return oss.str();
    }
}

} // namespace cqasm::utils
