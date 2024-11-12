#include "libqasm/v3x/instruction_set.hpp"

#include <fmt/format.h>

#include "libqasm/error.hpp"

namespace cqasm::v3x::instruction {

InstructionSet::InstructionSet()
: named_gate_map{
    { "CNOT", { std::nullopt, "QQ" } },
    { "CNOT", { std::nullopt, "QV" } },
    { "CNOT", { std::nullopt, "VQ" } },
    { "CNOT", { std::nullopt, "VV" } },
    { "CR", { 'f', "QQ" } },
    { "CR", { 'f', "QV" } },
    { "CR", { 'f', "VQ" } },
    { "CR", { 'f', "VV" } },
    { "CRk", { 'i', "QQ" } },
    { "CRk", { 'i', "QV" } },
    { "CRk", { 'i', "VQ" } },
    { "CRk", { 'i', "VV" } },
    { "CZ", { std::nullopt, "QQ" } },
    { "CZ", { std::nullopt, "QV" } },
    { "CZ", { std::nullopt, "VQ" } },
    { "CZ", { std::nullopt, "VV" } },
    { "H", { std::nullopt, "Q" } },
    { "H", { std::nullopt, "V" } },
    { "I", { std::nullopt, "Q" } },
    { "I", { std::nullopt, "V" } },
    { "mX90", { std::nullopt, "Q" } },
    { "mX90", { std::nullopt, "V" } },
    { "mY90", { std::nullopt, "Q" } },
    { "mY90", { std::nullopt, "V" } },
    { "Rx", { 'f', "Q" } },
    { "Rx", { 'f', "V" } },
    { "Ry", { 'f', "Q" } },
    { "Ry", { 'f', "V" } },
    { "Rz", { 'f', "Q" } },
    { "Rz", { 'f', "V" } },
    { "S", { std::nullopt, "Q" } },
    { "S", { std::nullopt, "V" } },
    { "Sdag", { std::nullopt, "Q" } },
    { "Sdag", { std::nullopt, "V" } },
    { "SWAP", { std::nullopt, "QQ" } },
    { "SWAP", { std::nullopt, "QV" } },
    { "SWAP", { std::nullopt, "VQ" } },
    { "SWAP", { std::nullopt, "VV" } },
    { "T", { std::nullopt, "Q" } },
    { "T", { std::nullopt, "V" } },
    { "Tdag", { std::nullopt, "Q" } },
    { "Tdag", { std::nullopt, "V" } },
    { "X", { std::nullopt, "Q" } },
    { "X", { std::nullopt, "V" } },
    { "X90", { std::nullopt, "Q" } },
    { "X90", { std::nullopt, "V" } },
    { "Y", { std::nullopt, "Q" } },
    { "Y", { std::nullopt, "V" } },
    { "Y90", { std::nullopt, "Q" } },
    { "Y90", { std::nullopt, "V" } },
    { "Z", { std::nullopt, "Q" } },
    { "Z", { std::nullopt, "V" } }
}
, non_gate_map{
    { "measure", { std::nullopt, "BQ" } },
    { "measure", { std::nullopt, "WV" } },
    { "measure", { std::nullopt, "BV" } },
    { "measure", { std::nullopt, "WQ" } },
    { "reset", { std::nullopt, std::nullopt } },
    { "reset", { std::nullopt, "Q" } },
    { "reset", { std::nullopt, "V" } },
    { "init", { std::nullopt, "Q" } },
    { "init", { std::nullopt, "V" } },
    { "barrier", { std::nullopt, "Q" } },
    { "barrier", { std::nullopt, "V" } },
    { "wait", { 'i', "Q" } },
    { "wait", { 'i', "V" } },
}
, gate_modifier_map{
    { "inv", std::nullopt },
    { "pow", 'f' },
    { "ctrl", std::nullopt },
}
, single_qubit_named_gate_list{
    "H", "I", "mX90", "mY90", "Rx", "Ry", "Rz", "S", "Sdag", "T", "Tdag", "X", "X90", "Y", "Y90", "Z"
}
, two_qubit_named_gate_list{
    "CNOT", "CR", "CRk", "CZ", "SWAP"
}
{}

[[nodiscard]] /* static */ InstructionSet& InstructionSet::get_instance() {
    static InstructionSet instance;
    return instance;
}

[[nodiscard]] const InstructionMapT& InstructionSet::get_named_gate_map() const {
    return named_gate_map;
}

[[nodiscard]] const InstructionMapT& InstructionSet::get_non_gate_map() const {
    return non_gate_map;
}

[[nodiscard]] const GateModifierMapT& InstructionSet::get_gate_modifier_map() const {
    return gate_modifier_map;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_single_qubit_named_gate_list() const {
    return single_qubit_named_gate_list;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_two_qubit_named_gate_list() const {
    return two_qubit_named_gate_list;
}

[[nodiscard]] bool InstructionSet::is_single_qubit_named_gate(const std::string& name) const {
    return single_qubit_named_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_named_gate(const std::string& name) const {
    return two_qubit_named_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_named_gate(const std::string& name) const {
    return is_single_qubit_named_gate(name) || is_two_qubit_named_gate(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate_composition(const std::string& name) const {
    return name.starts_with(single_qubit_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate_composition(const std::string& name) const {
    return name.starts_with(two_qubit_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_gate_composition(const std::string& name) const {
    return is_single_qubit_gate_composition(name) || is_two_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate(const std::string& name) const {
    return is_single_qubit_named_gate(name) || is_single_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate(const std::string& name) const {
    return is_two_qubit_named_gate(name) || is_two_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_gate(const std::string& name) const {
    return is_named_gate(name) || is_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_measure(const std::string& name) const {
    return name == measure_name;
}

[[nodiscard]] bool InstructionSet::is_reset(const std::string& name) const {
    return name == reset_name;
}

[[nodiscard]] bool InstructionSet::is_init(const std::string& name) const {
    return name == init_name;
}

[[nodiscard]] bool InstructionSet::is_barrier(const std::string& name) const {
    return name == barrier_name;
}

[[nodiscard]] bool InstructionSet::is_wait(const std::string& name) const {
    return name == wait_name;
}

[[nodiscard]] bool InstructionSet::is_non_gate(const std::string& name) const {
    return is_measure(name) || is_reset(name) || is_init(name) || is_barrier(name) || is_wait(name);
}

[[nodiscard]] bool InstructionSet::is_inv_gate_modifier(const std::string& name) const {
    return name == inv_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_pow_gate_modifier(const std::string& name) const {
    return name == pow_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_ctrl_gate_modifier(const std::string& name) const {
    return name == ctrl_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate_modifier(const std::string& name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate_modifier(const std::string& name) const {
    return is_ctrl_gate_modifier(name);
}

[[nodiscard]] bool InstructionSet::is_gate_modifier(const std::string& name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name) || is_ctrl_gate_modifier(name);
}

[[nodiscard]] std::optional<char> InstructionSet::get_named_gate_param_type(const std::string& name) const {
    if (const auto& it = named_gate_map.find(name); it != named_gate_map.end()) {
        const auto& pair_param_types_operand_types = it->second;
        return pair_param_types_operand_types.first;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate '{}'", name) };
}

[[nodiscard]] std::optional<char> InstructionSet::get_non_gate_param_type(const std::string& name) const {
    if (const auto& it = non_gate_map.find(name); it != non_gate_map.end()) {
        const auto& pair_param_types_operand_types = it->second;
        return pair_param_types_operand_types.first;
    }
    throw error::AnalysisError{ fmt::format("couldn't find non-unitary instruction '{}'", name) };
}

[[nodiscard]] std::optional<char> InstructionSet::get_gate_modifier_param_type(const std::string& name) const {
    if (const auto& it = gate_modifier_map.find(name); it != gate_modifier_map.end()) {
        return it->second;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate modifier '{}'", name) };
}

[[nodiscard]] std::optional<char> InstructionSet::get_instruction_param_type(const std::string& name) const {
    if (is_named_gate(name)) {
        return get_named_gate_param_type(name);
    } else if (is_non_gate(name)) {
        return get_non_gate_param_type(name);
    } else if (is_gate_modifier(name)) {
        return get_gate_modifier_param_type(name);
    }
    throw error::AnalysisError{ fmt::format("couldn't find instruction '{}'", name) };
}

}  // namespace cqasm::v3x::instruction
