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


namespace cqasm {
namespace v3x {
namespace parser {

ScannerAntlr::ScannerAntlr() {}

ScannerAntlr::~ScannerAntlr() {}

void ScannerAntlr::parse_(const std::string & /* file_name */, cqasm::v1x::parser::ParseResult &result,
                          antlr4::ANTLRInputStream &is) {
    CqasmLexer lexer{ &is };
    antlr4::CommonTokenStream tokens{ &lexer };
    CqasmParser parser{ &tokens };
    // v1x/cqasm-parse-helper.cpp, through cqasm-parser.y, passes a v1x/ParseHelper to the Flex/Bison code
    // That v1x/ParseHelper includes both the parse result and the file name used for logging purposes
    // The Flex/Bison code:
    //   1) builds AST nodes along the parsing, and, in the end,
    //   2) sets helper.result.root to the Program AST node.
    //
    // We have to do something similar here with ANTLR
    // This parse_ function should fill the parse result as a side effect of the ANTLR parsing
    // And, in case of logging any errors, use the file name
    auto ast = parser.program();
    auto tree_gen_ast = BuildTreeGenAstVisitor{}.visitProgram(ast);
    result.root = std::any_cast<cqasm::v1x::ast::One<cqasm::v1x::ast::Root>>(tree_gen_ast);
}

ScannerAntlrFile::ScannerAntlrFile(const std::string &file_path)
: ifs_{ file_path } {
    if (!ifs_.is_open()) {
        throw error::AnalysisError("ScannerAntlrFile couldn't access file.");
    }
}

void ScannerAntlrFile::parse(const std::string &file_name, cqasm::v1x::parser::ParseResult &result) {
    antlr4::ANTLRInputStream is{ ifs_ };
    parse_(file_name, result, is);
}

ScannerAntlrString::ScannerAntlrString(const std::string &data)
: data_{ data } {}

void ScannerAntlrString::parse(const std::string &file_name, cqasm::v1x::parser::ParseResult &result) {
    antlr4::ANTLRInputStream is{ data_ };
    parse_(file_name, result, is);
}

/**
 * Parse using the given file path.
 * Throws an AnalysisError if the file does not exist.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_file(const std::string &file_path, const std::string &file_name) {
    auto scanner_up = std::make_unique<ScannerAntlrFile>(file_path);
    return ParseHelper(std::move(scanner_up), file_name).parse();
}

/**
 * Parse the given string.
 * A file_name may be given in addition for use within error messages.
 */
cqasm::v1x::parser::ParseResult parse_string(const std::string &data, const std::string &file_name) {
    auto scanner_up = std::make_unique<ScannerAntlrString>(data);
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

} // namespace parser
} // namespace v3x
} // namespace cqasm
