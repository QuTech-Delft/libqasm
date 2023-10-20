/** \file
 * Implementation for \ref include/v3x/cqasm-analyzer.hpp "v3x/cqasm-analyzer.hpp".
 */

#define _USE_MATH_DEFINES

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-parse-helper.hpp"

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
: api_version(api_version) {
    if (!api_version.equal("3.0")) {
        throw std::invalid_argument("this analyzer only supports cQASM 3.0");
    }
}

/**
 * Helper class for analyzing a single AST.
 * This contains the stateful information that Analyzer can't have (to allow Analyzer to be reused).
 */
class AnalyzerHelper {
public:
    /**
     * The analyzer associated with this helper.
     */
    const Analyzer &analyzer;

    /**
     * The analysis result being constructed.
     */
    AnalysisResult result;

    /**
     * Analyzes the given AST using the given analyzer.
     */
    AnalyzerHelper(const Analyzer &analyzer, const ast::Program &ast);
};

/**
 * Analyzes the given AST.
 */
AnalysisResult Analyzer::analyze(const ast::Program &ast) const {
    auto result = AnalyzerHelper(*this, ast).result;
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw std::runtime_error("internal error: no semantic errors returned, but semantic tree is incomplete."
            " Tree was dumped.");
    }
    return result;
}

/**
 * Analyzes the given parse result.
 * If there are parse errors, they are copied into the AnalysisResult error list,
 * and the root node will be empty.
 */
AnalysisResult Analyzer::analyze(const parser::ParseResult &parse_result) const {
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
    const std::function<parser::ParseResult()> &parser
) const {
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
AnalysisResult Analyzer::analyze(const std::string &filename) const {
    return analyze(
        [=](){ return version::parse_file(filename); },
        [=](){ return parser::parse_file(filename); }
    );
}

/**
 * Parses and analyzes the given string.
 * The optional filename argument will be used only for error messages.
 */
AnalysisResult Analyzer::analyze_string(const std::string &data, const std::string &filename) const {
    return analyze(
        [=](){ return version::parse_string(data, filename); },
        [=](){ return parser::parse_string(data, filename); }
    );
}

/**
 * Analyzes the given AST using the given analyzer.
 */
AnalyzerHelper::AnalyzerHelper(const Analyzer &analyzer, const ast::Program &ast)
: analyzer(analyzer)
, result() {
    (void) ast;
    throw std::runtime_error{ "Unimplemented" };
}

} // namespace cqasm::v3x::analyzer
