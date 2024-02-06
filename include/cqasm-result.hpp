#pragma once

#include "tree-base.hpp"

#include <algorithm>  // transform
#include <fmt/format.h>
#include <numeric>  // accumulate
#include <string>
#include <vector>


namespace cqasm::result {

/**
 * Returns a vector of strings,
 * of which the first is reserved for the CBOR serialization of the syntactic or semantic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
template <typename Result>
std::vector<std::string> to_strings(const Result &result) {
    auto ret = std::vector<std::string>(1);
    if (result.errors.empty()) {
        ret[0] = ::tree::base::serialize(result.root);
    } else {
        ret.resize(result.errors.size() + 1);
        std::transform(result.errors.begin(), result.errors.end(), std::next(ret.begin()),
            [](const auto &error) { return error.what(); });
    }
    return ret;
}

template <typename Errors>
std::string errors_to_json(const Errors &errors) {
    return fmt::format(R"({{"errors":[{}]}})",
        std::accumulate(errors.begin(), errors.end(), std::string{},
            [first=true](auto total, const auto &error) mutable {
                total += (first ? "" : ",");
                total += error.to_json();
                first = false;
                return total;
        })
    );
}

template <typename Root>
std::string root_to_json(const Root &root) {
    std::ostringstream oss{};
    root->dump_json(oss);
    return oss.str();
}

/**
 * Returns a string with a JSON representation of a ParseResult or an AnalysisResult.
 */
template <typename Result>
std::string to_json(const Result &result) {
    return (result.errors.empty())
        ? root_to_json(result.root)
        : errors_to_json(result.errors);
}

} // namespace cqasm::result
