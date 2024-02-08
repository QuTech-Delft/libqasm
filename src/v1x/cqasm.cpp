/**
 * \dir
 * Contains the non-generated source files and private header files for libqasm.
 *
 * \file
 * Implementation for \ref include/v1x/cqasm.hpp "v1x/cqasm.hpp".
 */

#include "cqasm-version.hpp"
#include "v1x/cqasm.hpp"


namespace cqasm::v1x {

/**
 * Parses and analyzes the given file path with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
tree::One<semantic::Program> analyze_file(
    const std::string &file_path,
    const std::string &api_version
) {
    return default_analyzer(api_version).analyze_file(file_path).unwrap();
}

/**
 * Parses and analyzes the given file pointer with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<semantic::Program> analyze_file(
    FILE *fp,
    const std::optional<std::string> &file_name,
    const std::string &api_version
) {
    return default_analyzer(api_version).analyze_file(fp, file_name).unwrap();
}

/**
 * Parses and analyzes the given string with the default analyzer,
 * dumping error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional file_name is only used for error messages.
 */
tree::One<semantic::Program> analyze_string(
    const std::string &data,
    const std::optional<std::string> &file_name,
    const std::string &api_version
) {
    return default_analyzer(api_version).analyze_string(data, file_name).unwrap();
}

/**
 * Constructs an Analyzer object with the defaults for cQASM 1.0 already loaded into it.
 */
analyzer::Analyzer default_analyzer(const std::string &api_version) {
    analyzer::Analyzer analyzer{api_version};

    // Register the default mappings (true, false, pi, x, y, z, etc.) and
    // functions (operators, things like trigonometric functions, etc.).
    analyzer.register_default_functions_and_mappings();

    // Register the error models. Originally, depolarizing_channel accepted any
    // number of floating point arguments, but the new parser doesn't support
    // that, so we just brute-force the first 50 into it and call it a day.
    std::ostringstream args;
    for (int i = 0; i <= 50; i++) {
        analyzer.register_error_model("depolarizing_channel", args.str());
        args << "r";
    }

    // Register the cQASM 1.0 instruction set.
    analyzer.register_instruction("measure_all", "", false, false);
    analyzer.register_instruction("measure_parity", "QaQa", false, false, false, true);
    analyzer.register_instruction("x", "Q");
    analyzer.register_instruction("y", "Q");
    analyzer.register_instruction("z", "Q");
    analyzer.register_instruction("i", "Q");
    analyzer.register_instruction("h", "Q");
    analyzer.register_instruction("x90", "Q");
    analyzer.register_instruction("y90", "Q");
    analyzer.register_instruction("mx90", "Q");
    analyzer.register_instruction("my90", "Q");
    analyzer.register_instruction("s", "Q");
    analyzer.register_instruction("sdag", "Q");
    analyzer.register_instruction("t", "Q");
    analyzer.register_instruction("tdag", "Q");
    analyzer.register_instruction("u", "Qu");
    analyzer.register_instruction("prep", "Q", false);
    analyzer.register_instruction("prep_x", "Q", false);
    analyzer.register_instruction("prep_y", "Q", false);
    analyzer.register_instruction("prep_z", "Q", false);
    analyzer.register_instruction("measure", "Q", false);
    analyzer.register_instruction("measure_x", "Q", false);
    analyzer.register_instruction("measure_y", "Q", false);
    analyzer.register_instruction("measure_z", "Q", false);
    analyzer.register_instruction("rx", "Qr");
    analyzer.register_instruction("ry", "Qr");
    analyzer.register_instruction("rz", "Qr");
    analyzer.register_instruction("cnot", "QQ");
    analyzer.register_instruction("cz", "QQ");
    analyzer.register_instruction("swap", "QQ");
    analyzer.register_instruction("cr", "QQr");
    analyzer.register_instruction("crk", "QQi");
    analyzer.register_instruction("toffoli", "QQQ");
    analyzer.register_instruction("not", "B");
    analyzer.register_instruction("display", "", false, false);
    analyzer.register_instruction("display", "B", false, false);
    analyzer.register_instruction("display_binary", "", false, false);
    analyzer.register_instruction("display_binary", "B", false, false);
    analyzer.register_instruction("skip", "i", false, false);
    analyzer.register_instruction("wait", "Qi", false, false);
    analyzer.register_instruction("barrier", "Q", false, false);
    analyzer.register_instruction("reset-averaging", "", false, false);
    analyzer.register_instruction("reset-averaging", "Q", false, false);
    analyzer.register_instruction("load_state", "s", false, false);

    return analyzer;
}

} // namespace cqasm::v1x
