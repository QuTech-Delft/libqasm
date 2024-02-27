#include "v3x/cqasm-semantic-gen.hpp"
#include "v3x/cqasm-values.hpp"

#include <gtest/gtest.h>


namespace cqasm::v3x::values {

// bool, int, float, axis types
static const types::Type bool_type = tree::make<types::Bool>(1);
static const types::Type int_type = tree::make<types::Int>(1);
static const types::Type float_type = tree::make<types::Float>(1);
static const types::Type axis_type = tree::make<types::Axis>(3);

// bool, int, float array types
static const types::Type bool_array_of_3_type = tree::make<types::BoolArray>(3);
static const types::Type bool_array_of_4_type = tree::make<types::BoolArray>(4);
static const types::Type bool_array_of_10_type = tree::make<types::BoolArray>(10);
static const types::Type int_array_of_3_type = tree::make<types::IntArray>(3);
static const types::Type float_array_of_3_type = tree::make<types::FloatArray>(3);

// bool, int, float, axis values
    static const values::Value bool_false_value = tree::make<values::ConstBool>(false);
static const values::Value bool_true_value = tree::make<values::ConstBool>(true);
static const values::Value int_0_value = tree::make<values::ConstInt>(0);
static const values::Value int_1_value = tree::make<values::ConstInt>(1);
static const values::Value int_2_value = tree::make<values::ConstInt>(2);
static const values::Value int_3_value = tree::make<values::ConstInt>(3);
static const values::Value int_4_value = tree::make<values::ConstInt>(4);
static const values::Value int_5_value = tree::make<values::ConstInt>(5);
static const values::Value int_6_value = tree::make<values::ConstInt>(6);
static const values::Value float_0_0_value = tree::make<values::ConstFloat>(0.0);
static const values::Value float_1_0_value = tree::make<values::ConstFloat>(1.0);
static const values::Value axis_x_value = tree::make<values::ConstAxis>();

// bool, int, float array values
static const values::Value bool_array_of_2_value = tree::make<values::ConstBoolArray>(
    tree::Many<values::ConstBool>{ bool_true_value, bool_false_value });
static const values::Value bool_array_of_3_value = tree::make<values::ConstBoolArray>(
    tree::Many<values::ConstBool>{ bool_true_value, bool_false_value, bool_false_value });
static const values::Value int_array_of_3_value = tree::make<values::ConstIntArray>(
    tree::Many<values::ConstInt>{ int_1_value, int_0_value, int_0_value });
static const values::Value float_array_of_3_value = tree::make<values::ConstFloatArray>(
    tree::Many<values::ConstFloat>{ float_1_0_value, float_0_0_value, float_0_0_value });

// bool semantic variable
static const auto bool_variable = tree::make<semantic::Variable>("b", bool_type);

// bool array semantic variables
static const auto bool_array_of_3_variable = tree::make<semantic::Variable>("ba3", bool_array_of_3_type);
static const auto bool_array_of_10_variable = tree::make<semantic::Variable>("ba10", bool_array_of_10_type);

// bool variable ref value
static const values::Value bool_variable_ref = tree::make<values::VariableRef>(bool_variable);

// bool array variable ref value
static const values::Value bool_array_of_3_variable_ref = tree::make<values::VariableRef>(bool_array_of_3_variable);

// index ref values
static const auto index_2_value = tree::make<values::IndexRef>(bool_array_of_10_variable,
    tree::Many<values::ConstInt>{ int_2_value });
static const auto index_2_to_5_value = tree::make<values::IndexRef>(bool_array_of_10_variable,
    tree::Many<values::ConstInt>{ int_2_value, int_3_value, int_4_value, int_5_value });
static const auto index_2_4_6_value = tree::make<values::IndexRef>(bool_array_of_10_variable,
    tree::Many<values::ConstInt>{ int_2_value, int_4_value, int_6_value });

// Semantic function returning bool
static const auto function_returning_bool = tree::make<semantic::Function>("fb", bool_type);

// Semantic function returning bool array
static const auto function_returning_bool_array_of_3 = tree::make<semantic::Function>("fba", bool_array_of_3_type);

// Function returning bool value
static const values::Value function_returning_bool_value = tree::make<values::FunctionRef>(function_returning_bool);

// Function returning bool array value
static const values::Value function_returning_bool_array_of_3_value = tree::make<values::FunctionRef>(function_returning_bool_array_of_3);

// bool return value
static const values::Value bool_return_value = tree::make<values::FunctionCall>(tree::make<values::FunctionRef>(function_returning_bool));

// bool array return value
static const values::Value bool_array_of_3_return_value = tree::make<values::FunctionCall>(
    tree::make<values::FunctionRef>(function_returning_bool_array_of_3));

TEST(promote, bool_to_bool) { EXPECT_TRUE(promote(bool_true_value, bool_type)->equals(*bool_true_value)); }
TEST(promote, bool_to_int) { EXPECT_TRUE(promote(bool_true_value, int_type)->equals(*int_1_value)); }
TEST(promote, bool_to_float) { EXPECT_TRUE(promote(bool_true_value, float_type)->equals(*float_1_0_value)); }
TEST(promote, int_to_int) { EXPECT_TRUE(promote(int_1_value, int_type)->equals(*int_1_value)); }
TEST(promote, int_to_float) { EXPECT_TRUE(promote(int_1_value, float_type)->equals(*float_1_0_value)); }
TEST(promote, float_to_float) { EXPECT_TRUE(promote(float_1_0_value, float_type)->equals(*float_1_0_value)); }

TEST(promote, bool_array_to_bool_array) { EXPECT_TRUE(promote(bool_array_of_3_value, bool_array_of_3_type)->equals(*bool_array_of_3_value)); }
TEST(promote, bool_array_to_int_array) { EXPECT_TRUE(promote(bool_array_of_3_value, int_array_of_3_type)->equals(*int_array_of_3_value)); }
TEST(promote, bool_array_to_float_array) { EXPECT_TRUE(promote(bool_array_of_3_value, float_array_of_3_type)->equals(*float_array_of_3_value)); }
TEST(promote, int_array_to_int_array) { EXPECT_TRUE(promote(int_array_of_3_value, int_array_of_3_type)->equals(*int_array_of_3_value)); }
TEST(promote, int_array_to_float_array) { EXPECT_TRUE(promote(int_array_of_3_value, float_array_of_3_type)->equals(*float_array_of_3_value)); }
TEST(promote, float_array_to_float_array) { EXPECT_TRUE(promote(float_array_of_3_value, float_array_of_3_type)->equals(*float_array_of_3_value)); }

// For a variable or a return value, a promotion is just a check.
// If the check is successful, the variable or return value is returned
TEST(promote, bool_variable_ref_to_int) { EXPECT_TRUE(promote(bool_variable_ref, int_type)->equals(*bool_variable_ref)); }
TEST(promote, bool_array_variable_ref_to_int_array) { EXPECT_TRUE(promote(bool_array_of_3_variable_ref, int_array_of_3_type)->equals(*bool_array_of_3_variable_ref)); }
TEST(promote, bool_return_value_to_int) { EXPECT_TRUE(promote(bool_return_value, int_type)->equals(*bool_return_value)); }
TEST(promote, bool_array_return_value_to_int_array) { EXPECT_TRUE(promote(bool_array_of_3_return_value, int_array_of_3_type)->equals(*bool_array_of_3_return_value)); }

// bool, int, and float arrays of 3 elements, when promoted to axis, are converted to float arrays of 3 elements
TEST(promote, bool_array_of_3_to_axis) { EXPECT_TRUE(promote(bool_array_of_3_value, axis_type)->equals(*float_array_of_3_value)); }
TEST(promote, int_array_of_3_to_axis) { EXPECT_TRUE(promote(int_array_of_3_value, axis_type)->equals(*float_array_of_3_value)); }
TEST(promote, float_array_of_3_to_axis) { EXPECT_TRUE(promote(float_array_of_3_value, axis_type)->equals(*float_array_of_3_value)); }

TEST(promote, int_to_bool) { EXPECT_TRUE(promote(int_1_value, bool_type).empty()); }
TEST(promote, float_to_bool) { EXPECT_TRUE(promote(float_1_0_value, bool_type).empty()); }
TEST(promote, float_to_int) { EXPECT_TRUE(promote(float_1_0_value, int_type).empty()); }

TEST(promote, int_array_to_bool_array) { EXPECT_TRUE(promote(int_array_of_3_value, bool_array_of_3_type).empty()); }
TEST(promote, float_array_to_bool_array) { EXPECT_TRUE(promote(float_array_of_3_value, bool_array_of_3_type).empty()); }
TEST(promote, float_array_to_int_array) { EXPECT_TRUE(promote(float_array_of_3_value, int_array_of_3_type).empty()); }

TEST(promote, bool_array_to_bool) { EXPECT_TRUE(promote(bool_array_of_3_value, bool_type).empty()); }
TEST(promote, bool_array_variable_ref_to_bool) { EXPECT_TRUE(promote(bool_array_of_3_variable_ref, bool_type).empty()); }
TEST(promote, bool_array_return_value_to_bool) { EXPECT_TRUE(promote(bool_array_of_3_return_value, bool_type).empty()); }

TEST(promote, bool_to_axis) { EXPECT_TRUE(promote(bool_true_value, axis_type).empty()); }
TEST(promote, bool_variable_ref_to_axis) { EXPECT_TRUE(promote(bool_variable_ref, axis_type).empty()); }
TEST(promote, bool_return_value_to_axis) { EXPECT_TRUE(promote(bool_return_value, axis_type).empty()); }
TEST(promote, bool_array_of_2_to_axis) { EXPECT_TRUE(promote(bool_array_of_2_value, axis_type).empty()); }

TEST(type_of, index_2_value) { EXPECT_TRUE(types::type_check(type_of(index_2_value), bool_type)); }
TEST(type_of, index_2_to_5_value) { EXPECT_TRUE(types::type_check(type_of(index_2_to_5_value), bool_array_of_4_type)); }
TEST(type_of, index_2_4_6_value) { EXPECT_TRUE(types::type_check(type_of(index_2_4_6_value), bool_array_of_3_type)); }
TEST(type_of, bool_variable_ref) { EXPECT_TRUE(types::type_check(type_of(bool_variable_ref), bool_type)); }
TEST(type_of, function_with_bool_return_type) { EXPECT_TRUE(types::type_check(type_of(function_returning_bool_value), bool_type)); }
TEST(type_of, function_with_bool_array_return_type) { EXPECT_TRUE(types::type_check(type_of(function_returning_bool_array_of_3_value), bool_array_of_3_type)); }
TEST(type_of, function_call_to_function_with_bool_return_type) { EXPECT_TRUE(types::type_check(type_of(bool_return_value), bool_type)); }
TEST(type_of, function_call_to_function_with_bool_array_return_type) { EXPECT_TRUE(types::type_check(type_of(bool_array_of_3_return_value), bool_array_of_3_type)); }

TEST(size_of, const_axis) { EXPECT_EQ(size_of(axis_x_value), 3); }
TEST(size_of, const_bool) { EXPECT_EQ(size_of(bool_true_value), 1); }
TEST(size_of, const_int) { EXPECT_EQ(size_of(int_1_value), 1); }
TEST(size_of, const_float) { EXPECT_EQ(size_of(float_1_0_value), 1); }
TEST(size_of, const_bool_array) { EXPECT_EQ(size_of(bool_array_of_3_value), 3); }
TEST(size_of, const_int_array) { EXPECT_EQ(size_of(int_array_of_3_value), 3); }
TEST(size_of, const_float_array) { EXPECT_EQ(size_of(float_array_of_3_value), 3); }
TEST(size_of, bool_b_10__b_2) { EXPECT_EQ(size_of(index_2_value), 1); }
TEST(size_of, bool_b_10__b_2_to_5) { EXPECT_EQ(size_of(index_2_to_5_value), 4); }
TEST(size_of, bool_b_10__b_2_4_6) { EXPECT_EQ(size_of(index_2_4_6_value), 3); }
TEST(size_of, bool_variable_ref) { EXPECT_EQ(size_of(bool_variable_ref), 1); }
TEST(size_of, function_returning_bool) { EXPECT_EQ(size_of(function_returning_bool_value), 1); }
TEST(size_of, function_returning_bool_array_of_3) { EXPECT_EQ(size_of(function_returning_bool_array_of_3_value), 3); }
TEST(size_of, bool_return_value) { EXPECT_EQ(size_of(bool_return_value), 1); }
TEST(size_of, bool_array_of_3_return_value) { EXPECT_EQ(size_of(bool_array_of_3_return_value), 3); }

}  // namespace cqasm::v3x::values
