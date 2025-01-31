#pragma once

#include "libqasm/v3x/values.hpp"

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
    using FReturnValue = typename decltype(F)::return_value;
    using FParamValue = typename decltype(F)::param_value;

    auto values = values::Values{ { cqasm::tree::make<FParamValue>(p) } };
    auto ret = F(values);
    return dynamic_cast<FReturnValue&>(*ret).value;
}

template <auto F, typename ParamsType>
auto invoke_binary(ParamsType a, ParamsType b) {
    using FReturnValue = typename decltype(F)::return_value;
    using FParamValue = typename decltype(F)::param_value;

    auto values = values::Values{
        { cqasm::tree::make<FParamValue>(a), cqasm::tree::make<FParamValue>(b) }
    };
    auto ret = F(values);
    return dynamic_cast<FReturnValue&>(*ret).value;
}

template <auto F, typename ParamsType>
auto invoke_ternary(bool condition, ParamsType if_true, ParamsType if_false) {
    using FReturnValue = typename decltype(F)::return_value;
    using FParamValue = typename decltype(F)::param_value;

    auto values = values::Values{
        { cqasm::tree::make<values::ConstBool>(condition),
         cqasm::tree::make<FParamValue>(if_true),
         cqasm::tree::make<FParamValue>(if_false) }
    };
    auto ret = F(values);
    return dynamic_cast<FReturnValue&>(*ret).value;
}
