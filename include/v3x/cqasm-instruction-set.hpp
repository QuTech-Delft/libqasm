#pragma once

#include <string>
#include <string_view>
#include <map>
#include <optional>
#include <set>

namespace cqasm::v3x::instruction {

using InstructionNameT = std::string;
using ParamTypesT = std::optional<std::string>;
using InstructionMapT = std::multimap<InstructionNameT, ParamTypesT>;
using InstructionListT = std::set<InstructionNameT>;

class InstructionSet {
    InstructionMapT gate_map;
    InstructionMapT non_gate_map;
    InstructionListT single_qubit_gate_list;
    InstructionListT two_qubit_gate_list;

    InstructionSet();

public:
    // The use of '1q' and '2q' as gate prefixes avoids any possible conflict with user defined gates
    // since an identifier cannot start with a number
    std::string_view single_qubit_modified_gate_prefix = "1q";
    std::string_view two_qubit_modified_gate_prefix = "2q";

    std::string_view inv_gate_modifier_name = "inv";
    std::string_view pow_gate_modifier_name = "pow";
    std::string_view ctrl_gate_modifier_name = "ctrl";

    std::string_view measure_instruction_name = "measure";
    std::string_view reset_instruction_name = "reset";

public:
    [[nodiscard]] static InstructionSet& get_instance();

    [[nodiscard]] const InstructionMapT& get_gate_map() const;
    [[nodiscard]] const InstructionMapT& get_non_gate_map() const;
    [[nodiscard]] const InstructionListT& get_single_qubit_gate_list() const;
    [[nodiscard]] const InstructionListT& get_two_qubit_gate_list() const;

    bool is_single_qubit_gate(const std::string &name) const;
    bool is_two_qubit_gate(const std::string &name) const;
    bool is_gate(const std::string &name) const;

    bool is_single_qubit_modified_gate(const std::string &name) const;
    bool is_two_qubit_modified_gate(const std::string &name) const;
    bool is_modified_gate(const std::string &name) const;

    bool is_single_qubit_unitary_gate(const std::string &name) const;
    bool is_two_qubit_unitary_gate(const std::string &name) const;
    bool is_unitary_gate(const std::string &name) const;

    bool is_inv_gate_modifier(const std::string &name) const;
    bool is_pow_gate_modifier(const std::string &name) const;
    bool is_ctrl_gate_modifier(const std::string &name) const;
    bool is_single_qubit_gate_modifier(const std::string &name) const;
    bool is_two_qubit_gate_modifier(const std::string &name) const;
    bool is_gate_modifier(const std::string &name) const;

    bool is_measure_instruction(const std::string &name) const;
    bool is_reset_instruction(const std::string &name) const;
};

}  // namespace cqasm::v3x::instruction
