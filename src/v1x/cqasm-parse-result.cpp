#include "cqasm-result-to-json.hpp"
#include "v1x/cqasm-parse-result.hpp"


namespace cqasm::v1x::parser {

std::string ParseResult::to_json() const {
    return cqasm::parser::to_json(*this);
}

} // namespace cqasm::v1x::parser
