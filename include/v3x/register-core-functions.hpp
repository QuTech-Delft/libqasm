#pragma once

#include "v3x/cqasm-analyzer.hpp"


class Analyzer;


namespace cqasm::v3x::function {

/**
 * Registers a bunch of core functions.
 * That is, functions supported by the language, and that cannot be evaluated at compile time.
 * For functions supported by the language that can be evaluated at compile time, see ConstEvalCoreFunction.
 */
void register_core_functions(analyzer::Analyzer *analyzer);

} // namespace function
