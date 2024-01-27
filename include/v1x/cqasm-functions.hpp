#pragma once

#include "v1x/cqasm-primitives.hpp"
#include "v1x/cqasm-resolver.hpp"

#include <cmath>
#include <complex>
#include <cstdint>
#include <string>

namespace primitives = cqasm::v1x::primitives;


namespace cqasm::v1x::functions {

/**
 * Modulo.
 * This modulo keeps the sign of the divisor.
 */
static std::int64_t mod_floor(std::int64_t a, std::int64_t b) {
    std::int64_t rem = a % b;
    if (rem != 0 && ((rem < 0) != (b < 0))) {
        rem += b;
    }
    return rem;
}

/**
 * Division that always rounds down (towards negative infinity), like Python's integer division.
 * Because rounding to zero is a useless mechanic.
 */
static std::int64_t div_floor(std::int64_t a, std::int64_t b) {
    std::int64_t res = a / b;
    std::int64_t rem = a % b;
    // Correct division result downwards if up-rounding happened,
    // (for non-zero remainder of sign different from the divisor).
    std::int64_t corr = (rem != 0 && ((rem < 0) != (b < 0)));
    return res - corr;
}

template <typename ReturnType, typename ParamType, auto F>
values::Value unary_function_with_constant_parameter(const values::Values &vs) {
    values::check_const(vs);
    auto arg = dynamic_cast<ParamType&>(*vs[0]).value;
    return tree::make<ReturnType>(F(arg));
}
// Short alias
template <typename ReturnType, typename ParamType, auto F>
values::Value uf_cp(const values::Values &vs) {
    return unary_function_with_constant_parameter<ReturnType, ParamType, F>(vs);
}

template <typename ReturnType, typename ParamsType, auto F>
values::Value binary_function_with_constant_parameters(const values::Values &vs) {
    values::check_const(vs);
    auto a = dynamic_cast<ParamsType&>(*vs[0]).value;
    auto b = dynamic_cast<ParamsType&>(*vs[1]).value;
    return tree::make<ReturnType>(F(a, b));
}
// Short alias
template <typename ReturnType, typename ParamsType, auto F>
values::Value bf_cps(const values::Values &vs) {
    return binary_function_with_constant_parameters<ReturnType, ParamsType, F>(vs);
}

template <typename ParamsType, auto F>
values::Value ternary_function_with_constant_parameters(const values::Values &vs) {
    values::check_const(vs);
    auto condition = dynamic_cast<values::ConstBool&>(*vs[0]).value; 
    auto if_true = dynamic_cast<ParamsType&>(*vs[1]).value;
    auto if_false = dynamic_cast<ParamsType&>(*vs[2]).value;
    return tree::make<ParamsType>(F(condition, if_true, if_false));
}
// Short alias
template <typename ParamsType, auto F>
values::Value tf_cps(const values::Values &vs) {
    return ternary_function_with_constant_parameters<ParamsType, F>(vs);
}

constexpr auto op_neg_c = uf_cp<values::ConstComplex, values::ConstComplex, std::negate{}>;
constexpr auto op_neg_r = uf_cp<values::ConstReal, values::ConstReal, std::negate{}>;
constexpr auto op_neg_i = uf_cp<values::ConstInt, values::ConstInt, std::negate{}>;
constexpr auto op_add_cc = bf_cps<values::ConstComplex, values::ConstComplex, std::plus{}>;
constexpr auto op_add_rr = bf_cps<values::ConstReal, values::ConstReal, std::plus{}>;
constexpr auto op_add_ii = bf_cps<values::ConstInt, values::ConstInt, std::plus{}>;
constexpr auto add_ss = [](const primitives::Str &a, const primitives::Str &b) { return a + b; };
constexpr auto op_add_ss = bf_cps<values::ConstString, values::ConstString, add_ss>;
constexpr auto op_sub_cc = bf_cps<values::ConstComplex, values::ConstComplex, std::minus{}>;
constexpr auto op_sub_rr = bf_cps<values::ConstReal, values::ConstReal, std::minus{}>;
constexpr auto op_sub_ii = bf_cps<values::ConstInt, values::ConstInt, std::minus{}>;

constexpr auto op_mul_cc = bf_cps<values::ConstComplex, values::ConstComplex, std::multiplies{}>;
constexpr auto op_mul_rr = bf_cps<values::ConstReal, values::ConstReal, std::multiplies{}>;
constexpr auto op_mul_ii = bf_cps<values::ConstInt, values::ConstInt, std::multiplies{}>;
constexpr auto op_div_cc = bf_cps<values::ConstComplex, values::ConstComplex, std::divides{}>;
constexpr auto op_div_rr = bf_cps<values::ConstReal, values::ConstReal, std::divides{}>;
constexpr auto op_int_div_ii = bf_cps<values::ConstInt, values::ConstInt, div_floor>;
constexpr auto op_mod_ii = bf_cps<values::ConstInt, values::ConstInt, mod_floor>;

constexpr auto pow = [](auto base, auto exp) { return std::pow(base, exp); };
constexpr auto op_pow_cc = bf_cps<values::ConstComplex, values::ConstComplex, pow>;
constexpr auto op_pow_rr = bf_cps<values::ConstReal, values::ConstReal, pow>;

constexpr auto op_eq_cc = bf_cps<values::ConstBool, values::ConstComplex, std::equal_to{}>;
constexpr auto op_ne_cc = bf_cps<values::ConstBool, values::ConstComplex, std::not_equal_to{}>;

constexpr auto op_eq_rr = bf_cps<values::ConstBool, values::ConstReal, std::equal_to{}>;
constexpr auto op_ne_rr = bf_cps<values::ConstBool, values::ConstReal, std::not_equal_to{}>;
constexpr auto op_ge_rr = bf_cps<values::ConstBool, values::ConstReal, std::greater_equal{}>;
constexpr auto op_gt_rr = bf_cps<values::ConstBool, values::ConstReal, std::greater{}>;
constexpr auto op_le_rr = bf_cps<values::ConstBool, values::ConstReal, std::less_equal{}>;
constexpr auto op_lt_rr = bf_cps<values::ConstBool, values::ConstReal, std::less{}>;

constexpr auto op_eq_ii = bf_cps<values::ConstBool, values::ConstInt, std::equal_to{}>;
constexpr auto op_ne_ii = bf_cps<values::ConstBool, values::ConstInt, std::not_equal_to{}>;
constexpr auto op_ge_ii = bf_cps<values::ConstBool, values::ConstInt, std::greater_equal{}>;
constexpr auto op_gt_ii = bf_cps<values::ConstBool, values::ConstInt, std::greater{}>;
constexpr auto op_le_ii = bf_cps<values::ConstBool, values::ConstInt, std::less_equal{}>;
constexpr auto op_lt_ii = bf_cps<values::ConstBool, values::ConstInt, std::less{}>;

constexpr auto op_eq_bb = bf_cps<values::ConstBool, values::ConstBool, std::equal_to{}>;
constexpr auto op_ne_bb = bf_cps<values::ConstBool, values::ConstBool, std::not_equal_to{}>;
constexpr auto op_ge_bb = bf_cps<values::ConstBool, values::ConstBool, std::greater_equal{}>;
constexpr auto op_gt_bb = bf_cps<values::ConstBool, values::ConstBool, std::greater{}>;
constexpr auto op_le_bb = bf_cps<values::ConstBool, values::ConstBool, std::less_equal{}>;
constexpr auto op_lt_bb = bf_cps<values::ConstBool, values::ConstBool, std::less{}>;

constexpr auto op_binv_i = uf_cp<values::ConstInt, values::ConstInt, std::bit_not{}>;
constexpr auto op_band_ii = bf_cps<values::ConstInt, values::ConstInt, std::bit_and{}>;
constexpr auto op_bxor_ii = bf_cps<values::ConstInt, values::ConstInt, std::bit_xor{}>;
constexpr auto op_bor_ii = bf_cps<values::ConstInt, values::ConstInt, std::bit_or{}>;

constinit const auto shl = [](primitives::Int a, primitives::Int b) { return a << b; };
constexpr auto op_shl_ii = bf_cps<values::ConstInt, values::ConstInt, shl>;
constinit const auto sra = [](primitives::Int a, primitives::Int b) { return a >> b; };
constexpr auto op_sra_ii = bf_cps<values::ConstInt, values::ConstInt, sra>;
constinit const auto srl = [](primitives::Int a, primitives::Int b) {
    return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) >> b); };
