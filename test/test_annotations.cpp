#include <fmt/format.h>
#include <gmock/gmock.h>

#include <optional>

#include "libqasm/annotations.hpp"  // SourceLocation

using namespace ::testing;
using namespace cqasm::annotations;

TEST(source_location_constructor, null_file_name__no_line_numbers__no_column_numbers) {
    auto location = SourceLocation{ std::nullopt, {} };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>");
}
TEST(source_location_constructor, no_file_name__no_line_numbers__no_column_numbers) {
    auto location = SourceLocation{ "", {} };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>");
}
TEST(source_location_constructor, null_file_name__yes_line_numbers__no_column_numbers) {
    auto location = SourceLocation{
        std::nullopt, { { 10, 0 }, { 10, 0 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10");
}
TEST(source_location_constructor, no_file_name__yes_line_numbers__no_column_numbers) {
    auto location = SourceLocation{
        "", { { 10, 0 }, { 10, 0 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10");
}
TEST(source_location_constructor, null_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{
        std::nullopt, { { 10, 12 }, { 10, 15 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10:12..15");
}
TEST(source_location_constructor, no_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{
        "", { { 10, 12 }, { 10, 15 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10:12..15");
}
TEST(source_location_constructor, yes_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{
        "input.cq", { { 10, 12 }, { 10, 15 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "input.cq:10:12..15");
}
TEST(source_location_constructor, unknown_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{
        "<unknown file name>", { { 10, 12 }, { 10, 15 } }
    };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10:12..15");
}
