#include "libqasm/utils.hpp"

#include <gtest/gtest.h>

using namespace cqasm::utils;

// clang-format off
TEST(to_lowercase, empty_string) { EXPECT_TRUE(to_lowercase("").empty()); }
TEST(to_lowercase, all_lowercase) { EXPECT_EQ(to_lowercase("abcd"), "abcd"); }
TEST(to_lowercase, all_upercase) { EXPECT_EQ(to_lowercase("ABCD"), "abcd"); }
TEST(to_lowercase, contains_lowercase_and_uppercase) { EXPECT_EQ(to_lowercase("abCD"), "abcd"); }
TEST(to_lowercase, contains_numbers) { EXPECT_EQ(to_lowercase("abCD123"), "abcd123"); }
TEST(to_lowercase, contains_punctuation_signs) { EXPECT_EQ(to_lowercase("[abCD!]"), "[abcd!]"); }
TEST(to_lowercase, does_not_contain_letters) { EXPECT_EQ(to_lowercase("123"), "123"); }


TEST(equal_case_insensitive, empty_strings) { EXPECT_TRUE(equal_case_insensitive("", "")); }
TEST(equal_case_insensitive, both_all_lowercase) { EXPECT_TRUE(equal_case_insensitive("abcd", "abcd")); }
TEST(equal_case_insensitive, both_all_uppercase) { EXPECT_TRUE(equal_case_insensitive("ABCD", "ABCD")); }
TEST(equal_case_insensitive, both_contain_lowercase_and_uppercase) {
    EXPECT_TRUE(equal_case_insensitive("abCD", "ABcd")); }
TEST(equal_case_insensitive, both_contain_numbers) { EXPECT_TRUE(equal_case_insensitive("[abCD!]", "[ABcd!]")); }
TEST(equal_case_insensitive, non_contain_letters) { EXPECT_TRUE(equal_case_insensitive("123", "123")); }
TEST(equal_case_insensitive, different_strings) { EXPECT_FALSE(equal_case_insensitive("123", "ABC")); }


TEST(url_encode, empty_string) { EXPECT_TRUE(url_encode("").empty()); }
TEST(url_encode, random_string) { EXPECT_EQ(url_encode("a0 ^"), "a0%20%5E"); }
TEST(url_encode, windows_file_path_with_spaces) { EXPECT_EQ(url_encode("C:\\my file.txt"), "C%3A%5Cmy%20file.txt"); }


TEST(json_encode, empty_string) { EXPECT_TRUE(json_encode("").empty()); }
TEST(json_encode, random_string) { EXPECT_EQ(json_encode("a0 \"\\"), "a0 \\u0022\\u005C"); }
TEST(json_encode, windows_file_path_with_spaces) {
    EXPECT_EQ(json_encode("C:\\my file.txt"), "C:\\u005Cmy file.txt");
}
TEST(json_encode, error) {
    EXPECT_EQ(
        json_encode("failed to parse 'res/v1x/parsing/grammar/expression_recovery/input.cq'"),
        "failed to parse 'res/v1x/parsing/grammar/expression_recovery/input.cq'");
}
// clang-format on
