#pragma once

#include "cqasm-ast.hpp"
#include "cqasm-semantic.hpp"
#include "cqasm-resolver.hpp"
#include <cstdio>

namespace cqasm {
namespace analyzer {

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
 */
class AnalysisResult {
public:

    /**
     * Root node of the semantic tree, if analysis was successful. The node may
     * be empty or incomplete if errors is nonempty; use unwrap() to ensure it's
     * complete if you just want to use it and don't care about things like
     * pretty-printing error messages for your users in a different way than
     * what unwrap() does for you.
     */
    ast::One<semantic::Program> root;

    /**
     * List of accumulated errors. Analysis was successful if and only if
     * `errors.empty()`.
     */
    std::vector<std::string> errors;

    /**
     * "Unwraps" the result (as you would in Rust) to get the program node or
     * an exception. The exception is always an AnalysisFailed, deriving from
     * std::runtime_error. The actual error messages are in this case first
     * written to the given output stream, defaulting to stderr.
     */
    ast::One<semantic::Program> unwrap(std::ostream &out = std::cerr) const;

};

/**
 * Main class used for analyzing cQASM files.
 */
class Analyzer {
private:
    friend class AnalyzerHelper;

    /**
     * The set of "mappings" that the parser starts out with (map statements in
     * the cQASM code mutate a local copy of this).
     */
    resolver::MappingTable mappings;

    /**
     * The supported set of classical functions and operators. Functions have a
     * name (either a case-insensitively matched function name using the usual
     * function notation, or one of the supported operators), a signature
     * for the types of arguments it expects, and a C++ function that takes
     * value nodes of those expected types and returns the resulting value.
     * Note that, once runtime expressions are implemented, the resulting value
     * can be some expression of the incoming values.
     */
    resolver::FunctionTable functions;

    /**
     * The supported set of quantum/classical/mixed instructions, appearing in
     * the cQASM file as assembly-like commands. Instructions have a
     * case-insensitively matched name, a signature for the types of parameters
     * it expects, and some flags indicating how (much) error checking is to
     * be done. You can also add your own metadata through the Annotatable
     * interface.
     */
    resolver::InstructionTable instruction_set;

    /**
     * When set, instruction resolution is disabled. That is, instruction_set
     * is unused, no type promotion is (or can be) performed for instruction
     * parameters, and the instruction field of the semantic::Instruction nodes
     * is left uninitialized.
     */
    bool resolve_instructions;

    /**
     * The supported set of error models. Zero or one of these can be specified
     * in the cQASM file using the special "error_model" instruction. Error
     * models have a name and a signature for the types of parameters it
     * expects. You can also add your own metadata through the Annotatable
     * interface.
     */
    resolver::ErrorModelTable error_models;

    /**
     * When set, error model resolution is disabled. That is, error_models
     * is unused, no type promotion is (or can be) performed for instruction
     * parameters, and the model field of the semantic::ErrorModel node is left
     * uninitialized.
     */
    bool resolve_error_model;

public:

    /**
     * Creates a new semantic analyzer.
     */
    Analyzer();

    /**
     * Registers an initial mapping from the given name to the given value.
     */
    void register_mapping(const std::string &name, const values::Value &value);

    /**
     * Registers a function, usable within expressions.
     */
    void register_function(
        const std::string &name,
        const types::Types &param_types,
        const resolver::FunctionImpl &impl
    );

    /**
     * Convenience method for registering a function. The param_types are
     * specified as a string, converted to types::Types for the other overload
     * using types::from_spec.
     */
    void register_function(
        const std::string &name,
        const std::string &param_types,
        const resolver::FunctionImpl &impl
    );

    /**
     * Registers a number of default functions and mappings, such as the
     * operator functions, the usual trigonometric functions, mappings for pi,
     * eu (aka e, 2.718...), im (imaginary unit) and so on.
     */
    void register_default_functions_and_mappings();

    /**
     * Registers an instruction type. If you never call this, instructions are
     * not resolved (i.e. anything goes name- and operand type-wise). Once you
     * do, only instructions with signatures as added are legal, so anything
     * that doesn't match returns an error.
     */
    void register_instruction(const instruction::Instruction &instruction);

    /**
     * Convenience method for registering an instruction type. The arguments
     * are passed straight to instruction::Instruction's constructor.
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
     * Convenience method for registering an instruction type with a single
     * user-specified annotation. The arguments are passed straight to
     * instruction::Instruction's constructor and set_annotation.
     */
    template <typename T>
    void register_instruction_with_annotation(
        T &&annotation,
        const std::string &name,
        const std::string &param_types = "",
        bool allow_conditional = true,
        bool allow_parallel = true,
        bool allow_reused_qubits = false,
        bool allow_different_index_sizes = false
    ) {
        instruction::Instruction insn {
            name,
            param_types,
            allow_conditional,
            allow_parallel,
            allow_reused_qubits,
            allow_different_index_sizes
        };
        insn.set_annotation<T>(std::forward<T>(annotation));
        register_instruction(insn);
    }

    /**
     * Registers an error model. If you never call this, error models are not
     * resolved (i.e. anything goes name- and operand type-wise). Once you
     * do, only error models with signatures as added are legal, so anything
     * that doesn't match returns an error.
     */
    void register_error_model(const error_model::ErrorModel &error_model);

    /**
     * Convenience method for registering an error model. The arguments
     * are passed straight to error_model::ErrorModel's constructor.
     */
    void register_error_model(
        const std::string &name,
        const std::string &param_types = ""
    );

    /**
     * Convenience method for registering an error model with a single
     * user-specified annotation. The arguments are passed straight to
     * instruction::Instruction's constructor and set_annotation.
     */
    template <typename T>
    void register_error_model_with_annotation(
        T &&annotation,
        const std::string &name,
        const std::string &param_types = ""
    ) {
        error_model::ErrorModel model {
            name,
            param_types
        };
        model.set_annotation<T>(std::forward<T>(annotation));
        register_error_model(model);
    }

    /**
     * Analyzes the given program AST node.
     */
    AnalysisResult analyze(const ast::Program &program) const;

    /**
     * Analyzes the given parse result. If there are parse errors, they are copied
     * into the AnalysisResult error list, and the root node will be empty.
     */
    AnalysisResult analyze(const parser::ParseResult &parse_result) const;

    /**
     * Parses and analyzes the given file.
     */
    AnalysisResult analyze(const std::string &filename) const;

    /**
     * Parses and analyzes the given file pointer. The optional filename
     * argument will be used only for error messages.
     */
    AnalysisResult analyze(FILE *file, const std::string &filename = "<unknown>") const;

    /**
     * Parses and analyzes the given string. The optional filename argument
     * will be used only for error messages.
     */
    AnalysisResult analyze_string(const std::string &data, const std::string &filename = "<unknown>") const;

};

} // namespace analyzer
} // namespace cqasm
