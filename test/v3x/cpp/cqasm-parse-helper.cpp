#include "v3x/cqasm-parse-helper.hpp"

#include <fmt/format.h>
#include <gmock/gmock.h>

#include <cstdint>  // uint32_t
#include <memory>  // make_unique, unique_ptr
#include <stdexcept>  // runtime_error
#include <string>

#include "cqasm-error.hpp"
#include "cqasm-tree.hpp"
#include "cqasm-version.hpp"
#include "mock_scanner_adaptor.hpp"
#include "v3x/cqasm-ast.hpp"
#include "v3x/cqasm-parse-result.hpp"

using namespace ::testing;
using namespace cqasm::error;
using namespace cqasm::version;

namespace cqasm::v3x::parser {

class ParseHelperParseTest : public ::testing::Test {
protected:
    void SetUp() override { scanner_up = std::make_unique<MockScannerAdaptor>(); }
    void ExpectScannerParseThrowsParseError() {
        EXPECT_CALL(*scanner_up, parse())
            .WillRepeatedly(::testing::Throw(ParseError{ parse_error_message, file_name, range }));
    }
    void ExpectScannerParseThrowsRuntimeError() {
        EXPECT_CALL(*scanner_up, parse())
            .WillRepeatedly(::testing::Throw(std::runtime_error{ runtime_error_message.c_str() }));
    }
    void ExpectScannerParseReturnsIllFormedRoot() {
        auto parse_result = ParseResult{ tree::make<ast::Program>(), error::ParseErrors{} };
        EXPECT_CALL(*scanner_up, parse()).WillOnce(::testing::Return(parse_result));
    }
    void ExpectScannerParseReturnsWellFormedRoot() {
        auto one_version = tree::make<ast::Version>(version_3_0);
        auto one_global_block = tree::make<ast::GlobalBlock>();
        auto one_program = tree::make<ast::Program>(one_version, one_global_block);
        auto parse_result = ParseResult{ one_program, error::ParseErrors{} };
        EXPECT_CALL(*scanner_up, parse()).WillOnce(::testing::Return(parse_result));
    }

    std::string file_name = "input.cq";
    annotations::SourceLocation::Range range{ { 10, 12 }, { 10, 15 } };

    std::string parse_error_message = "parse error";
    std::string runtime_error_message = "runtime error";
    std::string ill_formed_root_message = "no parse errors returned, but AST is incomplete. AST was dumped.";

    version::Version version_3_0 = "3.0";

    std::unique_ptr<MockScannerAdaptor> scanner_up;
};

TEST_F(ParseHelperParseTest, scanner_parse_throws_parse_error) {
    ExpectScannerParseThrowsParseError();
    auto parse_helper = ParseHelper{ std::move(scanner_up), file_name };
    auto parse_result = parse_helper.parse();
    const auto &errors = parse_result.errors;
    EXPECT_TRUE(errors.size() == 1);
    EXPECT_EQ(fmt::format("{}", errors[0]),
        fmt::format("Error at {}:{}:{}..{}: {}",
            file_name,
            range.first.line,
            range.first.column,
            range.last.column,
            parse_error_message));
}
TEST_F(ParseHelperParseTest, scanner_parse_throws_runtime_error) {
    ExpectScannerParseThrowsRuntimeError();
    auto parse_helper = ParseHelper{ std::move(scanner_up), file_name };
    auto parse_result = parse_helper.parse();
    const auto &errors = parse_result.errors;
    EXPECT_TRUE(errors.size() == 1);
    EXPECT_EQ(fmt::format("{}", errors[0]), fmt::format("Error: {}", runtime_error_message));
}
TEST_F(ParseHelperParseTest, parse_result_errors_is_empty_and_root_is_ill_formed) {
    ExpectScannerParseReturnsIllFormedRoot();
    auto parse_helper = ParseHelper{ std::move(scanner_up), file_name };
    EXPECT_THAT([&]() { parse_helper.parse(); },
        ThrowsMessage<error::ParseError>(::testing::HasSubstr(ill_formed_root_message)));
}
TEST_F(ParseHelperParseTest, parse_result_errors_is_empty_and_root_is_well_formed) {
    ExpectScannerParseReturnsWellFormedRoot();
    auto parse_helper = ParseHelper{ std::move(scanner_up), file_name };
    auto parse_result = parse_helper.parse();
    auto program = parse_result.root->as_program();
    const auto &version = program->version->items;
    EXPECT_EQ(version, version_3_0);
}

}  // namespace cqasm::v3x::parser