constexpr auto op_srl_ii = bf_cps<values::ConstInt, values::ConstInt, srl>;

constexpr auto op_linv_b = uf_cp<values::ConstBool, values::ConstBool, std::logical_not{}>;
constexpr auto op_land_bb = bf_cps<values::ConstBool, values::ConstBool, std::logical_and{}>;
constinit const auto lxor = [](primitives::Bool lhs, primitives::Bool rhs) { return !lhs ^ !rhs; };
constexpr auto op_lxor_bb = bf_cps<values::ConstBool, values::ConstBool, lxor>;
constexpr auto op_lor_bb = bf_cps<values::ConstBool, values::ConstBool, std::logical_or{}>;

constinit const auto tcnd = [](bool condition, auto if_true, auto if_false) { return condition ? if_true : if_false; };
constexpr auto op_tcnd_bcc = tf_cps<values::ConstComplex, tcnd>;
constexpr auto op_tcnd_brr = tf_cps<values::ConstReal, tcnd>;
constexpr auto op_tcnd_bii = tf_cps<values::ConstInt, tcnd>;
constexpr auto op_tcnd_bbb = tf_cps<values::ConstBool, tcnd>;

constinit const auto sqrt = [](auto num) { return std::sqrt(num); };
constexpr auto fn_sqrt_c = uf_cp<values::ConstComplex, values::ConstComplex, sqrt>;
constexpr auto fn_sqrt_r = uf_cp<values::ConstReal, values::ConstReal, sqrt>;
constinit const auto exp = [](auto num) { return std::exp(num); };
constexpr auto fn_exp_c = uf_cp<values::ConstComplex, values::ConstComplex, exp>;
constexpr auto fn_exp_r = uf_cp<values::ConstReal, values::ConstReal, exp>;
constinit const auto log = [](auto num) { return std::log(num); };
constexpr auto fn_log_c = uf_cp<values::ConstComplex, values::ConstComplex, log>;
constexpr auto fn_log_r = uf_cp<values::ConstReal, values::ConstReal, log>;
constinit const auto sin = [](auto num) { return std::sin(num); };
constexpr auto fn_sin_c = uf_cp<values::ConstComplex, values::ConstComplex, sin>;
constexpr auto fn_sin_r = uf_cp<values::ConstReal, values::ConstReal, sin>;
constinit const auto cos = [](auto num) { return std::cos(num); };
constexpr auto fn_cos_c = uf_cp<values::ConstComplex, values::ConstComplex, cos>;
constexpr auto fn_cos_r = uf_cp<values::ConstReal, values::ConstReal, cos>;
constinit const auto tan = [](auto num) { return std::tan(num); };
constexpr auto fn_tan_c = uf_cp<values::ConstComplex, values::ConstComplex, tan>;
constexpr auto fn_tan_r = uf_cp<values::ConstReal, values::ConstReal, tan>;
constinit const auto sinh = [](auto num) { return std::sinh(num); };
constexpr auto fn_sinh_c = uf_cp<values::ConstComplex, values::ConstComplex, sinh>;
constexpr auto fn_sinh_r = uf_cp<values::ConstReal, values::ConstReal, sinh>;
constinit const auto cosh = [](auto num) { return std::cosh(num); };
constexpr auto fn_cosh_c = uf_cp<values::ConstComplex, values::ConstComplex, cosh>;
constexpr auto fn_cosh_r = uf_cp<values::ConstReal, values::ConstReal, cosh>;
constinit const auto tanh = [](auto num) { return std::tanh(num); };
constexpr auto fn_tanh_c = uf_cp<values::ConstComplex, values::ConstComplex, tanh>;
constexpr auto fn_tanh_r = uf_cp<values::ConstReal, values::ConstReal, tanh>;
constinit const auto asin = [](auto num) { return std::asin(num); };
constexpr auto fn_asin_c = uf_cp<values::ConstComplex, values::ConstComplex, asin>;
constexpr auto fn_asin_r = uf_cp<values::ConstReal, values::ConstReal, asin>;
constinit const auto acos = [](auto num) { return std::acos(num); };
constexpr auto fn_acos_c = uf_cp<values::ConstComplex, values::ConstComplex, acos>;
constexpr auto fn_acos_r = uf_cp<values::ConstReal, values::ConstReal, acos>;
constinit const auto atan = [](auto num) { return std::atan(num); };
constexpr auto fn_atan_c = uf_cp<values::ConstComplex, values::ConstComplex, atan>;
constexpr auto fn_atan_r = uf_cp<values::ConstReal, values::ConstReal, atan>;
constinit const auto asinh = [](auto num) { return std::asinh(num); };
constexpr auto fn_asinh_c = uf_cp<values::ConstComplex, values::ConstComplex, asinh>;
constexpr auto fn_asinh_r = uf_cp<values::ConstReal, values::ConstReal, asinh>;
constinit const auto acosh = [](auto num) { return std::acosh(num); };
constexpr auto fn_acosh_c = uf_cp<values::ConstComplex, values::ConstComplex, acosh>;
constexpr auto fn_acosh_r = uf_cp<values::ConstReal, values::ConstReal, acosh>;
constinit const auto atanh = [](auto num) { return std::atanh(num); };
constexpr auto fn_atanh_c = uf_cp<values::ConstComplex, values::ConstComplex, atanh>;
constexpr auto fn_atanh_r = uf_cp<values::ConstReal, values::ConstReal, atanh>;
constinit const auto abs = [](auto num) { return std::abs(num); };
constexpr auto fn_abs_r = uf_cp<values::ConstReal, values::ConstReal, abs>;
constexpr auto fn_abs_i = uf_cp<values::ConstInt, values::ConstInt, abs>;

