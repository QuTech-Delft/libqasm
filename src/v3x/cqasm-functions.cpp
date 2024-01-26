#include "v3x/cqasm-functions.hpp"

#include <cmath>


namespace cqasm::v3x::functions {

/**
 * Modulo.
 */
static int64_t mod_floor(int64_t a, int64_t b) {
    int64_t rem = a % b;
    if (rem != 0 && ((rem < 0) != (b < 0))) {
        rem += b;
    }
    return rem;
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

values::Value op_div_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(a / b);
}

values::Value op_mod_ii(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_int()->value;
    auto b = v[1]->as_const_int()->value;
    return tree::make<values::ConstInt>(mod_floor(a, b));
}

values::Value op_pow_rr(const values::Values &v) {
    values::check_const(v);
    auto a = v[0]->as_const_real()->value;
    auto b = v[1]->as_const_real()->value;
    return tree::make<values::ConstReal>(std::pow(a, b));
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

/**
 * Registers a bunch of functions usable during constant propagation into the given function table.
 */
void register_default_functions_into(resolver::FunctionTable &table) {
    table.add("operator+", types::from_spec("rr"), op_add_rr);
    table.add("operator+", types::from_spec("ii"), op_add_ii);
    table.add("operator-", types::from_spec("rr"), op_sub_rr);
    table.add("operator-", types::from_spec("ii"), op_sub_ii);
    table.add("operator-", types::from_spec("r"), op_neg_r);
    table.add("operator-", types::from_spec("i"), op_neg_i);
    table.add("operator*", types::from_spec("rr"), op_mul_rr);
    table.add("operator*", types::from_spec("ii"), op_mul_ii);
    table.add("operator/", types::from_spec("rr"), op_div_rr);
    table.add("operator%", types::from_spec("ii"), op_mod_ii);
    table.add("operator**", types::from_spec("rr"), op_pow_rr);
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
    table.add("operator!", types::from_spec("b"), op_linv_b);
    table.add("operator&&", types::from_spec("bb"), op_land_bb);
    table.add("operator^^", types::from_spec("bb"), op_lxor_bb);
    table.add("operator||", types::from_spec("bb"), op_lor_bb);
    table.add("operator?:", types::from_spec("brr"), op_tcnd_brr);
    table.add("operator?:", types::from_spec("bii"), op_tcnd_bii);
    table.add("operator?:", types::from_spec("bbb"), op_tcnd_bbb);
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
}

} // namespace cqasm::v3x::functions
