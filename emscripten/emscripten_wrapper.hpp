#pragma once

#include <emscripten/bind.h>

#include <string>

/**
 * Main class for parsing and analyzing cQASM files.
 *
 * This class works as a binding for accessing C++ code from Typescript.
 *
 * **Example**:
 *
 *     import { default as wrapper } from 'cqasm_emscripten.mjs';
 *
 *     wrapper().then(function(result: any) {
 *         let cqasm = new result["EmscriptenWrapper"]()
 *         let program = ...
 *         let output = cqasm.parse_string_to_json(program, ...)
 *         cqasm.delete()
 *     }).catch((error: any) => {
 *         console.error("unhandledRejection", error, "\n");
 *     });
 */
struct EmscriptenWrapper {
    EmscriptenWrapper() = default;

    /**
     * Returns the version of the `libqasm` library.
     *
     * **Example**:
     *
     *     let version = cqasm.get_version()
     */
    std::string get_version();

    /**
     * Parses a string containing a cQASM v3.0 program.
     *
     * No version check is performed.
     *
     * The `file_name` is only used when reporting errors.
     *
     * Returns a string.
     * If the parsing was successful, the string contains a syntactic Abstract Syntax Tree (AST).
     * Otherwise, it will contain a list of errors.
     * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
     * Every error is mapped to an LSP Diagnostic structure:
     * `severity` is hardcoded to 1 at the moment (value corresponding to an Error level).
     *
     *  **Example**:
     *
     *      let program = `
     *          version 3
     *          qubit[2] q
     *          bit[2] b
     *          H q[0]
     *          CNOT q[0], q[1]
     *          b = measure q
     *      `
     *      let output = parse_string_to_json(program, "bell.cq")
     */
    std::string parse_string_to_json(const std::string& data, const std::string& file_name);

    /**
     * Parses and analyzes a string containing a cQASM v3.0 program.
     *
     * No version check is performed.
     *
     * The `file_name` is only used when reporting errors.
     *
     * Returns a string.
     * If the parsing was successful, the string contains a semantic Abstract Syntax Tree (AST).
     * Otherwise, it will contain a list of errors.
     * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
     * Every error is mapped to an LSP Diagnostic structure:
     * `severity` is hardcoded to 1 at the moment (value corresponding to an Error level).
     *
     *  **Example**:
     *
     *      let program = `
     *          version 3
     *          qubit[2] q
     *          bit[2] b
     *          H q[0]
     *          CNOT q[0], q[1]
     *          b = measure q
     *      `
     *      let output = analyze_string_to_json(program, "bell.cq")
     */
    std::string analyze_string_to_json(const std::string& data, const std::string& file_name);
};

EMSCRIPTEN_BINDINGS(CqasmJS) {
    emscripten::class_<EmscriptenWrapper>("EmscriptenWrapper")
        .constructor()
        .function("get_version", &EmscriptenWrapper::get_version)
        .function("parse_string_to_json", &EmscriptenWrapper::parse_string_to_json)
        .function("analyze_string_to_json", &EmscriptenWrapper::analyze_string_to_json);
}
