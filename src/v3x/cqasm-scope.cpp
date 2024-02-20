#include "v3x/cqasm-scope.hpp"


namespace cqasm::v3x::analyzer {

/**
 * Creates a scope from a table of mappings, functions, and an instruction set.
 */
Scope::Scope(
    resolver::MappingTable mappings,
    resolver::FunctionTable functions,
    resolver::InstructionTable instruction_set,
    tree::One<semantic::Block> block)
: mappings{ std::move(mappings) }
, functions{ std::move(functions) }
, instruction_set{ std::move(instruction_set) }
, block{ std::move(block) }
{}

} // namespace cqasm::v3x::analyzer
