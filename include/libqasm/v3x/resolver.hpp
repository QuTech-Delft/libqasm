/** \file
 * Contains \ref cqasm::v3x::resolver::VariableTable "VariableTable",
 * \ref cqasm::v3x::resolver::ConstEvalCoreFunctionTable "ConstEvalCoreFunctionTable",
 * \ref cqasm::v3x::resolver::CoreFunctionTable "CoreFunctionTable",
 * \ref cqasm::v3x::resolver::FunctionTable "FunctionTable", and
 * \ref cqasm::v3x::resolver::InstructionTable "InstructionTable", representing the
 * various cQASM namespaces and their members in scope at some instant.
 */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <unordered_map>

#include "libqasm/error.hpp"
#include "libqasm/overload.hpp"
#include "libqasm/v3x/core_function.hpp"
#include "libqasm/v3x/instruction.hpp"
#include "libqasm/v3x/semantic.hpp"

/**
 * Namespace for everything to do with name and overload resolution in cQASM.
 */
namespace cqasm::v3x::resolver {

using Type = types::Type;
using Types = types::Types;
using Value = values::Value;
using Values = values::Values;

//-----------------//
// Analysis errors //
//-----------------//

/**
 * Exception for failed name resolutions.
 */
CQASM_ANALYSIS_ERROR(NameResolutionFailure);

/**
 * Exception for failed overload resolutions.
 */
CQASM_ANALYSIS_ERROR(OverloadResolutionFailure);

/**
 * Exception for failed resolutions.
 */
CQASM_ANALYSIS_ERROR(ResolutionFailure);

//------------------------//
// OverloadedNameResolver //
//------------------------//

template <class T>
using BaseOverladedNameResolver = cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::ValueBase>;

template <class T>
struct OverloadedNameResolver : public BaseOverladedNameResolver<T> {
    virtual ~OverloadedNameResolver() = default;

    void add_overload(const std::string &name, const T &tag, const types::Types &param_types) override {
        BaseOverladedNameResolver<T>::add_overload(name, tag, param_types);
    }

    [[nodiscard]] std::pair<T, Values> resolve(const std::string &name, const Values &args) override {
        try {
            return BaseOverladedNameResolver<T>::resolve(name, args);
        } catch (const cqasm::overload::NameResolutionFailure &) {
            throw NameResolutionFailure{ fmt::format("failed to resolve '{}'", name) };
        } catch (const cqasm::overload::OverloadResolutionFailure &) {
            throw OverloadResolutionFailure{ fmt::format(
                "failed to resolve overload for '{}' with argument pack ({})", name, types_of(args)) };
        }
    }
};

//---------------//
// VariableTable //
//---------------//

/**
 * Table of all variables within a certain scope.
 */
class VariableTable {
    std::unordered_map<std::string, Value> table;

public:
    /**
     * Adds a variable.
     */
    void add(const std::string &name, const Value &value);

    /**
     * Resolves a variable.
     * Throws NameResolutionFailure if no variable by the given name exists.
     */
    [[nodiscard]] Value resolve(const std::string &name) const;
};

//----------------------------//
// ConstEvalCoreFunctionTable //
//----------------------------//

/**
 * An overload of a function supported by the language, and that can can be evaluated at compile time.
 * This has to be a function accepting only constant arguments.
 */
using ConstEvalCoreFunction = std::function<Value(const Values &)>;

/**
 * Table of overloads of functions supported by the language, and that can be evaluated at compile time.
 */
class ConstEvalCoreFunctionTable {
    using resolver_t = OverloadedNameResolver<ConstEvalCoreFunction>;

    std::unique_ptr<resolver_t> resolver;

public:
    ConstEvalCoreFunctionTable();
    ~ConstEvalCoreFunctionTable();
    ConstEvalCoreFunctionTable(const ConstEvalCoreFunctionTable &t);
    ConstEvalCoreFunctionTable(ConstEvalCoreFunctionTable &&t) noexcept;
    ConstEvalCoreFunctionTable &operator=(const ConstEvalCoreFunctionTable &t);
    ConstEvalCoreFunctionTable &operator=(ConstEvalCoreFunctionTable &&t) noexcept;

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
    void add(const std::string &name, const Types &param_types, const ConstEvalCoreFunction &function);

    /**
     * Resolves a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
     * returns the value returned by the function.
     */
    [[nodiscard]] Value resolve(const std::string &name, const Values &args) const;
};

//------------------//
// InstructionTable //
//------------------//

/**
 * Table of overloads of instructions supported by the language.
 */
class InstructionTable {
    using resolver_t = OverloadedNameResolver<instruction::InstructionRef>;

    std::unique_ptr<resolver_t> resolver;

public:
    InstructionTable();
    ~InstructionTable();
    InstructionTable(const InstructionTable &t);
    InstructionTable(InstructionTable &&t) noexcept;
    InstructionTable &operator=(const InstructionTable &t);
    InstructionTable &operator=(InstructionTable &&t) noexcept;

    /**
     * Registers an instruction type.
     */
    void add(const instruction::Instruction &type);

    /**
     * Resolves a GateInstruction type.
     * Throws NameResolutionFailure if no instruction by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
     * returns the resolved instruction node.
     */
    [[nodiscard]] tree::One<semantic::Instruction> resolve(const std::string &name,
        const tree::One<semantic::Gate> &gate, const Values &args) const;

    /**
     * Resolves a NonGateInstruction type.
     * Throws NameResolutionFailure if no instruction by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
     * returns the resolved instruction node.
     */
    [[nodiscard]] tree::One<semantic::Instruction> resolve(const std::string &name, const Values &args) const;
};

}  // namespace cqasm::v3x::resolver
