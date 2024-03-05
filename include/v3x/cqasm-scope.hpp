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
     * The list of function implementations.
     * That is, functions for which we have a C++ implementation.
     */
    resolver::FunctionImplTable function_impl_table;

    /**
     * The list of functions, other than function implementations.
     * That is, functions defined in the cQASM file.
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
     * The list of functions defined in the global scope.
     */
    tree::Any<semantic::Function> functions;

    Scope() = default;
    Scope(const Scope &other) = default;
    Scope(Scope &&other) noexcept = default;
    Scope& operator=(const Scope &other) = default;
    Scope& operator=(Scope &&other) noexcept = default;

    /**
     * Creates a scope from a table of variables, functions, and instructions.
     */
    Scope(
        resolver::VariableTable variable_table,
        resolver::FunctionImplTable function_impl_table,
        resolver::FunctionTable function_table,
        resolver::InstructionTable instruction_table,
        tree::One<semantic::Block> block);
};

} // namespace cqasm::v3x::analyzer
