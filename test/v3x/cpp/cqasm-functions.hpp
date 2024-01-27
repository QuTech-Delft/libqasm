#pragma once

#include "v3x/cqasm-values.hpp"

namespace values = cqasm::v3x::values;


/*
 * Convenience function for invoking unary and binary operators, and functions:
 *
 * - F is the operator or function being invoked.
 *   FRT is the return type of F, and
 *   FParamsType is the type of the parameters of F.
 * - The caller to invoke uses simple types as Params, e.g., doubles 2.0 and 2.0 for testing op_eq_rr, and
 *   receives a simple type as RT, e.g., a Bool when testing op_eq_rr.
 */
template <typename RT, typename FRT, typename FParamsType, auto F, typename... Params>
RT invoke(Params... ps) {
    auto values = values::Values{{ cqasm::tree::make<FParamsType>(ps)... }};
    auto ret = F(values);
    return dynamic_cast<FRT&>(*ret).value;
}

/*
 * Convenience function for invoking ternary operators:
 *
 * - The first parameter, p, is the condition
 *   Which always translates to a ConstBool value
 */
template <typename RT, typename FRT, typename FParamsType, auto F, typename... Params>
RT invoke_ternary(bool p, Params... ps) {
    auto values = values::Values{{ cqasm::tree::make<values::ConstBool>(p), cqasm::tree::make<FParamsType>(ps)... }};
    auto ret = F(values);
    return dynamic_cast<FRT&>(*ret).value;
}
