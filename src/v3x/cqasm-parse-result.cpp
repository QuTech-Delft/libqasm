#include "cqasm-utils.hpp"
#include "v3x/cqasm-parse-result.hpp"


namespace cqasm::v3x::parser {

std::string ParseResult::to_json() const {
    return cqasm::utils::to_json(*this);
}

} // namespace cqasm::v3x::parser
