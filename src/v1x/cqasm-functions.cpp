#include "v1x/cqasm-functions.hpp"


namespace cqasm::v1x::functions {

/**
 * Registers a bunch of functions usable during constant propagation into the given function table.
 */
void register_default_functions_into(resolver::FunctionTable &table) {
    table.add("operator-", types::from_spec("c"), op_neg_c);
    table.add("operator-", types::from_spec("r"), op_neg_r);
    table.add("operator-", types::from_spec("i"), op_neg_i);

    table.add("operator+", types::from_spec("cc"), op_add_cc);
    table.add("operator+", types::from_spec("rr"), op_add_rr);
    table.add("operator+", types::from_spec("ii"), op_add_ii);
    table.add("operator+", types::from_spec("ss"), op_add_ss);
    table.add("operator-", types::from_spec("cc"), op_sub_cc);
    table.add("operator-", types::from_spec("rr"), op_sub_rr);
    table.add("operator-", types::from_spec("ii"), op_sub_ii);

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

} // namespace cqasm::v1x::functions
