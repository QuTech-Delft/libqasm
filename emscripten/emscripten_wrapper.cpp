#include "emscripten_wrapper.hpp"

#include "libqasm/v3x/cqasm_python.hpp"
#include "libqasm/versioning.hpp"


/**
 * Returns libqasm version.
 */
std::string EmscriptenWrapper::get_version() {
    return cqasm::get_version();
}


/**
 * Parses a data string containing a v3.x program.
 * The file_name is only used when reporting errors.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x syntactic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
std::string EmscriptenWrapper::parse_string_to_json(const std::string &data, const std::string &file_name) {
    return V3xAnalyzer::parse_string_to_json(data, file_name);
}


/**
 * Parses and analyzes a data string containing a v3.x program.
 * The file_name is only used when reporting errors.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x semantic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
std::string EmscriptenWrapper::analyze_string_to_json(const std::string &data, const std::string &file_name) {
    return V3xAnalyzer{}.analyze_string_to_json(data, file_name);
}
