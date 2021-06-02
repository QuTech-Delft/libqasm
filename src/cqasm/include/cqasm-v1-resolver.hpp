/** \file
 * Contains \ref cqasm::v1::resolver::MappingTable "MappingTable",
 * \ref cqasm::v1::resolver::FunctionTable "FunctionTable", and
 * \ref cqasm::v1::resolver::ErrorModelTable "ErrorModelTable", representing the
 * various cQASM namespaces and their members in scope at some instant.
 */

#pragma once

#include <functional>
#include <algorithm>
#include "cqasm-v1-error-model.hpp"
#include "cqasm-v1-instruction.hpp"
#include "cqasm-v1-semantic.hpp"
#include "cqasm-error.hpp"

namespace cqasm {
namespace v1 {

/**
 * Namespace for everything to do with name and overload resolution in cQASM.
 */
namespace resolver {

/**
 * Exception for failed name resolutions.
 */
CQASM_ANALYSIS_ERROR(NameResolutionFailure);

/**
 * Exception for failed overload resolutions.
 */
CQASM_ANALYSIS_ERROR(OverloadResolutionFailure);

/**
 * Table of all mappings within a certain scope.
 */
class MappingTable {
private:
    std::unordered_map<std::string, std::pair<const values::Value, tree::Maybe<ast::Mapping>>> table;
public:

    /**
     * Adds a mapping.
     */
    void add(
        const std::string &name,
        const values::Value &value,
        const tree::Maybe<ast::Mapping> &node = tree::Maybe<ast::Mapping>()
    );

    /**
     * Resolves a mapping. Throws NameResolutionFailure if no mapping by the
     * given name exists.
     */
    values::Value resolve(const std::string &name) const;

    /**
     * Grants read access to the underlying map.
     */
    const std::unordered_map<std::string, std::pair<const values::Value, tree::Maybe<ast::Mapping>>> &get_table() const;

};

// Forward declaration for the name resolver template class. This class is
// defined entirely in the C++ file to cut back on compile time.
template <class T>
class OverloadedNameResolver;

/**
 * C++ function representing (one of the overloads of) a function usable in
 * cQASM constant expressions.
 */
using FunctionImpl = std::function<values::Value(const values::Values&)>;

/**
 * Table of all overloads of all constant propagation functions.
 */
class FunctionTable {
private:
    std::unique_ptr<OverloadedNameResolver<FunctionImpl>> resolver;
public:

    // The following things *are all default*. Unfortunately, the compiler
    // can't infer them because OverloadedNameResolver is incomplete.
    FunctionTable();
    ~FunctionTable();
    FunctionTable(const FunctionTable& t);
    FunctionTable(FunctionTable&& t);
    FunctionTable& operator=(const FunctionTable& t);
    FunctionTable& operator=(FunctionTable&& t);

    /**
     * Registers a function. The name should be lowercase; matching will be done
     * case-insensitively. The param_types variadic specifies the amount and
     * types of the parameters that (this particular overload of) the function
     * expects. The C++ implementation of the function can assume that the
     * value list it gets is of the right size and the values are of the right
     * types.
     *
     * This method does not contain any intelligence to override previously
     * added overloads. However, the overload resolution engine will always use
     * the last applicable overload it finds, so adding does have the effect of
     * overriding.
     */
    void add(const std::string &name, const types::Types &param_types, const FunctionImpl &impl);

    /**
     * Calls a function. Throws NameResolutionFailure if no function by the
     * given name exists, OverloadResolutionFailure if no overload of the
     * function exists for the given arguments, or otherwise returns the value
     * returned by the function.
     */
    values::Value call(const std::string &name, const values::Values &args) const;

};

/**
 * Table of the supported instructions and their overloads.
 */
class ErrorModelTable {
private:
    std::unique_ptr<OverloadedNameResolver<error_model::ErrorModel>> resolver;
public:

    // The following things *are all default*. Unfortunately, the compiler
    // can't infer them because OverloadedNameResolver is incomplete.
    ErrorModelTable();
    ~ErrorModelTable();
    ErrorModelTable(const ErrorModelTable& t);
    ErrorModelTable(ErrorModelTable&& t);
    ErrorModelTable& operator=(const ErrorModelTable& t);
    ErrorModelTable& operator=(ErrorModelTable&& t);

    /**
     * Registers an error model.
     */
    void add(const error_model::ErrorModel &type);

    /**
     * Resolves an error model. Throws NameResolutionFailure if no error model
     * by the given name exists, OverloadResolutionFailure if no overload
     * exists for the given arguments, or otherwise returns the resolved error
     * model node. Annotation data and line number information still needs to
     * be set by the caller.
     */
    tree::One<semantic::ErrorModel> resolve(
        const std::string &name,
        const values::Values &args
    ) const;

};

/**
 * Table of the supported instructions and their overloads.
 */
class InstructionTable {
private:
    std::unique_ptr<OverloadedNameResolver<instruction::Instruction>> resolver;
public:

    // The following things *are all default*. Unfortunately, the compiler
    // can't infer them because OverloadedNameResolver is incomplete.
    InstructionTable();
    ~InstructionTable();
    InstructionTable(const InstructionTable& t);
    InstructionTable(InstructionTable&& t);
    InstructionTable& operator=(const InstructionTable& t);
    InstructionTable& operator=(InstructionTable&& t);

    /**
     * Registers an instruction type.
     */
    void add(const instruction::Instruction &type);

    /**
     * Resolves an instruction. Throws NameResolutionFailure if no instruction
     * by the given name exists, OverloadResolutionFailure if no overload
     * exists for the given arguments, or otherwise returns the resolved
     * instruction node. Annotation data, line number information, and the
     * condition still need to be set by the caller.
     */
    tree::One<semantic::Instruction> resolve(
        const std::string &name,
        const values::Values &args
    ) const;

};

} // namespace resolver
} // namespace v1
} // namespace cqasm
