/**
 * \file
 * Main include file for parsing v3x files.
 */

#pragma once

#include <optional>

#include "libqasm/tree.hpp"
#include "libqasm/v3x/analyzer.hpp"
#include "libqasm/v3x/semantic.hpp"

/**
 * Namespace for the cQASM v3x API.
 * Its contents are pulled into the main cQASM namespace when you include "cqasm.hpp" for compatibility.
 */
namespace cqasm::v3x {

/**
 * Parses and analyzes the given file path with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<cqasm::v3x::semantic::Program> analyze_file(
    const std::string &file_path, const std::string &api_version = "3.0");

/**
 * Parses and analyzes the given string with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<cqasm::v3x::semantic::Program> analyze_string(
    const std::string &data, const std::optional<std::string> &file_name, const std::string &api_version = "3.0");

/**
 * Constructs an Analyzer object with the defaults for cQASM 3.0 already loaded into it.
 */
analyzer::Analyzer default_analyzer(const std::string &api_version = "3.0");

}  // namespace cqasm::v3x
