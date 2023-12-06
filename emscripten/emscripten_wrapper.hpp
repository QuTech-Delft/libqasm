#pragma once

#include <emscripten/bind.h>
#include <string>


struct EmscriptenWrapper {
    EmscriptenWrapper();

    /**
     * Returns libqasm version.
     */
    std::string get_version();

    /**
     * Parses the given string.
     * The file must be in v3.x syntax.
     * No version check or conversion is performed.
     * Returns a vector of strings,
     * of which the first is reserved for the CBOR serialization of the v3.x AST.
     * Any additional strings represent error messages.
     * The filename, if specified, is only used when reporting errors.
     */
    std::string parse_string_to_json(const std::string &data, const std::string &filename = "<unknown>");

    /**
     * Parses and analyzes the given string.
     * Returns a vector of strings,
     * of which the first is reserved for the CBOR serialization of the v3.x semantic tree.
     * Any additional strings represent error messages.
     * The filename, if specified, is only used when reporting errors.
     */
    std::string analyze_string_to_json(const std::string &data, const std::string &filename = "<unknown>");
};


EMSCRIPTEN_BINDINGS(CqasmJS) {
    emscripten::class_<EmscriptenWrapper>("EmscriptenWrapper")
    .constructor()
    .function("get_version", &EmscriptenWrapper::get_version)
    .function("parse_string_to_json", &EmscriptenWrapper::parse_string_to_json)
    .function("analyze_string_to_json", &EmscriptenWrapper::analyze_string_to_json);
}
