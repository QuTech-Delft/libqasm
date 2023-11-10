/** \file
 * Implementation for \ref include/v3x/cqasm-analyzer.hpp "v3x/cqasm-analyzer.hpp".
 */

#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-parse-helper.hpp"

#include <memory>  // make_unique
#include <numbers>
#include <stdexcept>  // runtime_error


namespace cqasm::v3x::analyzer {

/**
 * "Unwraps" the result (as you would in Rust) to get the program node or an exception.
 * The exception is always an AnalysisFailed, deriving from std::runtime_error.
 * The actual error messages are in this case first written to the given output stream, defaulting to stderr.
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
: api_version{ api_version }
, instruction_set{} {
    if (api_version != "3.0") {
        throw std::invalid_argument{ "this analyzer only supports cQASM 3.0" };
    }
}

/**
 * Registers mappings for pi, eu (aka e, 2.718...), tau and im (imaginary unit).
 */
void Analyzer::register_default_mappings() {
    static constexpr double tau = 2 * std::numbers::pi;
    register_mapping("pi", tree::make<values::ConstReal>(std::numbers::pi));
    register_mapping("eu", tree::make<values::ConstReal>(std::numbers::e));
    register_mapping("tau", tree::make<values::ConstReal>(tau));
    register_mapping("im", tree::make<values::ConstComplex>(primitives::Complex(0.0, 1.0)));
}

/**
 * Registers an initial mapping from the given name to the given value.
 */
void Analyzer::register_mapping(const std::string &name, const values::Value &value) {
    mappings.add(name, value);
}

/**
 * Registers an instruction type.
 */
void Analyzer::register_instruction(const instruction::Instruction &instruction) {
    instruction_set.add(instruction);
}

/**
 * Convenience method for registering an instruction type. The arguments
 * are passed straight to instruction::Instruction's constructor.
 */
void Analyzer::register_instruction(const std::string &name, const std::string &param_types,
    bool request_qubit_and_bit_indices_have_same_size) {

    register_instruction(instruction::Instruction(name, param_types, request_qubit_and_bit_indices_have_same_size));
}

/**
 * Analyzes the given AST.
 */
AnalysisResult Analyzer::analyze(const ast::Program &ast) {
    auto analyze_visitor_up = std::make_unique<AnalyzeTreeGenAstVisitor>(*this);
    auto result =  analyze_visitor_up->visitProgram(ast);
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw std::runtime_error{ "internal error: no semantic errors returned, but semantic tree is incomplete."
            " Tree was dumped." };
    }
    return result;
}

/**
 * Analyzes the given parse result.
 * If there are parse errors, they are copied into the AnalysisResult error list,
 * and the root node will be empty.
 */
AnalysisResult Analyzer::analyze(const parser::ParseResult &parse_result) {
    if (!parse_result.errors.empty()) {
        AnalysisResult result;
        result.errors = parse_result.errors;
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
        auto version = version_parser();
        if (version > api_version) {
            std::ostringstream ss;
            ss << "cQASM file version is " << version << ", but at most ";
            ss << api_version << " is supported here";
            result.errors.push_back(ss.str());
            return result;
        }
    } catch (error::AnalysisError &e) {
        result.errors.push_back(e.get_message());
        return result;
    }
    return analyze(parser());
}

/**
 * Parses and analyzes the given file.
 */
AnalysisResult Analyzer::analyze(const std::string &filename) {
    return analyze(
        [=](){ return version::parse_file(filename); },
        [=](){ return parser::parse_file(filename); }
    );
}

/**
 * Parses and analyzes the given string.
 * The optional filename argument will be used only for error messages.
 */
AnalysisResult Analyzer::analyze_string(const std::string &data, const std::string &filename) {
    return analyze(
        [=](){ return version::parse_string(data, filename); },
        [=](){ return parser::parse_string(data, filename); }
    );
}

/**
 * Resolves a mapping.
 * Throws NameResolutionFailure if no mapping by the given name exists.
 */
values::Value Analyzer::resolve_mapping(const std::string &name) const {
    return mappings.resolve(name);
}

/**
 * Adds a mapping.
 */
void Analyzer::add_mapping(const std::string &name, const values::Value &value) {
    mappings.add(name, value);
}

/**
 * Resolves an instruction.
 * Throws NameResolutionFailure if no instruction by the given name exists,
 * OverloadResolutionFailure if no overload exists for the given arguments, or otherwise
 * returns the resolved instruction node.
 * Annotation data, line number information, and the condition still need to be set by the caller.
 */
[[nodiscard]] tree::One<semantic::Instruction> Analyzer::resolve_instruction(
    const std::string &name, const values::Values &args) const {

    return instruction_set.resolve(name,  args);
}

} // namespace cqasm::v3x::analyzer