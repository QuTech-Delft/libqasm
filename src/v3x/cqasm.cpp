/**
 * \dir
 * Contains the non-generated source files and private header files for libqasm.
 *
 * \file
 * Implementation for \ref include/v3x/cqasm.hpp "v3x/cqasm.hpp".
 */

#include "cqasm-version.hpp"
#include "v3x/cqasm.hpp"
#include "v3x/cqasm-parse-helper.hpp"

#include <stdexcept>  // runtime_error


namespace cqasm::v3x {

/**
 * Parses and analyzes the given file path with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<cqasm::v3x::semantic::Program> analyze(
    const std::string &file_path,
    const std::string &api_version
) {
    return cqasm::v3x::default_analyzer(api_version).analyze(
            [&file_path]() { return version::parse_file(file_path); },
            [&file_path]() { return cqasm::v3x::parser::parse_file(file_path); }
        ).unwrap();
}

/**
 * Parses and analyzes the given string with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<cqasm::v3x::semantic::Program> analyze_string(
    const std::string &data,
    const std::string &file_name,
    const std::string &api_version
) {
    return cqasm::v3x::default_analyzer(api_version).analyze(
            [&data, &file_name]() { return version::parse_string(data, file_name); },
            [&data, &file_name]() { return cqasm::v3x::parser::parse_string(data, file_name); }
        ).unwrap();
}

/**
 * Constructs an Analyzer object with the defaults for cQASM 3.0 already loaded into it.
 */
analyzer::Analyzer default_analyzer(const std::string &api_version) {
    return analyzer::Analyzer{ api_version };
}

} // namespace cqasm::v3x
