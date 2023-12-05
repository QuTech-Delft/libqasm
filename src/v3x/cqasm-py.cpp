/** \file
 * Implementation for the internal Python-wrapped functions and classes.
 */

#include "cqasm-utils.hpp"
#include "cqasm-version.hpp"
#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm-py.hpp"
#include "v3x/cqasm.hpp"

#include <memory>

namespace v3x = cqasm::v3x;

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
V3xAnalyzer::V3xAnalyzer(const std::string &max_version, bool without_defaults) {
    if (without_defaults) {
        analyzer = std::make_unique<v3x::analyzer::Analyzer>(max_version);
        analyzer->register_default_mappings();
    } else {
        analyzer = std::make_unique<v3x::analyzer::Analyzer>(v3x::default_analyzer(max_version));
    }
}

/**
 * std::unique_ptr<T> requires T to be a complete class for the ~T operation.
 * Since we are using a forward declaration for Analyzer, we need to declare ~T in the header file,
 * and implement it in the source file.
 */
V3xAnalyzer::~V3xAnalyzer() = default;

/**
 * Registers an instruction type.
 * The arguments are passed straight to instruction::Instruction's constructor.
 */
void V3xAnalyzer::register_instruction(const std::string &name, const std::string &param_types) {
    analyzer->register_instruction(name, param_types);
}

/**
 * Only parses the given file.
 * The file must be in v3.x syntax.
 * No version check or conversion is performed.
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> V3xAnalyzer::parse_file(const std::string &filename) {
    if (auto parse_result = v3x::parser::parse_file(filename); parse_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(parse_result.root) };
    } else {
        parse_result.errors.insert(parse_result.errors.begin(), "");
        return parse_result.errors;
    }
}

/**
 * Counterpart of parse_file that returns a string with a JSON representation of the ParseResult.
 */
std::string V3xAnalyzer::parse_file_to_json(const std::string &filename) {
    return cqasm::utils::to_json(v3x::parser::parse_file(filename));
}

/**
 * Same as parse_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
std::vector<std::string> V3xAnalyzer::parse_string(const std::string &data, const std::string &filename) {
    if (auto parse_result = v3x::parser::parse_string(data, filename); parse_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(parse_result.root) };
    } else {
        parse_result.errors.insert(parse_result.errors.begin(), "");
        return parse_result.errors;
    }
}

/**
 * Counterpart of parse_string that returns a string with a JSON representation of the ParseResult.
 */
std::string V3xAnalyzer::parse_string_to_json(const std::string &data, const std::string &filename) {
    return cqasm::utils::to_json(v3x::parser::parse_string(data, filename));
}

/**
 * Parses and analyzes the given file.
 * If the file is written in a later file version,
 * this function may try to reduce it to the maximum v3.x API version support advertised
 * using this object's constructor.
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x semantic tree.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> V3xAnalyzer::analyze_file(const std::string &filename) const {
    auto analysis_result = analyzer->analyze(
        [=](){ return cqasm::version::parse_file(filename); },
        [=](){ return v3x::parser::parse_file(filename); }
    );
    if (analysis_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(analysis_result.root) };
    } else {
        analysis_result.errors.insert(analysis_result.errors.begin(), "");
        return analysis_result.errors;
    }
}

/**
 * Counterpart of analyze_file that returns a string with a JSON representation of the AnalysisResult.
 */
[[nodiscard]] std::string V3xAnalyzer::analyze_file_to_json(const std::string &filename) const {
    return cqasm::utils::to_json(analyzer->analyze(
        [=](){ return cqasm::version::parse_file(filename); },
        [=](){ return v3x::parser::parse_file(filename); }
    ));
}

/**
 * Same as analyze_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
std::vector<std::string> V3xAnalyzer::analyze_string(const std::string &data, const std::string &filename) const {
    auto analysis_result = analyzer->analyze(
        [=](){ return cqasm::version::parse_string(data, filename); },
        [=](){ return v3x::parser::parse_string(data, filename); }
    );
    if (analysis_result.errors.empty()) {
        return std::vector<std::string>{ tree::base::serialize(analysis_result.root) };
    } else {
        analysis_result.errors.insert(analysis_result.errors.begin(), "");
        return analysis_result.errors;
    }
}

/**
 * Counterpart of analyze_string that returns a string with a JSON representation of the AnalysisResult.
 */
[[nodiscard]] std::string V3xAnalyzer::analyze_string_to_json(const std::string &data, const std::string &filename) const {
    return cqasm::utils::to_json(analyzer->analyze(
        [=](){ return cqasm::version::parse_string(data, filename); },
        [=](){ return v3x::parser::parse_string(data, filename); }
    ));
}
