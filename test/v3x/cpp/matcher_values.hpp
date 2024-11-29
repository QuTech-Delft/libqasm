#pragma once

#include <gmock/gmock.h>

#include <iosfwd>  //ostream

#include "libqasm/v3x/values.hpp"

namespace cqasm::v3x::values {

class ValuesEqMatcher {
public:
    using is_gtest_matcher = void;

    ValuesEqMatcher(const values::Values& expected_value);
    bool MatchAndExplain(const values::Values& args, std::ostream* os) const;
    void DescribeTo(std::ostream* os) const;
    void DescribeNegationTo(std::ostream* os) const;

private:
    const values::Values& expected_value_;
};

::testing::Matcher<const values::Values&> ValuesEq(const values::Values& expected_value);

}  // namespace cqasm::v3x::values
