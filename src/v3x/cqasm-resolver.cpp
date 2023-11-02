/** \file
 * Implementation for \ref include/v3x/cqasm-resolver.hpp "v3x/cqasm-resolver.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-utils.hpp"
#include "v3x/cqasm-resolver.hpp"

#include <fmt/format.h>
#include <memory>
#include <unordered_map>


namespace cqasm::v3x::resolver {

using Type = types::Type;
using Types = types::Types;
using Value = values::Value;
using Values = values::Values;

/**
 * Adds a mapping.
 */
void MappingTable::add(
    const std::string &name,
    const values::Value &value,
    const tree::Maybe<ast::Mapping> &node
) {
    auto it = table.find(name);
    if (it != table.end()) {
        table.erase(it);
    }
    table.insert(
        std::make_pair(
            name,
            std::pair<const values::Value, tree::Maybe<ast::Mapping>>(value, node)
        )
    );
}

/**
 * Resolves a mapping.
 * Throws NameResolutionFailure if no mapping by the given name exists.
 */
Value MappingTable::resolve(const std::string &name) const {
    if (auto entry = table.find(name); entry == table.end()) {
        throw NameResolutionFailure{ fmt::format("failed to resolve mapping '{}'", name) };
    } else {
        return Value(entry->second.first->clone());
    }
}

/**
 * Grants read access to the underlying map.
 */
const std::unordered_map<std::string, std::pair<const values::Value, tree::Maybe<ast::Mapping>>> &MappingTable::get_table() const {
    return table;
}

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
tree::One<semantic::Instruction> InstructionTable::resolve(const std::string &name, const Values &args) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::Instruction>(
        tree::make<instruction::Instruction>(resolved.first), name, resolved.second);
}

} // namespace cqasm::v3x::resolver
