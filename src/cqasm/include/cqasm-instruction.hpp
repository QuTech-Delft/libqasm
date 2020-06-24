#pragma once

#include "cqasm-types.hpp"
#include "cqasm-values.hpp"

namespace cqasm {
namespace instruction {

/**
 * Representation of an available instruction (a.k.a. gate) in the instruction
 * set, without parameters bound to it (note that libqasm cannot match
 * instructions based on which qubits are used; you'll need to do that on your
 * own). A number of these can be registered into libqasm by the program or
 * library using it, to inform libqasm of the supported instruction set. For
 * each instruction, libqasm needs to know its name, which parameters it
 * expects, and a few extra flags in order to be able to resolve the
 * instruction and check for errors in the cQASM file. The resolved instruction
 * type is part of the cqasm::semantic::Instruction node present in the
 * semantic tree returned through the parse result structure.
 *
 * Note that it is legal to have multiple instructions with the same name, as
 * long as they can be distinguished through their parameter types (i.e.
 * instructions can be overloaded).
 *
 * You can add any data you like to these through the Annotatable interface
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
     * Creates a new instruction. param_types is a shorthand type specification
     * string as parsed by cqasm::types::from_spec(). If you need more control,
     * you can also manipulate param_types directly.
     *
     * allow_conditional specifies whether the instruction can be made
     * conditional with c- notation. allow_parallel specifies whether it may
     * appear bundled with other instructions. allow_reused_qubits specifies
     * whether it is legal for the instruction to use a qubit more than once in
     * its parameter list.
     */
    Instruction(
        const std::string &name,
        const std::string &param_types = "",
        bool allow_conditional = true,
        bool allow_parallel = true,
        bool allow_reused_qubits = false
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

} // namespace instruction
} // namespace cqasm

/**
 * Stream << overload for instructions.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::instruction::Instruction& insn);

/**
 * Stream << overload for instruction references.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::instruction::InstructionRef& insn);
