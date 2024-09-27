/** \file
 * Implementation for \ref include/v3x/cqasm-instruction.hpp "v3x/cqasm-instruction.hpp".
 */

#include "v3x/cqasm-instruction.hpp"

#include "v3x/cqasm-semantic.hpp"

#include <fmt/format.h>

namespace cqasm::v3x::instruction {

/**
 * Creates a new instruction.
 * operand_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
 * If you need more control, you can also manipulate param_types directly.
 */
Instruction::Instruction(std::string name, const std::optional<std::string> &operand_types)
: name{ std::move(name) }
, operand_types{ types::from_spec(operand_types.value_or("")) }
{}

/**
 * Equality operator.
 */
bool Instruction::operator==(const Instruction &rhs) const {
    return name == rhs.name && operand_types == rhs.operand_types;
}

/**
 * Stream << overload for instructions.
 */
std::ostream &operator<<(std::ostream &os, const Instruction &instruction) {
    return os << fmt::format("{} {}", instruction.name, instruction.operand_types);
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
    auto operand_types_array = map.append_array("operand_types");
    for (const auto &t : obj->operand_types) {
        operand_types_array.append_binary(::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{ t.get_ptr() }));
    }
    operand_types_array.close();
}

template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map) {
    if (!map.count("name")) {
        return {};
    }
    auto ret = tree::make<instruction::Instruction>();
    ret->name = map.at("name").as_string();
    auto operand_types_array = map.at("operand_types").as_array();
    for (const auto &element : operand_types_array) {
        ret->operand_types.add(::tree::base::deserialize<types::Node>(element.as_binary()));
    }
    return ret;
}

}  // namespace cqasm::v3x::primitives
