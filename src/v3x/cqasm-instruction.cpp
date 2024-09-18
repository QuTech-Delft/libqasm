/** \file
 * Implementation for \ref include/v3x/cqasm-instruction.hpp "v3x/cqasm-instruction.hpp".
 */

#include "v3x/cqasm-instruction.hpp"
#include "v3x/cqasm-semantic.hpp"

#include <fmt/format.h>


namespace cqasm::v3x::instruction {

/**
 * Creates a new instruction.
 * param_types is a shorthand type specification string as parsed by cqasm::types::from_spec().
 * return_type is a shorthand type specification character as parsed by cqasm::types::from_spec().
 * If you need more control, you can also manipulate param_types directly.
 */
Instruction::Instruction(std::string name, const std::optional<std::string> &param_types, const char &return_type)
: name{ std::move(name) }
, param_types{ types::from_spec(param_types.value_or("")) }
, return_type{ types::from_spec(return_type) }
{}

/**
 * Equality operator.
 */
bool Instruction::operator==(const Instruction &rhs) const {
    return name == rhs.name &&
        param_types == rhs.param_types &&
        return_type == rhs.return_type;
}

/**
 * Stream << overload for instructions.
 */
std::ostream &operator<<(std::ostream &os, const Instruction &instruction) {
    return os << fmt::format("{}({}) -> {}", instruction.name, instruction.param_types, instruction.return_type);
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
    map.append_string("n", obj->name);
    auto aw = map.append_array("pt");
    for (const auto &t : obj->param_types) {
        aw.append_binary(::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{ t.get_ptr() }));
    }
    aw.close();
    map.append_binary("rt", ::tree::base::serialize(::tree::base::Maybe<types::TypeBase>{
        obj->return_type.get_ptr() }));
}

template <>
instruction::InstructionRef deserialize(const ::tree::cbor::MapReader &map) {
    if (!map.count("n")) {
        return {};
    }
    auto ret = tree::make<instruction::Instruction>(map.at("n").as_string(), "", 'n');
    auto ar = map.at("pt").as_array();
    for (const auto &element : ar) {
        ret->param_types.add(::tree::base::deserialize<types::Node>(element.as_binary()));
    }
    ret->return_type = ::tree::base::deserialize<types::Node>(map.at("rt").as_binary());
    return ret;
}

}  // namespace cqasm::v3x::primitives
