#include "libqasm/v3x/instruction_set.hpp"

#include <fmt/format.h>

#include "libqasm/error.hpp"

namespace cqasm::v3x::instruction {

// NOLINTBEGIN
InstructionSet::InstructionSet()
: named_gate_map{
    { "CCNOT", { std::nullopt, "QQQ" } },
    { "CCNOT", { std::nullopt, "QQV" } },
    { "CCNOT", { std::nullopt, "QVQ" } },
    { "CCNOT", { std::nullopt, "QVV" } },
    { "CCNOT", { std::nullopt, "VQQ" } },
    { "CCNOT", { std::nullopt, "VQV" } },
    { "CCNOT", { std::nullopt, "VVQ" } },
    { "CCNOT", { std::nullopt, "VVV" } },
    { "CCX", { std::nullopt, "QQQ" } },
    { "CCX", { std::nullopt, "QQV" } },
    { "CCX", { std::nullopt, "QVQ" } },
    { "CCX", { std::nullopt, "QVV" } },
    { "CCX", { std::nullopt, "VQQ" } },
    { "CCX", { std::nullopt, "VQV" } },
    { "CCX", { std::nullopt, "VVQ" } },
    { "CCX", { std::nullopt, "VVV" } },
    { "CSWAP", { std::nullopt, "QQQ" } },
    { "CSWAP", { std::nullopt, "QQV" } },
    { "CSWAP", { std::nullopt, "QVQ" } },
    { "CSWAP", { std::nullopt, "QVV" } },
    { "CSWAP", { std::nullopt, "VQQ" } },
    { "CSWAP", { std::nullopt, "VQV" } },
    { "CSWAP", { std::nullopt, "VVQ" } },
    { "CSWAP", { std::nullopt, "VVV" } },
    { "CNOT", { std::nullopt, "QQ" } },
    { "CNOT", { std::nullopt, "QV" } },
    { "CNOT", { std::nullopt, "VQ" } },
    { "CNOT", { std::nullopt, "VV" } },
    { "CR", { "f", "QQ" } },
    { "CR", { "f", "QV" } },
    { "CR", { "f", "VQ" } },
    { "CR", { "f", "VV" } },
    { "CRk", { "i", "QQ" } },
    { "CRk", { "i", "QV" } },
    { "CRk", { "i", "VQ" } },
    { "CRk", { "i", "VV" } },
    { "CV", { std::nullopt, "QQ" } },
    { "CV", { std::nullopt, "QV" } },
    { "CV", { std::nullopt, "VQ" } },
    { "CV", { std::nullopt, "VV" } },
    { "CY", { std::nullopt, "QQ" } },
    { "CY", { std::nullopt, "QV" } },
    { "CY", { std::nullopt, "VQ" } },
    { "CY", { std::nullopt, "VV" } },
    { "CZ", { std::nullopt, "QQ" } },
    { "CZ", { std::nullopt, "QV" } },
    { "CZ", { std::nullopt, "VQ" } },
    { "CZ", { std::nullopt, "VV" } },
    { "DCNOT", { std::nullopt, "QQ" } },
    { "DCNOT", { std::nullopt, "QV" } },
    { "DCNOT", { std::nullopt, "VQ" } },
    { "DCNOT", { std::nullopt, "VV" } },
    { "ECR", { std::nullopt, "QQ" } },
    { "ECR", { std::nullopt, "QV" } },
    { "ECR", { std::nullopt, "VQ" } },
    { "ECR", { std::nullopt, "VV" } },
    { "H", { std::nullopt, "Q" } },
    { "H", { std::nullopt, "V" } },
    { "I", { std::nullopt, "Q" } },
    { "I", { std::nullopt, "V" } },
    { "InvSqrtSWAP", { std::nullopt, "QQ" } },
    { "InvSqrtSWAP", { std::nullopt, "QV" } },
    { "InvSqrtSWAP", { std::nullopt, "VQ" } },
    { "InvSqrtSWAP", { std::nullopt, "VV" } },
    { "ISWAP", { std::nullopt, "QQ" } },
    { "ISWAP", { std::nullopt, "QV" } },
    { "ISWAP", { std::nullopt, "VQ" } },
    { "ISWAP", { std::nullopt, "VV" } },
    { "M", { std::nullopt, "QQ" } },
    { "M", { std::nullopt, "QV" } },
    { "M", { std::nullopt, "VQ" } },
    { "M", { std::nullopt, "VV" } },
    { "MS", { std::nullopt, "QQ" } },
    { "MS", { std::nullopt, "QV" } },
    { "MS", { std::nullopt, "VQ" } },
    { "MS", { std::nullopt, "VV" } },
    { "mX90", { std::nullopt, "Q" } },
    { "mX90", { std::nullopt, "V" } },
    { "mY90", { std::nullopt, "Q" } },
    { "mY90", { std::nullopt, "V" } },
    { "mZ90", { std::nullopt, "Q" } },
    { "mZ90", { std::nullopt, "V" } },
    { "Rn", { "fffff", "Q" } },
    { "Rn", { "fffff", "V" } },
    { "Rx", { "f", "Q" } },
    { "Rx", { "f", "V" } },
    { "Ry", { "f", "Q" } },
    { "Ry", { "f", "V" } },
    { "Rz", { "f", "Q" } },
    { "Rz", { "f", "V" } },
    { "S", { std::nullopt, "Q" } },
    { "S", { std::nullopt, "V" } },
    { "Sdag", { std::nullopt, "Q" } },
    { "Sdag", { std::nullopt, "V" } },
    { "SqrtISWAP", { std::nullopt, "QQ" } },
    { "SqrtISWAP", { std::nullopt, "QV" } },
    { "SqrtISWAP", { std::nullopt, "VQ" } },
    { "SqrtISWAP", { std::nullopt, "VV" } },
    { "SqrtSWAP", { std::nullopt, "QQ" } },
    { "SqrtSWAP", { std::nullopt, "QV" } },
    { "SqrtSWAP", { std::nullopt, "VQ" } },
    { "SqrtSWAP", { std::nullopt, "VV" } },
    { "SWAP", { std::nullopt, "QQ" } },
    { "SWAP", { std::nullopt, "QV" } },
    { "SWAP", { std::nullopt, "VQ" } },
    { "SWAP", { std::nullopt, "VV" } },
    { "T", { std::nullopt, "Q" } },
    { "T", { std::nullopt, "V" } },
    { "Tdag", { std::nullopt, "Q" } },
    { "Tdag", { std::nullopt, "V" } },
    { "U", { "fff", "Q" } },
    { "U", { "fff", "V" } },
    { "X", { std::nullopt, "Q" } },
    { "X", { std::nullopt, "V" } },
    { "X90", { std::nullopt, "Q" } },
    { "X90", { std::nullopt, "V" } },
    { "Y", { std::nullopt, "Q" } },
    { "Y", { std::nullopt, "V" } },
    { "Y90", { std::nullopt, "Q" } },
    { "Y90", { std::nullopt, "V" } },
    { "Z", { std::nullopt, "Q" } },
    { "Z", { std::nullopt, "V" } },
    { "Z90", { std::nullopt, "Q" } },
    { "Z90", { std::nullopt, "V" } }
}
, non_gate_map{
    { "measure", { std::nullopt, "BQ" } },
    { "measure", { std::nullopt, "WV" } },
    { "measure", { std::nullopt, "BV" } },
    { "measure", { std::nullopt, "WQ" } },
    { "measureX", { std::nullopt, "BQ" } },
    { "measureX", { std::nullopt, "WV" } },
    { "measureX", { std::nullopt, "BV" } },
    { "measureX", { std::nullopt, "WQ" } },
    { "measureY", { std::nullopt, "BQ" } },
    { "measureY", { std::nullopt, "WV" } },
    { "measureY", { std::nullopt, "BV" } },
    { "measureY", { std::nullopt, "WQ" } },
    { "measureZ", { std::nullopt, "BQ" } },
    { "measureZ", { std::nullopt, "WV" } },
    { "measureZ", { std::nullopt, "BV" } },
    { "measureZ", { std::nullopt, "WQ" } },
    { "measure", { "fff", "BQ" } },
    { "measure", { "fff", "WV" } },
    { "measure", { "fff", "BV" } },
    { "measure", { "fff", "WQ" } },
    { "reset", { std::nullopt, "Q" } },
    { "reset", { std::nullopt, "V" } },
    { "init", { std::nullopt, "Q" } },
    { "init", { std::nullopt, "V" } },
    { "barrier", { std::nullopt, "Q" } },
    { "barrier", { std::nullopt, "V" } },
    { "wait", { "i", "Q" } },
    { "wait", { "i", "V" } },
}
, gate_modifier_map{
    { "inv", std::nullopt },
    { "pow", "f" },
    { "ctrl", std::nullopt },
}
, single_qubit_named_gate_list{
    "H", "I", "mX90", "mY90", "mZ90", "Rn", "Rx", "Ry", "Rz", "S", "Sdag", "T", "Tdag", "U", "X", "X90", "Y", "Y90", "Z", "Z90"
}
, two_qubit_named_gate_list{
    "CNOT", "CR", "CRk", "CV", "CY", "CZ", "DCNOT", "ECR", "InvSqrtSWAP", "ISWAP", "M", "MS", "SqrtISWAP", "SqrtSWAP", "SWAP"
}
, three_qubit_named_gate_list{
    "CCNOT", "CCX", "CSWAP"
}
, non_gate_list{
    "measure", "measureX", "measureY", "measureZ", "reset", "init", "barrier", "wait"
}
, measure_list{
    "measure", "measureX", "measureY", "measureZ"
}
{}
// NOLINTEND

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

[[nodiscard]] const InstructionListT& InstructionSet::get_three_qubit_named_gate_list() const {
    return three_qubit_named_gate_list;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_non_gate_list() const {
    return non_gate_list;
}

[[nodiscard]] const InstructionListT& InstructionSet::get_measure_list() const {
    return measure_list;
}

[[nodiscard]] bool InstructionSet::is_single_qubit_named_gate(const std::string& name) const {
    return single_qubit_named_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_named_gate(const std::string& name) const {
    return two_qubit_named_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_three_qubit_named_gate(const std::string& name) const {
    return three_qubit_named_gate_list.contains(name);
}

[[nodiscard]] bool InstructionSet::is_named_gate(const std::string& name) const {
    return is_single_qubit_named_gate(name) || is_two_qubit_named_gate(name) || is_three_qubit_named_gate(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate_composition(const std::string& name) const {
    return name.starts_with(single_qubit_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate_composition(const std::string& name) const {
    return name.starts_with(two_qubit_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_three_qubit_gate_composition(const std::string& name) const {
    return name.starts_with(three_qubit_gate_composition_prefix);
}

[[nodiscard]] bool InstructionSet::is_gate_composition(const std::string& name) const {
    return is_single_qubit_gate_composition(name) || is_two_qubit_gate_composition(name) ||
        is_three_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_single_qubit_gate(const std::string& name) const {
    return is_single_qubit_named_gate(name) || is_single_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_two_qubit_gate(const std::string& name) const {
    return is_two_qubit_named_gate(name) || is_two_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_three_qubit_gate(const std::string& name) const {
    return is_three_qubit_named_gate(name) || is_three_qubit_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_gate(const std::string& name) const {
    return is_named_gate(name) || is_gate_composition(name);
}

[[nodiscard]] bool InstructionSet::is_measure(const std::string& name) const {
    return measure_list.contains(name);
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
    return non_gate_list.contains(name);
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

[[nodiscard]] std::optional<std::string> InstructionSet::get_named_gate_param_types(const std::string& name) const {
    if (const auto& it = named_gate_map.find(name); it != named_gate_map.end()) {
        const auto& pair_param_types_operand_types = it->second;
        return pair_param_types_operand_types.first;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate '{}'", name) };
}

[[nodiscard]] std::optional<std::string> InstructionSet::get_non_gate_param_types(const std::string& name) const {
    if (const auto& it = non_gate_map.find(name); it != non_gate_map.end()) {
        const auto& pair_param_types_operand_types = it->second;
        return pair_param_types_operand_types.first;
    }
    throw error::AnalysisError{ fmt::format("couldn't find non-unitary instruction '{}'", name) };
}

[[nodiscard]] std::optional<std::string> InstructionSet::get_gate_modifier_param_types(const std::string& name) const {
    if (const auto& it = gate_modifier_map.find(name); it != gate_modifier_map.end()) {
        return it->second;
    }
    throw error::AnalysisError{ fmt::format("couldn't find gate modifier '{}'", name) };
}

[[nodiscard]] std::optional<std::string> InstructionSet::get_instruction_param_types(const std::string& name) const {
    if (is_named_gate(name)) {
        return get_named_gate_param_types(name);
    } else if (is_non_gate(name)) {
        return get_non_gate_param_types(name);
    } else if (is_gate_modifier(name)) {
        return get_gate_modifier_param_types(name);
    }
    throw error::AnalysisError{ fmt::format("couldn't find instruction '{}'", name) };
}

[[nodiscard]] std::optional<std::string> InstructionSet::get_non_gate_param_types_with_param_count(
    const std::string& name, size_t param_count) const {
    const auto& range = non_gate_map.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
        const auto& param_types = it->second.first;
        if (!param_types.has_value()) {
            if (param_count == 0) {
                return param_types;
            }
        } else {
            if (param_types->size() == param_count) {
                return param_types;
            }
        }
    }
    return std::nullopt;
}

}  // namespace cqasm::v3x::instruction
