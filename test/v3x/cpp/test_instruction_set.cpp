#include <gmock/gmock.h>

#include "libqasm/error.hpp"
#include "libqasm/v3x/instruction_set.hpp"

using namespace cqasm::error;
using namespace cqasm::v3x::instruction;

class InstructionSetTest : public ::testing::Test {
protected:
    void SetUp() override {}

    const InstructionSet& instruction_set = InstructionSet::get_instance();
    const InstructionSet& instruction_set_2 = InstructionSet::get_instance();
    const InstructionMapT& named_gate_map = instruction_set.get_named_gate_map();
    const InstructionMapT& non_gate_map = instruction_set.get_non_gate_map();
    const GateModifierMapT& gate_modifier_map = instruction_set.get_gate_modifier_map();
    const InstructionListT& single_qubit_named_gate_list = instruction_set.get_single_qubit_named_gate_list();
    const InstructionListT& two_qubit_named_gate_list = instruction_set.get_two_qubit_named_gate_list();

    const size_t number_of_gate_map_entries = 52;
    const size_t number_of_non_gate_map_entries = 13;
    const size_t number_of_gate_modifier_map_entries = 3;
    const size_t number_of_single_qubit_named_gates = 16;
    const size_t number_of_two_qubit_named_gates = 5;
};

