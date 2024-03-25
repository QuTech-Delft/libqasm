#include "v3x/register-instructions.hpp"

#include <cassert>


namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer *analyzer) {
    assert(analyzer);

    analyzer->register_instruction("cnot", "QQ");
    analyzer->register_instruction("cnot", "QV");
    analyzer->register_instruction("cnot", "VQ");
    analyzer->register_instruction("cnot", "VV");
    analyzer->register_instruction("cr", "QQf");
    analyzer->register_instruction("cr", "QVf");
    analyzer->register_instruction("cr", "VQf");
    analyzer->register_instruction("cr", "VVf");
    analyzer->register_instruction("crk", "QQi");
    analyzer->register_instruction("crk", "QVi");
    analyzer->register_instruction("crk", "VQi");
    analyzer->register_instruction("crk", "VVi");
    analyzer->register_instruction("cz", "QQ");
    analyzer->register_instruction("cz", "QV");
    analyzer->register_instruction("cz", "VQ");
    analyzer->register_instruction("cz", "VV");
    analyzer->register_instruction("h", "Q");
    analyzer->register_instruction("h", "V");
    analyzer->register_instruction("i", "Q");
    analyzer->register_instruction("measure", "BQ");
    analyzer->register_instruction("measure", "WV");
    analyzer->register_instruction("measure", "BV");
    analyzer->register_instruction("measure", "WQ");
    analyzer->register_instruction("mx90", "Q");
    analyzer->register_instruction("mx90", "V");
    analyzer->register_instruction("my90", "Q");
    analyzer->register_instruction("my90", "V");
    analyzer->register_instruction("rx", "Qf");
    analyzer->register_instruction("rx", "Vf");
    analyzer->register_instruction("ry", "Qf");
    analyzer->register_instruction("ry", "Vf");
    analyzer->register_instruction("rz", "Qf");
    analyzer->register_instruction("rz", "Vf");
    analyzer->register_instruction("s", "Q");
    analyzer->register_instruction("s", "V");
    analyzer->register_instruction("sdag", "Q");
    analyzer->register_instruction("sdag", "V");
    analyzer->register_instruction("x", "Q");
    analyzer->register_instruction("x", "V");
    analyzer->register_instruction("x90", "Q");
    analyzer->register_instruction("x90", "V");
    analyzer->register_instruction("y", "Q");
    analyzer->register_instruction("y", "V");
    analyzer->register_instruction("y90", "Q");
    analyzer->register_instruction("y90", "V");
    analyzer->register_instruction("z", "Q");
    analyzer->register_instruction("z", "V");
}

} // namespace cqasm::v3x::instruction
