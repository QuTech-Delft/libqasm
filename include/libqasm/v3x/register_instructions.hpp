#pragma once

#include "libqasm/v3x/analyzer.hpp"

namespace cqasm::v3x::instruction {

/**
 * Registers the cQASM 3.0 instruction set.
 */
void register_instructions(analyzer::Analyzer* analyzer);

}  // namespace cqasm::v3x::instruction
