/** \file
 * Forward reference for tree::semantic::Variable,
 * so the values tree can use it (resolves circular dependency).
 */

#pragma once


namespace cqasm::v3x::semantic {

// NOTE JvS:
// This forward declaration is needed in order to allow the value tree to link to Variable nodes in the semantic tree.
// It's possible to get rid of this kludge by merging the value (and type) trees into the semantic tree,
// as probably should have been done initially, but a lot of refactoring would result.
class Variable;
class Function;

} // namespace cqasm::v3x::semantic
