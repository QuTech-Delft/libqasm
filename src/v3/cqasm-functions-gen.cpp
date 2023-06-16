
/** \file
 * Source file generated by \ref func-gen.
 */

#include <cmath>
#include <complex>
#include "v3/cqasm-functions-gen.hpp"

namespace cqasm {
namespace v3 {

/**
 * Namespace for the functions generated by \ref func-gen.
 */
namespace functions {

/**
 * Division that always rounds down (towards negative infinity), like Python's
 * integer division. Because rounding to zero is a useless mechanic.
 */
static int64_t div_floor(int64_t a, int64_t b) {
    int64_t res = a / b;
    int64_t rem = a % b;
    // Correct division result downwards if up-rounding happened,
    // (for non-zero remainder of sign different than the divisor).
    int64_t corr = (rem != 0 && ((rem < 0) != (b < 0)));
    return res - corr;
}

/**
 * Modulo to go along with div_floor.
 */
static int64_t mod_floor(int64_t a, int64_t b) {
    int64_t rem = a % b;
    if (rem != 0 && ((rem < 0) != (b < 0))) {
        rem += b;
    }
    return rem;
}


values::Value op_add_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(a + b);
}

values::Value op_add_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(a + b);
}

values::Value op_add_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a + b);
}

values::Value op_add_ss(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_string()->value;
    auto b = v[1]->as_const_string()->value;
    return tree::make<values::ConstString>(a + b);
}

values::Value op_sub_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(a - b);
}

values::Value op_sub_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(a - b);
}

values::Value op_sub_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a - b);
}

values::Value op_neg_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(-a);
}

values::Value op_neg_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(-a);
}

values::Value op_neg_i(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    return tree::make<values::ConstInt>(-a);
}

values::Value op_mul_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(a * b);
}

values::Value op_mul_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(a * b);
}

values::Value op_mul_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a * b);
}

values::Value op_div_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(a / b);
}

values::Value op_div_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(a / b);
}

values::Value op_int_div_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(div_floor(a, b));
}

values::Value op_mod_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(mod_floor(a, b));
}

values::Value op_pow_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::pow(a, b));
}

values::Value op_pow_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::pow(a, b));
}

values::Value op_eq_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstBool>(a == b);
}

values::Value op_ne_cc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    auto b = v[1]->as_const_complex()->value;
    return tree::make<values::ConstBool>(a != b);
}

values::Value op_eq_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a == b);
}

values::Value op_ne_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a != b);
}

values::Value op_ge_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a >= b);
}

values::Value op_gt_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a > b);
}

values::Value op_le_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a <= b);
}

values::Value op_lt_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstBool>(a < b);
}

values::Value op_eq_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a == b);
}

values::Value op_ne_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a != b);
}

values::Value op_ge_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a >= b);
}

values::Value op_gt_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a > b);
}

values::Value op_le_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a <= b);
}

values::Value op_lt_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstBool>(a < b);
}

values::Value op_eq_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a == b);
}

values::Value op_ne_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a != b);
}

values::Value op_ge_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a >= b);
}

values::Value op_gt_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a > b);
}

values::Value op_le_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a <= b);
}

values::Value op_lt_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a < b);
}

values::Value op_binv_i(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    return tree::make<values::ConstInt>(~a);
}

values::Value op_band_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a & b);
}

values::Value op_bxor_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a ^ b);
}

values::Value op_bor_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a | b);
}

values::Value op_shl_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a << b);
}

values::Value op_sra_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(a >> b);
}

values::Value op_srl_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>((int64_t)(((uint64_t)a) >> b));
}

values::Value op_linv_b(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    return tree::make<values::ConstBool>(!a);
}

values::Value op_land_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a && b);
}

values::Value op_lxor_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(!a ^ !b);
}

values::Value op_lor_bb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a || b);
}

values::Value op_tcnd_bcc(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_complex()->value;
    auto c = v[2]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(a ? b : c);
}

values::Value op_tcnd_brr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_real()->value;
    auto c = v[2]->as_const_real()->value;
    return tree::make<values::ConstReal>(a ? b : c);
}

values::Value op_tcnd_bii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_int()->value;
    auto c = v[2]->as_const_int()->value;
    return tree::make<values::ConstInt>(a ? b : c);
}

values::Value op_tcnd_bbb(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_bool()->value;
    auto b = v[1]->as_const_bool()->value;
    auto c = v[2]->as_const_bool()->value;
    return tree::make<values::ConstBool>(a ? b : c);
}

values::Value fn_sqrt_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::sqrt(a));
}

