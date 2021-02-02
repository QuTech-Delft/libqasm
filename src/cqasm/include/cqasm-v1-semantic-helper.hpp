/** \file
 * Forward reference for tree::semantic::Variable, so the values tree can use
 * it (resolves circular dependency).
 */

#pragma once

namespace cqasm {
namespace v1 {
namespace semantic {

// NOTE JvS: This forward declaration is needed in order to allow the value tree
// to link to Variable nodes in the semantic tree. It's possible to get rid of
// this kludge by merging the value (and type) trees into the semantic tree, as
// probably should have been done initially, but a lot of refactoring would
// result. Instead I'll just keep it in mind when designing cQASM 2.0; the
// current plan is that it would have its own codebase anyway, and refer back
// to this one based on the version header if need be.
class Variable;

} // namespace semantic
} // namespace v1
} // namespace cqasm
