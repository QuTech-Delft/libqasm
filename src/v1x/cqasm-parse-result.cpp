#include "v1x/cqasm-parse-result.hpp"
#include "v1x/cqasm-result.hpp"

#include <fmt/format.h>


namespace cqasm::v1x::parser {

/**
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v1.x syntactic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> ParseResult::to_strings() const {
    return cqasm::v1x::to_strings(*this);
}

std::string ParseResult::to_json() const {
    return cqasm::v1x::to_json(*this);
}

} // namespace cqasm::v1x::parser
