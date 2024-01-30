#include "cqasm-functions.hpp"
#include "v3x/cqasm-functions.hpp"
#include "v3x/cqasm-primitives.hpp"

#include <gmock/gmock.h>

namespace functions = cqasm::v3x::functions;
namespace primitives = cqasm::v3x::primitives;
namespace values = cqasm::v3x::values;


TEST(op_neg_r, r_0) { EXPECT_EQ((invoke_unary<functions::op_neg_r>(0.)), 0.); }
TEST(op_neg_r, r_5) { EXPECT_EQ((invoke_unary<functions::op_neg_r>(5.)), -5.); }
TEST(op_neg_r, r_m5) { EXPECT_EQ((invoke_unary<functions::op_neg_r>(-5.)), 5.); }

TEST(op_neg_i, i_0) { EXPECT_EQ((invoke_unary<functions::op_neg_i>(0)), 0); }
TEST(op_neg_i, i_5) { EXPECT_EQ((invoke_unary<functions::op_neg_i>(5)), -5); }
TEST(op_neg_i, i_m5) { EXPECT_EQ((invoke_unary<functions::op_neg_i>(-5)), 5); }

TEST(op_add_rr, rr_m5_5) { EXPECT_EQ((invoke_binary<functions::op_add_rr>(-5., 5.)), 0.); }
TEST(op_add_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<functions::op_add_ii>(-5, 5)), 0); }

TEST(op_sub_rr, rr_m5_5) { EXPECT_EQ((invoke_binary<functions::op_sub_rr>(-5., 5.)), -10.); }
TEST(op_sub_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<functions::op_sub_ii>(-5, 5)), -10); }

TEST(op_mul_rr, rr_m5_5) { EXPECT_EQ((invoke_binary<functions::op_mul_rr>(-5., 5.)), -25.); }
TEST(op_mul_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<functions::op_mul_ii>(-5, 5)), -25); }

TEST(op_div_rr, rr_m5_5) { EXPECT_EQ((invoke_binary<functions::op_div_rr>(-5., 5.)), -1.); }
TEST(op_div_ii, ii_m5_5) { EXPECT_EQ((invoke_binary<functions::op_div_ii>(-5, 5)), -1); }
TEST(op_div_ii, ii_8_3) { EXPECT_EQ((invoke_binary<functions::op_div_ii>(8, 3)), 2); }

TEST(op_mod_ii, ii_m1_3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(-1, 3)), -1); }
TEST(op_mod_ii, ii_0_3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(-0, 3)), 0); }
TEST(op_mod_ii, ii_1_3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(1, 3)), 1); }
TEST(op_mod_ii, ii_m1_m3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(-1, -3)), -1); }
TEST(op_mod_ii, ii_0_m3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(0, -3)), 0); }
TEST(op_mod_ii, ii_1_m3) { EXPECT_EQ((invoke_binary<functions::op_mod_ii>(1, -3)), 1); }

TEST(op_pow_rr, r_m5_5) { EXPECT_EQ((invoke_binary<functions::op_pow_rr>(-5., 5.)), -3125.); }

TEST(op_eq_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_eq_rr>(2., 2.)), true); }
TEST(op_ne_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_ne_rr>(2., 2.)), false); }
TEST(op_ge_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_ge_rr>(2., 2.)), true); }
TEST(op_gt_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_gt_rr>(2., 2.)), false); }
TEST(op_le_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_le_rr>(2., 2.)), true); }
TEST(op_lt_rr, rr_2_2) { EXPECT_EQ((invoke_binary<functions::op_lt_rr>(2., 2.)), false); }

TEST(op_eq_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_eq_ii>(2, 2)), true); }
TEST(op_ne_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_ne_ii>(2, 2)), false); }
TEST(op_ge_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_ge_ii>(2, 2)), true); }
TEST(op_gt_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_gt_ii>(2, 2)), false); }
TEST(op_le_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_le_ii>(2, 2)), true); }
TEST(op_lt_ii, ii_2_2) { EXPECT_EQ((invoke_binary<functions::op_lt_ii>(2, 2)), false); }

