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
#include <string>
#include <vector>

// Forward declarations for internal types.
namespace cqasm::v3x::analyzer { class Analyzer; }

/**
 * Main class for parsing and analyzing cQASM files with the v3.x API.
 */
class V3xAnalyzer {
    /**
     * Reference to the actual C++ analyzer that this wraps.
     */
    std::unique_ptr<cqasm::v3x::analyzer::Analyzer> a;

public:
    /**
     * Creates a new v3.x semantic analyzer.
     * When without_defaults is specified,
     * the default instruction set and error models are not loaded into the instruction and error model tables,
     * so you have to specify the entire instruction set using register_instruction() and register_error_model().
     * Otherwise, those functions only add to the defaults.
     * Unlike the C++ version of the analyzer class,
     * the initial mappings and functions are not configurable at all.
     * The defaults for these are always used.
     */
    V3xAnalyzer(
        const std::string &max_version = "3.0",
        bool without_defaults=false
    );

    /**
     * Registers an instruction type.
     * The arguments are passed straight to instruction::Instruction's constructor.
     */
    void register_instruction(const std::string &name, const std::string &param_types = "");

    /**
     * Only parses the given file.
     * The file must be in v3.x syntax.
     * No version check or conversion is performed.
     * Returns a vector of strings,
     * of which the first is always present and is the CBOR serialization of the v3.x AST.
     * Any additional strings represent error messages.
     */
    static std::vector<std::string> parse_file(
        const std::string &filename
    );

    /**
     * Same as parse_file(), but instead receives the file contents directly.
     * The filename, if specified, is only used when reporting errors.
     */
    static std::vector<std::string> parse_string(
        const std::string &data,
        const std::string &filename = "<unknown>"
    );

    /**
     * Parses and analyzes the given file.
     * If the file is written in a later file version,
     * this function may try to reduce it to the maximum v3.x API version support advertised
     * using this object's constructor.
     * Returns a vector of strings,
     * of which the first is always present and is the CBOR serialization of the v3.x semantic tree.
     * Any additional strings represent error messages.
     */
    std::vector<std::string> analyze_file(
        const std::string &filename
    ) const;

    /**
     * Same as analyze_file(), but instead receives the file contents directly.
     * The filename, if specified, is only used when reporting errors.
     */
    std::vector<std::string> analyze_string(
        const std::string &data,
        const std::string &filename = "<unknown>"
    ) const;
};
