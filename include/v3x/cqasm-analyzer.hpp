/** \file
 * This file contains the \ref cqasm::v3x::analyzer::Analyzer "Analyzer" class and support classes,
 * used to manage semantic analysis.
 *
 * While the Analyzer class itself only manages the semantic analysis phase,
 * it also has some convenience methods that drive lexical analysis and parsing in addition.
 */

#pragma once

#include "cqasm-ast.hpp"
#include "cqasm-parse-helper.hpp"
#include "cqasm-resolver.hpp"
#include "cqasm-semantic.hpp"

#include <cstdio>
#include <functional>


/**
 * Namespace for the \ref cqasm::analyzer::Analyzer "Analyzer" class and support classes.
 */
namespace cqasm::v3x::analyzer {

/**
 * Exception thrown by AnalysisResult::unwrap() when the cQASM file fails to
 * parse.
 */
class AnalysisFailed: public std::runtime_error {
public:
    AnalysisFailed() : std::runtime_error("cQASM analysis failed") {};
};

/**
 * Analysis result class.
 *
 * An object of this type is returned by the various `analyze*()` methods of the Analyzer class.
 * There are three possibilities:
 *
 *  - Analysis was successful.
 *    In this case, \ref errors is empty, and \ref root contains a valid \ref semantic::Program "Program" tree.
 *  - Analysis failed.
 *    In this case, there is at least one string in \ref errors.
 *    \ref root may be an empty reference, a partial tree, or even a complete but somehow invalid tree.
 *  - Analysis failed spectacularly with an internal error,
 *    in which case an exception is thrown instead of this object being returned.
 *
 * If you don't want to manage printing error messages yourself and would like an exception upon failure in all cases,
 * you can call unwrap() on the returned object.
 */
class AnalysisResult {
public:
    /**
     * Root node of the semantic tree, if analysis was successful.
     * The node may be empty or incomplete if errors is non-empty;
     * use unwrap() to ensure it's complete if you just want to use it and
     * don't care about things like pretty-printing error messages for your users
     * in a different way than what unwrap() does for you.
     */
    ast::One<semantic::Program> root;

    /**
     * List of accumulated errors.
     * Analysis was successful if and only if `errors.empty()`.
     */
    std::vector<std::string> errors;

    /**
     * "Unwraps" the result (as you would in Rust) to get the program node or an exception.
     * The exception is always an AnalysisFailed, deriving from std::runtime_error.
     * The actual error messages are in this case first written to the given output stream, defaulting to stderr.
     */
    ast::One<semantic::Program> unwrap(std::ostream &out = std::cerr) const;
};

/**
 * Main class used for analyzing cQASM files.
 *
 * Construction of this class is the entry point for libqasm whenever you need
 * to modify the default instruction set, have a different set of supported
 * error models, or want to add additional initial mappings, operators, or
 * functions. The process is simple:
 *
 *  - Construct an Analyzer object with the default constructor.
 *  - Use zero or more of the various `register_*()` methods to configure the
 *    Analyzer.
 *  - Use one or more of the `analyze*()` methods to analyze cQASM files or
 *    string representations thereof.
 *
 * Note that the only state maintained by the Analyzer object is its
 * configuration, and the `analyze*()` functions never change this state
 * (hence they are const).
 */
class Analyzer {
    /**
     * The maximum cQASM version that this analyzer supports.
     */
    primitives::Version api_version;

    /**
     * The supported set of quantum/classical/mixed instructions,
     * appearing in the cQASM file as assembly-like commands.
     * Instructions have a case-sensitively matched name, and
     * a signature for the types of parameters it expects.
     */
    resolver::InstructionTable instruction_set;

public:
    /**
     * Creates a new semantic analyzer.
     */
    explicit Analyzer(const primitives::Version &api_version = "3.0");

    /**
     * Registers an instruction type.
     */
    void register_instruction(const instruction::Instruction &instruction);

    /**
     * Convenience method for registering an instruction type.
     * The arguments are passed straight to instruction::Instruction's constructor.
     */
    void register_instruction(
        const std::string &name,
        const std::string &param_types = ""
    );

    /**
     * Analyzes the given program AST node.
     */
    [[nodiscard]] AnalysisResult analyze(const ast::Program &program) const;

    /**
     * Analyzes the given parse result.
     * If there are parse errors, they are copied into the AnalysisResult error list, and
     * the root node will be empty.
     */
    [[nodiscard]] AnalysisResult analyze(const parser::ParseResult &parse_result) const;

    /**
     * Parses and analyzes using the given version and parser closures.
     */
    [[nodiscard]] AnalysisResult analyze(
        const std::function<version::Version()> &version_parser,
        const std::function<parser::ParseResult()> &parser
    ) const;

    /**
     * Parses and analyzes the given file.
     */
    [[nodiscard]] AnalysisResult analyze(const std::string &filename) const;

    /**
     * Parses and analyzes the given string.
     * The optional filename argument will be used only for error messages.
     */
    [[nodiscard]] AnalysisResult analyze_string(const std::string &data, const std::string &filename = "<unknown>") const;

    /**
     * Returns the API version.
     */
    [[nodiscard]] primitives::Version get_api_version() const;
};

} // namespace cqasm::v3x::analyzer
