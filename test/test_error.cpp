#include <fmt/format.h>
#include <gmock/gmock.h>
#include <memory>  // make_shared

#include "libqasm/annotations.hpp"  // SourceLocation
#include "libqasm/error.hpp"
#include "libqasm/tree.hpp"  // Annotatable

using namespace ::testing;
using namespace cqasm::error;
using namespace cqasm::annotations;
using namespace tree::annotatable;

class FakeNode : public Annotatable {};

TEST(constructor_message_node, empty_message) {
    auto err = Error{ "" };
    EXPECT_EQ(fmt::format("{}", err), "Error: <unknown error message>");
}
TEST(constructor_message_node, message_and_null_node) {
    auto err = Error{ "syntax error" };
    EXPECT_EQ(fmt::format("{}", err), "Error: syntax error");
}
TEST(constructor_message_node, message_and_node_without_location) {
    auto node = FakeNode{};
    auto err = Error{ "syntax error", &node };
    EXPECT_EQ(fmt::format("{}", err), "Error: syntax error");
}
TEST(constructor_message_node, message_and_node_with_empty_location) {
    auto node = FakeNode{};
    node.set_annotation(SourceLocation{});
    auto err = Error{ "syntax error", &node };
    EXPECT_EQ(fmt::format("{}", err), "Error at <unknown file name>: syntax error");
}
TEST(constructor_message_node, message_and_node_with_location) {
    auto node = FakeNode{};
    node.set_annotation(SourceLocation{ "input.cq", { { 10, 12 }, { 10, 15 } } });
    auto err = Error{ "syntax error", &node };
    EXPECT_EQ(fmt::format("{}", err), "Error at input.cq:10:12..15: syntax error");
}

TEST(constructor_message_location, empty_message) {
    auto err = Error{ "", std::make_shared<SourceLocation>() };
    EXPECT_EQ(fmt::format("{}", err), "Error at <unknown file name>: <unknown error message>");
}
TEST(constructor_message_location, message_and_empty_location) {
    auto err = Error{ "syntax error", std::make_shared<SourceLocation>() };
    EXPECT_EQ(fmt::format("{}", err), "Error at <unknown file name>: syntax error");
}
TEST(constructor_message_location, message_and_location) {
    auto err = Error{ "syntax error",
        std::make_shared<SourceLocation>("input.cq", SourceLocation::Range{ { 10, 12 }, { 10, 15 } }) };
    EXPECT_EQ(fmt::format("{}", err), "Error at input.cq:10:12..15: syntax error");
}

TEST(constructor_message_location_fields, empty_message) {
    auto err = Error{ "", std::nullopt, SourceLocation::Range{} };
    EXPECT_EQ(fmt::format("{}", err), "Error at <unknown file name>: <unknown error message>");
}
TEST(constructor_message_location_fields, message_and_empty_location) {
    auto err = Error{ "syntax error", std::nullopt, SourceLocation::Range{} };
    EXPECT_EQ(fmt::format("{}", err), "Error at <unknown file name>: syntax error");
}
TEST(constructor_message_location_fields, message_and_location) {
    auto err = Error{ "syntax error", "input.cq", SourceLocation::Range{ { 10, 12 }, { 10, 15 } } };
    EXPECT_EQ(fmt::format("{}", err), "Error at input.cq:10:12..15: syntax error");
}

TEST(context, location) {
    auto node_1 = FakeNode{};
    node_1.set_annotation(SourceLocation("input.cq", SourceLocation::Range{ { 10, 12 }, { 10, 15 } }));
    auto err = Error{ "syntax error", &node_1 };
    auto node_2 = FakeNode{};
    node_2.set_annotation(SourceLocation("input.cq", SourceLocation::Range{ { 20, 22 }, { 20, 25 } }));
    err.context(node_2);
    EXPECT_EQ(fmt::format("{}", err), "Error at input.cq:10:12..15: syntax error");
}
TEST(context, no_location_and_node_does_not_have_source_location) {
    auto err = Error{ "syntax error" };
    auto node_2 = FakeNode{};
    err.context(node_2);
    EXPECT_EQ(fmt::format("{}", err), "Error: syntax error");
}
TEST(context, no_location_and_node_has_source_location) {
    auto err = Error{ "syntax error" };
    auto node_2 = FakeNode{};
    node_2.set_annotation(SourceLocation("input.cq", SourceLocation::Range{ { 20, 22 }, { 20, 25 } }));
    err.context(node_2);
    EXPECT_EQ(fmt::format("{}", err), "Error at input.cq:20:22..25: syntax error");
}

