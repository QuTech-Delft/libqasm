#pragma once

#include "v3x/cqasm-resolver.hpp"


namespace cqasm::v3x::functions {

values::Value op_add_rr(const values::Values &v);
values::Value op_add_ii(const values::Values &v);
values::Value op_sub_rr(const values::Values &v);
values::Value op_sub_ii(const values::Values &v);
values::Value op_neg_r(const values::Values &v);
values::Value op_neg_i(const values::Values &v);
values::Value op_mul_rr(const values::Values &v);
values::Value op_mul_ii(const values::Values &v);
values::Value op_div_rr(const values::Values &v);
values::Value op_mod_ii(const values::Values &v);
values::Value op_pow_rr(const values::Values &v);
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
values::Value op_linv_b(const values::Values &v);
values::Value op_land_bb(const values::Values &v);
values::Value op_lxor_bb(const values::Values &v);
values::Value op_lor_bb(const values::Values &v);
values::Value op_tcnd_brr(const values::Values &v);
values::Value op_tcnd_bii(const values::Values &v);
values::Value op_tcnd_bbb(const values::Values &v);
values::Value fn_abs_i(const values::Values &v);

/**
 * Registers a bunch of functions usable during constant propagation into the given function table.
 */
void register_default_functions_into(resolver::FunctionTable &table);

} // namespace cqasm::v3x::functions
