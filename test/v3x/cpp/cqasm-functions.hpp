#pragma once

#include "v3x/cqasm-values.hpp"

namespace values = cqasm::v3x::values;


/**
 * Convenience function templates for invoking operators and functions:
 *
 * - F is the operator or function being invoked.
 *   FReturnType is the return type of F, and
 *   FParamsType is the type of the parameters of F.
 * - The caller uses simple types as ParamsType, e.g., doubles 2.0 and 2.0 for testing op_eq_rr, and
 *   receives a simple type as return type, e.g., a Bool when testing op_eq_rr.
 */

template <auto F, typename ParamType>
auto invoke_unary(ParamType p) {
    using FReturnType = typename decltype(F)::return_type;
    using FParamType = typename decltype(F)::param_type;

    auto values = values::Values{{ cqasm::tree::make<FParamType>(p) }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}

template <auto F, typename ParamsType>
auto invoke_binary(ParamsType a, ParamsType b) {
    using FReturnType = typename decltype(F)::return_type;
    using FParamType = typename decltype(F)::param_type;

    auto values = values::Values{{ cqasm::tree::make<FParamType>(a), cqasm::tree::make<FParamType>(b) }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}

template <auto F, typename ParamsType>
auto invoke_ternary(bool condition, ParamsType if_true, ParamsType if_false) {
    using FReturnType = typename decltype(F)::return_type;
    using FParamType = typename decltype(F)::param_type;

    auto values = values::Values{{
        cqasm::tree::make<values::ConstBool>(condition),
        cqasm::tree::make<FParamType>(if_true),
        cqasm::tree::make<FParamType>(if_false)
    }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}
