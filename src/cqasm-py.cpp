/** \file
 * Implementation for the internal Python-wrapped functions and classes.
 */

#include "cqasm-version.hpp"
#include "cqasm-py.hpp"
#include "v1x/cqasm.hpp"
#include "v1x/cqasm-parse-helper.hpp"

namespace v1x = cqasm::v1x;

/**
 * Creates a new 1.x semantic analyzer. When without_defaults is specified,
 * the default instruction set and error models are not loaded into the
 * instruction and error model tables, so you have to specify the entire
 * instruction set using register_instruction() and register_error_model();
 * otherwise those functions only add to the defaults. Unlike the C++
 * version of the analyzer class, the initial mappings and functions are not
 * configurable at all; the defaults for these are always used.
 */
V1xAnalyzer::V1xAnalyzer(const std::string &max_version, bool without_defaults) {
    if (without_defaults) {
        a = std::unique_ptr<v1x::analyzer::Analyzer>(
            new v1x::analyzer::Analyzer(max_version)
        );
        a->register_default_functions_and_mappings();
    } else {
        a = std::unique_ptr<v1x::analyzer::Analyzer>(
            new v1x::analyzer::Analyzer(v1x::default_analyzer(max_version))
        );
    }
}

/**
 * Registers an instruction type. The arguments are passed straight to
 * instruction::Instruction's constructor.
 */
void V1xAnalyzer::register_instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits,
    bool allow_different_index_sizes
) {
    a->register_instruction(
        name,
        param_types,
        allow_conditional,
        allow_parallel,
        allow_reused_qubits,
        allow_different_index_sizes
    );
}

/**
 * Registers an error model. The arguments are passed straight to
 * error_model::ErrorModel's constructor.
 */
void V1xAnalyzer::register_error_model(
    const std::string &name,
    const std::string &param_types
) {
    a->register_error_model(name, param_types);
}

/**
 * Only parses the given file. The file must be in 1.x syntax; no version
 * check or conversion is performed. Returns a vector of strings, of which the
 * first is always present and is the CBOR serialization of the v1x AST. Any
 * additional strings represent error messages.
 */
std::vector<std::string> V1xAnalyzer::parse_file(
    const std::string &filename
) {
    auto result = v1x::parser::parse_file(filename);
    std::vector<std::string> retval{""};
    if (result.errors.empty()) {
        retval[0] = ::tree::base::serialize(result.root);
    }
    retval.insert(retval.end(), result.errors.begin(), result.errors.end());
    return retval;
}

/**
 * Same as parse_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
std::vector<std::string> V1xAnalyzer::parse_string(
    const std::string &data,
    const std::string &filename
) {
    auto result = v1x::parser::parse_string(data, filename);
    std::vector<std::string> retval{""};
    if (result.errors.empty()) {
        retval[0] = ::tree::base::serialize(result.root);
    }
    retval.insert(retval.end(), result.errors.begin(), result.errors.end());
    return retval;
}

/**
 * Parses and analyzes the given file. If the file is written in a later
 * file version, this function may try to reduce it to the maximum 1.x API
 * version support advertised using this object's constructor. Returns a
 * vector of strings, of which the first is always present and is the CBOR
 * serialization of the v1x semantic tree. Any additional strings represent
 * error messages.
 */
std::vector<std::string> V1xAnalyzer::analyze_file(
    const std::string &filename
) const {
    auto result = a->analyze(
        [=](){ return cqasm::version::parse_file(filename); },
        [=](){ return v1x::parser::parse_file(filename); }
    );
    std::vector<std::string> retval{""};
    if (result.errors.empty()) {
        retval[0] = ::tree::base::serialize(result.root);
    }
    retval.insert(retval.end(), result.errors.begin(), result.errors.end());
    return retval;
}

/**
 * Same as analyze_file(), but instead receives the file contents directly.
 * The filename, if specified, is only used when reporting errors.
 */
std::vector<std::string> V1xAnalyzer::analyze_string(
    const std::string &data,
    const std::string &filename
) const {
    auto result = a->analyze(
        [=](){ return cqasm::version::parse_string(data, filename); },
        [=](){ return v1x::parser::parse_string(data, filename); }
    );
    std::vector<std::string> retval{""};
    if (result.errors.empty()) {
        retval[0] = ::tree::base::serialize(result.root);
    }
    retval.insert(retval.end(), result.errors.begin(), result.errors.end());
    return retval;
}
