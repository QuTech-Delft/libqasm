/** \file
 * This file contains the \ref cqasm::v1::instruction::Instruction "Instruction"
 * class and support types, each instance representing an instruction
 * (also known as gate) supported by the user.
 */

#pragma once

#include "cqasm-v1-types.hpp"
#include "cqasm-v1-values.hpp"

namespace cqasm {
namespace v1 {

/**
 * Namespace for the \ref instruction::Instruction "Instruction" class and
 * helper types.
 */
namespace instruction {

/**
 * Representation of an available instruction (also known as gate) in the
 * instruction set, without parameters bound to it (note that libqasm cannot
 * match instructions based on which qubits are used; you'll need to do that on
 * your own).
 *
 * A number of these can be registered into libqasm by the program or library
 * using it through \ref cqasm::analyzer::Analyzer::register_instruction(const instruction::Instruction&)
 * "register_instruction()", to inform libqasm of the supported instruction set.
 * For each instruction, libqasm needs to know its name, which parameters it
 * expects, and a few extra flags in order to be able to resolve the
 * instruction and check for errors in the cQASM file. The resolved instruction
 * type is part of the cqasm::semantic::Instruction node present in the
 * semantic tree returned through the parse result structure.
 *
 * Note that it is legal to have multiple instructions with the same name, as
 * long as they can be distinguished through their parameter types (i.e.
 * instructions can be overloaded).
 *
 * You can add any data you like to these through the
 * \ref cqasm::annotatable::Annotatable "Annotatable" interface
 * for your own bookkeeping, so you don't have to maintain an additional map
 * from this error model structure to your own internal structure if you're
 * okay with using this one.
 */
class Instruction : public tree::Base {
public:

    /**
     * The name of the instruction. Names are matched case insensitively.
     */
    std::string name;

    /**
     * The vector of parameter types that this instruction expects.
     */
    types::Types param_types;

    /**
     * Whether this instruction supports conditional execution by means of the
     * c- notation. This is normally true.
     */
    bool allow_conditional;

    /**
     * Whether this instruction can be used in a bundle. This is normally true.
     */
    bool allow_parallel;

    /**
     * Whether to allow usage of the same qubit in different arguments. This is
     * normally false, as this makes no sense in QM, in which case libqasm will
     * report an error to the user if a qubit is reused. Setting this to true
     * just disables that check.
     */
    bool allow_reused_qubits;

    /**
     * Whether different index sizes are allowed. This is normally false, as
     * index lists are normally used to designate parallel instructions, and it
     * makes no sense for the lists to mismatch in that case.
     */
    bool allow_different_index_sizes;

    /**
     * Creates a new instruction. param_types is a shorthand type specification
     * string as parsed by cqasm::types::from_spec(). If you need more control,
     * you can also manipulate param_types directly.
     *
     * allow_conditional specifies whether the instruction can be made
     * conditional with c- notation. allow_parallel specifies whether it may
     * appear bundled with other instructions. allow_reused_qubits specifies
     * whether it is legal for the instruction to use a qubit more than once in
     * its parameter list. allow_different_index_sizes specifies whether it's
     * legal to have different "index sizes" for different parameters, for
     * instance q[1,2] in one parameter and q[1,2,3,4,5] in another.
     */
    explicit Instruction(
        const std::string &name,
        const std::string &param_types = "",
        bool allow_conditional = true,
        bool allow_parallel = true,
        bool allow_reused_qubits = false,
        bool allow_different_index_sizes = false
    );

    /**
     * Equality operator.
     */
    bool operator==(const Instruction& rhs) const;

    /**
     * Inequality operator.
     */
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
std::ostream &operator<<(std::ostream &os, const Instruction &insn);

/**
 * Stream << overload for instruction references.
 */
std::ostream &operator<<(std::ostream &os, const InstructionRef &insn);

} // namespace instruction

namespace primitives {

template <>
void serialize(const instruction::InstructionRef &obj, ::tree::cbor::MapWriter &map);
template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map);

} // namespace primitives
} // namespace v1
} // namespace cqasm
