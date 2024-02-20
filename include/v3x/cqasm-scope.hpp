#pragma once

#include "cqasm-tree.hpp"
#include "v3x/cqasm-resolver.hpp"
#include "v3x/cqasm-semantic-gen.hpp"


namespace cqasm::v3x::analyzer {

/**
 * Scope information.
 */
struct Scope {
    /**
     * The variables visible within this scope.
     */
    resolver::VariableTable variable_table;

    /**
     * The functions visible within this scope.
     */
    resolver::FunctionTable function_table;

    /**
     * The instructions visible within this scope.
     * Instructions have a case-sensitively matched name,
     * and a signature for the types of parameters they expect.
     */
    resolver::InstructionTable instruction_table;

    /**
     * The block associated with this scope.
     */
    tree::One<semantic::Block> block;

    /**
     * The list of variables declared in this scope.
     */
    tree::Any<semantic::Variable> variables;

    /**
     * The list of functions declared in the global scope.
     */
    tree::Any<semantic::Function> functions;

    Scope() = default;
    Scope(const Scope &other) = default;
    Scope(Scope &&other) noexcept = default;
    Scope& operator=(const Scope &other) = default;
    Scope& operator=(Scope &&other) noexcept = default;

    /**
     * Creates a scope from a table of variables, functions, and an instruction set.
     */
    Scope(
        resolver::VariableTable variable_table,
        resolver::FunctionTable functions,
        resolver::InstructionTable instruction_set,
        tree::One<semantic::Block> block);
};

} // namespace cqasm::v3x::analyzer
