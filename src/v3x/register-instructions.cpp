#include "v3x/cqasm-instruction.hpp"  // unitary_1_gate_prefix, unitary_2_gate_prefix
#include "v3x/register-instructions.hpp"

#include <cassert>
#include <fmt/format.h>

namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer *analyzer) {
    assert(analyzer);
    
    // Gates
    analyzer->register_instruction("CNOT", "QQ");
    analyzer->register_instruction("CNOT", "QV");
    analyzer->register_instruction("CNOT", "VQ");
    analyzer->register_instruction("CNOT", "VV");
    analyzer->register_instruction("CR", "QQf");
    analyzer->register_instruction("CR", "QVf");
    analyzer->register_instruction("CR", "VQf");
    analyzer->register_instruction("CR", "VVf");
    analyzer->register_instruction("CRk", "QQi");
    analyzer->register_instruction("CRk", "QVi");
    analyzer->register_instruction("CRk", "VQi");
    analyzer->register_instruction("CRk", "VVi");
    analyzer->register_instruction("CZ", "QQ");
    analyzer->register_instruction("CZ", "QV");
    analyzer->register_instruction("CZ", "VQ");
    analyzer->register_instruction("CZ", "VV");
    analyzer->register_instruction("H", "Q");
    analyzer->register_instruction("H", "V");
    analyzer->register_instruction("I", "Q");
    analyzer->register_instruction("I", "V");
    analyzer->register_instruction("mX90", "Q");
    analyzer->register_instruction("mX90", "V");
    analyzer->register_instruction("mY90", "Q");
    analyzer->register_instruction("mY90", "V");
    analyzer->register_instruction("Rx", "Qf");
    analyzer->register_instruction("Rx", "Vf");
    analyzer->register_instruction("Ry", "Qf");
    analyzer->register_instruction("Ry", "Vf");
    analyzer->register_instruction("Rz", "Qf");
    analyzer->register_instruction("Rz", "Vf");
    analyzer->register_instruction("S", "Q");
    analyzer->register_instruction("S", "V");
    analyzer->register_instruction("Sdag", "Q");
    analyzer->register_instruction("Sdag", "V");
    analyzer->register_instruction("T", "Q");
    analyzer->register_instruction("T", "V");
    analyzer->register_instruction("Tdag", "Q");
    analyzer->register_instruction("Tdag", "V");
    analyzer->register_instruction("X", "Q");
    analyzer->register_instruction("X", "V");
    analyzer->register_instruction("X90", "Q");
    analyzer->register_instruction("X90", "V");
    analyzer->register_instruction("Y", "Q");
    analyzer->register_instruction("Y", "V");
    analyzer->register_instruction("Y90", "Q");
    analyzer->register_instruction("Y90", "V");
    analyzer->register_instruction("Z", "Q");
    analyzer->register_instruction("Z", "V");

    // Single-qubit unitary gates, e.g., inv(X), pow(X, 2), or pow(inv(X), 2)
    analyzer->register_instruction(fmt::format("{}_H", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_H", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_I", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_I", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_1_gate_prefix), "Qf");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_1_gate_prefix), "Vf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_1_gate_prefix), "Qf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_1_gate_prefix), "Vf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_1_gate_prefix), "Qf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_1_gate_prefix), "Vf");
    analyzer->register_instruction(fmt::format("{}_S", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_S", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_T", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_T", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_X", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_X", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_1_gate_prefix), "V");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_1_gate_prefix), "Q");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_1_gate_prefix), "V");

    // Two-qubit unitary gates, e.g., ctrl(X), ctrl(inv(X)), ctrl(pow(X, 2)), or ctrl(pow(inv(X), 2))
    analyzer->register_instruction(fmt::format("{}_H", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_H", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_H", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_H", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_I", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_I", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_I", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_I", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_mX90", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_mY90", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_2_gate_prefix), "QQf");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_2_gate_prefix), "QVf");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_2_gate_prefix), "VQf");
    analyzer->register_instruction(fmt::format("{}_Rx", unitary_2_gate_prefix), "VVf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_2_gate_prefix), "QQf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_2_gate_prefix), "QVf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_2_gate_prefix), "VQf");
    analyzer->register_instruction(fmt::format("{}_Ry", unitary_2_gate_prefix), "VVf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_2_gate_prefix), "QQf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_2_gate_prefix), "QVf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_2_gate_prefix), "VQf");
    analyzer->register_instruction(fmt::format("{}_Rz", unitary_2_gate_prefix), "VVf");
    analyzer->register_instruction(fmt::format("{}_S", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_S", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_S", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_S", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_Sdag", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_T", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_T", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_T", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_T", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_Tdag", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_X", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_X", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_X", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_X", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_X90", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_Y", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_Y90", unitary_2_gate_prefix), "VV");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_2_gate_prefix), "QQ");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_2_gate_prefix), "QV");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_2_gate_prefix), "VQ");
    analyzer->register_instruction(fmt::format("{}_Z", unitary_2_gate_prefix), "VV");

    analyzer->register_instruction("measure", "BQ");
    analyzer->register_instruction("measure", "WV");
    analyzer->register_instruction("measure", "BV");
    analyzer->register_instruction("measure", "WQ");
    analyzer->register_instruction("reset", std::nullopt);
    analyzer->register_instruction("reset", "Q");
    analyzer->register_instruction("reset", "V");
}

}  // namespace cqasm::v3x::instruction
