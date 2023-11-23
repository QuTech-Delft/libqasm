#include "cqasm-result-to-json.hpp"
#include "v3x/cqasm-parse-result.hpp"


namespace cqasm::v3x::parser {

std::string ParseResult::to_json() const {
    return cqasm::parser::to_json(*this);
}

} // namespace cqasm::v3x::parser
