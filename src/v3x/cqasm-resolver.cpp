/** \file
 * Implementation for \ref include/v3x/cqasm-resolver.hpp "v3x/cqasm-resolver.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-utils.hpp"
#include "v3x/cqasm-resolver.hpp"

#include <memory>
#include <unordered_map>


namespace cqasm::v3x::resolver {

using Type = types::Type;
using Types = types::Types;
using Value = values::Value;
using Values = values::Values;

InstructionTable::InstructionTable()
: resolver(std::make_unique<OverloadedNameResolver<instruction::Instruction>>()) {}
InstructionTable::~InstructionTable() = default;
InstructionTable::InstructionTable(const InstructionTable& t)
: resolver(std::make_unique<OverloadedNameResolver<instruction::Instruction>>(*t.resolver)) {}
InstructionTable::InstructionTable(InstructionTable&& t) noexcept
: resolver(std::move(t.resolver)) {}
InstructionTable& InstructionTable::operator=(const InstructionTable& t) {
    resolver = std::make_unique<OverloadedNameResolver<instruction::Instruction>>(
        OverloadedNameResolver<instruction::Instruction>(*t.resolver));
    return *this;
}
InstructionTable& InstructionTable::operator=(InstructionTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers an instruction type.
 */
void InstructionTable::add(const instruction::Instruction &type) {
    resolver->add_overload(type.name, type, type.param_types);
}

/**
 * Resolves an instruction.
 * Throws NameResolutionFailure if no instruction by the given name exists,
 * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
 * returns the resolved instruction node.
 * Annotation data, line number information, and the condition still need to be set by the caller.
 */
tree::One<semantic::Instruction> InstructionTable::resolve(
    const std::string &name,
    const Values &args
) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::Instruction>(
        tree::make<instruction::Instruction>(resolved.first),
        name);
}

} // namespace cqasm::v3x::resolver
