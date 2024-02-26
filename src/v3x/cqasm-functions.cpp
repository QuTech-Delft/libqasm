#include "v3x/cqasm-functions.hpp"

namespace cqasm::v3x::functions {

/**
 * Registers a bunch of functions for which we have a C++ implementation into the given function table.
 */
void register_default_function_impls_into(resolver::FunctionImplTable &table) {
    table.add("operator-", types::from_spec("f"), op_neg_f);
    table.add("operator-", types::from_spec("i"), op_neg_i);

    table.add("operator+", types::from_spec("ff"), op_add_ff);
    table.add("operator+", types::from_spec("ii"), op_add_ii);
    table.add("operator-", types::from_spec("ff"), op_sub_ff);
    table.add("operator-", types::from_spec("ii"), op_sub_ii);

    table.add("operator*", types::from_spec("ff"), op_mul_ff);
    table.add("operator*", types::from_spec("ii"), op_mul_ii);
    table.add("operator/", types::from_spec("ff"), op_div_ff);
    table.add("operator/", types::from_spec("ii"), op_div_ii);
    table.add("operator%", types::from_spec("ii"), op_mod_ii);

    table.add("operator**", types::from_spec("ff"), op_pow_ff);

    table.add("operator==", types::from_spec("ff"), op_eq_ff);
    table.add("operator!=", types::from_spec("ff"), op_ne_ff);
    table.add("operator>=", types::from_spec("ff"), op_ge_ff);
    table.add("operator>", types::from_spec("ff"), op_gt_ff);
    table.add("operator<=", types::from_spec("ff"), op_le_ff);
    table.add("operator<", types::from_spec("ff"), op_lt_ff);

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
    table.add("operator>>", types::from_spec("ii"), op_shr_ii);

    table.add("operator!", types::from_spec("b"), op_linv_b);
    table.add("operator&&", types::from_spec("bb"), op_land_bb);
    table.add("operator^^", types::from_spec("bb"), op_lxor_bb);
    table.add("operator||", types::from_spec("bb"), op_lor_bb);

    table.add("operator?:", types::from_spec("bff"), op_tcnd_bff);
    table.add("operator?:", types::from_spec("bii"), op_tcnd_bii);
    table.add("operator?:", types::from_spec("bbb"), op_tcnd_bbb);

    table.add("sqrt", types::from_spec("f"), fn_sqrt_f);
    table.add("exp", types::from_spec("f"), fn_exp_f);
    table.add("log", types::from_spec("f"), fn_log_f);
    table.add("sin", types::from_spec("f"), fn_sin_f);
    table.add("cos", types::from_spec("f"), fn_cos_f);
    table.add("tan", types::from_spec("f"), fn_tan_f);
    table.add("sinh", types::from_spec("f"), fn_sinh_f);
    table.add("cosh", types::from_spec("f"), fn_cosh_f);
    table.add("tanh", types::from_spec("f"), fn_tanh_f);
    table.add("asin", types::from_spec("f"), fn_asin_f);
    table.add("acos", types::from_spec("f"), fn_acos_f);
    table.add("atan", types::from_spec("f"), fn_atan_f);
    table.add("asinh", types::from_spec("f"), fn_asinh_f);
    table.add("acosh", types::from_spec("f"), fn_acosh_f);
    table.add("atanh", types::from_spec("f"), fn_atanh_f);
    table.add("abs", types::from_spec("f"), fn_abs_f);
    table.add("abs", types::from_spec("i"), fn_abs_i);
}

} // namespace cqasm::v3x::functions
