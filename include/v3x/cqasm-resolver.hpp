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
            throw NameResolutionFailure{ "failed to resolve " + name };
        } catch (const cqasm::overload::OverloadResolutionFailure &) {
            throw OverloadResolutionFailure{
                fmt::format("failed to resolve overload for {} with argument pack {}",
                    name, values::types_of(args)) };
        }
    }
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
