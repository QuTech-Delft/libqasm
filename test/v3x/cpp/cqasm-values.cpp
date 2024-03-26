#include "v3x/cqasm-semantic-gen.hpp"
#include "v3x/cqasm-values.hpp"

#include <gtest/gtest.h>


namespace cqasm::v3x::values {

// bool, int, float, axis types
static const types::Type bool_type = tree::make<types::Bool>(1);
static const types::Type int_type = tree::make<types::Int>(1);
static const types::Type float_type = tree::make<types::Float>(1);

// bool, int, float, axis values
static const values::Value bool_true_value = tree::make<values::ConstBool>(true);
static const values::Value int_1_value = tree::make<values::ConstInt>(1);
static const values::Value float_1_0_value = tree::make<values::ConstFloat>(1.0);

// bool semantic variable
static const auto bool_variable = tree::make<semantic::Variable>("b", bool_type);

// bool variable ref value
static const values::Value bool_variable_ref = tree::make<values::VariableRef>(bool_variable);

// empty parameters
static const auto empty_parameter_list = tree::Any<types::TypeBase>{};

TEST(promote, bool_to_bool) { EXPECT_TRUE(promote(bool_true_value, bool_type)->equals(*bool_true_value)); }
TEST(promote, bool_to_int) { EXPECT_TRUE(promote(bool_true_value, int_type)->equals(*int_1_value)); }
TEST(promote, bool_to_float) { EXPECT_TRUE(promote(bool_true_value, float_type)->equals(*float_1_0_value)); }
TEST(promote, int_to_int) { EXPECT_TRUE(promote(int_1_value, int_type)->equals(*int_1_value)); }
TEST(promote, int_to_float) { EXPECT_TRUE(promote(int_1_value, float_type)->equals(*float_1_0_value)); }
TEST(promote, float_to_float) { EXPECT_TRUE(promote(float_1_0_value, float_type)->equals(*float_1_0_value)); }

// For a variable or a return value, a promotion is just a check.
// If the check is successful, the variable or return value is returned
TEST(promote, bool_variable_ref_to_int) { EXPECT_TRUE(promote(bool_variable_ref, int_type)->equals(*bool_variable_ref)); }

TEST(promote, int_to_bool) { EXPECT_TRUE(promote(int_1_value, bool_type).empty()); }
TEST(promote, float_to_bool) { EXPECT_TRUE(promote(float_1_0_value, bool_type).empty()); }
TEST(promote, float_to_int) { EXPECT_TRUE(promote(float_1_0_value, int_type).empty()); }

TEST(size_of, const_bool) { EXPECT_EQ(size_of(bool_true_value), 1); }
TEST(size_of, const_int) { EXPECT_EQ(size_of(int_1_value), 1); }
TEST(size_of, const_float) { EXPECT_EQ(size_of(float_1_0_value), 1); }

}  // namespace cqasm::v3x::values
