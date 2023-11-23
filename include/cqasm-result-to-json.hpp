#pragma once

#include <fmt/format.h>
#include <sstream>  // ostringstream
#include <string>


/**
 * Namespace for the parser functions and classes.
 */
namespace cqasm::parser {

/**
 * Returns a string with a JSON representation of a Result.
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

} // namespace cqasm::parser
