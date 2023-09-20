/** \file
 * Implementation for \ref include/v3x/cqasm-parse-helper.hpp "v3x/cqasm-parse-helper.hpp".
 */

#include "v1x/cqasm-ast.hpp"
#include "v1x/cqasm-parse-result.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"
#include "v3x/cqasm-parse-helper.hpp"
#include "v3x/CqasmLexer.h"
#include "v3x/CqasmParser.h"

#include <fmt/format.h>
#include <filesystem>

namespace fs = std::filesystem;


namespace cqasm::v3x::parser {

using namespace cqasm::v1x::ast;

ScannerAdaptor::~ScannerAdaptor() {}

ScannerAntlr::ScannerAntlr(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up)
: build_visitor_up_{ std::move(build_visitor_up) }
, error_listener_up_{ std::move(error_listener_up) } {}

ScannerAntlr::~ScannerAntlr() {}

cqasm::v1x::parser::ParseResult ScannerAntlr::parse_(antlr4::ANTLRInputStream &is) {
    CqasmLexer lexer{ &is };
    lexer.removeErrorListeners();
    lexer.addErrorListener(error_listener_up_.get());
    antlr4::CommonTokenStream tokens{ &lexer };
    CqasmParser parser{ &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(error_listener_up_.get());
    auto ast = parser.program();
    auto custom_ast = build_visitor_up_->visitProgram(ast);
    return cqasm::v1x::parser::ParseResult{ std::any_cast<One<Program>>(custom_ast), {} };
}

ScannerAntlrFile::ScannerAntlrFile(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up, const std::string &file_path)
: ScannerAntlr{ std::move(build_visitor_up), std::move(error_listener_up) }, file_path_{ file_path } {
    if (!fs::exists(file_path_) || !fs::is_regular_file(file_path_)) {
        throw error::AnalysisError{ fmt::format("ScannerAntlrFile couldn't access file '{}'.", file_path_) };
    }
}
ScannerAntlrFile::~ScannerAntlrFile() {}

cqasm::v1x::parser::ParseResult ScannerAntlrFile::parse() {
    antlr4::ANTLRFileStream ifs{};
    ifs.loadFromFile(file_path_);
    return parse_(ifs);
}

ScannerAntlrString::ScannerAntlrString(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up, const std::string &data)
: ScannerAntlr{ std::move(build_visitor_up), std::move(error_listener_up) }, data_{ data } {}

ScannerAntlrString::~ScannerAntlrString() {}

cqasm::v1x::parser::ParseResult ScannerAntlrString::parse() {
    antlr4::ANTLRInputStream is{ data_ };
    return parse_(is);
}

/**
 * Parse using the given file path.
 * Throws an AnalysisError if the file does not exist.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_file(const std::string &file_path, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>();
    auto error_listener_up = std::make_unique<CustomErrorListener>(file_name);
    auto scanner_up = std::make_unique<ScannerAntlrFile>(
            std::move(builder_visitor_up), std::move(error_listener_up), file_path);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_string(const std::string &data, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>();
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
cqasm::v1x::parser::ParseResult ParseHelper::parse() {
    cqasm::v1x::parser::ParseResult result;
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
