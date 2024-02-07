#include "cqasm-annotations.hpp"  // SourceLocation

#include <fmt/format.h>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace cqasm::annotations;


TEST(source_location_constructor, no_file_name__no_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{ "", 0, 0, 0, 0 };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>");
    EXPECT_FALSE(location.file_name_known());
}
TEST(source_location_constructor, no_file_name__yes_line_numbers__no_column_numbers) {
    auto location = SourceLocation{ "", 10, 0, 10, 0 };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10");
    EXPECT_FALSE(location.file_name_known());
}
TEST(source_location_constructor, no_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{ "", 10, 12, 10, 15 };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10:12..15");
    EXPECT_FALSE(location.file_name_known());
}
TEST(source_location_constructor, yes_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{ "input.cq", 10, 12, 10, 15 };
    EXPECT_EQ(fmt::format("{}", location), "input.cq:10:12..15");
    EXPECT_TRUE(location.file_name_known());
}
TEST(source_location_constructor, unknown_file_name__yes_line_numbers__yes_column_numbers) {
    auto location = SourceLocation{ "<unknown file name>", 10, 12, 10, 15 };
    EXPECT_EQ(fmt::format("{}", location), "<unknown file name>:10:12..15");
    EXPECT_FALSE(location.file_name_known());
}
