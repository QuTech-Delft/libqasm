#pragma once

#include <emscripten/bind.h>
#include <string>


struct EmscriptenWrapper {
    EmscriptenWrapper() = default;

    /**
     * Returns libqasm version.
     */
    std::string get_version();

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
    std::string parse_string_to_json(const std::string &data, const std::string &file_name);

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
    std::string analyze_string_to_json(const std::string &data, const std::string &file_name);
};


EMSCRIPTEN_BINDINGS(CqasmJS) {
    emscripten::class_<EmscriptenWrapper>("EmscriptenWrapper")
    .constructor()
    .function("get_version", &EmscriptenWrapper::get_version)
    .function("parse_string_to_json", &EmscriptenWrapper::parse_string_to_json)
    .function("analyze_string_to_json", &EmscriptenWrapper::analyze_string_to_json);
}
