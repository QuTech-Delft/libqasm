/** \file
 * Contains \ref cqasm::v3x::resolver::MappingTable "MappingTable",
 * \ref cqasm::v3x::resolver::FunctionTable "FunctionTable", and
 * \ref cqasm::v3x::resolver::ErrorModelTable "ErrorModelTable", representing the
 * various cQASM namespaces and their members in scope at some instant.
 */

#pragma once

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

/**
 * Exception for failed name resolutions.
 */
CQASM_ANALYSIS_ERROR(NameResolutionFailure);

/**
 * Exception for failed overload resolutions.
 */
CQASM_ANALYSIS_ERROR(OverloadResolutionFailure);

template <class T>
struct OverloadedNameResolver : public cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::Node> {
    virtual ~OverloadedNameResolver() = default;

    void add_overload(const std::string &name, const T &tag, const types::Types &param_types) override {
        cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::Node>::add_overload(name, tag, param_types);
    }

    [[nodiscard]] std::pair<T, values::Values> resolve(const std::string &name, const values::Values &args) override {
        try {
            return cqasm::overload::OverloadedNameResolver<T, types::TypeBase, values::Node>::resolve(name, args);
        } catch (const cqasm::overload::NameResolutionFailure &) {
            throw NameResolutionFailure{ fmt::format("failed to resolve instruction '{}'", name) };
        } catch (const cqasm::overload::OverloadResolutionFailure &) {
            throw OverloadResolutionFailure{
                fmt::format("failed to resolve overload for {} with argument pack {}",
                    name, values::types_of(args)) };
        }
    }
};

/**
 * Table of all mappings within a certain scope.
 */
class MappingTable {
    std::unordered_map<std::string, values::Value> table;

public:
    /**
     * Adds a mapping.
     */
    void add(const std::string &name, const values::Value &value);

    /**
     * Resolves a mapping.
     * Throws NameResolutionFailure if no mapping by the given name exists.
     */
    values::Value resolve(const std::string &name) const;

    /**
     * Grants read access to the underlying map.
     */
    const std::unordered_map<std::string, values::Value> &get_table() const;
};

/**
 * C++ function representing (one of the overloads of) a function usable in cQASM constant expressions.
 */
using FunctionImpl = std::function<values::Value(const values::Values&)>;

/**
 * Table of all overloads of all constant propagation functions.
 */
class FunctionTable {
    std::unique_ptr<OverloadedNameResolver<FunctionImpl>> resolver;

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
     * (this particular overload of) the function expects.
     * The C++ implementation of the function can assume that
     * the value list it gets is of the right size and the values are of the right types.
     *
     * This method does not contain any intelligence to override previously added overloads.
     * However, the overload resolution engine will always use the last applicable overload it finds,
     * so adding does have the effect of overriding.
     */
    void add(const std::string &name, const types::Types &param_types, const FunctionImpl &impl);

    /**
     * Calls a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
     * returns the value returned by the function.
     */
    [[nodiscard]] values::Value call(const std::string &name, const values::Values &args) const;
};

/**
 * Table of the supported instructions and their overloads.
 */
class InstructionTable {
    std::unique_ptr<OverloadedNameResolver<instruction::Instruction>> resolver;

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
    [[nodiscard]] tree::One<semantic::Instruction> resolve(
        const std::string &name,
        const values::Values &args
    ) const;
};

} // namespace cqasm::v3x::resolver
