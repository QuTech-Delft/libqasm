#include "cqasm-utils.hpp"

#include <gmock/gmock.h>

using namespace cqasm::utils;


TEST(lowercase, empty_string) { EXPECT_TRUE(lowercase("").empty()); }
TEST(lowercase, all_lowercase) { EXPECT_EQ(lowercase("abcd"), "abcd"); }
TEST(lowercase, all_upercase) { EXPECT_EQ(lowercase("ABCD"), "abcd"); }
TEST(lowercase, contains_lowercase_and_uppercase) { EXPECT_EQ(lowercase("abCD"), "abcd"); }
TEST(lowercase, contains_numbers) { EXPECT_EQ(lowercase("abCD123"), "abcd123"); }
TEST(lowercase, contains_punctuation_signs) { EXPECT_EQ(lowercase("[abCD!]"), "[abcd!]"); }
TEST(lowercase, does_not_contain_letters) { EXPECT_EQ(lowercase("123"), "123"); }

TEST(case_insensitive_equals, empty_strings) { EXPECT_TRUE(case_insensitive_equals("", "")); }
TEST(case_insensitive_equals, both_all_lowercase) { EXPECT_TRUE(case_insensitive_equals("abcd", "abcd")); }
TEST(case_insensitive_equals, both_all_uppercase) { EXPECT_TRUE(case_insensitive_equals("ABCD", "ABCD")); }
TEST(case_insensitive_equals, both_contain_lowercase_and_uppercase) {
    EXPECT_TRUE(case_insensitive_equals("abCD", "ABcd")); }
TEST(case_insensitive_equals, both_contain_numbers) { EXPECT_TRUE(case_insensitive_equals("[abCD!]", "[ABcd!]")); }
TEST(case_insensitive_equals, non_contain_letters) { EXPECT_TRUE(case_insensitive_equals("123", "123")); }
TEST(case_insensitive_equals, different_strings) { EXPECT_FALSE(case_insensitive_equals("123", "ABC")); }
