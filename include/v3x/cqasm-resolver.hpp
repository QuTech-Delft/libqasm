/** \file
 * Contains \ref cqasm::v3x::resolver::MappingTable "MappingTable",
 * \ref cqasm::v3x::resolver::ConstEvalCoreFunctionTable "ConstEvalCoreFunctionTable", and
 * \ref cqasm::v3x::resolver::ErrorModelTable "ErrorModelTable", representing the
 * various cQASM namespaces and their members in scope at some instant.
 */

#pragma once

#include "cqasm-core-function.hpp"
#include "cqasm-error.hpp"
#include "cqasm-instruction.hpp"
#include "cqasm-overload.hpp"
#include "cqasm-semantic.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <functional>
#include <memory>
#include <unordered_map>


/**
 * Namespace for everything to do with name and overload resolution in cQASM.
 */
namespace cqasm::v3x::resolver {

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
struct OverloadedNameResolver : public cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::ValueBase> {
    virtual ~OverloadedNameResolver() = default;

    void add_overload(const std::string &name, const T &tag, const types::Types &param_types) override {
        cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::ValueBase>::add_overload(name, tag, param_types);
    }

    [[nodiscard]] std::pair<T, values::Values> resolve(const std::string &name, const values::Values &args) override {
        try {
            return cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::ValueBase>::resolve(name, args);
        } catch (const cqasm::overload::NameResolutionFailure &) {
            throw NameResolutionFailure{ fmt::format("failed to resolve '{}'", name) };
        } catch (const cqasm::overload::OverloadResolutionFailure &) {
            throw OverloadResolutionFailure{
                fmt::format("failed to resolve overload for '{}' with argument pack {}",
                    name, values::types_of(args)) };
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
    std::unordered_map<std::string, values::Value> table;

public:
    /**
     * Adds a variable.
     */
    void add(const std::string &name, const values::Value &value);

    /**
     * Resolves a variable.
     * Throws NameResolutionFailure if no variable by the given name exists.
     */
    values::Value resolve(const std::string &name) const;
};


//----------------------------//
// ConstEvalCoreFunctionTable //
//----------------------------//

/**
 * An overload of a function supported by the language, and that can can be evaluated at compile time.
 * This has to be a function accepting only constant arguments.
 */
using ConstEvalCoreFunction = std::function<values::Value(const values::Values&)>;

/**
 * Table of overloads of functions supported by the language, and that can be evaluated at compile time.
 */
class ConstEvalCoreFunctionTable {
    using resolver_t = OverloadedNameResolver<ConstEvalCoreFunction>;

    std::unique_ptr<resolver_t> resolver;

public:
    ConstEvalCoreFunctionTable();
    ~ConstEvalCoreFunctionTable();
    ConstEvalCoreFunctionTable(const ConstEvalCoreFunctionTable& t);
    ConstEvalCoreFunctionTable(ConstEvalCoreFunctionTable&& t) noexcept;
    ConstEvalCoreFunctionTable& operator=(const ConstEvalCoreFunctionTable& t);
    ConstEvalCoreFunctionTable& operator=(ConstEvalCoreFunctionTable&& t) noexcept;

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
    void add(const std::string &name, const types::Types &param_types, const ConstEvalCoreFunction &impl);

    /**
     * Resolves a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
     * returns the value returned by the function.
     */
    [[nodiscard]] values::Value resolve(const std::string &name, const values::Values &args) const;
};


//-------------------//
// CoreFunctionTable //
//-------------------//

/**
 * Table of overloads of functions supported functions, and that cannot be evaluated at compile time.
 * This has to be a function accepting at least one variable argument.
 */
class CoreFunctionTable {
    using resolver_t = OverloadedNameResolver<function::CoreFunctionRef>;

    std::unique_ptr<resolver_t> resolver;

public:
    CoreFunctionTable();
    ~CoreFunctionTable();
    CoreFunctionTable(const CoreFunctionTable& t);
    CoreFunctionTable(CoreFunctionTable&& t) noexcept;
    CoreFunctionTable& operator=(const CoreFunctionTable& t);
    CoreFunctionTable& operator=(CoreFunctionTable&& t) noexcept;

    /**
     * Registers a core function type.
     */
    void add(const function::CoreFunction &type);

    /**
     * Resolves a core function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
     * returns the resolved function node.
     */
    [[nodiscard]] values::Value resolve(const std::string &name, const values::Values &args) const;
};


//---------------//
// FunctionTable //
//---------------//

/**
 * Table of overloads of functions defined by the user in the cQASM file.
 */
class FunctionTable {
    using resolver_t = OverloadedNameResolver<values::Value>;

    std::unique_ptr<resolver_t> resolver;

public:
    FunctionTable();
    ~FunctionTable();
    FunctionTable(const FunctionTable& t);
    FunctionTable(FunctionTable&& t) noexcept;
    FunctionTable& operator=(const FunctionTable& t);
    FunctionTable& operator=(FunctionTable&& t) noexcept;

    /**
     * Registers a function.
     * Matching will be done case-sensitively.
     * The param_types variadic specifies the amount and types of the parameters that
     * this particular overload of the function expects.
     * value should be of type values::FunctionRef.
     *
     * This method does not contain any intelligence to override previously added overloads.
     * However, the overload resolution engine will always use the last applicable overload it finds,
     * so adding does have the effect of overriding.
     */
    void add(const std::string &name, const types::Types &param_types, const values::Value &value);

    /**
     * Resolves a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
     * returns the value returned by the function.
     */
    [[nodiscard]] values::Value resolve(const std::string &name, const values::Values &args) const;
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
    InstructionTable(const InstructionTable& t);
    InstructionTable(InstructionTable&& t) noexcept;
    InstructionTable& operator=(const InstructionTable& t);
    InstructionTable& operator=(InstructionTable&& t) noexcept;

    /**
     * Registers an instruction type.
     */
    void add(const instruction::Instruction &type);

    /**
     * Resolves an instruction.
     * Throws NameResolutionFailure if no instruction by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
     * returns the resolved instruction node.
     */
    [[nodiscard]] tree::One<semantic::Instruction> resolve(const std::string &name, const values::Values &args) const;
};

} // namespace cqasm::v3x::resolver
