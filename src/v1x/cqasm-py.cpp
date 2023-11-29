/** \file
 * Implementation for the internal Python-wrapped functions and classes.
 */

#include "cqasm-version.hpp"
#include "v1x/cqasm-analyzer.hpp"
#include "v1x/cqasm-parse-helper.hpp"
#include "v1x/cqasm-py.hpp"
#include "v1x/cqasm.hpp"

#include <memory>

namespace v1x = cqasm::v1x;

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
V1xAnalyzer::V1xAnalyzer(const std::string &max_version, bool without_defaults) {
    if (without_defaults) {
        analyzer = std::make_unique<v1x::analyzer::Analyzer>(max_version);
        analyzer->register_default_functions_and_mappings();
    } else {
        analyzer = std::make_unique<v1x::analyzer::Analyzer>(v1x::default_analyzer(max_version));
    }
}

/**
 * std::unique_ptr<T> requires T to be a complete class for the ~T operation.
 * Since we are using a forward declaration for Analyzer, we need to declare ~T in the header file,
 * and implement it in the source file.
 */
V1xAnalyzer::~V1xAnalyzer() = default;

/**
 * Registers an instruction type.
 * The arguments are passed straight to instruction::Instruction's constructor.
 */
void V1xAnalyzer::register_instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits,
    bool allow_different_index_sizes
) {
    analyzer->register_instruction(
        name,
        param_types,
        allow_conditional,
        allow_parallel,
        allow_reused_qubits,
        allow_different_index_sizes
    );
}

/**
 * Registers an error model.
 * The arguments are passed straight to error_model::ErrorModel's constructor.
 */
void V1xAnalyzer::register_error_model(
    const std::string &name,
    const std::string &param_types
) {
    analyzer->register_error_model(name, param_types);
}

/**
 * Only parses the given file.
 * The file must be in v1.x syntax.
 * No version check or conversion is performed.
 * Returns a vector of strings,
 * of which the first is always present and is the CBOR serialization of the v1.x AST.
 * Any additional strings represent error messages.
 */
std::vector<std::string> V1xAnalyzer::parse_file(
    const std::string &filename
) {
    if (auto parse_result = v1x::parser::parse_file(filename); parse_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(parse_result.root) };
    } else {
        parse_result.errors.insert(parse_result.errors.begin(), "");
        return parse_result.errors;
    }
}

/**
 * Same as parse_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
std::vector<std::string> V1xAnalyzer::parse_string(
    const std::string &data,
    const std::string &filename
) {
    if (auto parse_result = v1x::parser::parse_string(data, filename); parse_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(parse_result.root) };
    } else {
        parse_result.errors.insert(parse_result.errors.begin(), "");
        return parse_result.errors;
    }
}

/**
 * Parses and analyzes the given file.
 * If the file is written in a later file version,
 * this function may try to reduce it to the maximum v1.x API version support advertised
 * using this object's constructor.
 * Returns a vector of strings,
 * of which the first is always present and is the CBOR serialization of the v1x semantic tree.
 * Any additional strings represent error messages.
 */
[[nodiscard]] std::vector<std::string> V1xAnalyzer::analyze_file(
    const std::string &filename
) const {
    auto analysis_result = analyzer->analyze(
        [=](){ return cqasm::version::parse_file(filename); },
        [=](){ return v1x::parser::parse_file(filename); }
    );
    if (analysis_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(analysis_result.root) };
    } else {
        analysis_result.errors.insert(analysis_result.errors.begin(), "");
        return analysis_result.errors;
    }
}

/**
 * Same as analyze_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
[[nodiscard]] std::vector<std::string> V1xAnalyzer::analyze_string(
    const std::string &data,
    const std::string &filename
) const {
    auto analysis_result = analyzer->analyze(
        [=](){ return cqasm::version::parse_string(data, filename); },
        [=](){ return v1x::parser::parse_string(data, filename); }
    );
    if (analysis_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(analysis_result.root) };
    } else {
        analysis_result.errors.insert(analysis_result.errors.begin(), "");
        return analysis_result.errors;
    }
}
