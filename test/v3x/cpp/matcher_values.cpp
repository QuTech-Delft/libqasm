#include "matcher_values.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <algorithm>  // equal
#include <gmock/gmock.h>
#include <ostream>


namespace cqasm::v3x::values {

ValuesEqMatcher::ValuesEqMatcher(const Values &expectedValue)
: expectedValue_(expectedValue)
{}

bool ValuesEqMatcher::MatchAndExplain(const Values &args, std::ostream* /* os */) const {
    const auto &expected_vec = expectedValue_.get_vec();
    const auto &actual_vec = args.get_vec();
    return std::equal(expected_vec.begin(), expected_vec.end(), actual_vec.begin(), actual_vec.end(),
        [](const auto expected, const auto actual) {
            return !expected.empty() && !actual.empty() && expected.equals(actual);
    });
}

void ValuesEqMatcher::DescribeTo(std::ostream *os) const {
    *os << "contains values equal to the expected";
}

void ValuesEqMatcher::DescribeNegationTo(std::ostream *os) const {
    *os << "does not contain values equal to the expected";
}

::testing::Matcher<const Values&> ValuesEq(const Values &expectedValue) {
    return ValuesEqMatcher(expectedValue);
}

} // namespace cqasm::v3x::values
