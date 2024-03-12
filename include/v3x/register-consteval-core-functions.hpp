#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-primitives.hpp"

#include <cmath>
#include <functional>
#include <numeric>


namespace primitives = cqasm::v3x::primitives;


/**
 * Changes regarding v1:
 *
 * - There are not functions operating on complex types yet
 * - There are not functions operating on string types yet
 * - op_int_div_ii has been renamed to op_div_ii (integer division)
 * - op_div_ii is implemented using std::divides, not div_floor
 * - op_mod_ii is implemented using std::modulus, not mod_floor
 *   Whereas mod_floor keeps the sign of the divisor, std::modulus keeps the sign of the dividend
 * - op_sra_ii has been renamed to op_shr_ii (shift right arithmetic)
 * - op_srl_ii (shift right logic) has been dropped
 */

namespace cqasm::v3x::function {
    
/**
 * Function with constant parameters 
 */
template <typename ReturnValue, typename ParamValue, auto F>
struct f_cp {
    using return_value = ReturnValue;
    using param_value = ParamValue;
};

/**
 * Unary function with constant parameter
 */
template <typename ReturnValue, typename ParamValue, auto F>
struct uf_cp : public f_cp<ReturnValue, ParamValue, F> {
    values::Value operator()(const values::Values &vs) const {
        values::check_const(vs);
        auto arg = vs[0].as<ParamValue>()->value;
        return tree::make<ReturnValue>(F(arg));
    }
};

/**
 * Binary function with constant parameters 
 */
template <typename ReturnValue, typename ParamValue, auto F>
struct bf_cp : public f_cp<ReturnValue, ParamValue, F> {
    values::Value operator()(const values::Values &vs) const {
        values::check_const(vs);
        auto a = vs[0].as<ParamValue>()->value;
        auto b = vs[1].as<ParamValue>()->value;
        return tree::make<ReturnValue>(F(a, b));
    }
};

/**
 * Ternary function with constant parameters 
 */
template <typename ParamValue, auto F>
struct tf_cp : public f_cp<ParamValue, ParamValue, F> {
    values::Value operator()(const values::Values &vs) const {
        values::check_const(vs);
        auto condition = vs[0]->as_const_bool()->value;
        auto if_true = vs[1].as<ParamValue>()->value;
        auto if_false = vs[2].as<ParamValue>()->value;
        return tree::make<ParamValue>(F(condition, if_true, if_false));
    }
};

constexpr auto op_neg_f = uf_cp<values::ConstFloat, values::ConstFloat, std::negate<double>{}>{};
constexpr auto op_neg_i = uf_cp<values::ConstInt, values::ConstInt, std::negate{}>{};

constexpr auto op_add_ff = bf_cp<values::ConstFloat, values::ConstFloat, std::plus{}>{};
constexpr auto op_add_ii = bf_cp<values::ConstInt, values::ConstInt, std::plus{}>{};
constexpr auto op_sub_ff = bf_cp<values::ConstFloat, values::ConstFloat, std::minus{}>{};
constexpr auto op_sub_ii = bf_cp<values::ConstInt, values::ConstInt, std::minus{}>{};

constexpr auto op_mul_ff = bf_cp<values::ConstFloat, values::ConstFloat, std::multiplies{}>{};
constexpr auto op_mul_ii = bf_cp<values::ConstInt, values::ConstInt, std::multiplies{}>{};
constexpr auto op_div_ff = bf_cp<values::ConstFloat, values::ConstFloat, std::divides{}>{};
constexpr auto op_div_ii = bf_cp<values::ConstInt, values::ConstInt, std::divides{}>{};
constexpr auto op_mod_ii = bf_cp<values::ConstInt, values::ConstInt, std::modulus{}>{};

constexpr auto pow = [](auto base, auto exp) { return std::pow(base, exp); };
constexpr auto op_pow_ff = bf_cp<values::ConstFloat, values::ConstFloat, pow>{};

constexpr auto op_eq_ff = bf_cp<values::ConstBool, values::ConstFloat, std::equal_to{}>{};
constexpr auto op_ne_ff = bf_cp<values::ConstBool, values::ConstFloat, std::not_equal_to{}>{};
constexpr auto op_ge_ff = bf_cp<values::ConstBool, values::ConstFloat, std::greater_equal{}>{};
constexpr auto op_gt_ff = bf_cp<values::ConstBool, values::ConstFloat, std::greater{}>{};
constexpr auto op_le_ff = bf_cp<values::ConstBool, values::ConstFloat, std::less_equal{}>{};
constexpr auto op_lt_ff = bf_cp<values::ConstBool, values::ConstFloat, std::less{}>{};

constexpr auto op_eq_ii = bf_cp<values::ConstBool, values::ConstInt, std::equal_to{}>{};
constexpr auto op_ne_ii = bf_cp<values::ConstBool, values::ConstInt, std::not_equal_to{}>{};
constexpr auto op_ge_ii = bf_cp<values::ConstBool, values::ConstInt, std::greater_equal{}>{};
constexpr auto op_gt_ii = bf_cp<values::ConstBool, values::ConstInt, std::greater{}>{};
constexpr auto op_le_ii = bf_cp<values::ConstBool, values::ConstInt, std::less_equal{}>{};
constexpr auto op_lt_ii = bf_cp<values::ConstBool, values::ConstInt, std::less{}>{};

constexpr auto op_eq_bb = bf_cp<values::ConstBool, values::ConstBool, std::equal_to{}>{};
constexpr auto op_ne_bb = bf_cp<values::ConstBool, values::ConstBool, std::not_equal_to{}>{};
constexpr auto op_ge_bb = bf_cp<values::ConstBool, values::ConstBool, std::greater_equal{}>{};
constexpr auto op_gt_bb = bf_cp<values::ConstBool, values::ConstBool, std::greater{}>{};
constexpr auto op_le_bb = bf_cp<values::ConstBool, values::ConstBool, std::less_equal{}>{};
constexpr auto op_lt_bb = bf_cp<values::ConstBool, values::ConstBool, std::less{}>{};

constexpr auto op_binv_i = uf_cp<values::ConstInt, values::ConstInt, std::bit_not{}>{};
constexpr auto op_band_ii = bf_cp<values::ConstInt, values::ConstInt, std::bit_and{}>{};
constexpr auto op_bxor_ii = bf_cp<values::ConstInt, values::ConstInt, std::bit_xor{}>{};
constexpr auto op_bor_ii = bf_cp<values::ConstInt, values::ConstInt, std::bit_or{}>{};

constexpr auto shl = [](primitives::Int a, primitives::Int b) { return a << b; };
constexpr auto op_shl_ii = bf_cp<values::ConstInt, values::ConstInt, shl>{};
constexpr auto shr = [](primitives::Int a, primitives::Int b) { return a >> b; };
constexpr auto op_shr_ii = bf_cp<values::ConstInt, values::ConstInt, shr>{};

constexpr auto op_linv_b = uf_cp<values::ConstBool, values::ConstBool, std::logical_not{}>{};
constexpr auto op_land_bb = bf_cp<values::ConstBool, values::ConstBool, std::logical_and{}>{};
constexpr auto lxor = [](primitives::Bool lhs, primitives::Bool rhs) { return !lhs ^ !rhs; };
constexpr auto op_lxor_bb = bf_cp<values::ConstBool, values::ConstBool, lxor>{};
constexpr auto op_lor_bb = bf_cp<values::ConstBool, values::ConstBool, std::logical_or{}>{};

constexpr auto tcnd = [](bool condition, auto if_true, auto if_false) { return condition ? if_true : if_false; };
constexpr auto op_tcnd_bff = tf_cp<values::ConstFloat, tcnd>{};
constexpr auto op_tcnd_bii = tf_cp<values::ConstInt, tcnd>{};
constexpr auto op_tcnd_bbb = tf_cp<values::ConstBool, tcnd>{};

constexpr auto sqrt = [](auto num) { return std::sqrt(num); };
constexpr auto fn_sqrt_f = uf_cp<values::ConstFloat, values::ConstFloat, sqrt>{};
constexpr auto exp = [](auto num) { return std::exp(num); };
constexpr auto fn_exp_f = uf_cp<values::ConstFloat, values::ConstFloat, exp>{};
constexpr auto log = [](auto num) { return std::log(num); };
constexpr auto fn_log_f = uf_cp<values::ConstFloat, values::ConstFloat, log>{};
constexpr auto sin = [](auto num) { return std::sin(num); };
constexpr auto fn_sin_f = uf_cp<values::ConstFloat, values::ConstFloat, sin>{};
constexpr auto cos = [](auto num) { return std::cos(num); };
constexpr auto fn_cos_f = uf_cp<values::ConstFloat, values::ConstFloat, cos>{};
constexpr auto tan = [](auto num) { return std::tan(num); };
constexpr auto fn_tan_f = uf_cp<values::ConstFloat, values::ConstFloat, tan>{};
constexpr auto sinh = [](auto num) { return std::sinh(num); };
constexpr auto fn_sinh_f = uf_cp<values::ConstFloat, values::ConstFloat, sinh>{};
constexpr auto cosh = [](auto num) { return std::cosh(num); };
constexpr auto fn_cosh_f = uf_cp<values::ConstFloat, values::ConstFloat, cosh>{};
constexpr auto tanh = [](auto num) { return std::tanh(num); };
constexpr auto fn_tanh_f = uf_cp<values::ConstFloat, values::ConstFloat, tanh>{};
constexpr auto asin = [](auto num) { return std::asin(num); };
constexpr auto fn_asin_f = uf_cp<values::ConstFloat, values::ConstFloat, asin>{};
constexpr auto acos = [](auto num) { return std::acos(num); };
constexpr auto fn_acos_f = uf_cp<values::ConstFloat, values::ConstFloat, acos>{};
constexpr auto atan = [](auto num) { return std::atan(num); };
constexpr auto fn_atan_f = uf_cp<values::ConstFloat, values::ConstFloat, atan>{};
constexpr auto asinh = [](auto num) { return std::asinh(num); };
constexpr auto fn_asinh_f = uf_cp<values::ConstFloat, values::ConstFloat, asinh>{};
constexpr auto acosh = [](auto num) { return std::acosh(num); };
constexpr auto fn_acosh_f = uf_cp<values::ConstFloat, values::ConstFloat, acosh>{};
constexpr auto atanh = [](auto num) { return std::atanh(num); };
constexpr auto fn_atanh_f = uf_cp<values::ConstFloat, values::ConstFloat, atanh>{};
constexpr auto abs = [](auto num) { return std::abs(num); };
constexpr auto fn_abs_f = uf_cp<values::ConstFloat, values::ConstFloat, abs>{};
constexpr auto fn_abs_i = uf_cp<values::ConstInt, values::ConstInt, abs>{};

/**
 * Registers a bunch of consteval core functions.
 * That is, functions supported by the language, and that can be evaluated at compile time.
 * For functions supported by the language that cannot be evaluated at compile time, see CoreFunction.
 */
void register_consteval_core_functions(analyzer::Analyzer *analyzer);

} // namespace function
