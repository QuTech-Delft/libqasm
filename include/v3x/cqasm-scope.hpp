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
     * The list of function supported by the language, and that can be evaluated at compile time.
     */
    resolver::ConstEvalCoreFunctionTable consteval_core_function_table;

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
};

}  // namespace cqasm::v3x::analyzer
