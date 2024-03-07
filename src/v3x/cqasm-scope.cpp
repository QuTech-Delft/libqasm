#include "v3x/cqasm-scope.hpp"


namespace cqasm::v3x::analyzer {

/**
 * Creates a scope from a table of variables, functions, and instructions.
 */
Scope::Scope(
    resolver::VariableTable variable_table,
    resolver::FunctionImplTable function_impl_table,
    resolver::FunctionTable function_table,
    resolver::InstructionTable instruction_table,
    tree::One<semantic::Block> block)
: variable_table{ std::move(variable_table) }
, function_impl_table{ std::move(function_impl_table) }
, function_table{ std::move(function_table) }
, instruction_table{ std::move(instruction_table) }
, block{ std::move(block) }
{}

} // namespace cqasm::v3x::analyzer
