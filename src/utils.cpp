/** \file
 * Implementation for \ref include/libqasm/utils.hpp "libqasm/utils.hpp".
 */

#include "libqasm/utils.hpp"

#include <fmt/format.h>

#include <algorithm>  // transform
#include <cctype>  // isalnum, tolower, toupper
#include <range/v3/algorithm/equal.hpp>
#include <range/v3/range/conversion.hpp>  // to
#include <range/v3/view/transform.hpp>

namespace cqasm::utils {

// clang-format off

/**
* Makes a string lowercase.
*/
std::string to_lowercase(const std::string &name) {
    return name
        | ranges::views::transform([](unsigned char c) { return std::tolower(c); })
        | ranges::to<std::string>();
}

/**
 * Case-insensitive string compare.
 */
bool equal_case_insensitive(const std::string &lhs, const std::string &rhs) {
    return ranges::equal(lhs, rhs, [](unsigned char l, unsigned char r) {
        return std::tolower(l) == std::tolower(r);
    });
}

/**
 * Encodes a string in URL format.
 */
std::string url_encode(const std::string &str) {
    auto ret = std::string{};
    std::for_each(str.begin(), str.end(), [&ret](unsigned char c) {
        ret += (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            ? std::string{ static_cast<char>(c) }
            : fmt::format("%{:02X}", static_cast<unsigned int>(c));
    });
    return ret;
}

/**
 * Encodes a string in JSON format.
 */
std::string json_encode(const std::string &str) {
    auto ret = std::string{};
    std::for_each(str.begin(), str.end(), [&ret](char c) {
        ret += (c == '"' || c == '\\' || c <= '\x1f')
            ? fmt::format("\\u{:04X}", static_cast<int>(c))
            : std::string{ c };
    });
    return ret;
}

// clang-format on

}  // namespace cqasm::utils
