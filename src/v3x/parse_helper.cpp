/** \file
 * Implementation for \ref include/libqasm/v3x/parse_helper.hpp "v3x/parse_helper.hpp".
 */

#include "libqasm/v3x/parse_helper.hpp"

#include "libqasm/annotations_constants.hpp"
#include "libqasm/v3x/antlr_custom_error_listener.hpp"
#include "libqasm/v3x/antlr_scanner.hpp"
#include "libqasm/v3x/parse_result.hpp"
#include "libqasm/v3x/syntactic_analyzer.hpp"

namespace cqasm::v3x::parser {

/**
 * Parse using the given file path.
 * Throws a ParseError if the file does not exist.
 * A file_name may be given in addition for use within error messages.
 */
ParseResult parse_file(const std::string& file_path, const std::optional<std::string>& file_name) {
    auto builder_visitor_up = std::make_unique<SyntacticAnalyzer>(file_name);
    auto error_listener_up = std::make_unique<AntlrCustomErrorListener>(file_name);
    auto scanner_up =
        std::make_unique<FileAntlrScanner>(std::move(builder_visitor_up), std::move(error_listener_up), file_path);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
ParseResult parse_string(const std::string& data, const std::optional<std::string>& file_name) {
    auto builder_visitor_up = std::make_unique<SyntacticAnalyzer>(file_name);
    auto error_listener_up = std::make_unique<AntlrCustomErrorListener>(file_name);
    auto scanner_up =
        std::make_unique<StringAntlrScanner>(std::move(builder_visitor_up), std::move(error_listener_up), data);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

ParseHelper::ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, const std::optional<std::string>& file_name)
: scanner_up_{ std::move(scanner_up) }
, file_name_{ file_name.value_or(annotations::unknown_file_name) } {
    if (file_name_.empty()) {
        file_name_ = annotations::unknown_file_name;
    }
}

/**
 * Does the actual parsing.
 */
ParseResult ParseHelper::parse() {
    ParseResult result;
    try {
        result = scanner_up_->parse();
    } catch (error::AnalysisError& err) {
        result.errors.push_back(std::move(err));
    } catch (const std::runtime_error& err) {
        result.errors.emplace_back(err.what());
    }

    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw error::ParseError("ParseHelper::parse: no parse errors returned, but AST is incomplete. AST was dumped.");
    }
    return result;
}

}  // namespace cqasm::v3x::parser
