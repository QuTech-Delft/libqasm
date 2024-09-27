#include "v3x/cqasm-instruction-set.hpp"

namespace cqasm::v3x::instruction {

InstructionSet::InstructionSet()
: gate_map{
    { "CNOT", "QQ" },
    { "CNOT", "QV" },
    { "CNOT", "VQ" },
    { "CNOT", "VV" },
    { "CR", "QQf" },
    { "CR", "QVf" },
    { "CR", "VQf" },
    { "CR", "VVf" },
    { "CRk", "QQi" },
    { "CRk", "QVi" },
    { "CRk", "VQi" },
    { "CRk", "VVi" },
    { "CZ", "QQ" },
    { "CZ", "QV" },
    { "CZ", "VQ" },
    { "CZ", "VV" },
    { "H", "Q" },
    { "H", "V" },
    { "I", "Q" },
    { "I", "V" },
    { "mX90", "Q" },
    { "mX90", "V" },
    { "mY90", "Q" },
    { "mY90", "V" },
    { "Rx", "Qf" },
    { "Rx", "Vf" },
    { "Ry", "Qf" },
    { "Ry", "Vf" },
    { "Rz", "Qf" },
    { "Rz", "Vf" },
    { "S", "Q" },
    { "S", "V" },
    { "Sdag", "Q" },
    { "Sdag", "V" },
    { "T", "Q" },
    { "T", "V" },
    { "Tdag", "Q" },
    { "Tdag", "V" },
    { "X", "Q" },
    { "X", "V" },
    { "X90", "Q" },
    { "X90", "V" },
    { "Y", "Q" },
    { "Y", "V" },
    { "Y90", "Q" },
    { "Y90", "V" },
    { "Z", "Q" },
    { "Z", "V" }
}
, non_gate_map{
    { "measure", "BQ" },
    { "measure", "WV" },
    { "measure", "BV" },
    { "measure", "WQ" },
    { "reset", std::nullopt },
    { "reset", "Q" },
    { "reset", "V" }
}
, single_qubit_gate_list{
    "H", "I", "mX90", "mY90", "Rx", "Ry", "Rz", "S", "Sdag", "T", "Tdag", "X", "X90", "Y", "Y90", "Z"
}
, two_qubit_gate_list{
    "CNOT", "CR", "CRk", "CZ"
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

bool InstructionSet::is_single_qubit_gate(const std::string &name) const {
    return single_qubit_gate_list.contains(name);
}

bool InstructionSet::is_two_qubit_gate(const std::string &name) const {
    return two_qubit_gate_list.contains(name);
}

bool InstructionSet::is_gate(const std::string &name) const {
    return is_single_qubit_gate(name) || is_two_qubit_gate(name);
}

bool InstructionSet::is_single_qubit_modified_gate(const std::string &name) const {
    return name.starts_with(single_qubit_modified_gate_prefix);
}

bool InstructionSet::is_two_qubit_modified_gate(const std::string &name) const {
    return name.starts_with(two_qubit_modified_gate_prefix);
}

bool InstructionSet::is_modified_gate(const std::string &name) const {
    return is_single_qubit_modified_gate(name) || is_two_qubit_modified_gate(name);
}

bool InstructionSet::is_single_qubit_unitary_gate(const std::string &name) const {
    return is_single_qubit_gate(name) || is_single_qubit_modified_gate(name);
}

bool InstructionSet::is_two_qubit_unitary_gate(const std::string &name) const {
    return is_two_qubit_gate(name) || is_two_qubit_modified_gate(name);
}

bool InstructionSet::is_unitary_gate(const std::string &name) const {
    return is_single_qubit_unitary_gate(name) || is_two_qubit_unitary_gate(name);
}

bool InstructionSet::is_inv_gate_modifier(const std::string &name) const {
    return name == inv_gate_modifier_name;
}

bool InstructionSet::is_pow_gate_modifier(const std::string &name) const {
    return name == pow_gate_modifier_name;
}

bool InstructionSet::is_ctrl_gate_modifier(const std::string &name) const {
    return name == ctrl_gate_modifier_name;
}

bool InstructionSet::is_single_qubit_gate_modifier(const std::string &name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name);
}

bool InstructionSet::is_two_qubit_gate_modifier(const std::string &name) const {
    return is_ctrl_gate_modifier(name);
}

bool InstructionSet::is_gate_modifier(const std::string &name) const {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name) || is_ctrl_gate_modifier(name);
}

bool InstructionSet::is_measure_instruction(const std::string &name) const {
    return name == measure_instruction_name;
}

bool InstructionSet::is_reset_instruction(const std::string &name) const {
    return name == reset_instruction_name;
}

}  // namespace cqasm::v3x::instruction
