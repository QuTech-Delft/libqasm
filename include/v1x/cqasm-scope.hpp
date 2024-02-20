#pragma once

#include "cqasm-tree.hpp"
#include "v1x/cqasm-resolver.hpp"
#include "v1x/cqasm-semantic-gen.hpp"


namespace cqasm::v1x::analyzer {

/**
 * Scope information.
 */
struct Scope {
    /**
     * The mappings visible within this scope.
     */
    resolver::MappingTable mappings;

    /**
     * The functions visible within this scope.
     */
    resolver::FunctionTable functions;

    /**
     * The instruction set visible within this scope.
     */
    resolver::InstructionTable instruction_set;

    /**
     * The block associated with this scope, if any.
     * If this is empty, this is the global scope, and the active block is that of the current subcircuit,
     * which is lazily created when needed in case no subcircuit label is explicitly specified,
     * and can thus not easily be populated here.
     */
    tree::Maybe<semantic::Block> block;

    /**
     * Whether we're within at least one for, foreach, while, or repeat-until loop.
     * This is a necessary condition for break and continue statements to be allowed.
     */
    bool within_loop;

    /**
     * Creates a scope from a table of mappings, functions, and an instruction set.
     */
    Scope(
        resolver::MappingTable mappings,
        resolver::FunctionTable functions,
        resolver::InstructionTable instruction_set);
};

} // namespace cqasm::v1x::analyzer
