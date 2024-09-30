#include "v3x/cqasm-instruction-set.hpp"

#include "cqasm-error.hpp"

#include <fmt/format.h>

namespace cqasm::v3x::instruction {

InstructionSet::InstructionSet()
: gate_map{
    { "CNOT", { std::nullopt, "QQ" } },
    { "CNOT", { std::nullopt, "QV" } },
    { "CNOT", { std::nullopt, "VQ" } },
    { "CNOT", { std::nullopt, "VV" } },
    { "CR", { 'f', "QQ" } },
    { "CR", { 'f', "QV" } },
    { "CR", { 'f', "VQ" } },
    { "CR", { 'f', "VV" } },
    { "CRk", { std::nullopt, "QQ" } },
    { "CRk", { std::nullopt, "QV" } },
    { "CRk", { std::nullopt, "VQ" } },
    { "CRk", { std::nullopt, "VV" } },
    { "CZ", { 'i', "QQ" } },
    { "CZ", { 'i', "QV" } },
    { "CZ", { 'i', "VQ" } },
    { "CZ", { 'i', "VV" } },
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
, single_qubit_gate_list{
    "H", "I", "mX90", "mY90", "Rx", "Ry", "Rz", "S", "Sdag", "T", "Tdag", "X", "X90", "Y", "Y90", "Z"
}
, two_qubit_gate_list{
    "CNOT", "CR", "CRk", "CZ"
}
, non_gate_map{
    { "measure", { std::nullopt, "BQ" } },
    { "measure", { std::nullopt, "WV" } },
    { "measure", { std::nullopt, "BV" } },
    { "measure", { std::nullopt, "WQ" } },
    { "reset", { std::nullopt, std::nullopt } },
    { "reset", { std::nullopt, "Q" } },
    { "reset", { std::nullopt, "V" } }
}
, gate_modifier_map{
    { "inv", std::nullopt },
    { "pow", 'f' },
    { "ctrl", std::nullopt },
}
{}


[[nodiscard]] /* static */ InstructionSet& InstructionSet::get_instance() {
    static InstructionSet instance;
    return instance;
}

[[nodiscard]] const InstructionMapT& InstructionSet::get_gate_map() const {
    return gate_map;
}

[[nodiscard]] const InstructionMapT& InstructionSet::get_non_gate_map() const {
    return non_gate_map;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_single_qubit_gate_list() const {
    return single_qubit_gate_list;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_two_qubit_gate_list() const {
    return two_qubit_gate_list;
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate(const std::string &name) const {
    return single_qubit_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate(const std::string &name) const {
    return two_qubit_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_gate(const std::string &name) const {
    return is_single_qubit_gate(name) || is_two_qubit_gate(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_unitary_gate_composition(const std::string &name) const {
    return name.starts_with(single_qubit_unitary_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_unitary_gate_composition(const std::string &name) const {
    return name.starts_with(two_qubit_unitary_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_unitary_gate_composition(const std::string &name) const {
    return is_single_qubit_unitary_gate_composition(name) || is_two_qubit_unitary_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_unitary_gate(const std::string &name) const {
    return is_single_qubit_gate(name) || is_single_qubit_unitary_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_unitary_gate(const std::string &name) const {
    return is_two_qubit_gate(name) || is_two_qubit_unitary_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_unitary_gate(const std::string &name) const {
    return is_single_qubit_unitary_gate(name) || is_two_qubit_unitary_gate(name);
}

[[nodiscard]] bool InstructionSet::is_measure(const std::string &name) const {
    return name == measure_instruction_name;
}

[[nodiscard]] bool InstructionSet::is_reset(const std::string &name) const {
    return name == reset_instruction_name;
}

[[nodiscard]] bool InstructionSet::is_non_gate(const std::string &name) const {
    return is_measure(name) || is_reset(name);
}

[[nodiscard]] bool InstructionSet::is_inv_gate_modifier(const std::string &name) const {
    return name == inv_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_pow_gate_modifier(const std::string &name) const {
    return name == pow_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_ctrl_gate_modifier(const std::string &name) const {
    return name == ctrl_gate_modifier_name;
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate_modifier(const std::string &name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate_modifier(const std::string &name) const {
    return is_ctrl_gate_modifier(name);
}

[[nodiscard]] bool InstructionSet::is_gate_modifier(const std::string &name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name) || is_ctrl_gate_modifier(name);
}

[[nodiscard]] std::optional<char> InstructionSet::get_gate_param_type(const std::string &name) const {
    if (const auto &it = gate_map.find(name); it != gate_map.end()) {
        const auto &pair_param_types_operand_types = it->second;
        return pair_param_types_operand_types.first;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate '{}'", name) };
}

[[nodiscard]] std::optional<char> InstructionSet::get_gate_modifier_param_type(const std::string &name) const {
    if (const auto &it = gate_modifier_map.find(name); it != gate_modifier_map.end()) {
        return it->second;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate modifier '{}'", name) };
}

[[nodiscard]] std::optional<char> InstructionSet::get_instruction_param_type(const std::string &name) const {
    if (is_gate(name)) {
        return get_gate_param_type(name);
    } else if (is_non_gate(name)) {
        return std::nullopt;
    } else if (is_gate_modifier(name)) {
        return get_gate_modifier_param_type(name);
    }
    throw error::AnalysisError{ fmt::format("couldn't find instruction '{}'", name) };
}

}  // namespace cqasm::v3x::instruction
