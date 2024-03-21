/** \file
 * This file contains the \ref cqasm::v3x::instruction::Instruction "Instruction" class and support types,
 * each instance representing an instruction (also known as gate) supported by the user.
 */

#pragma once

#include "cqasm-types.hpp"
#include "cqasm-values.hpp"

#include <fmt/ostream.h>
#include <optional>


namespace cqasm::v3x {

namespace instruction {

/**
 * Representation of an available instruction (also known as gate) in the instruction set,
 * without parameters bound to it
 * (note that libqasm cannot match instructions based on which qubits are used; you'll need to do that on your own).
 *
 * A number of these can be registered into libqasm by the program or library using it through
 * \ref cqasm::analyzer::Analyzer::register_instruction(const instruction::Instruction&) "register_instruction",
 * to inform libqasm of the supported instruction set.
 * For each instruction, libqasm needs to know its name, which parameters it expects,
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
    Instruction(std::string name, const std::optional<std::string> &param_types);

    bool operator==(const Instruction& rhs) const;
    inline bool operator!=(const Instruction& rhs) const {
        return !(*this == rhs);
    }
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

} // namespace instruction


namespace primitives {

template <>
void serialize(const instruction::InstructionRef &obj, ::tree::cbor::MapWriter &map);
template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map);

} // namespace primitives

} // namespace cqasm::v3x

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <> struct fmt::formatter<cqasm::v3x::instruction::Instruction> : ostream_formatter {};
