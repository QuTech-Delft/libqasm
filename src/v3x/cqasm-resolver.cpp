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


//---------------//
// VariableTable //
//---------------//

/**
 * Adds a variable.
 */
void VariableTable::add(const std::string &name, const Value &value) {
    table.erase(name);
    table.insert(std::make_pair(name, value));
}

/**
 * Resolves a variable.
 * Throws NameResolutionFailure if no variable by the given name exists.
 */
Value VariableTable::resolve(const std::string &name) const {
    if (auto entry = table.find(name); entry != table.end()) {
        return entry->second->clone();
    }
    throw NameResolutionFailure{ fmt::format("failed to resolve variable '{}'", name) };
}


//----------------------------//
// ConstEvalCoreFunctionTable //
//----------------------------//

ConstEvalCoreFunctionTable::ConstEvalCoreFunctionTable()
: resolver{ std::make_unique<resolver_t>() } {}
ConstEvalCoreFunctionTable::~ConstEvalCoreFunctionTable() = default;
ConstEvalCoreFunctionTable::ConstEvalCoreFunctionTable(const ConstEvalCoreFunctionTable& t)
: resolver{ std::make_unique<resolver_t>(*t.resolver) } {}
ConstEvalCoreFunctionTable::ConstEvalCoreFunctionTable(ConstEvalCoreFunctionTable&& t) noexcept
: resolver{ std::move(t.resolver) } {}
ConstEvalCoreFunctionTable& ConstEvalCoreFunctionTable::operator=(const ConstEvalCoreFunctionTable& t) {
    resolver = std::make_unique<resolver_t>(resolver_t{ *t.resolver });
    return *this;
}
ConstEvalCoreFunctionTable& ConstEvalCoreFunctionTable::operator=(ConstEvalCoreFunctionTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers a function.
 * Matching will be done case-sensitively.
 * The param_types variadic specifies the amount and types of the parameters that
 * this particular overload of the function expects.
 * The implementation of the function can assume that:
 * - the value list it gets is of the right size, and
 * - the values are of the right types.
 *
 * This method does not contain any intelligence to override previously added overloads.
 * However, the overload resolution engine will always use the last applicable overload it finds,
 * so adding does have the effect of overriding.
 */
void ConstEvalCoreFunctionTable::add(const std::string &name, const Types &param_types, const ConstEvalCoreFunction &impl) {
    resolver->add_overload(name, impl, param_types);
}

/**
 * Resolves a function.
 * Throws NameResolutionFailure if no function by the given name exists,
 * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
 * returns the value returned by the function.
 */
Value ConstEvalCoreFunctionTable::resolve(const std::string &name, const Values &args) const {
    // Resolve the function and type-check/promote the argument list.
    auto resolution = resolver->resolve(name, args);

    // Call the function with the type-checked/promoted argument list, and return its result.
    return resolution.first(resolution.second);
}


//-------------------//
// CoreFunctionTable //
//-------------------//

CoreFunctionTable::CoreFunctionTable()
: resolver{ std::make_unique<resolver_t>() } {}
CoreFunctionTable::~CoreFunctionTable() = default;
CoreFunctionTable::CoreFunctionTable(const CoreFunctionTable& t)
: resolver{ std::make_unique<resolver_t>(*t.resolver) } {}
CoreFunctionTable::CoreFunctionTable(CoreFunctionTable&& t) noexcept
: resolver{ std::move(t.resolver) } {}
CoreFunctionTable& CoreFunctionTable::operator=(const CoreFunctionTable& t) {
    resolver = std::make_unique<resolver_t>(resolver_t{ *t.resolver });
    return *this;
}
CoreFunctionTable& CoreFunctionTable::operator=(CoreFunctionTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers a core function type.
 */
void CoreFunctionTable::add(const function::CoreFunction &type) {
    resolver->add_overload(type.name, tree::make<function::CoreFunction>(type), type.param_types);
}

/**
 * Resolves a core function.
 * Throws NameResolutionFailure if no function by the given name exists,
 * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
 * returns the resolved function node.
 * Annotation data, line number information, and the condition still need to be set by the caller.
 */
Value CoreFunctionTable::resolve(const std::string &name, const Values &args) const {
    auto [core_function_ref, promoted_args] = resolver->resolve(name, args);
    auto value_function_ref = tree::make<values::CoreFunctionRef>(core_function_ref);
    return tree::make<values::FunctionCall>(value_function_ref, promoted_args);
}


//---------------//
// FunctionTable //
//---------------//

FunctionTable::FunctionTable()
: resolver{ std::make_unique<OverloadedNameResolver<values::Value>>() } {}
FunctionTable::~FunctionTable() = default;
FunctionTable::FunctionTable(const FunctionTable& t)
: resolver{ std::make_unique<resolver_t>(*t.resolver) } {}
FunctionTable::FunctionTable(FunctionTable&& t) noexcept
: resolver{ std::move(t.resolver) } {}
FunctionTable& FunctionTable::operator=(const FunctionTable& t) {
    resolver = std::make_unique<resolver_t>(resolver_t{ *t.resolver });
    return *this;
}
FunctionTable& FunctionTable::operator=(FunctionTable&& t) noexcept {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers a function.
 * The param_types variadic specifies the amount and types of the parameters that
 * this particular overload of the function expects.
 * value should be of type values::FunctionRef.
 *
 * This method does not contain any intelligence to override previously added overloads.
 * However, the overload resolution engine will always use the last applicable overload it finds,
 * so adding does have the effect of overriding.
 */
void FunctionTable::add(const std::string &name, const Types &param_types, const values::Value &value) {
    resolver->add_overload(name, value, param_types);
}

/**
 * Resolves a function.
 * Throws NameResolutionFailure if no function by the given name exists,
 * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
 * returns the value returned by the function.
 */
Value FunctionTable::resolve(const std::string &name, const Values &args) const {
    auto [value_function_ref, promoted_args] = resolver->resolve(name, args);
    return tree::make<values::FunctionCall>(value_function_ref, promoted_args);
}


//------------------//
// InstructionTable //
//------------------//

InstructionTable::InstructionTable()
: resolver{ std::make_unique<OverloadedNameResolver<instruction::InstructionRef>>() } {}
InstructionTable::~InstructionTable() = default;
InstructionTable::InstructionTable(const InstructionTable& t)
: resolver{ std::make_unique<resolver_t>(*t.resolver) } {}
InstructionTable::InstructionTable(InstructionTable&& t) noexcept
: resolver{ std::move(t.resolver) } {}
InstructionTable& InstructionTable::operator=(const InstructionTable& t) {
    resolver = std::make_unique<resolver_t>(resolver_t{ *t.resolver });
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
    resolver->add_overload(type.name, tree::make<instruction::Instruction>(type), type.param_types);
}

/**
 * Resolves an instruction.
 * Throws NameResolutionFailure if no instruction by the given name exists,
 * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
 * returns the resolved instruction node.
 * Annotation data, line number information, and the condition still need to be set by the caller.
 */
tree::One<semantic::Instruction> InstructionTable::resolve(const std::string &name, const Values &args) const {
    auto [instruction_ref, promoted_args] = resolver->resolve(name, args);
    return tree::make<semantic::Instruction>(instruction_ref, name, promoted_args);
}

} // namespace cqasm::v3x::resolver
