/** \file
 * This file contains the \ref cqasm::v3x::analyzer::Analyzer "Analyzer" class and support classes,
 * used to manage semantic analysis.
 *
 * While the Analyzer class itself only manages the semantic analysis phase,
 * it also has some convenience methods that drive lexical analysis and parsing in addition.
 */

#pragma once

#include "cqasm-analysis-result.hpp"
#include "cqasm-analyzer.hpp"
#include "cqasm-ast.hpp"
#include "cqasm-parse-helper.hpp"
#include "cqasm-resolver.hpp"
#include "cqasm-semantic.hpp"

#include <functional>
#include <string>


/**
 * Namespace for the \ref cqasm::analyzer::Analyzer "Analyzer" class and support classes.
 */
namespace cqasm::v3x::analyzer {

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
public:
    /**
     * The maximum cQASM version that this analyzer supports.
     */
    primitives::Version api_version;

private:
    /**
     * The set of "mappings" that the parser starts out with.
     */
    resolver::MappingTable mappings;

    /**
     * The functions visible to the analyzer.
     */
    resolver::FunctionTable functions;

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
     * Registers mappings for pi, eu (aka e, 2.718...), tau and im (imaginary unit).
     */
    void register_default_mappings();

    /**
     * Registers a number of default functions, such as the operator functions, and the usual trigonometric functions.
     */
    void register_default_functions();

    /**
     * Analyzes the given program AST node.
     */
    [[nodiscard]] AnalysisResult analyze(ast::Program &program);

    /**
     * Analyzes the given parse result.
     * If there are parse errors, they are copied into the AnalysisResult error list, and
     * the root node will be empty.
     */
    [[nodiscard]] AnalysisResult analyze(const parser::ParseResult &parse_result);

    /**
     * Parses and analyzes using the given version and parser closures.
     */
    [[nodiscard]] AnalysisResult analyze(
        const std::function<version::Version()> &version_parser,
        const std::function<parser::ParseResult()> &parser
    );

    /**
     * Parses and analyzes the given file.
     */
    [[nodiscard]] AnalysisResult analyze_file(const std::string &filename);

    /**
     * Parses and analyzes the given string.
     * The optional filename argument will be used only for error messages.
     */
    [[nodiscard]] AnalysisResult analyze_string(
        const std::string &data, const std::string &filename = "<unknown>");

    /**
     * Resolves a mapping.
     * Throws NameResolutionFailure if no mapping by the given name exists.
     */
    [[nodiscard]] values::Value resolve_mapping(const std::string &name) const;

    /**
     * Registers a mapping.
     */
    void register_mapping(const std::string &name, const values::Value &value);

    /**
     * Calls a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments, or otherwise
     * returns the value returned by the function.
     */
    [[nodiscard]] values::Value call_function(const std::string &name, const values::Values &args) const;

    /**
     * Registers a function, usable within expressions.
     */
    void register_function(
        const std::string &name,
        const types::Types &param_types,
        const resolver::FunctionImpl &impl);

    /**
     * Convenience method for registering a function.
     * The param_types are specified as a string,
     * converted to types::Types for the other overload using types::from_spec.
     */
    void register_function(
        const std::string &name,
        const std::string &param_types,
        const resolver::FunctionImpl &impl);

    /**
     * Resolves an instruction.
     * Throws NameResolutionFailure if no instruction by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
     * returns the resolved instruction node.
     * Annotation data, line number information, and the condition still need to be set by the caller.
     */
    [[nodiscard]] tree::One<semantic::Instruction> resolve_instruction(
        const std::string &name, const values::Values &args) const;

    /**
     * Registers an instruction type.
     */
    void register_instruction(const instruction::Instruction &instruction);

    /**
     * Convenience method for registering an instruction type.
     * The arguments are passed straight to instruction::Instruction's constructor.
     */
    void register_instruction(const std::string &name, const std::string &param_types = "");
};

} // namespace cqasm::v3x::analyzer