TEST(what, empty_message) {
    auto err = Error{ "" };
    EXPECT_EQ(std::string{ err.what() }, "Error: <unknown error message>");
}
TEST(what, message_and_null_location) {
    auto err = Error{ "syntax error" };
    EXPECT_EQ(std::string{ err.what() }, "Error: syntax error");
}
TEST(what, message_and_empty_location) {
    auto err = Error{ "syntax error", std::make_shared<SourceLocation>() };
    EXPECT_EQ(std::string{ err.what() }, "Error at <unknown file name>: syntax error");
}
TEST(what, message_and_location_with_unknown_file_name) {
    auto err = Error{ "syntax error", std::nullopt, SourceLocation::Range{ { 10, 12 }, { 10, 15 } } };
    EXPECT_EQ(std::string{ err.what() }, "Error at <unknown file name>:10:12..15: syntax error");
}
TEST(what, message_and_location_with_known_file_name) {
    auto err = Error{ "syntax error", "input.cq", SourceLocation::Range{ { 10, 12 }, { 10, 15 } } };
    EXPECT_EQ(std::string{ err.what() }, "Error at input.cq:10:12..15: syntax error");
}

TEST(to_json, empty_message) {
    auto err = Error{ "" };
    EXPECT_EQ(Error{ "" }.to_json(),
        R"({)"
        R"("range":{)"
        R"("start":{"line":0,"character":0})"
        R"(,"end":{"line":0,"character":0})"
        R"(})"
        R"(,"message":"<unknown error message>")"
        R"(,"severity":1)"
        R"(})");
}
TEST(to_json, message_and_null_location) {
    auto err = Error{ "syntax error" };
    EXPECT_EQ(err.to_json(),
        R"({)"
        R"("range":{)"
        R"("start":{"line":0,"character":0})"
        R"(,"end":{"line":0,"character":0})"
        R"(})"
        R"(,"message":"syntax error")"
        R"(,"severity":1)"
        R"(})");
}
TEST(to_json, message_and_empty_location) {
    auto err = Error{ "syntax error" };
    EXPECT_EQ(err.to_json(),
        R"({)"
        R"("range":{)"
        R"("start":{"line":0,"character":0})"
        R"(,"end":{"line":0,"character":0})"
        R"(})"
        R"(,"message":"syntax error")"
        R"(,"severity":1)"
        R"(})");
}
TEST(to_json, message_and_location_with_unknown_file_name) {
    auto err = Error{ "syntax error", std::nullopt, SourceLocation::Range{ { 10, 12 }, { 10, 15 } } };
    EXPECT_EQ(err.to_json(),
        R"({)"
        R"("range":{)"
        R"("start":{"line":10,"character":12})"
        R"(,"end":{"line":10,"character":15})"
        R"(})"
        R"(,"message":"syntax error")"
        R"(,"severity":1)"
        R"(})");
}
TEST(to_json, message_and_location_with_known_file_name) {
    auto err = Error{ "syntax error", "input.cq", SourceLocation::Range{ { 10, 12 }, { 10, 15 } } };
    EXPECT_EQ(err.to_json(),
        R"({)"
        R"("range":{)"
        R"("start":{"line":10,"character":12})"
        R"(,"end":{"line":10,"character":15})"
        R"(})"
        R"(,"message":"syntax error")"
        R"(,"severity":1)"
        R"(,"relatedInformation":[{)"
        R"("location":{)"
        R"("uri":"file:///input.cq")"
        R"(,"range":{)"
        R"("start":{"line":0,"character":0})"
        R"(,"end":{"line":0,"character":0})"
        R"(})"
        R"(})"
        R"(,"message":"<unknown error message>")"
        R"(}])"
        R"(})");
}
