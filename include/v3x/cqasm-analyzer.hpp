/** \file
 * This file contains the \ref cqasm::v3x::analyzer::Analyzer "Analyzer" class and support classes,
 * used to manage semantic analysis.
 *
 * While the Analyzer class itself only manages the semantic analysis phase,
 * it also has some convenience methods that drive lexical analysis and parsing in addition.
 */

#pragma once

#include <functional>
#include <list>
#include <optional>
#include <string>

#include "cqasm-analysis-result.hpp"
#include "cqasm-analyzer.hpp"
#include "cqasm-ast.hpp"
#include "cqasm-core-function.hpp"
#include "cqasm-parse-helper.hpp"
#include "cqasm-resolver.hpp"
#include "cqasm-scope.hpp"
#include "cqasm-semantic.hpp"

/**
 * Namespace for the \ref cqasm::analyzer::Analyzer "Analyzer" class and support classes.
 */
namespace cqasm::v3x::analyzer {

/**
 * Main class used for analyzing cQASM files.
 *
 * Construction of this class is the entry point for libqasm
 * whenever you need to modify the default instruction set,
 * or want to add additional initial mappings, operators, or functions.
 * The process is simple:
 *
 *  - Construct an Analyzer object with the default constructor.
 *  - Use zero or more of the various `register_*()` methods to configure the Analyzer.
 *  - Use one or more of the `analyze*()` methods to analyze cQASM files
 *    or string representations thereof.
 *
 * Note that the only state maintained by the Analyzer object is its configuration,
 * and the `analyze*()` functions never change this state (hence they are const).
 */
class Analyzer {
    friend class AnalyzeTreeGenAstVisitor;

public:
    /**
     * The maximum cQASM version that this analyzer supports.
     */
    primitives::Version api_version;

protected:
    std::list<Scope> scope_stack_;

    [[nodiscard]] Scope &global_scope();
    [[nodiscard]] Scope &current_scope();
    [[nodiscard]] tree::One<semantic::Block> current_block();
    [[nodiscard]] tree::Any<semantic::Variable> &current_variables();

    [[nodiscard]] const Scope &global_scope() const;
    [[nodiscard]] const Scope &current_scope() const;
    [[nodiscard]] const tree::Any<semantic::Variable> &current_variables() const;

public:
    /**
     * Creates a new semantic analyzer.
     */
    explicit Analyzer(const primitives::Version &api_version = "3.0");

    /**
     * Destroys a semantic analyzer.
     */
    virtual ~Analyzer() = default;

    /**
     * Registers constants for pi, eu (aka e, 2.718...), tau and im (imaginary unit).
     */
    virtual void register_default_constants();

    /**
     * Registers a number of default functions, such as the operator functions, and the usual trigonometric functions.
     */
    virtual void register_default_functions();

    /**
     * Registers the cQASM 3.0 instruction set.
     */
    virtual void register_default_instructions();

    /**
     * Analyzes the given program AST node.
     */
    [[nodiscard]] virtual AnalysisResult analyze(ast::Program &program);

    /**
     * Analyzes the given parse result.
     * If there are parse errors, they are moved into the AnalysisResult error list,
     * and the root node will be empty.
     */
    [[nodiscard]] virtual AnalysisResult analyze(const parser::ParseResult &parse_result);

    /**
     * Parses and analyzes the given file.
     */
    [[nodiscard]] virtual AnalysisResult analyze_file(const std::string &file_name);

    /**
     * Parses and analyzes the given string.
     * The optional file_name argument will be used only for error messages.
     */
    [[nodiscard]] virtual AnalysisResult analyze_string(
        const std::string &data, const std::optional<std::string> &file_name);

    /**
     * Pushes a new empty scope to the top of the scope stack.
     */
    void push_scope();

    /**
     * Pops a scope from the top of the scope stack.
     */
    void pop_scope();

    /**
     * Adds a statement to the current scope.
     */
    virtual void add_statement_to_current_scope(const tree::One<semantic::Statement> &statement);

    /**
     * Adds a variable to the current scope.
     */
    virtual void add_variable_to_current_scope(const tree::One<semantic::Variable> &variable);

    /**
     * Resolves a variable.
     * Throws NameResolutionFailure if no variable by the given name exists.
     */
    [[nodiscard]] virtual values::Value resolve_variable(const std::string &name) const;

    /**
     * Registers a variable.
     */
    virtual void register_variable(const std::string &name, const values::Value &value);

    /**
     * Resolves a function.
     * Tries to call a function implementation first.
     * If it doesn't succeed, tries to call a function.
     * Throws NameResolutionFailure if no function by the given name exists,
     * OverloadResolutionFailure if no overload of the function exists for the given arguments,
     * or otherwise returns the value returned by the function.
     */
    [[nodiscard]] virtual values::Value resolve_function(const std::string &name, const values::Values &args) const;

    /**
     * Registers a consteval core function.
     */
    virtual void register_consteval_core_function(
        const std::string &name, const types::Types &param_types, const resolver::ConstEvalCoreFunction &function);

    /**
     * Convenience method for registering a consteval core function.
     * The param_types are specified as a string,
     * converted to types::Types for the other overload using types::from_spec.
     */
    virtual void register_consteval_core_function(
        const std::string &name, const std::string &param_types, const resolver::ConstEvalCoreFunction &function);

    /**
     * Resolves an instruction to a values::InstructionCall node.
     * Throws NameResolutionFailure if no instruction by the given name exists,
     * OverloadResolutionFailure if no overload exists for the given arguments,
     * or otherwise returns the resolved instruction node.
     * Annotation data, line number information, and the condition still need to be set by the caller.
     */
    [[nodiscard]] virtual values::Value resolve_instruction(const std::string &name, const values::Values &args) const;

    /**
     * Registers an instruction type.
     */
    virtual void register_instruction(const instruction::Instruction &instruction);

    /**
     * Convenience method for registering an instruction type.
     * The arguments are passed straight to instruction::Instruction's constructor.
     */
    virtual void register_instruction(const std::string &name, const std::optional<std::string> &param_types,
        const char return_type);
};

}  // namespace cqasm::v3x::analyzer
