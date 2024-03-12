#include "cqasm-functions.hpp"
#include "v3x/register-consteval-core-functions.hpp"
#include "v3x/cqasm-primitives.hpp"

#include <gmock/gmock.h>

namespace function = cqasm::v3x::function;
namespace primitives = cqasm::v3x::primitives;
namespace values = cqasm::v3x::values;


TEST(op_neg_f, f_0) { EXPECT_EQ((invoke_unary<function::op_neg_f>(0.)), 0.); }
TEST(op_neg_f, f_5) { EXPECT_EQ((invoke_unary<function::op_neg_f>(5.)), -5.); }
TEST(op_neg_f, f_m5) { EXPECT_EQ((invoke_unary<function::op_neg_f>(-5.)), 5.); }

TEST(op_neg_i, i_0) { EXPECT_EQ((invoke_unary<function::op_neg_i>(0)), 0); }
TEST(op_neg_i, i_5) { EXPECT_EQ((invoke_unary<function::op_neg_i>(5)), -5); }
TEST(op_neg_i, i_m5) { EXPECT_EQ((invoke_unary<function::op_neg_i>(-5)), 5); }

TEST(op_add_ff, ff_m5_5) { EXPECT_EQ((invoke_binary<function::op_add_ff>(-5., 5.)), 0.); }
TEST(op_add_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<function::op_add_ii>(-5, 5)), 0); }

TEST(op_sub_ff, ff_m5_5) { EXPECT_EQ((invoke_binary<function::op_sub_ff>(-5., 5.)), -10.); }
TEST(op_sub_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<function::op_sub_ii>(-5, 5)), -10); }

TEST(op_mul_ff, ff_m5_5) { EXPECT_EQ((invoke_binary<function::op_mul_ff>(-5., 5.)), -25.); }
TEST(op_mul_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<function::op_mul_ii>(-5, 5)), -25); }

TEST(op_div_ff, ff_m5_5) { EXPECT_EQ((invoke_binary<function::op_div_ff>(-5., 5.)), -1.); }
TEST(op_div_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<function::op_div_ii>(-5, 5)), -1); }
TEST(op_div_ii, ii_8_3) { EXPECT_EQ((invoke_binary<function::op_div_ii>(8, 3)), 2); }

TEST(op_mod_ii, ii_m1_3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(-1, 3)), -1); }
TEST(op_mod_ii, ii_0_3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(-0, 3)), 0); }
TEST(op_mod_ii, ii_1_3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(1, 3)), 1); }
TEST(op_mod_ii, ii_m1_m3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(-1, -3)), -1); }
TEST(op_mod_ii, ii_0_m3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(0, -3)), 0); }
TEST(op_mod_ii, ii_1_m3) { EXPECT_EQ((invoke_binary<function::op_mod_ii>(1, -3)), 1); }

TEST(op_pow_ff, f_m5_5) { EXPECT_EQ((invoke_binary<function::op_pow_ff>(-5., 5.)), -3125.); }

TEST(op_eq_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_eq_ff>(2., 2.)), true); }
TEST(op_ne_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_ne_ff>(2., 2.)), false); }
TEST(op_ge_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_ge_ff>(2., 2.)), true); }
TEST(op_gt_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_gt_ff>(2., 2.)), false); }
TEST(op_le_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_le_ff>(2., 2.)), true); }
TEST(op_lt_ff, ff_2_2) { EXPECT_EQ((invoke_binary<function::op_lt_ff>(2., 2.)), false); }

TEST(op_eq_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_eq_ii>(2, 2)), true); }
TEST(op_ne_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_ne_ii>(2, 2)), false); }
TEST(op_ge_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_ge_ii>(2, 2)), true); }
TEST(op_gt_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_gt_ii>(2, 2)), false); }
TEST(op_le_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_le_ii>(2, 2)), true); }
TEST(op_lt_ii, ii_2_2) { EXPECT_EQ((invoke_binary<function::op_lt_ii>(2, 2)), false); }

