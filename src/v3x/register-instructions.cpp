#include "v3x/register-instructions.hpp"

#include <cassert>

namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer *analyzer) {
    assert(analyzer);
    
    analyzer->register_instruction("CNOT", "QQ", 'G');
    analyzer->register_instruction("CNOT", "QV", 'G');
    analyzer->register_instruction("CNOT", "VQ", 'G');
    analyzer->register_instruction("CNOT", "VV", 'G');
    analyzer->register_instruction("CR", "QQf", 'G');
    analyzer->register_instruction("CR", "QVf", 'G');
    analyzer->register_instruction("CR", "VQf", 'G');
    analyzer->register_instruction("CR", "VVf", 'G');
    analyzer->register_instruction("CRk", "QQi", 'G');
    analyzer->register_instruction("CRk", "QVi", 'G');
    analyzer->register_instruction("CRk", "VQi", 'G');
    analyzer->register_instruction("CRk", "VVi", 'G');
    analyzer->register_instruction("CZ", "QQ", 'G');
    analyzer->register_instruction("CZ", "QV", 'G');
    analyzer->register_instruction("CZ", "VQ", 'G');
    analyzer->register_instruction("CZ", "VV", 'G');
    analyzer->register_instruction("H", "Q", 'G');
    analyzer->register_instruction("H", "V", 'G');
    analyzer->register_instruction("I", "Q", 'G');
    analyzer->register_instruction("I", "V", 'G');
    analyzer->register_instruction("mX90", "Q", 'G');
    analyzer->register_instruction("mX90", "V", 'G');
    analyzer->register_instruction("mY90", "Q", 'G');
    analyzer->register_instruction("mY90", "V", 'G');
    analyzer->register_instruction("Rx", "Qf", 'G');
    analyzer->register_instruction("Rx", "Vf", 'G');
    analyzer->register_instruction("Ry", "Qf", 'G');
    analyzer->register_instruction("Ry", "Vf", 'G');
    analyzer->register_instruction("Rz", "Qf", 'G');
    analyzer->register_instruction("Rz", "Vf", 'G');
    analyzer->register_instruction("S", "Q", 'G');
    analyzer->register_instruction("S", "V", 'G');
    analyzer->register_instruction("Sdag", "Q", 'G');
    analyzer->register_instruction("Sdag", "V", 'G');
    analyzer->register_instruction("T", "Q", 'G');
    analyzer->register_instruction("T", "V", 'G');
    analyzer->register_instruction("Tdag", "Q", 'G');
    analyzer->register_instruction("Tdag", "V", 'G');
    analyzer->register_instruction("X", "Q", 'G');
    analyzer->register_instruction("X", "V", 'G');
    analyzer->register_instruction("X90", "Q", 'G');
    analyzer->register_instruction("X90", "V", 'G');
    analyzer->register_instruction("Y", "Q", 'G');
    analyzer->register_instruction("Y", "V", 'G');
    analyzer->register_instruction("Y90", "Q", 'G');
    analyzer->register_instruction("Y90", "V", 'G');
    analyzer->register_instruction("Z", "Q", 'G');
    analyzer->register_instruction("Z", "V", 'G');

    analyzer->register_instruction("measure", "BQ", 'n');
    analyzer->register_instruction("measure", "WV", 'n');
    analyzer->register_instruction("measure", "BV", 'n');
    analyzer->register_instruction("measure", "WQ", 'n');
    analyzer->register_instruction("reset", std::nullopt, 'n');
    analyzer->register_instruction("reset", "Q", 'n');
    analyzer->register_instruction("reset", "V", 'n');

    analyzer->register_instruction("inv", "G", 'G');
    analyzer->register_instruction("pow", "fG", 'G');
    analyzer->register_instruction("ctrl", "QG", 'G');
}

}  // namespace cqasm::v3x::instruction
