/** \file
 * Implementation for \ref include/v3x/cqasm-parse-helper.hpp "v3x/cqasm-parse-helper.hpp".
 */

#include "v3x/cqasm-parse-result.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/ScannerAntlr.hpp"


namespace cqasm::v3x::parser {

/**
 * Parse using the given file path.
 * Throws an AnalysisError if the file does not exist.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v3x::parser::ParseResult parse_file(const std::string &file_path, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>(file_name);
    auto error_listener_up = std::make_unique<CustomErrorListener>(file_name);
    auto scanner_up = std::make_unique<ScannerAntlrFile>(
            std::move(builder_visitor_up), std::move(error_listener_up), file_path);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v3x::parser::ParseResult parse_string(const std::string &data, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>(file_name);
    auto error_listener_up = std::make_unique<CustomErrorListener>(file_name);
    auto scanner_up = std::make_unique<ScannerAntlrString>(
            std::move(builder_visitor_up), std::move(error_listener_up), data);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}


ParseHelper::ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, std::string file_name)
: scanner_up_(std::move(scanner_up)), file_name_(std::move(file_name)) {}

/**
 * Does the actual parsing.
 */
cqasm::v3x::parser::ParseResult ParseHelper::parse() {
    cqasm::v3x::parser::ParseResult result;
    try {
        result = scanner_up_->parse();
    } catch (const std::runtime_error &err) {
        result.errors.emplace_back(err.what());
    }
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw cqasm::error::AnalysisError(
            "ParseHelper::parse: no parse errors returned, but AST is incomplete. AST was dumped.");
    }
    return result;
}

} // namespace cqasm::v3x::parser