TEST(op_eq_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_eq_bb>(true, true)), true); }
TEST(op_ne_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_ne_bb>(true, true)), false); }
TEST(op_ge_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_ge_bb>(true, true)), true); }
TEST(op_gt_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_gt_bb>(true, true)), false); }
TEST(op_le_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_le_bb>(true, true)), true); }
TEST(op_lt_bb, bb_2_2) { EXPECT_EQ((invoke_binary<function::op_lt_bb>(true, true)), false); }

TEST(op_binv_i, i_172) { EXPECT_EQ((invoke_unary<function::op_binv_i>(172)), -173); }
TEST(op_band_ii, ii_172_163) { EXPECT_EQ((invoke_binary<function::op_band_ii>(172, 163)), 160); }
TEST(op_bxor_ii, ii_172_163) { EXPECT_EQ((invoke_binary<function::op_bxor_ii>(172, 163)), 15); }
TEST(op_bor_ii, ii_172_163) { EXPECT_EQ((invoke_binary<function::op_bor_ii>(172, 163)), 175); }

TEST(op_shl_ii, ii_172_3) { EXPECT_EQ((invoke_binary<function::op_shl_ii>(172, 3)), 1376); }
TEST(op_shr_ii, ii_172_3) { EXPECT_EQ((invoke_binary<function::op_shr_ii>(172, 3)), 21); }

TEST(op_linv_b, b_true) { EXPECT_EQ((invoke_unary<function::op_linv_b>(true)), false); }
TEST(op_land_bb, bb_true_true) { EXPECT_EQ((invoke_binary<function::op_land_bb>(true, true)), true); }
TEST(op_lxor_bb, bb_true_true) { EXPECT_EQ((invoke_binary<function::op_lxor_bb>(true, true)), false); }
TEST(op_lor_bb, bb_true_true) { EXPECT_EQ((invoke_binary<function::op_lor_bb>(true, true)), true); }

TEST(op_tcnd_bff, bff_true_m5_5) { EXPECT_EQ((invoke_ternary<function::op_tcnd_bff>(true, -5., 5.)), -5.); }
TEST(op_tcnd_bii, bii_true_m5_5) { EXPECT_EQ((invoke_ternary<function::op_tcnd_bii>(true, -5, 5)), -5); }
TEST(op_tcnd_bbb, bbb_true_true_false) { EXPECT_EQ((invoke_ternary<function::op_tcnd_bbb>(true, true, false)), true); }

TEST(fn_sqrt_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_sqrt_f>(2.)), 1.41421, 0.00001); }
TEST(fn_exp_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_exp_f>(2.)), 7.38905, 0.00001); }
TEST(fn_log_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_log_f>(2.)), 0.69314, 0.00001); }
TEST(fn_sin_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_sin_f>(2.)), 0.90929, 0.00001); }
TEST(fn_cos_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_cos_f>(2.)), -0.41614, 0.00001); }
TEST(fn_tan_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_tan_f>(2.)), -2.18503, 0.00001); }
TEST(fn_sinh_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_sinh_f>(2.)), 3.62686, 0.00001); }
TEST(fn_cosh_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_cosh_f>(2.)), 3.76219, 0.00001); }
TEST(fn_tanh_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_tanh_f>(2.)), 0.96402, 0.00001); }
TEST(fn_asin_f, f_point_2) { EXPECT_NEAR((invoke_unary<function::fn_asin_f>(.2)), 0.20135, 0.00001); }
TEST(fn_acos_f, f_point_2) { EXPECT_NEAR((invoke_unary<function::fn_acos_f>(.2)), 1.36943, 0.00001); }
TEST(fn_atan_f, f_point_2) { EXPECT_NEAR((invoke_unary<function::fn_atan_f>(.2)), 0.19739, 0.00001); }
TEST(fn_asinh_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_asinh_f>(2.)), 1.44363, 0.00001); }
TEST(fn_acosh_f, f_2) { EXPECT_NEAR((invoke_unary<function::fn_acosh_f>(2.)), 1.31695, 0.00001); }
TEST(fn_atanh_f, f_point_2) { EXPECT_NEAR((invoke_unary<function::fn_atanh_f>(.2)), 0.20273, 0.00001); }
