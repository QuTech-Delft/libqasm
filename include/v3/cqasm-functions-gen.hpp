
/** \file
 * Header file generated by \ref func-gen.
 */

#pragma once

#include "v3/cqasm-resolver.hpp"

namespace cqasm {
namespace v3 {
namespace functions {
values::Value op_add_cc(const values::Values &v);
values::Value op_add_rr(const values::Values &v);
values::Value op_add_ii(const values::Values &v);
values::Value op_add_ss(const values::Values &v);
values::Value op_sub_cc(const values::Values &v);
values::Value op_sub_rr(const values::Values &v);
values::Value op_sub_ii(const values::Values &v);
values::Value op_neg_c(const values::Values &v);
values::Value op_neg_r(const values::Values &v);
values::Value op_neg_i(const values::Values &v);
values::Value op_mul_cc(const values::Values &v);
values::Value op_mul_rr(const values::Values &v);
values::Value op_mul_ii(const values::Values &v);
values::Value op_div_cc(const values::Values &v);
values::Value op_div_rr(const values::Values &v);
values::Value op_int_div_ii(const values::Values &v);
values::Value op_mod_ii(const values::Values &v);
values::Value op_pow_cc(const values::Values &v);
values::Value op_pow_rr(const values::Values &v);
values::Value op_eq_cc(const values::Values &v);
values::Value op_ne_cc(const values::Values &v);
values::Value op_eq_rr(const values::Values &v);
values::Value op_ne_rr(const values::Values &v);
values::Value op_ge_rr(const values::Values &v);
values::Value op_gt_rr(const values::Values &v);
values::Value op_le_rr(const values::Values &v);
values::Value op_lt_rr(const values::Values &v);
values::Value op_eq_ii(const values::Values &v);
values::Value op_ne_ii(const values::Values &v);
values::Value op_ge_ii(const values::Values &v);
values::Value op_gt_ii(const values::Values &v);
values::Value op_le_ii(const values::Values &v);
values::Value op_lt_ii(const values::Values &v);
values::Value op_eq_bb(const values::Values &v);
values::Value op_ne_bb(const values::Values &v);
values::Value op_ge_bb(const values::Values &v);
values::Value op_gt_bb(const values::Values &v);
values::Value op_le_bb(const values::Values &v);
values::Value op_lt_bb(const values::Values &v);
values::Value op_binv_i(const values::Values &v);
values::Value op_band_ii(const values::Values &v);
values::Value op_bxor_ii(const values::Values &v);
values::Value op_bor_ii(const values::Values &v);
values::Value op_shl_ii(const values::Values &v);
values::Value op_sra_ii(const values::Values &v);
values::Value op_srl_ii(const values::Values &v);
values::Value op_linv_b(const values::Values &v);
values::Value op_land_bb(const values::Values &v);
values::Value op_lxor_bb(const values::Values &v);
values::Value op_lor_bb(const values::Values &v);
values::Value op_tcnd_bcc(const values::Values &v);
values::Value op_tcnd_brr(const values::Values &v);
values::Value op_tcnd_bii(const values::Values &v);
values::Value op_tcnd_bbb(const values::Values &v);
values::Value fn_sqrt_c(const values::Values &v);
values::Value fn_exp_c(const values::Values &v);
values::Value fn_log_c(const values::Values &v);
values::Value fn_sin_c(const values::Values &v);
values::Value fn_cos_c(const values::Values &v);
values::Value fn_tan_c(const values::Values &v);
values::Value fn_sinh_c(const values::Values &v);
values::Value fn_cosh_c(const values::Values &v);
values::Value fn_tanh_c(const values::Values &v);
values::Value fn_asin_c(const values::Values &v);
values::Value fn_acos_c(const values::Values &v);
values::Value fn_atan_c(const values::Values &v);
values::Value fn_asinh_c(const values::Values &v);
values::Value fn_acosh_c(const values::Values &v);
values::Value fn_atanh_c(const values::Values &v);
values::Value fn_sqrt_r(const values::Values &v);
values::Value fn_exp_r(const values::Values &v);
values::Value fn_log_r(const values::Values &v);
values::Value fn_sin_r(const values::Values &v);
values::Value fn_cos_r(const values::Values &v);
values::Value fn_tan_r(const values::Values &v);
values::Value fn_sinh_r(const values::Values &v);
values::Value fn_cosh_r(const values::Values &v);
values::Value fn_tanh_r(const values::Values &v);
values::Value fn_asin_r(const values::Values &v);
values::Value fn_acos_r(const values::Values &v);
values::Value fn_atan_r(const values::Values &v);
values::Value fn_asinh_r(const values::Values &v);
values::Value fn_acosh_r(const values::Values &v);
values::Value fn_atanh_r(const values::Values &v);
values::Value fn_abs_r(const values::Values &v);
values::Value fn_abs_i(const values::Values &v);
values::Value fn_complex_rr(const values::Values &v);
values::Value fn_polar_rr(const values::Values &v);
values::Value fn_real_c(const values::Values &v);
values::Value fn_imag_c(const values::Values &v);
values::Value fn_arg_c(const values::Values &v);
values::Value fn_norm_c(const values::Values &v);
values::Value fn_conj_c(const values::Values &v);

/**
 * Registers a bunch of functions usable during constant propagation into the
 * given function table.
 */
void register_into(resolver::FunctionTable &table);

} // namespace functions
} // namespace v3
} // namespace cqasm