values::Value fn_exp_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::exp(a));
}

values::Value fn_log_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::log(a));
}

values::Value fn_sin_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::sin(a));
}

values::Value fn_cos_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::cos(a));
}

values::Value fn_tan_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::tan(a));
}

values::Value fn_sinh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::sinh(a));
}

values::Value fn_cosh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::cosh(a));
}

values::Value fn_tanh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::tanh(a));
}

values::Value fn_asin_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::asin(a));
}

values::Value fn_acos_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::acos(a));
}

values::Value fn_atan_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::atan(a));
}

values::Value fn_asinh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::asinh(a));
}

values::Value fn_acosh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::acosh(a));
}

values::Value fn_atanh_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::atanh(a));
}

values::Value fn_sqrt_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::sqrt(a));
}

values::Value fn_exp_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::exp(a));
}

values::Value fn_log_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::log(a));
}

values::Value fn_sin_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::sin(a));
}

values::Value fn_cos_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::cos(a));
}

values::Value fn_tan_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::tan(a));
}

values::Value fn_sinh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::sinh(a));
}

values::Value fn_cosh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::cosh(a));
}

values::Value fn_tanh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::tanh(a));
}

values::Value fn_asin_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::asin(a));
}

values::Value fn_acos_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::acos(a));
}

values::Value fn_atan_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::atan(a));
}

values::Value fn_asinh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::asinh(a));
}

values::Value fn_acosh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::acosh(a));
}

values::Value fn_atanh_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::atanh(a));
}

values::Value fn_abs_r(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::abs(a));
}

values::Value fn_abs_i(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    return tree::make<values::ConstInt>(std::abs(a));
}

values::Value fn_complex_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstComplex>(primitives::Complex(a, b));
}

values::Value fn_polar_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstComplex>(std::polar<double>(a, b));
}

values::Value fn_real_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstReal>(std::real<double>(a));
}

values::Value fn_imag_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstReal>(std::imag<double>(a));
}

values::Value fn_arg_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstReal>(std::arg<double>(a));
}

values::Value fn_norm_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstReal>(std::norm<double>(a));
}

values::Value fn_conj_c(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_complex()->value;
    return tree::make<values::ConstComplex>(std::conj<double>(a));
}

/**
 * Registers a bunch of functions usable during constant propagation into the
 * given function table.
 */
