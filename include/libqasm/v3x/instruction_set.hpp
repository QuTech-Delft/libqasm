#pragma once

#include <string>
#include <map>
#include <optional>
#include <set>
#include <utility>  // pair

namespace cqasm::v3x::instruction {

using InstructionNameT = std::string;
using ParamTypeT = std::optional<char>;
using OperandTypesT = std::optional<std::string>;
using KeyT = InstructionNameT;
using ValueT = std::pair<ParamTypeT, OperandTypesT>;
using InstructionMapT = std::multimap<KeyT, ValueT>;
using InstructionListT = std::set<InstructionNameT>;
using GateModifierMapT = std::map<KeyT, ParamTypeT>;

class InstructionSet {
    InstructionMapT named_gate_map;
    InstructionMapT non_gate_map;
    GateModifierMapT gate_modifier_map;

    InstructionListT single_qubit_named_gate_list;
    InstructionListT two_qubit_named_gate_list;

    InstructionSet();

public:
    // The use of '1q' and '2q' as gate prefixes avoids any possible conflict with user defined gates
    // since an identifier cannot start with a number
    std::string single_qubit_gate_composition_prefix = "1q";
    std::string two_qubit_gate_composition_prefix = "2q";

    std::string measure_name = "measure";
    std::string reset_name = "reset";

    std::string inv_gate_modifier_name = "inv";
    std::string pow_gate_modifier_name = "pow";
    std::string ctrl_gate_modifier_name = "ctrl";

public:
    [[nodiscard]] static InstructionSet& get_instance();

    [[nodiscard]] const InstructionMapT& get_named_gate_map() const;
    [[nodiscard]] const InstructionMapT& get_non_gate_map() const;
    [[nodiscard]] const GateModifierMapT& get_gate_modifier_map() const;
    [[nodiscard]] const InstructionListT& get_single_qubit_named_gate_list() const;
    [[nodiscard]] const InstructionListT& get_two_qubit_named_gate_list() const;

    [[nodiscard]] bool is_single_qubit_named_gate(const std::string &name) const;
    [[nodiscard]] bool is_two_qubit_named_gate(const std::string &name) const;
    [[nodiscard]] bool is_named_gate(const std::string &name) const;

    [[nodiscard]] bool is_single_qubit_gate_composition(const std::string &name) const;
    [[nodiscard]] bool is_two_qubit_gate_composition(const std::string &name) const;
    [[nodiscard]] bool is_gate_composition(const std::string &name) const;

    [[nodiscard]] bool is_single_qubit_gate(const std::string &name) const;
    [[nodiscard]] bool is_two_qubit_gate(const std::string &name) const;
    [[nodiscard]] bool is_gate(const std::string &name) const;

    [[nodiscard]] bool is_measure(const std::string &name) const;
    [[nodiscard]] bool is_reset(const std::string &name) const;
    [[nodiscard]] bool is_non_gate(const std::string &name) const;

    [[nodiscard]] bool is_inv_gate_modifier(const std::string &name) const;
    [[nodiscard]] bool is_pow_gate_modifier(const std::string &name) const;
    [[nodiscard]] bool is_ctrl_gate_modifier(const std::string &name) const;
    [[nodiscard]] bool is_single_qubit_gate_modifier(const std::string &name) const;
    [[nodiscard]] bool is_two_qubit_gate_modifier(const std::string &name) const;
    [[nodiscard]] bool is_gate_modifier(const std::string &name) const;

    [[nodiscard]] std::optional<char> get_named_gate_param_type(const std::string &name) const;
    [[nodiscard]] std::optional<char> get_gate_modifier_param_type(const std::string &name) const;
    [[nodiscard]] std::optional<char> get_instruction_param_type(const std::string &name) const;
};

}  // namespace cqasm::v3x::instruction
