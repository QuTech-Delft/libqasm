#pragma once

#include <gmock/gmock.h>
#include <string>

#include "libqasm/v3x/parse_helper.hpp"
#include "libqasm/v3x/parse_result.hpp"

namespace cqasm::v3x::parser {

struct MockScannerAdaptor : public ScannerAdaptor {
    MOCK_METHOD((ParseResult), parse, (), (override));
};

}  // namespace cqasm::v3x::parser