void register_into(resolver::FunctionTable &table) {
    table.add("operator+", types::from_spec("cc"), op_add_cc);
    table.add("operator+", types::from_spec("rr"), op_add_rr);
    table.add("operator+", types::from_spec("ii"), op_add_ii);
    table.add("operator+", types::from_spec("ss"), op_add_ss);
    table.add("operator-", types::from_spec("cc"), op_sub_cc);
    table.add("operator-", types::from_spec("rr"), op_sub_rr);
    table.add("operator-", types::from_spec("ii"), op_sub_ii);
    table.add("operator-", types::from_spec("c"), op_neg_c);
    table.add("operator-", types::from_spec("r"), op_neg_r);
    table.add("operator-", types::from_spec("i"), op_neg_i);
    table.add("operator*", types::from_spec("cc"), op_mul_cc);
    table.add("operator*", types::from_spec("rr"), op_mul_rr);
    table.add("operator*", types::from_spec("ii"), op_mul_ii);
    table.add("operator/", types::from_spec("cc"), op_div_cc);
    table.add("operator/", types::from_spec("rr"), op_div_rr);
    table.add("operator//", types::from_spec("ii"), op_int_div_ii);
    table.add("operator%", types::from_spec("ii"), op_mod_ii);
    table.add("operator**", types::from_spec("cc"), op_pow_cc);
    table.add("operator**", types::from_spec("rr"), op_pow_rr);
    table.add("operator==", types::from_spec("cc"), op_eq_cc);
    table.add("operator!=", types::from_spec("cc"), op_ne_cc);
    table.add("operator==", types::from_spec("rr"), op_eq_rr);
    table.add("operator!=", types::from_spec("rr"), op_ne_rr);
    table.add("operator>=", types::from_spec("rr"), op_ge_rr);
    table.add("operator>", types::from_spec("rr"), op_gt_rr);
    table.add("operator<=", types::from_spec("rr"), op_le_rr);
    table.add("operator<", types::from_spec("rr"), op_lt_rr);
    table.add("operator==", types::from_spec("ii"), op_eq_ii);
    table.add("operator!=", types::from_spec("ii"), op_ne_ii);
    table.add("operator>=", types::from_spec("ii"), op_ge_ii);
    table.add("operator>", types::from_spec("ii"), op_gt_ii);
    table.add("operator<=", types::from_spec("ii"), op_le_ii);
    table.add("operator<", types::from_spec("ii"), op_lt_ii);
    table.add("operator==", types::from_spec("bb"), op_eq_bb);
    table.add("operator!=", types::from_spec("bb"), op_ne_bb);
    table.add("operator>=", types::from_spec("bb"), op_ge_bb);
    table.add("operator>", types::from_spec("bb"), op_gt_bb);
    table.add("operator<=", types::from_spec("bb"), op_le_bb);
    table.add("operator<", types::from_spec("bb"), op_lt_bb);
    table.add("operator~", types::from_spec("i"), op_binv_i);
    table.add("operator&", types::from_spec("ii"), op_band_ii);
    table.add("operator^", types::from_spec("ii"), op_bxor_ii);
    table.add("operator|", types::from_spec("ii"), op_bor_ii);
    table.add("operator<<", types::from_spec("ii"), op_shl_ii);
    table.add("operator>>", types::from_spec("ii"), op_sra_ii);
    table.add("operator>>>", types::from_spec("ii"), op_srl_ii);
    table.add("operator!", types::from_spec("b"), op_linv_b);
    table.add("operator&&", types::from_spec("bb"), op_land_bb);
    table.add("operator^^", types::from_spec("bb"), op_lxor_bb);
    table.add("operator||", types::from_spec("bb"), op_lor_bb);
    table.add("operator?:", types::from_spec("bcc"), op_tcnd_bcc);
    table.add("operator?:", types::from_spec("brr"), op_tcnd_brr);
    table.add("operator?:", types::from_spec("bii"), op_tcnd_bii);
    table.add("operator?:", types::from_spec("bbb"), op_tcnd_bbb);
    table.add("sqrt", types::from_spec("c"), fn_sqrt_c);
    table.add("exp", types::from_spec("c"), fn_exp_c);
    table.add("log", types::from_spec("c"), fn_log_c);
    table.add("sin", types::from_spec("c"), fn_sin_c);
    table.add("cos", types::from_spec("c"), fn_cos_c);
    table.add("tan", types::from_spec("c"), fn_tan_c);
    table.add("sinh", types::from_spec("c"), fn_sinh_c);
    table.add("cosh", types::from_spec("c"), fn_cosh_c);
    table.add("tanh", types::from_spec("c"), fn_tanh_c);
    table.add("asin", types::from_spec("c"), fn_asin_c);
    table.add("acos", types::from_spec("c"), fn_acos_c);
    table.add("atan", types::from_spec("c"), fn_atan_c);
    table.add("asinh", types::from_spec("c"), fn_asinh_c);
    table.add("acosh", types::from_spec("c"), fn_acosh_c);
    table.add("atanh", types::from_spec("c"), fn_atanh_c);
    table.add("sqrt", types::from_spec("r"), fn_sqrt_r);
    table.add("exp", types::from_spec("r"), fn_exp_r);
    table.add("log", types::from_spec("r"), fn_log_r);
    table.add("sin", types::from_spec("r"), fn_sin_r);
    table.add("cos", types::from_spec("r"), fn_cos_r);
    table.add("tan", types::from_spec("r"), fn_tan_r);
    table.add("sinh", types::from_spec("r"), fn_sinh_r);
    table.add("cosh", types::from_spec("r"), fn_cosh_r);
    table.add("tanh", types::from_spec("r"), fn_tanh_r);
    table.add("asin", types::from_spec("r"), fn_asin_r);
    table.add("acos", types::from_spec("r"), fn_acos_r);
    table.add("atan", types::from_spec("r"), fn_atan_r);
    table.add("asinh", types::from_spec("r"), fn_asinh_r);
    table.add("acosh", types::from_spec("r"), fn_acosh_r);
    table.add("atanh", types::from_spec("r"), fn_atanh_r);
    table.add("abs", types::from_spec("r"), fn_abs_r);
    table.add("abs", types::from_spec("i"), fn_abs_i);
    table.add("complex", types::from_spec("rr"), fn_complex_rr);
    table.add("polar", types::from_spec("rr"), fn_polar_rr);
    table.add("real", types::from_spec("c"), fn_real_c);
    table.add("imag", types::from_spec("c"), fn_imag_c);
    table.add("arg", types::from_spec("c"), fn_arg_c);
    table.add("norm", types::from_spec("c"), fn_norm_c);
    table.add("conj", types::from_spec("c"), fn_conj_c);

}

} // namespace functions
} // namespace v3
} // namespace cqasm