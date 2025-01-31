#include "matcher_values.hpp"

#include <gmock/gmock.h>

#include <ostream>

#include "libqasm/v3x/semantic_generated.hpp"

namespace cqasm::v3x::values {

ValuesEqMatcher::ValuesEqMatcher(const Values& expected_value)
: expected_value_(expected_value) {}

bool ValuesEqMatcher::MatchAndExplain(const Values& args, std::ostream* /* os */) const {
    return args.equals(expected_value_);
}

void ValuesEqMatcher::DescribeTo(std::ostream* os) const {
    *os << "contains values equal to the expected";
}

void ValuesEqMatcher::DescribeNegationTo(std::ostream* os) const {
    *os << "does not contain values equal to the expected";
}

::testing::Matcher<const Values&> ValuesEq(const values::Values& expected_value) {
    return ValuesEqMatcher(expected_value);
}

}  // namespace cqasm::v3x::values
