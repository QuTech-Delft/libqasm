/** \file
 * Implementation for \ref include/cqasm-utils.hpp "cqasm-utils.hpp".
 */

#include "cqasm-utils.hpp"

#include <cctype>  // tolower, toupper
#include <range/v3/algorithm/equal.hpp>
#include <range/v3/range/conversion.hpp>  // to
#include <range/v3/view/transform.hpp>


namespace cqasm::utils {

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

} // namespace cqasm::utils