TEST_F(InstructionSetTest, get_instance) {
    EXPECT_EQ(&instruction_set, &instruction_set_2);
}
TEST_F(InstructionSetTest, get_named_gate_map) {
    EXPECT_EQ(named_gate_map.size(), number_of_gate_map_entries);
}
TEST_F(InstructionSetTest, get_non_gate_map) {
    EXPECT_EQ(non_gate_map.size(), number_of_non_gate_map_entries);
}
TEST_F(InstructionSetTest, get_gate_modifier_map) {
    EXPECT_EQ(gate_modifier_map.size(), number_of_gate_modifier_map_entries);
}
TEST_F(InstructionSetTest, get_single_qubit_named_gate_list) {
    EXPECT_EQ(single_qubit_named_gate_list.size(), number_of_single_qubit_named_gates);
}
TEST_F(InstructionSetTest, get_two_qubit_named_gate_list) {
    EXPECT_EQ(two_qubit_named_gate_list.size(), number_of_two_qubit_named_gates);
}
TEST_F(InstructionSetTest, is_single_qubit_named_gate) {
    EXPECT_TRUE(instruction_set.is_single_qubit_named_gate("H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_named_gate("h"));
    EXPECT_FALSE(instruction_set.is_single_qubit_named_gate("CNOT"));
    EXPECT_FALSE(instruction_set.is_single_qubit_named_gate("1q_H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_named_gate("inv"));
}
TEST_F(InstructionSetTest, is_two_qubit_named_gate) {
    EXPECT_TRUE(instruction_set.is_two_qubit_named_gate("CNOT"));
    EXPECT_FALSE(instruction_set.is_two_qubit_named_gate("cnot"));
    EXPECT_FALSE(instruction_set.is_two_qubit_named_gate("H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_named_gate("2q_H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_named_gate("ctrl"));
}
TEST_F(InstructionSetTest, is_named_gate) {
    EXPECT_TRUE(instruction_set.is_named_gate("H"));
    EXPECT_TRUE(instruction_set.is_named_gate("CNOT"));
    EXPECT_FALSE(instruction_set.is_named_gate("h"));
    EXPECT_FALSE(instruction_set.is_named_gate("cnot"));
    EXPECT_FALSE(instruction_set.is_named_gate("1q_H"));
    EXPECT_FALSE(instruction_set.is_named_gate("2q_H"));
    EXPECT_FALSE(instruction_set.is_named_gate("inv"));
}
TEST_F(InstructionSetTest, is_single_qubit_gate_composition) {
    EXPECT_TRUE(instruction_set.is_single_qubit_gate_composition("1q_H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate_composition("H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate_composition("CNOT"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate_composition("2q_H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate_composition("inv"));
}
TEST_F(InstructionSetTest, is_two_qubit_gate_composition) {
    EXPECT_TRUE(instruction_set.is_two_qubit_gate_composition("2q_H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_composition("H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_composition("CNOT"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_composition("1q_H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_composition("inv"));
}
TEST_F(InstructionSetTest, is_single_qubit_gate) {
    EXPECT_TRUE(instruction_set.is_single_qubit_gate("H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate("CNOT"));
    EXPECT_TRUE(instruction_set.is_single_qubit_gate("1q_H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate("2q_H"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate("inv"));
}
TEST_F(InstructionSetTest, is_two_qubit_gate) {
    EXPECT_FALSE(instruction_set.is_two_qubit_gate("H"));
    EXPECT_TRUE(instruction_set.is_two_qubit_gate("CNOT"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate("1q_H"));
    EXPECT_TRUE(instruction_set.is_two_qubit_gate("2q_H"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate("inv"));
}
TEST_F(InstructionSetTest, is_gate) {
    EXPECT_TRUE(instruction_set.is_gate("H"));
    EXPECT_TRUE(instruction_set.is_gate("CNOT"));
    EXPECT_TRUE(instruction_set.is_gate("1q_H"));
    EXPECT_TRUE(instruction_set.is_gate("2q_H"));
    EXPECT_FALSE(instruction_set.is_gate("inv"));
    EXPECT_FALSE(instruction_set.is_gate("measure"));
    EXPECT_FALSE(instruction_set.is_gate("reset"));
}
TEST_F(InstructionSetTest, is_measure) {
    EXPECT_TRUE(instruction_set.is_measure("measure"));
    EXPECT_FALSE(instruction_set.is_measure("MEASURE"));
}
TEST_F(InstructionSetTest, is_reset) {
    EXPECT_TRUE(instruction_set.is_reset("reset"));
    EXPECT_FALSE(instruction_set.is_reset("RESET"));
}
TEST_F(InstructionSetTest, is_non_gate) {
    EXPECT_TRUE(instruction_set.is_non_gate("measure"));
    EXPECT_TRUE(instruction_set.is_non_gate("reset"));
    EXPECT_FALSE(instruction_set.is_non_gate("inv"));
}
TEST_F(InstructionSetTest, is_inv_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_inv_gate_modifier("inv"));
    EXPECT_FALSE(instruction_set.is_inv_gate_modifier("INV"));
}
TEST_F(InstructionSetTest, is_pow_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_pow_gate_modifier("pow"));
    EXPECT_FALSE(instruction_set.is_pow_gate_modifier("POW"));
}
TEST_F(InstructionSetTest, is_ctrl_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_ctrl_gate_modifier("ctrl"));
    EXPECT_FALSE(instruction_set.is_ctrl_gate_modifier("CTRL"));
}
TEST_F(InstructionSetTest, is_single_qubit_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_single_qubit_gate_modifier("inv"));
    EXPECT_TRUE(instruction_set.is_single_qubit_gate_modifier("pow"));
    EXPECT_FALSE(instruction_set.is_single_qubit_gate_modifier("ctrl"));
}
TEST_F(InstructionSetTest, is_two_qubit_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_two_qubit_gate_modifier("ctrl"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_modifier("inv"));
    EXPECT_FALSE(instruction_set.is_two_qubit_gate_modifier("pow"));
}
TEST_F(InstructionSetTest, is_gate_modifier) {
    EXPECT_TRUE(instruction_set.is_gate_modifier("inv"));
    EXPECT_TRUE(instruction_set.is_gate_modifier("pow"));
    EXPECT_TRUE(instruction_set.is_gate_modifier("ctrl"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("H"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("CNOT"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("1q_H"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("2q_H"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("measure"));
    EXPECT_FALSE(instruction_set.is_gate_modifier("reset"));
}
TEST_F(InstructionSetTest, get_named_gate_param_type) {
    EXPECT_EQ(instruction_set.get_named_gate_param_type("H"), std::nullopt);
    EXPECT_EQ(instruction_set.get_named_gate_param_type("Rz"), 'f');
    EXPECT_THAT([this]() { (void)instruction_set.get_named_gate_param_type("inv"); },
        ThrowsMessage<AnalysisError>(::testing::HasSubstr("couldn't find gate")));
}
TEST_F(InstructionSetTest, get_gate_modifier_param_type) {
    EXPECT_EQ(instruction_set.get_gate_modifier_param_type("inv"), std::nullopt);
    EXPECT_EQ(instruction_set.get_gate_modifier_param_type("pow"), 'f');
    EXPECT_THAT([this]() { (void)instruction_set.get_gate_modifier_param_type("H"); },
        ThrowsMessage<AnalysisError>(::testing::HasSubstr("couldn't find gate modifier")));
}
TEST_F(InstructionSetTest, get_instruction_param_type) {
    EXPECT_EQ(instruction_set.get_instruction_param_type("H"), std::nullopt);
    EXPECT_EQ(instruction_set.get_instruction_param_type("measure"), std::nullopt);
    EXPECT_EQ(instruction_set.get_instruction_param_type("pow"), 'f');
    EXPECT_THAT([this]() { (void)instruction_set.get_instruction_param_type("1q_H"); },
        ThrowsMessage<AnalysisError>(::testing::HasSubstr("couldn't find instruction")));
}
