#include "v3x/register-consteval-core-functions.hpp"

#include <cassert>

namespace cqasm::v3x::function {

/**
 * Registers a bunch of consteval core functions.
 * That is, functions supported by the language, and that can be evaluated at compile time.
 * For functions supported by the language that cannot be evaluated at compile time, see CoreFunction.
 */
void register_consteval_core_functions(analyzer::Analyzer *analyzer) {
    assert(analyzer);

    analyzer->register_consteval_core_function("operator-", "f", op_neg_f);
    analyzer->register_consteval_core_function("operator-", "i", op_neg_i);

    analyzer->register_consteval_core_function("operator+", "ff", op_add_ff);
    analyzer->register_consteval_core_function("operator+", "ii", op_add_ii);
    analyzer->register_consteval_core_function("operator-", "ff", op_sub_ff);
    analyzer->register_consteval_core_function("operator-", "ii", op_sub_ii);

    analyzer->register_consteval_core_function("operator*", "ff", op_mul_ff);
    analyzer->register_consteval_core_function("operator*", "ii", op_mul_ii);
    analyzer->register_consteval_core_function("operator/", "ff", op_div_ff);
    analyzer->register_consteval_core_function("operator/", "ii", op_div_ii);
    analyzer->register_consteval_core_function("operator%", "ii", op_mod_ii);

    analyzer->register_consteval_core_function("operator**", "ff", op_pow_ff);

    analyzer->register_consteval_core_function("operator==", "ff", op_eq_ff);
    analyzer->register_consteval_core_function("operator!=", "ff", op_ne_ff);
    analyzer->register_consteval_core_function("operator>=", "ff", op_ge_ff);
    analyzer->register_consteval_core_function("operator>", "ff", op_gt_ff);
    analyzer->register_consteval_core_function("operator<=", "ff", op_le_ff);
    analyzer->register_consteval_core_function("operator<", "ff", op_lt_ff);

    analyzer->register_consteval_core_function("operator==", "ii", op_eq_ii);
    analyzer->register_consteval_core_function("operator!=", "ii", op_ne_ii);
    analyzer->register_consteval_core_function("operator>=", "ii", op_ge_ii);
    analyzer->register_consteval_core_function("operator>", "ii", op_gt_ii);
    analyzer->register_consteval_core_function("operator<=", "ii", op_le_ii);
    analyzer->register_consteval_core_function("operator<", "ii", op_lt_ii);

    analyzer->register_consteval_core_function("operator==", "bb", op_eq_bb);
    analyzer->register_consteval_core_function("operator!=", "bb", op_ne_bb);
    analyzer->register_consteval_core_function("operator>=", "bb", op_ge_bb);
    analyzer->register_consteval_core_function("operator>", "bb", op_gt_bb);
    analyzer->register_consteval_core_function("operator<=", "bb", op_le_bb);
    analyzer->register_consteval_core_function("operator<", "bb", op_lt_bb);

    analyzer->register_consteval_core_function("operator~", "i", op_binv_i);
    analyzer->register_consteval_core_function("operator&", "ii", op_band_ii);
    analyzer->register_consteval_core_function("operator^", "ii", op_bxor_ii);
    analyzer->register_consteval_core_function("operator|", "ii", op_bor_ii);

    analyzer->register_consteval_core_function("operator<<", "ii", op_shl_ii);
    analyzer->register_consteval_core_function("operator>>", "ii", op_shr_ii);

    analyzer->register_consteval_core_function("operator!", "b", op_linv_b);
    analyzer->register_consteval_core_function("operator&&", "bb", op_land_bb);
    analyzer->register_consteval_core_function("operator^^", "bb", op_lxor_bb);
    analyzer->register_consteval_core_function("operator||", "bb", op_lor_bb);

    analyzer->register_consteval_core_function("operator?:", "bff", op_tcnd_bff);
    analyzer->register_consteval_core_function("operator?:", "bii", op_tcnd_bii);
    analyzer->register_consteval_core_function("operator?:", "bbb", op_tcnd_bbb);

    analyzer->register_consteval_core_function("sqrt", "f", fn_sqrt_f);
    analyzer->register_consteval_core_function("exp", "f", fn_exp_f);
    analyzer->register_consteval_core_function("log", "f", fn_log_f);
    analyzer->register_consteval_core_function("sin", "f", fn_sin_f);
    analyzer->register_consteval_core_function("cos", "f", fn_cos_f);
    analyzer->register_consteval_core_function("tan", "f", fn_tan_f);
    analyzer->register_consteval_core_function("sinh", "f", fn_sinh_f);
    analyzer->register_consteval_core_function("cosh", "f", fn_cosh_f);
    analyzer->register_consteval_core_function("tanh", "f", fn_tanh_f);
    analyzer->register_consteval_core_function("asin", "f", fn_asin_f);
    analyzer->register_consteval_core_function("acos", "f", fn_acos_f);
    analyzer->register_consteval_core_function("atan", "f", fn_atan_f);
    analyzer->register_consteval_core_function("asinh", "f", fn_asinh_f);
    analyzer->register_consteval_core_function("acosh", "f", fn_acosh_f);
    analyzer->register_consteval_core_function("atanh", "f", fn_atanh_f);
    analyzer->register_consteval_core_function("abs", "f", fn_abs_f);
    analyzer->register_consteval_core_function("abs", "i", fn_abs_i);
}

}  // namespace cqasm::v3x::function
