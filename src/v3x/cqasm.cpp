/**
 * \dir
 * Contains the non-generated source files and private header files for libqasm.
 *
 * \file
 * Implementation for \ref include/libqasm/v3x/cqasm.hpp "v3x/cqasm.hpp".
 */

#include "libqasm/v3x/cqasm.hpp"

#include <stdexcept>  // runtime_error

#include "libqasm/v3x/parse_helper.hpp"

namespace cqasm::v3x {

/**
 * Parses and analyzes the given file path with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<cqasm::v3x::semantic::Program> analyze_file(const std::string& file_path, const std::string& api_version) {
    const auto& parse_result = cqasm::v3x::parser::parse_file(file_path, std::nullopt);
    const auto& analysis_result = cqasm::v3x::default_analyzer(api_version).analyze(parse_result);
    return analysis_result.unwrap();
}

/**
 * Parses and analyzes the given string with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<cqasm::v3x::semantic::Program> analyze_string(
    const std::string& data, const std::optional<std::string>& file_name, const std::string& api_version) {
    const auto& parse_result = cqasm::v3x::parser::parse_string(data, file_name);
    const auto& analysis_result = cqasm::v3x::default_analyzer(api_version).analyze(parse_result);
    return analysis_result.unwrap();
}

/**
 * Constructs an Analyzer object with the defaults for cQASM 3.0 already loaded into it.
 */
analyzer::Analyzer default_analyzer(const std::string& api_version) {
    analyzer::Analyzer analyzer{ api_version };

    analyzer.register_default_constants();
    analyzer.register_default_functions();
    analyzer.register_default_instructions();

    return analyzer;
}

}  // namespace cqasm::v3x
