/** \file
 * Implementation for \ref include/v1x/cqasm-analyzer.hpp "v1x/cqasm-analyzer.hpp".
 */

#define _USE_MATH_DEFINES

#include "cqasm-tree.hpp"  // signed_size_t
#include "cqasm-utils.hpp"
#include "v1x/cqasm-analyzer.hpp"
#include "v1x/cqasm-analyzer-helper.hpp"
#include "v1x/cqasm-functions.hpp"
#include "v1x/cqasm-parse-helper.hpp"
#include "v1x/cqasm-scope.hpp"

#include <cmath>
#include <fmt/format.h>
#include <utility>


namespace cqasm::v1x::analyzer {

/**
 * "Unwraps" the result (as you would in Rust) to get the program node or
 * an exception. The exception is always an AnalysisFailed, deriving from
 * std::runtime_error. The actual error messages are in this case first
 * written to the given output stream, defaulting to stderr.
 */
ast::One<semantic::Program> AnalysisResult::unwrap(std::ostream &out) const {
    if (errors.empty()) {
        return root;
    }
    for (const auto &error : errors) {
        out << error << std::endl;
    }
    throw AnalysisFailed();
}

/**
 * Creates a new semantic analyzer.
 */
Analyzer::Analyzer(const primitives::Version &api_version)
    : api_version(api_version), resolve_instructions(false), resolve_error_model(false)
{
    if (api_version > "1.2") {
        throw std::invalid_argument("this analyzer only supports up to cQASM 1.2");
    }
}

/**
 * Registers an initial mapping from the given name to the given value.
 */
void Analyzer::register_mapping(const std::string &name, const values::Value &value) {
    mappings.add(name, value);
}

/**
 * Registers a function, usable within expressions.
 */
void Analyzer::register_function(
    const std::string &name,
    const types::Types &param_types,
    const resolver::FunctionImpl &impl
) {
    functions.add(name, param_types, impl);
}

/**
 * Convenience method for registering a function.
 * The param_types are specified as a string,
 * converted to types::Types for the other overload using types::from_spec.
 */
void Analyzer::register_function(
    const std::string &name,
    const std::string &param_types,
    const resolver::FunctionImpl &impl
) {
    functions.add(name, types::from_spec(param_types), impl);
}

/**
 * Registers a number of default functions and mappings, such as the
 * operator functions, the usual trigonometric functions, mappings for pi,
 * eu (aka e, 2.718...), im (imaginary unit) and so on.
 */
void Analyzer::register_default_functions_and_mappings() {
    register_mapping("x", tree::make<values::ConstAxis>(primitives::Axis::X));
    register_mapping("y", tree::make<values::ConstAxis>(primitives::Axis::Y));
    register_mapping("z", tree::make<values::ConstAxis>(primitives::Axis::Z));
    register_mapping("true", tree::make<values::ConstBool>(true));
    register_mapping("false", tree::make<values::ConstBool>(false));
    register_mapping("pi", tree::make<values::ConstReal>(M_PI));
    register_mapping("eu", tree::make<values::ConstReal>(M_E));
    register_mapping("im", tree::make<values::ConstComplex>(primitives::Complex(0.0, 1.0)));
    cqasm::v1x::functions::register_default_functions_into(functions);
}

/**
 * Registers an instruction type. If you never call this, instructions are
 * not resolved (i.e. anything goes name- and operand type-wise). Once you
 * do, only instructions with signatures as added are legal, so anything
 * that doesn't match returns an error.
 */
void Analyzer::register_instruction(const instruction::Instruction &instruction) {
    resolve_instructions = true;
    instruction_set.add(instruction);
}

/**
 * Convenience method for registering an instruction type. The arguments
 * are passed straight to instruction::Instruction's constructor.
 */
void Analyzer::register_instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits,
    bool allow_different_index_sizes
) {
    register_instruction(instruction::Instruction(
        name, param_types, allow_conditional, allow_parallel,
        allow_reused_qubits, allow_different_index_sizes));
}

/**
 * Registers an error model. If you never call this, error models are not
 * resolved (i.e. anything goes name- and operand type-wise). Once you
 * do, only error models with signatures as added are legal, so anything
 * that doesn't match returns an error.
 */
void Analyzer::register_error_model(const error_model::ErrorModel &error_model) {
    resolve_error_model = true;
    error_models.add(error_model);
}

/**
 * Convenience method for registering an error model. The arguments
 * are passed straight to error_model::ErrorModel's constructor.
 */
void Analyzer::register_error_model(
    const std::string &name,
    const std::string &param_types
) {
    register_error_model(error_model::ErrorModel(name, param_types));
}

/**
 * Analyzes the given AST.
 */
AnalysisResult Analyzer::analyze(ast::Program &ast) {
    auto result = AnalyzerHelper(*this, ast).result;
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw std::runtime_error("internal error: no semantic errors returned, but semantic tree is incomplete. Tree was dumped.");
    }
    return result;
}

/**
 * Analyzes the given parse result.
 * If there are parse errors, they are moved into the AnalysisResult error list,
 * and the root node will be empty.
 */
AnalysisResult Analyzer::analyze(parser::ParseResult &&parse_result) {
    if (!parse_result.errors.empty()) {
        AnalysisResult result;
        result.errors = std::move(parse_result.errors);
        return result;
    } else {
        return analyze(*parse_result.root->as_program());
    }
}

/**
 * Parses and analyzes using the given version and parser closures.
 */
AnalysisResult Analyzer::analyze(
    const std::function<version::Version()> &version_parser,
    const std::function<parser::ParseResult()> &parser) {

    AnalysisResult result;
    try {
        if (auto version = version_parser(); version > api_version) {
            result.errors.emplace_back(fmt::format(
                "cQASM file version is {}, but at most {} is supported here", version, api_version));
            return result;
        }
    } catch (error::AnalysisError &err) {
        result.errors.push_back(std::move(err));
        return result;
    }
    return analyze(parser());
}

/**
 * Parses and analyzes the given file.
 */
AnalysisResult Analyzer::analyze_file(const std::string &file_name) {
    return analyze(
        [=](){ return version::parse_file(file_name); },
        [=](){ return parser::parse_file(file_name); }
    );
}

/**
 * Parses and analyzes the given file pointer.
 * The optional file_name argument will be used only for error messages.
 */
AnalysisResult Analyzer::analyze_file(FILE *file, const std::optional<std::string> &file_name) {
    return analyze(
        [=](){ return version::parse_file(file, file_name); },
        [=](){ return parser::parse_file(file, file_name); }
    );
}

/**
 * Parses and analyzes the given string.
 * The optional file_name argument will be used only for error messages.
 */
AnalysisResult Analyzer::analyze_string(const std::string &data, const std::optional<std::string> &file_name) {
    return analyze(
        [=](){ return version::parse_string(data, file_name); },
        [=](){ return parser::parse_string(data, file_name); }
    );
}

} // namespace cqasm::v1x::analyzer
