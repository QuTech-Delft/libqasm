#include "v3x/cqasm-functions.hpp"

namespace cqasm::v3x::functions {

/**
 * Registers a bunch of functions usable during constant propagation into the given function table.
 */
void register_default_functions_into(resolver::FunctionTable &table) {
    table.add("operator-", types::from_spec("r"), op_neg_r);
    table.add("operator-", types::from_spec("i"), op_neg_i);

    table.add("operator+", types::from_spec("rr"), op_add_rr);
    table.add("operator+", types::from_spec("ii"), op_add_ii);
    table.add("operator-", types::from_spec("rr"), op_sub_rr);
    table.add("operator-", types::from_spec("ii"), op_sub_ii);

    table.add("operator*", types::from_spec("rr"), op_mul_rr);
    table.add("operator*", types::from_spec("ii"), op_mul_ii);
    table.add("operator/", types::from_spec("rr"), op_div_rr);
    table.add("operator/", types::from_spec("rr"), op_div_ii);
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
    table.add("operator>>", types::from_spec("ii"), op_shr_ii);

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
