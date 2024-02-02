#pragma once

#include "tree-base.hpp"

#include <algorithm>  // transform
#include <fmt/format.h>
#include <string>
#include <vector>


namespace cqasm::v1x {

/**
 * Returns a vector of strings,
 * of which the first is reserved for the CBOR serialization of the v1.x syntactic or semantic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
template <typename Result>
std::vector<std::string> to_strings(const Result &result) {
    auto ret = std::vector<std::string>{};
    if (result.errors.empty()) {
        ret.push_back( { ::tree::base::serialize(result.root) } );
    } else {
        ret = result.errors;
        ret.insert(ret.begin(), "");
    }
    return ret;
}

/**
 * Returns a string with a JSON representation of a ParseResult or an AnalysisResult.
 */
template <typename Result>
std::string to_json(const Result &result) {
    if (!result.errors.empty()) {
        return fmt::format(R"({{"errors":["{}"]}})",  // first quote of first error message, and
                                                      // last quote of last error message
            fmt::join(result.errors, R"(",")"));  // last quote of any intermediate error message, and
                                                  // first quote of the following
    } else {
        std::ostringstream oss{};
        result.root->dump_json(oss);
        return oss.str();
    }
}

} // namespace cqasm::v1x
