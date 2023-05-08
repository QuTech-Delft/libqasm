/**
 * \file
 * Main include file for parsing v3 files.
 */

#pragma once

#include "cqasm-parse-helper.hpp"
#include "cqasm-analyzer.hpp"

#include <string>

/**
 * Toplevel namespace with entry points for the v3 API.
 */
namespace cqasm {

/**
 * Namespace for the "new" cQASM 3.x API.
 */
namespace v3 {

static const std::string default_filename{ "<unknown>" };
static const std::string default_api_version{ "3.0" };

/**
 * Parses and analyzes the given file with the default analyzer, dumping error
 * messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<semantic::Program>  analyze(
    const std::string &filename,
    const std::string &api_version = default_api_version
);

/**
 * Parses and analyzes the given file pointer with the default analyzer, dumping
 * error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional filename is only used for error messages.
 */
tree::One<semantic::Program>  analyze(
    FILE *file,
    const std::string &filename = default_filename,
    const std::string &api_version = default_api_version
);

/**
 * Parses and analyzes the given string with the default analyzer, dumping
 * error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional filename is only used for error messages.
 */
tree::One<semantic::Program>  analyze_string(
    const std::string &data,
    const std::string &filename = default_filename,
    const std::string &api_version = default_api_version
);

/**
 * Constructs an Analyzer object with the defaults for cQASM 1.0 already loaded
 * into it.
 */
    analyzer::Analyzer default_analyzer(
        const std::string &api_version = default_api_version
    );

} // namespace v3
} // namespace cqasm