TEST(op_eq_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_eq_bb>(true, true)), true); }
TEST(op_ne_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_ne_bb>(true, true)), false); }
TEST(op_ge_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_ge_bb>(true, true)), true); }
TEST(op_gt_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_gt_bb>(true, true)), false); }
TEST(op_le_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_le_bb>(true, true)), true); }
TEST(op_lt_bb, bb_2_2) { EXPECT_EQ((invoke_binary<functions::op_lt_bb>(true, true)), false); }

TEST(op_binv_i, i_172) { EXPECT_EQ((invoke_unary<functions::op_binv_i>(172)), -173); }
TEST(op_band_ii, ii_172_163) { EXPECT_EQ((invoke_binary<functions::op_band_ii>(172, 163)), 160); }
TEST(op_bxor_ii, ii_172_163) { EXPECT_EQ((invoke_binary<functions::op_bxor_ii>(172, 163)), 15); }
TEST(op_bor_ii, ii_172_163) { EXPECT_EQ((invoke_binary<functions::op_bor_ii>(172, 163)), 175); }

TEST(op_shl_ii, ii_172_3) { EXPECT_EQ((invoke_binary<functions::op_shl_ii>(172, 3)), 1376); }
TEST(op_shr_ii, ii_172_3) { EXPECT_EQ((invoke_binary<functions::op_shr_ii>(172, 3)), 21); }

TEST(op_linv_b, b_true) { EXPECT_EQ((invoke_unary<functions::op_linv_b>(true)), false); }
TEST(op_land_bb, bb_true_true) { EXPECT_EQ((invoke_binary<functions::op_land_bb>(true, true)), true); }
TEST(op_lxor_bb, bb_true_true) { EXPECT_EQ((invoke_binary<functions::op_lxor_bb>(true, true)), false); }
TEST(op_lor_bb, bb_true_true) { EXPECT_EQ((invoke_binary<functions::op_lor_bb>(true, true)), true); }

TEST(op_tcnd_brr, brr_true_m5_5) { EXPECT_EQ((invoke_ternary<functions::op_tcnd_brr>(true, -5., 5.)), -5.); }
TEST(op_tcnd_bii, bii_true_m5_5) { EXPECT_EQ((invoke_ternary<functions::op_tcnd_bii>(true, -5, 5)), -5); }
TEST(op_tcnd_bbb, bbb_true_true_false) { EXPECT_EQ((invoke_ternary<functions::op_tcnd_bbb>(true, true, false)), true); }

TEST(fn_sqrt_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_sqrt_r>(2.)), 1.41421, 0.00001); }
TEST(fn_exp_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_exp_r>(2.)), 7.38905, 0.00001); }
TEST(fn_log_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_log_r>(2.)), 0.69314, 0.00001); }
TEST(fn_sin_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_sin_r>(2.)), 0.90929, 0.00001); }
TEST(fn_cos_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_cos_r>(2.)), -0.41614, 0.00001); }
TEST(fn_tan_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_tan_r>(2.)), -2.18503, 0.00001); }
TEST(fn_sinh_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_sinh_r>(2.)), 3.62686, 0.00001); }
TEST(fn_cosh_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_cosh_r>(2.)), 3.76219, 0.00001); }
TEST(fn_tanh_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_tanh_r>(2.)), 0.96402, 0.00001); }
TEST(fn_asin_r, r_point_2) { EXPECT_NEAR((invoke_unary<functions::fn_asin_r>(.2)), 0.20135, 0.00001); }
TEST(fn_acos_r, r_point_2) { EXPECT_NEAR((invoke_unary<functions::fn_acos_r>(.2)), 1.36943, 0.00001); }
TEST(fn_atan_r, r_point_2) { EXPECT_NEAR((invoke_unary<functions::fn_atan_r>(.2)), 0.19739, 0.00001); }
TEST(fn_asinh_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_asinh_r>(2.)), 1.44363, 0.00001); }
TEST(fn_acosh_r, r_2) { EXPECT_NEAR((invoke_unary<functions::fn_acosh_r>(2.)), 1.31695, 0.00001); }
TEST(fn_atanh_r, r_point_2) { EXPECT_NEAR((invoke_unary<functions::fn_atanh_r>(.2)), 0.20273, 0.00001); }
