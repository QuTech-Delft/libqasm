#include "cqasm-result.hpp"
#include "v3x/cqasm-parse-result.hpp"


namespace cqasm::v3x::parser {

/**
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x syntactic AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> ParseResult::to_strings() const {
    return cqasm::result::to_strings(*this);
}

/**
 * Returns a string with a JSON representation of a ParseResult.
 */
std::string ParseResult::to_json() const {
    return cqasm::result::to_json(*this);
}

} // namespace cqasm::v3x::parser