constinit const auto complex_rr = [](auto a, auto b) { return primitives::Complex(a, b); };
constexpr auto fn_complex_rr = bf_cps<values::ConstComplex, values::ConstReal, complex_rr>;
constinit const auto polar_rr = [](auto a, auto b) { return std::polar(a, b); };
constexpr auto fn_polar_rr = bf_cps<values::ConstComplex, values::ConstReal, polar_rr>;
constinit const auto real_c = [](auto a) { return std::real(a); };
constexpr auto fn_real_c = uf_cp<values::ConstReal, values::ConstComplex, real_c>;
constinit const auto imag_c = [](auto a) { return std::imag(a); };
constexpr auto fn_imag_c = uf_cp<values::ConstReal, values::ConstComplex, imag_c>;
constinit const auto arg_c = [](auto a) { return std::arg(a); };
constexpr auto fn_arg_c = uf_cp<values::ConstReal, values::ConstComplex, arg_c>;
constinit const auto norm_c = [](auto a) { return std::norm(a); };
constexpr auto fn_norm_c = uf_cp<values::ConstReal, values::ConstComplex, norm_c>;
constinit const auto conj_c = [](auto a) { return std::conj(a); };
constexpr auto fn_conj_c = uf_cp<values::ConstComplex, values::ConstComplex, conj_c>;

/**
 * Registers a bunch of functions usable during constant propagation into the given function table.
 */
void register_default_functions_into(resolver::FunctionTable &table);

} // namespace functions
