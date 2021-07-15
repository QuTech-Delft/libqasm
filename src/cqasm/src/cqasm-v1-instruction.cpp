/** \file
 * Implementation for \ref include/cqasm-v1-instruction.hpp "cqasm-v1-instruction.hpp".
 */

#include "cqasm-utils.hpp"
#include "cqasm-v1-instruction.hpp"
#include "cqasm-v1-semantic.hpp"

namespace cqasm {
namespace v1 {
namespace instruction {

/**
 * Creates a new instruction. param_types is a shorthand type specification
 * string as parsed by cqasm::types::from_spec(). If you need more control,
 * you can also manipulate param_types directly.
 *
 * allow_conditional specifies whether the instruction can be made
 * conditional with c- notation. allow_parallel specifies whether it may
 * appear bundled with other instructions. allow_reused_qubits specifies
 * whether it is legal for the instruction to use a qubit more than once in
 * its parameter list. allow_different_index_sizes specifies whether it's legal
 * to have different "index sizes" for different parameters, for instance
 * q[1,2] in one parameter and q[1,2,3,4,5] in another.
 */
Instruction::Instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits,
    bool allow_different_index_sizes
) :
    name(name),
    param_types(types::from_spec(param_types)),
    allow_conditional(allow_conditional),
    allow_parallel(allow_parallel),
    allow_reused_qubits(allow_reused_qubits),
    allow_different_index_sizes(allow_different_index_sizes)
{}

/**
 * Equality operator.
 */
bool Instruction::operator==(const Instruction& rhs) const {
    return utils::case_insensitive_equals(name, rhs.name)
            && param_types == rhs.param_types
            && allow_conditional == rhs.allow_conditional
            && allow_parallel == rhs.allow_parallel
            && allow_reused_qubits == rhs.allow_reused_qubits;
}

/**
 * Stream << overload for instructions.
 */
std::ostream &operator<<(std::ostream &os, const Instruction &insn) {
    os << insn.name << insn.param_types;
    return os;
}

/**
 * Stream << overload for instruction references.
 */
std::ostream &operator<<(std::ostream &os, const InstructionRef &insn) {
    if (insn.empty()) {
        os << "unresolved";
    } else {
        os << *insn;
    }
    return os;
}

} // namespace instruction

namespace primitives {

template <>
void serialize(const instruction::InstructionRef &obj, ::tree::cbor::MapWriter &map) {
    if (obj.empty()) {
        return;
    }
    map.append_string("n", obj->name);
    map.append_bool("c", obj->allow_conditional);
    map.append_bool("p", obj->allow_parallel);
    map.append_bool("r", obj->allow_reused_qubits);
    map.append_bool("d", obj->allow_different_index_sizes);
    auto aw = map.append_array("t");
    for (const auto &t : obj->param_types) {
        aw.append_binary(::tree::base::serialize(t));
    }
    aw.close();
}

template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map) {
    if (!map.count("n")) {
        return {};
    }
    auto insn = tree::make<instruction::Instruction>(
        map.at("n").as_string(),
        "",
        map.at("c").as_bool(),
        map.at("p").as_bool(),
        map.at("r").as_bool(),
        map.at("d").as_bool()
    );
    auto ar = map.at("t").as_array();
    for (size_t i = 0; i < ar.size(); i++) {
        insn->param_types.add(::tree::base::deserialize<types::Node>(ar.at(i).as_binary()));
    }
    return std::move(insn);
}

} // namespace primitives
} // namespace v1
} // namespace cqasm
