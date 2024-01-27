#pragma once

#include "v3x/cqasm-values.hpp"

namespace values = cqasm::v3x::values;


/*
 * Convenience function templates for invoking operators and functions:
 *
 * - F is the operator or function being invoked.
 *   FReturnType is the return type of F, and
 *   FParamsType is the type of the parameters of F.
 * - The caller uses simple types as ParamsType, e.g., doubles 2.0 and 2.0 for testing op_eq_rr, and
 *   receives a simple type as ReturnType, e.g., a Bool when testing op_eq_rr.
 */

template <typename ReturnType, typename FReturnType, typename FParamType, auto F, typename ParamType>
ReturnType invoke_unary(ParamType p) {
    auto values = values::Values{{ cqasm::tree::make<FParamType>(p) }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}

template <typename ReturnType, typename FReturnType, typename FParamsType, auto F, typename ParamsType>
ReturnType invoke_binary(ParamsType a, ParamsType b) {
    auto values = values::Values{{ cqasm::tree::make<FParamsType>(a), cqasm::tree::make<FParamsType>(b) }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}

template <typename ReturnType, typename FReturnType, typename FParamsType, auto F, typename ParamsType>
ReturnType invoke_ternary(bool condition, ParamsType if_true, ParamsType if_false) {
    auto values = values::Values{{
        cqasm::tree::make<values::ConstBool>(condition),
        cqasm::tree::make<FParamsType>(if_true),
        cqasm::tree::make<FParamsType>(if_false)
    }};
    auto ret = F(values);
    return dynamic_cast<FReturnType&>(*ret).value;
}
