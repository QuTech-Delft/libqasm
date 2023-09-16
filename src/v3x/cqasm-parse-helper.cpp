/** \file
 * Implementation for \ref include/v3x/cqasm-parse-helper.hpp "v3x/cqasm-parse-helper.hpp".
 */

#include "v1x/cqasm-ast.hpp"
#include "v1x/cqasm-parse-result.hpp"
#include "v3x/BuildTreeGenAstVisitor.h"
#include "v3x/CqasmLexer.h"
#include "v3x/CqasmParser.h"
#include "v3x/cqasm-parse-helper.hpp"

#include "antlr4-runtime/antlr4-runtime.h"

#include <filesystem>
#include <fmt/format.h>
#include <fstream>  // ifstream
#include <stdexcept>  // runtime_error

namespace fs = std::filesystem;


namespace cqasm::v3x::parser {

ScannerAdaptor::~ScannerAdaptor() {}

ScannerAntlr::ScannerAntlr(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up)
: build_visitor_up_{ std::move(build_visitor_up) } {}

ScannerAntlr::~ScannerAntlr() {}

void ScannerAntlr::parse_(antlr4::ANTLRInputStream &is, const std::string & /* file_name */,
    cqasm::v1x::parser::ParseResult &result) {

    CqasmLexer lexer{ &is };
    antlr4::CommonTokenStream tokens{ &lexer };
    CqasmParser parser{ &tokens };
    auto ast = parser.program();
    auto custom_ast = build_visitor_up_->visitProgram(ast);
    result.root = std::any_cast<cqasm::v1x::ast::One<cqasm::v1x::ast::Root>>(custom_ast);
}

ScannerAntlrFile::ScannerAntlrFile(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up, const std::string &file_path)
: ScannerAntlr{ std::move(build_visitor_up) }, ifs_{ file_path } {
    if (!ifs_.is_open()) {
        throw error::AnalysisError("ScannerAntlrFile couldn't access file.");
    }
}

ScannerAntlrFile::~ScannerAntlrFile() {}

void ScannerAntlrFile::parse(const std::string &file_name, cqasm::v1x::parser::ParseResult &result) {
    antlr4::ANTLRInputStream is{ ifs_ };
    parse_(is, file_name, result);
}

ScannerAntlrString::ScannerAntlrString(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up, const std::string &data)
: ScannerAntlr{ std::move(build_visitor_up) }, data_{ data } {}

ScannerAntlrString::~ScannerAntlrString() {}

void ScannerAntlrString::parse(const std::string &file_name, cqasm::v1x::parser::ParseResult &result) {
    antlr4::ANTLRInputStream is{ data_ };
    parse_(is, file_name, result);
}

/**
 * Parse using the given file path.
 * Throws an AnalysisError if the file does not exist.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_file(const std::string &file_path, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>();
    auto scanner_up = std::make_unique<ScannerAntlrFile>(std::move(builder_visitor_up), file_path);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_string(const std::string &data, const std::string &file_name) {
    auto builder_visitor_up = std::make_unique<BuildTreeGenAstVisitor>();
    auto scanner_up = std::make_unique<ScannerAntlrString>(std::move(builder_visitor_up), data);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}


ParseHelper::ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, std::string file_name)
: scanner_up_(std::move(scanner_up)), file_name(std::move(file_name)) {}

/**
 * Does the actual parsing.
 */
cqasm::v1x::parser::ParseResult ParseHelper::parse() {
    cqasm::v1x::parser::ParseResult result;
    scanner_up_->parse(file_name, result);
    if (result.errors.empty() && !result.root.is_well_formed()) {
        std::cerr << *result.root;
        throw error::AnalysisError(
            "ParseHelper::parse: no parse errors returned, but AST is incomplete. AST was dumped.");
    }
    return result;
}

} // namespace cqasm::v3x::parser
