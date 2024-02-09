#pragma once

#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-parse-result.hpp"

#include <gmock/gmock.h>
#include <string>


namespace cqasm::v3x::parser {

struct MockScannerAdaptor : public ScannerAdaptor {
    MOCK_METHOD((ParseResult), parse, (), (override));
};

} // namespace cqasm::v3x::parser
