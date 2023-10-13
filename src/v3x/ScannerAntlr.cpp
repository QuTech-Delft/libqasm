#include "v3x/cqasm-ast.hpp"
#include "v3x/cqasm-parse-result.hpp"
#include "v3x/BuildTreeGenAstVisitor.hpp"
#include "v3x/CqasmLexer.h"
#include "v3x/CqasmParser.h"
#include "v3x/ScannerAntlr.hpp"

#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;


namespace cqasm::v3x::parser {

ScannerAdaptor::~ScannerAdaptor() {}

ScannerAntlr::ScannerAntlr(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up)
: build_visitor_up_{ std::move(build_visitor_up) }
, error_listener_up_{std::move(error_listener_up)} {}

ScannerAntlr::~ScannerAntlr() {}

cqasm::v3x::parser::ParseResult ScannerAntlr::parse_(antlr4::ANTLRInputStream &is) {
    CqasmLexer lexer{ &is };
    lexer.removeErrorListeners();
    lexer.addErrorListener(error_listener_up_.get());

    antlr4::CommonTokenStream tokens{ &lexer };

    CqasmParser parser{ &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(error_listener_up_.get());

    auto ast = parser.program();
    auto custom_ast = build_visitor_up_->visitProgram(ast);
    return cqasm::v3x::parser::ParseResult{
        std::any_cast<cqasm::v3x::ast::One<cqasm::v3x::ast::Program>>(custom_ast),  // root
        {}  // error
    };
}

ScannerAntlrFile::ScannerAntlrFile(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up,
    const std::string &file_path)
: ScannerAntlr{ std::move(build_visitor_up) , std::move(error_listener_up) }
, file_path_{ file_path } {
    if (!fs::exists(file_path_) || !fs::is_regular_file(file_path_)) {
        throw cqasm::error::AnalysisError{ fmt::format("ScannerAntlrFile couldn't access file '{}'.", file_path_) };
    }
}

ScannerAntlrFile::~ScannerAntlrFile() {}

cqasm::v3x::parser::ParseResult ScannerAntlrFile::parse() {
    antlr4::ANTLRFileStream ifs{};
    ifs.loadFromFile(file_path_);
    return parse_(ifs);
}

ScannerAntlrString::ScannerAntlrString(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
    std::unique_ptr<CustomErrorListener> error_listener_up,
    const std::string &data)
: ScannerAntlr{ std::move(build_visitor_up), std::move(error_listener_up) }
, data_{ data } {}

ScannerAntlrString::~ScannerAntlrString() {}

cqasm::v3x::parser::ParseResult ScannerAntlrString::parse() {
    antlr4::ANTLRInputStream is{ data_ };
    return parse_(is);
}

}  // namespace cqasm::v3x::parser
