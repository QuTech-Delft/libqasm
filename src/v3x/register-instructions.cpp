#include "v3x/register-instructions.hpp"

#include <cassert>

namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer *analyzer) {
    assert(analyzer);

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
    analyzer->register_instruction("measure", "BQ");
    analyzer->register_instruction("measure", "WV");
    analyzer->register_instruction("measure", "BV");
    analyzer->register_instruction("measure", "WQ");
    analyzer->register_instruction("mX90", "Q");
    analyzer->register_instruction("mX90", "V");
    analyzer->register_instruction("mY90", "Q");
    analyzer->register_instruction("mY90", "V");
    analyzer->register_instruction("reset", std::nullopt);
    analyzer->register_instruction("reset", "Q");
    analyzer->register_instruction("reset", "V");
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
}

}  // namespace cqasm::v3x::instruction
