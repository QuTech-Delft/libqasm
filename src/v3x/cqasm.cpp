/**
 * \dir
 * Contains the non-generated source files and private header files for libqasm.
 *
 * \file
 * Implementation for \ref include/v3x/cqasm.hpp "v3x/cqasm.hpp".
 */

#include "cqasm-version.hpp"
#include "v1x/cqasm-parse-result.hpp"
#include "v3x/cqasm.hpp"
#include "v3x/cqasm-parse-helper.hpp"

#include <stdexcept>  // runtime_error


namespace cqasm {
namespace v3x {

/**
 * Parses and analyzes the given file path with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<cqasm::v1x::semantic::Program> analyze(
    const std::string &file_path,
    const std::string &api_version
) {
    return default_analyzer(api_version).analyze(
            [=](){ return version::parse_file(file_path); },
            [=](){ return parser::parse_file(file_path); }
        ).unwrap();
}

/**
 * Parses and analyzes the given string with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<cqasm::v1x::semantic::Program> analyze_string(
    const std::string &data,
    const std::string &file_name,
    const std::string &api_version
) {
    return default_analyzer(api_version).analyze(
            [=](){ return version::parse_string(data, file_name); },
            [=](){ return parser::parse_string(data, file_name); }
        ).unwrap();
}

/**
 * Constructs an Analyzer object with the defaults for cQASM 1.0 already loaded into it.
 */
cqasm::v1x::analyzer::Analyzer default_analyzer(const std::string &api_version) {
    cqasm::v1x::analyzer::Analyzer analyzer{api_version};

    // Register the default mappings (true, false, pi, x, y, z, etc.) and
    // functions (operators, things like trigonometric functions, etc.).
    analyzer.register_default_functions_and_mappings();

    // Register the error models.
    // Originally, depolarizing_channel accepted any number of floating point arguments,
    // but the new parser doesn't support that,
    // so we just brute-force the first 50 into it and call it a day.
    std::ostringstream args;
    for (int i = 0; i <= 50; i++) {
        analyzer.register_error_model("depolarizing_channel", args.str());
        args << "r";
    }

    // Register a toy version of the cQASM 1.0 instruction set.
    analyzer.register_instruction("measure_all", "", false, false);
    analyzer.register_instruction("x", "Q");
    analyzer.register_instruction("y", "Q");
    analyzer.register_instruction("z", "Q");
    analyzer.register_instruction("i", "Q");
    analyzer.register_instruction("h", "Q");
    analyzer.register_instruction("prep", "Q", false);
    analyzer.register_instruction("prep_x", "Q", false);
    analyzer.register_instruction("prep_y", "Q", false);
    analyzer.register_instruction("prep_z", "Q", false);
    analyzer.register_instruction("measure", "Q", false);
    analyzer.register_instruction("measure_x", "Q", false);
    analyzer.register_instruction("measure_y", "Q", false);
    analyzer.register_instruction("measure_z", "Q", false);
    analyzer.register_instruction("cnot", "QQ");

    return analyzer;
}

} // namespace v3x
} // namespace cqasm
