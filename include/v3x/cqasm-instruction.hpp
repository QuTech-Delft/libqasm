/** \file
 * This file contains the \ref cqasm::v3x::instruction::Instruction "Instruction" class and support types,
 * each instance representing an instruction (also known as gate) supported by the user.
 */

#pragma once

#include <fmt/ostream.h>

#include <optional>

#include "cqasm-types.hpp"

namespace cqasm::v3x::instruction {

constexpr const char* cnot_gate_name = "CNOT";
constexpr const char* cr_gate_name = "CR";
constexpr const char* crk_gate_name = "CRk";
constexpr const char* cz_gate_name = "CZ";
constexpr const char* h_gate_name = "H";
constexpr const char* i_gate_name = "I";
constexpr const char* mx90_gate_name = "mX90";
constexpr const char* my90_gate_name = "mY90";
constexpr const char* rx_gate_name = "Rx";
constexpr const char* ry_gate_name = "Ry";
constexpr const char* rz_gate_name = "Rz";
constexpr const char* s_gate_name = "S";
constexpr const char* sdag_gate_name = "Sdag";
constexpr const char* t_gate_name = "T";
constexpr const char* tdag_gate_name = "Tdag";
constexpr const char* x_gate_name = "X";
constexpr const char* y_gate_name = "Y";
constexpr const char* z_gate_name = "Z";
constexpr const char* x90_gate_name = "X90";
constexpr const char* y90_gate_name = "Y90";

// The use of '1q' and '2q' as gate prefixes avoids any possible conflict with user defined gates
// since an identifier cannot start with a number
constexpr const char* single_qubit_modified_gate_prefix = "1q";
constexpr const char* two_qubit_modified_gate_prefix = "2q";

constexpr const char* inv_gate_modifier_name = "inv";
constexpr const char* pow_gate_modifier_name = "pow";
constexpr const char* ctrl_gate_modifier_name = "ctrl";

constexpr const char* measure_instruction_name = "measure";
constexpr const char* reset_instruction_name = "reset";

bool is_one_qubit_gate(const std::string &name);
bool is_two_qubit_gate(const std::string &name);
bool is_gate(const std::string &name);
bool is_single_qubit_modified_gate(const std::string &name);
bool is_two_qubit_modified_gate(const std::string &name);
bool is_single_qubit_unitary_gate(const std::string &name);
bool is_two_qubit_unitary_gate(const std::string &name);
bool is_unitary_gate(const std::string &name);

bool is_inv_gate_modifier(const std::string &name);
bool is_pow_gate_modifier(const std::string &name);
bool is_ctrl_gate_modifier(const std::string &name);
bool is_single_qubit_gate_modifier(const std::string &name);
bool is_two_qubit_gate_modifier(const std::string &name);
bool is_gate_modifier(const std::string &name);

/**
 * Representation of an available instruction in the instruction set.
 * An instruction can be: a gate, a gate modifier, a measure instruction, or a reset instruction.
 *
 * A number of these can be registered into libqasm by the program or library using it through
 * \ref cqasm::analyzer::Analyzer::register_instruction(const instruction::Instruction&) "register_instruction",
 * to inform libqasm of the supported instruction set.
 * For each instruction, libqasm needs to know its name, which parameters it expects, its return type,
 * and a few extra flags in order to be able to resolve the instruction and check for errors in the cQASM file.
 * The resolved instruction type is part of the cqasm::semantic::Instruction node present in the semantic tree
 * returned through the parse result structure.
 *
 * Note that it is legal to have multiple instructions with the same name,
 * as long as they can be distinguished through their parameter types (i.e. instructions can be overloaded).
 */
class Instruction : public tree::Base {
public:
    /**
     * The name of the instruction. Names are matched case sensitively.
     */
    std::string name;

    /**
     * The vector of parameter types that this instruction expects.
     */
    types::Types param_types;

    /**
     * Creates a new instruction.
     * param_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
     * If you need more control, you can also manipulate param_types directly.
     */
    Instruction() = default;
    Instruction(std::string name, const std::optional<std::string> &param_types);

    bool operator==(const Instruction &rhs) const;
    inline bool operator!=(const Instruction &rhs) const { return !(*this == rhs); }
};

/**
 * Optional reference to an instruction, used within the semantic tree.
 */
using InstructionRef = tree::Maybe<Instruction>;

/**
 * Stream << overload for instructions.
 */
std::ostream &operator<<(std::ostream &os, const Instruction &instruction);

/**
 * Stream << overload for instruction references.
 */
std::ostream &operator<<(std::ostream &os, const InstructionRef &instruction);

}  // namespace cqasm::v3x::instruction


namespace cqasm::v3x::primitives {

template <>
void serialize(const instruction::InstructionRef &obj, ::tree::cbor::MapWriter &map);
template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map);

}  // namespace cqasm::v3x::primitives


/**
 * std::ostream support via fmt (uses operator<<).
 */
template <>
struct fmt::formatter<cqasm::v3x::instruction::Instruction> : ostream_formatter {};
