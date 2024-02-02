#include "cqasm-utils.hpp"

#include <gtest/gtest.h>

using namespace cqasm::utils;


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
