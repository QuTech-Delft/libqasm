#include "emscripten_wrapper.hpp"

#include "v3x/cqasm-py.hpp"
#include "version.hpp"


EmscriptenWrapper::EmscriptenWrapper() {}


/**
 * Returns libqasm version.
 */
std::string EmscriptenWrapper::get_version() {
    return cqasm::get_version();
}


/**
 * Parses the given string.
 * The file must be in v3.x syntax.
 * No version check or conversion is performed.
 * Returns a vector of strings,
 * of which the first is reserved for the CBOR serialization of the v3.x AST.
 * Any additional strings represent error messages.
 * The filename, if specified, is only used when reporting errors.
 */
std::string EmscriptenWrapper::parse_string_to_json(const std::string &data, const std::string &filename) {
    return {};
//    return V3xAnalyzer::parse_string_to_json(data, filename);
}


/**
 * Parses and analyzes the given string.
 * Returns a vector of strings,
 * of which the first is reserved for the CBOR serialization of the v3.x semantic tree.
 * Any additional strings represent error messages.
 * The filename, if specified, is only used when reporting errors.
 */
std::string EmscriptenWrapper::analyze_string_to_json(const std::string &data, const std::string &filename) {
    return {};
//    return V3xAnalyzer{}.analyze_string_to_json(data, filename);
}
