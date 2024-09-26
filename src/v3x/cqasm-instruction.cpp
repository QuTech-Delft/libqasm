/** \file
 * Implementation for \ref include/v3x/cqasm-instruction.hpp "v3x/cqasm-instruction.hpp".
 */

#include "v3x/cqasm-instruction.hpp"
#include "v3x/cqasm-semantic.hpp"

#include <fmt/format.h>


namespace cqasm::v3x::instruction {

bool is_single_qubit_gate(const std::string &name) {
    return name == h_gate_name ||
        name == i_gate_name ||
        name == mx90_gate_name ||
        name == my90_gate_name ||
        name == rx_gate_name ||
        name == ry_gate_name ||
        name == rz_gate_name ||
        name == s_gate_name ||
        name == sdag_gate_name ||
        name == t_gate_name ||
        name == tdag_gate_name ||
        name == x_gate_name ||
        name == y_gate_name ||
        name == z_gate_name ||
        name == x90_gate_name ||
        name == y90_gate_name;
}

bool is_two_qubit_gate(const std::string &name) {
    return name == cnot_gate_name ||
        name == cr_gate_name ||
        name == crk_gate_name ||
        name == cz_gate_name;
}

bool is_gate(const std::string &name) {
    return is_single_qubit_gate(name) || is_two_qubit_gate(name);
}

bool is_single_qubit_modified_gate(const std::string &name) {
    return name.starts_with(instruction::single_qubit_modified_gate_prefix);
}

bool is_two_qubit_modified_gate(const std::string &name) {
    return name.starts_with(instruction::two_qubit_modified_gate_prefix);
}

bool is_modified_gate(const std::string &name) {
    return is_single_qubit_modified_gate(name) || is_two_qubit_modified_gate(name);
}

bool is_single_qubit_unitary_gate(const std::string &name) {
    return is_single_qubit_gate(name) || is_single_qubit_modified_gate(name);
}

bool is_two_qubit_unitary_gate(const std::string &name) {
    return is_two_qubit_gate(name) || is_two_qubit_modified_gate(name);
}

bool is_unitary_gate(const std::string &name) {
    return is_single_qubit_unitary_gate(name) || is_two_qubit_unitary_gate(name);
}

bool is_inv_gate_modifier(const std::string &name) {
    return name == inv_gate_modifier_name;
}

bool is_pow_gate_modifier(const std::string &name) {
    return name == pow_gate_modifier_name;
}

bool is_ctrl_gate_modifier(const std::string &name) {
    return name == ctrl_gate_modifier_name;
}

bool is_single_qubit_gate_modifier(const std::string &name) {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name);
}

bool is_two_qubit_gate_modifier(const std::string &name) {
    return is_ctrl_gate_modifier(name);
}

bool is_gate_modifier(const std::string &name) {
    return is_inv_gate_modifier(name) || is_pow_gate_modifier(name) || is_ctrl_gate_modifier(name);
}

/**
 * Creates a new instruction.
 * param_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
 * If you need more control, you can also manipulate param_types directly.
 */
Instruction::Instruction(std::string name, const std::optional<std::string> &param_types)
: name{ std::move(name) }
, param_types{ types::from_spec(param_types.value_or("")) }
{}

/**
 * Equality operator.
 */
bool Instruction::operator==(const Instruction &rhs) const {
    return name == rhs.name &&param_types == rhs.param_types;
}

/**
 * Stream << overload for instructions.
 */
std::ostream &operator<<(std::ostream &os, const Instruction &instruction) {
    return os << fmt::format("{} {}", instruction.name, instruction.param_types);
}

/**
 * Stream << overload for instruction references.
 */
std::ostream &operator<<(std::ostream &os, const InstructionRef &instruction) {
    return instruction.empty() ? os << "unresolved" : os << *instruction;
}

}  // namespace cqasm::v3x::instruction


namespace cqasm::v3x::primitives {

template <>
void serialize(const instruction::InstructionRef &obj, ::tree::cbor::MapWriter &map) {
    if (obj.empty()) {
        return;
    }
    map.append_string("name", obj->name);
    auto aw = map.append_array("param_types");
    for (const auto &t : obj->param_types) {
        aw.append_binary(::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{ t.get_ptr() }));
    }
    aw.close();
}

template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map) {
    if (!map.count("name")) {
        return {};
    }
    auto ret = tree::make<instruction::Instruction>();
    ret->name = map.at("name").as_string();
    auto ar = map.at("param_types").as_array();
    for (const auto &element : ar) {
        ret->param_types.add(::tree::base::deserialize<types::Node>(element.as_binary()));
    }
    return ret;
}

}  // namespace cqasm::v3x::primitives
