#include "v3x/cqasm-instruction-set.hpp"
#include "v3x/register-instructions.hpp"

#include <cassert>
#include <fmt/format.h>

namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer *analyzer) {
    assert(analyzer);

    const auto& instruction_set = InstructionSet::get_instance();
    const auto& gate_map = instruction_set.get_gate_map();
    const auto& non_gate_map = instruction_set.get_non_gate_map();

    // Gates
    for (const auto &[gate_name, pair_params_types_operands_types] : gate_map) {
        const auto &operand_types = pair_params_types_operands_types.second;
        analyzer->register_instruction(gate_name, operand_types);
    };

    // Single-qubit unitary gates, e.g., inv(X), pow(X, 2), or pow(inv(X), 2)
    for (const auto &[gate_name, pair_params_types_operands_types] : gate_map) {
        if (instruction_set.is_single_qubit_gate(gate_name)) {
            const auto &operand_types = pair_params_types_operands_types.second;
            const auto &single_qubit_unitary_gate_name = fmt::format("{}_{}",
                instruction_set.single_qubit_modified_gate_prefix, gate_name);
            analyzer->register_instruction(single_qubit_unitary_gate_name, operand_types);
        }
    };

    // Two-qubit unitary gates, e.g., ctrl(X), ctrl(inv(X)), ctrl(pow(X, 2)), or ctrl(pow(inv(X), 2))
    for (const auto &[gate_name, pair_params_types_operands_types] : gate_map) {
        if (instruction_set.is_single_qubit_gate(gate_name)) {
            // For a single qubit gate such as Ry, the gate map will hold two entries, namely "Ry Qf" and "Ry Vf"
            // For each of those entries, e.g., "Ry Qf", the code below registers 2 two-qubit gates "2q_Ry",
            // each acting on a pair of qubit operands, one on "QQf" and the other on "VQf"
            const auto &operand_types = pair_params_types_operands_types.second;
            assert(operand_types.has_value());
            const auto &operand_types_1 = fmt::format("{}{}", "Q", operand_types.value());
            const auto &operand_types_2 = fmt::format("{}{}", "V", operand_types.value());
            const auto &two_qubit_unitary_gate_name = fmt::format("{}_{}",
                instruction_set.two_qubit_modified_gate_prefix, gate_name);
            analyzer->register_instruction(two_qubit_unitary_gate_name, operand_types_1);
            analyzer->register_instruction(two_qubit_unitary_gate_name, operand_types_2);
        }
    };

    // Non-gate instructions
    for (const auto &[non_gate_name, pair_params_types_operands_types] : non_gate_map) {
        const auto &operand_types = pair_params_types_operands_types.second;
        analyzer->register_instruction(non_gate_name, operand_types);
    };
}

}  // namespace cqasm::v3x::instruction
