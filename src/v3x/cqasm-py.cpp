/** \file
 * Implementation for the internal Python-wrapped functions and classes.
 */

#include "v3x/cqasm-py.hpp"

#include <memory>
#include <optional>

#include "cqasm-version.hpp"
#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/cqasm.hpp"

namespace v3x = cqasm::v3x;

/**
 * Creates a new v3.x semantic analyzer.
 * When without_defaults is specified, the default instruction set are not loaded into the instruction table,
 * so you have to specify the entire instruction set using register_instruction().
 * Otherwise, those functions only add to the defaults.
 * Unlike the C++ version of the analyzer class,
 * the initial mappings and functions are not configurable at all.
 * The defaults for these are always used.
 */
V3xAnalyzer::V3xAnalyzer(const std::string &max_version, bool without_defaults) {
    if (without_defaults) {
        analyzer = std::make_unique<v3x::analyzer::Analyzer>(max_version);
        analyzer->register_default_constants();
        analyzer->register_default_functions();
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
void V3xAnalyzer::register_instruction(const std::string &name, const std::optional<std::string> &param_types,
    const char return_type) {
    analyzer->register_instruction(name, param_types, return_type);
}

/**
 * Only parses the given file.
 * The file must be in v3.x syntax.
 * No version check or conversion is performed.
 * Returns a vector of strings, of which the first is reserved for the CBOR serialization of the v3.x AST.
 * Any additional strings represent error messages.
 * Notice that the AST and error messages won't be available at the same time.
 */
std::vector<std::string> V3xAnalyzer::parse_file(const std::string &file_name) {
    const auto &parse_result = v3x::parser::parse_file(file_name, std::nullopt);
    return parse_result.to_strings();
}

/**
 * Parses a file containing a v3.x program.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x syntactic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
std::string V3xAnalyzer::parse_file_to_json(const std::string &file_name) {
    const auto &parse_result = v3x::parser::parse_file(file_name, std::nullopt);
    return parse_result.to_json();
}

/**
 * Same as parse_file(), but instead receives the file contents directly.
 * The file_name, if non-empty, is only used when reporting errors.
 */
std::vector<std::string> V3xAnalyzer::parse_string(const std::string &data, const std::string &file_name) {
    auto file_name_op = !file_name.empty() ? std::optional<std::string>{ file_name } : std::nullopt;
    const auto &parse_result = v3x::parser::parse_string(data, file_name_op);
    return parse_result.to_strings();
}

/**
 * Parses a data string containing a v3.x program.
 * The file_name is only used when reporting errors.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x syntactic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
std::string V3xAnalyzer::parse_string_to_json(const std::string &data, const std::string &file_name) {
    auto file_name_op = !file_name.empty() ? std::optional<std::string>{ file_name } : std::nullopt;
    const auto &parse_result = v3x::parser::parse_string(data, file_name_op);
    return parse_result.to_json();
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
std::vector<std::string> V3xAnalyzer::analyze_file(const std::string &file_name) const {
    const auto &parse_result = v3x::parser::parse_file(file_name, std::nullopt);
    const auto &analysis_result = analyzer->analyze(parse_result);
    return analysis_result.to_strings();
}

/**
 * Parses and analyzes a file containing a v3.x program.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x semantic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
[[nodiscard]] std::string V3xAnalyzer::analyze_file_to_json(const std::string &file_name) const {
    const auto &parse_result = v3x::parser::parse_file(file_name, std::nullopt);
    const auto &analysis_result = analyzer->analyze(parse_result);
    return analysis_result.to_json();
}

/**
 * Same as analyze_file(), but instead receives the file contents directly.
 * The file_name, if specified, is only used when reporting errors.
 */
std::vector<std::string> V3xAnalyzer::analyze_string(const std::string &data, const std::string &file_name) const {
    auto file_name_op = !file_name.empty() ? std::optional<std::string>{ file_name } : std::nullopt;
    const auto &parse_result = v3x::parser::parse_string(data, file_name_op);
    const auto &analysis_result = analyzer->analyze(parse_result);
    return analysis_result.to_strings();
}

/**
 * Parses and analyzes a data string containing a v3.x program.
 * The file_name is only used when reporting errors.
 * No version check or conversion is performed.
 * Returns a string.
 * If the parsing was successful, that string contains a v3.x semantic AST.
 * Otherwise, it will contain a list of errors.
 * The JSON representation of each error follows the Language Server Protocol (LSP) specification.
 * Every error is mapped to an LSP Diagnostic structure:
 * severity is hardcoded to 1 at the moment (value corresponding to an Error level).
 */
[[nodiscard]] std::string V3xAnalyzer::analyze_string_to_json(
    const std::string &data, const std::string &file_name) const {
    auto file_name_op = !file_name.empty() ? std::optional<std::string>{ file_name } : std::nullopt;
    const auto &parse_result = v3x::parser::parse_string(data, file_name_op);
    const auto &analysis_result = analyzer->analyze(parse_result);
    return analysis_result.to_json();
}
