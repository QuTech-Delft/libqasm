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
namespace cqasm::v1x::parser {
    class ParseResult;
}
namespace cqasm::v1x::analyzer {
    class Analyzer;
    class AnalysisResult;
}


/**
 * Main class for parsing and analyzing cQASM files with the v1.x API.
 */
class V1xAnalyzer {
    /**
     * Reference to the actual C++ analyzer that this wraps.
     */
    std::unique_ptr<cqasm::v1x::analyzer::Analyzer> a;

public:
    /**
     * Creates a new v1.x semantic analyzer.
     * When without_defaults is specified,
     * the default instruction set and error models are not loaded into the instruction and error model tables,
     * so you have to specify the entire instruction set using register_instruction() and register_error_model().
     * Otherwise, those functions only add to the defaults.
     * Unlike the C++ version of the analyzer class,
     * the initial mappings and functions are not configurable at all.
     * The defaults for these are always used.
     */
    explicit V1xAnalyzer(
        const std::string &max_version = "1.0",
        bool without_defaults = false
    );

    ~V1xAnalyzer();

    /**
     * Registers an instruction type.
     * The arguments are passed straight to instruction::Instruction's constructor.
     */
    void register_instruction(
        const std::string &name,
        const std::string &param_types = "",
        bool allow_conditional = true,
        bool allow_parallel = true,
        bool allow_reused_qubits = false,
        bool allow_different_index_sizes = false
    );

    /**
     * Registers an error model.
     * The arguments are passed straight to error_model::ErrorModel's constructor.
     */
    void register_error_model(
        const std::string &name,
        const std::string &param_types = ""
    );

    /**
     * Only parses the given file.
     * The file must be in v1.x syntax.
     * No version check or conversion is performed.
     * Returns a vector of strings,
     * of which the first is always present and is the CBOR serialization of the v1.x AST.
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
     * this function may try to reduce it to the maximum v1.x API version support advertised
     * using this object's constructor.
     * Returns a vector of strings,
     * of which the first is always present and is the CBOR serialization of the v1.x semantic tree.
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


class V1xParseResult {
    std::unique_ptr<cqasm::v1x::parser::ParseResult> parse_result;
public:
    V1xParseResult();
    ~V1xParseResult();
    std::string to_json() const;
};


class V1xAnalysisResult {
    std::unique_ptr<cqasm::v1x::analyzer::AnalysisResult> analysis_result;
public:
    V1xAnalysisResult();
    ~V1xAnalysisResult();
    std::string to_json() const;
};
