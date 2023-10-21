/** \file
 * Implementation for \ref include/v1x/cqasm-resolver.hpp "v1x/cqasm-resolver.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-utils.hpp"
#include "v1x/cqasm-resolver.hpp"

#include <memory>
#include <unordered_map>


namespace cqasm::v1x::resolver {

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
    auto entry = table.find(name);
    if (entry == table.end()) {
        throw NameResolutionFailure("failed to resolve " + name);
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

FunctionTable::FunctionTable()
: resolver(std::make_unique<OverloadedNameResolver<FunctionImpl>>()) {}
FunctionTable::~FunctionTable() = default;
FunctionTable::FunctionTable(const FunctionTable& t)
: resolver(std::make_unique<OverloadedNameResolver<FunctionImpl>>(*t.resolver)) {}
FunctionTable::FunctionTable(FunctionTable&& t) noexcept
: resolver(std::move(t.resolver)) {}
FunctionTable& FunctionTable::operator=(const FunctionTable& t) {
    resolver = std::make_unique<OverloadedNameResolver<FunctionImpl>>(
        OverloadedNameResolver<FunctionImpl>(*t.resolver));
    return *this;
}
FunctionTable& FunctionTable::operator=(FunctionTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers a function.
 * Matching will be done case-sensitively.
 * The param_types variadic specifies the amount and types of the parameters that
 * (this particular overload of) the function expects.
 * The C++ implementation of the function can assume that
 * the value list it gets is of the right size and the values are of the right types.
 *
 * This method does not contain any intelligence to override previously added overloads.
 * However, the overload resolution engine will always use the last applicable overload it finds,
 * so adding does have the effect of overriding.
 */
void FunctionTable::add(const std::string &name, const Types &param_types, const FunctionImpl &impl) {
    resolver->add_overload(name, impl, param_types);
}

/**
 * Calls a function.
 * Throws NameResolutionFailure if no function by the given name exists,
 * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
 * returns the value returned by the function.
 */
Value FunctionTable::call(const std::string &name, const Values &args) const {
    // Resolve the function and type-check/promote the argument list.
    auto resolution = resolver->resolve(name, args);

    // Call the function with the type-checked/promoted argument list, and return its result.
    return resolution.first(resolution.second);
}

ErrorModelTable::ErrorModelTable()
: resolver(std::make_unique<OverloadedNameResolver<error_model::ErrorModel>>()) {}
ErrorModelTable::~ErrorModelTable() = default;
ErrorModelTable::ErrorModelTable(const ErrorModelTable& t)
: resolver(std::make_unique<OverloadedNameResolver<error_model::ErrorModel>>(*t.resolver)) {}
ErrorModelTable::ErrorModelTable(ErrorModelTable&& t) noexcept
: resolver(std::move(t.resolver)) {}
ErrorModelTable& ErrorModelTable::operator=(const ErrorModelTable& t) {
    resolver = std::make_unique<OverloadedNameResolver<error_model::ErrorModel>>(
        OverloadedNameResolver<error_model::ErrorModel>(*t.resolver));
    return *this;
}
ErrorModelTable& ErrorModelTable::operator=(ErrorModelTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers an error model.
 */
void ErrorModelTable::add(const error_model::ErrorModel &type) {
    resolver->add_overload(type.name, type, type.param_types);
}

/**
 * Resolves an error model.
 * Throws NameResolutionFailure if no error model by the given name exists,
 * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
 * returns the resolved error model node.
 * Annotation data and line number information still needs to be set by the caller.
 */
tree::One<semantic::ErrorModel> ErrorModelTable::resolve(const std::string &name, const Values &args) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::ErrorModel>(
        tree::make<error_model::ErrorModel>(resolved.first),
        name,
        resolved.second,
        tree::Any<semantic::AnnotationData>());
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
tree::One<semantic::Instruction> InstructionTable::resolve(
    const std::string &name,
    const Values &args
) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::Instruction>(
        tree::make<instruction::Instruction>(resolved.first),
        name,
        values::Value(),
        resolved.second,
        tree::Any<semantic::AnnotationData>());
}

} // namespace cqasm::v1x::resolver
