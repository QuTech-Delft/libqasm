/** \file
 * Defines SWIG'd things for the Python interface.
 * This is just the *internal* interface to the library.
 * The user-facing API is written in pure-Python and wraps around this.
 */

#pragma once

// Don't include any libqasm headers!
// We don't want SWIG to generate Python wrappers for the entire world.
// Those headers are only included in the source file that provides the implementations.
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Forward declarations for internal types.
namespace cqasm::v3x::analyzer {
class Analyzer;
}

/**
 * Main class for parsing and analyzing cQASM v3.0 files.
 *
 * This class works as a binding for accessing C++ code from Python.
 *
 * The parsing methods are static because they do not change the status of the analyzer.
 * Instead, they just invoke free functions that create a temporary instance of a parser.
 *
 * None of the parsing or the analyzing methods perform any version check.
 *
 * `parse_file`, `parse_string`, `analyze_file`, and `analyze_string`:
 *
 *   - Return a vector of strings.
 *     The first string is reserved for the CBOR serialization
 *     of the syntactic/semantic (in the case of parsing/analyzing) Abstract Syntax Tree (AST)
 *     of the input program.
 *     Any additional strings represent error messages.
 *     Notice that the AST and error messages will not be available at the same time.
 *
 * `parse_file_to_json`, `parse_string_to_json`, `analyze_file_to_json`, and `analyze_string_to_json`:
 *
 *   - Return a string in JSON format.
 *     If the parsing was successful, that string contains a JSON representation
 *     of the AST of the input program.
 *     Otherwise, it will contain a list of errors.
 *     The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 *     Every error is mapped to an LSP Diagnostic structure:
 *     `severity` is hardcoded to 1 at the moment (value corresponding to an Error level).
 *
 *   `parse_string`, `parse_string_to_json`, `analyze_string, and `analyze_string_to_json`:
 *
 *     - have an optional second argument: `file_name`. It is only used when reporting errors.
 *
 *  **Example**:
 *
 *      auto result = analyze_file("grover.cq");
 *
 *  **Example**:
 *
 *      auto program = std::string{ R"(
 *          version 3.0
 *          qubit[2] q
 *          bit[2] b
 *          H q[0]
 *          CNOT q[0], q[1]
 *          b = measure q
 *      )" };
 *      auto result = parse_string_to_json(program, "bell.cq");
 */
class V3xAnalyzer {
    /**
     * Reference to the actual C++ analyzer that this wraps.
     */
    std::unique_ptr<cqasm::v3x::analyzer::Analyzer> analyzer;

public:
    /**
     * Creates a new cQASM v3.0 semantic analyzer.
     *
     * `max_version` is optional. It has a default value of `3.0`. The maximum cQASM version supported by the analyzer.
     *
     * `without_defaults` is optional. If set, the default instruction set is not loaded into the instruction table,
     * so you have to specify the entire instruction set using `register_instruction()`.
     * Otherwise, those functions only add to the defaults.
     *
     * The initial mappings and functions are not configurable.
     * The defaults for these are always used.
     */
    V3xAnalyzer(const std::string& max_version = "3.0", bool without_defaults = false);

    /**
     * Default destructor.
     */
    /*
     * std::unique_ptr<T> requires T to be a complete class for the ~T operation.
     * Since we are using a forward declaration for Analyzer, we need to declare ~V3xAnalyzer in the header file
     * and implement it in the source file.
     */
    ~V3xAnalyzer();

    /**
     * Registers an instruction type.
     *
     * The `param_types` can be:
     *   - `Q` = qubit.
     *   - `V` = qubit array.
     *   - `B` = bit.
     *   - `W` = bit array.
     *   - `i` = int.
     *   - `f` = float.
     *
     *  **Example**:
     *
     *      register_instruction("H", "Q");
     */
    /*
     * The arguments are passed directly to `instruction::Instruction`'s constructor.
     */
    void register_instruction(const std::string& name, const std::optional<std::string>& param_types);

    /**
     * Parses a file containing a cQASM v3.0 program.
     */
    static std::vector<std::string> parse_file(const std::string& file_name);

    /**
     * Parses a file containing a cQASM v3.0 program.
     */
    static std::string parse_file_to_json(const std::string& file_name);

    /**
     * Parses a string containing a cQASM v3.0 program.
     *
     * `file_name` is optional. It is only used when reporting errors.
     */
    static std::vector<std::string> parse_string(const std::string& data, const std::string& file_name = "");

    /**
     * Parses a string containing a cQASM v3.0 program.
     *
     * `file_name` is optional. It is only used when reporting errors.
     */
    static std::string parse_string_to_json(const std::string& data, const std::string& file_name = "");

    /**
     * Parses and analyzes a file containing a cQASM v3.0 program.
     */
    std::vector<std::string> analyze_file(const std::string& file_name) const;

    /**
     * Parses and analyzes a file containing a cQASM v3.0 program.
     */
    std::string analyze_file_to_json(const std::string& file_name) const;

    /**
     * Parses and analyzes a string containing a cQASM v3.0 program.
     *
     * `file_name` is optional. It is only used when reporting errors.
     */
    std::vector<std::string> analyze_string(const std::string& data, const std::string& file_name = "") const;

    /**
     * Parses and analyzes a string containing a cQASM v3.0 program.
     *
     * `file_name` is optional. It is only used when reporting errors.
     */
    std::string analyze_string_to_json(const std::string& data, const std::string& file_name = "") const;
};
