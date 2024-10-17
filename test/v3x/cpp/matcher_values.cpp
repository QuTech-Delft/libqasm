#include "matcher_values.hpp"

#include <gmock/gmock.h>

#include <ostream>

#include "libqasm/v3x/semantic_generated.hpp"

namespace cqasm::v3x::values {

ValuesEqMatcher::ValuesEqMatcher(const Values& expectedValue)
: expectedValue_(expectedValue) {}

bool ValuesEqMatcher::MatchAndExplain(const Values& args, std::ostream* /* os */) const {
    return args.equals(expectedValue_);
}

void ValuesEqMatcher::DescribeTo(std::ostream* os) const {
    *os << "contains values equal to the expected";
}

void ValuesEqMatcher::DescribeNegationTo(std::ostream* os) const {
    *os << "does not contain values equal to the expected";
}

::testing::Matcher<const Values&> ValuesEq(const Values& expectedValue) {
    return ValuesEqMatcher(expectedValue);
}

}  // namespace cqasm::v3x::values
