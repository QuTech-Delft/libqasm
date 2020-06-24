#include <algorithm>
#include <cctype>
#include "cqasm-utils.hpp"

namespace cqasm {
namespace utils {

/**
* Makes a string lowercase.
*/
std::string lowercase(const std::string &name) {
    std::string name_lower = name;
    std::for_each(name_lower.begin(), name_lower.end(), [](char &c){
        c = std::tolower(c);
    });
    return name_lower;
}

/**
 * Case-insensitive string compare.
 */
bool case_insensitive_equals(const std::string &lhs, const std::string &rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); i++) {
        if (std::tolower(lhs[i]) != std::tolower(rhs[i])) return false;
    }
    return true;
}

} // namespace utils
} // namespace cqasm
