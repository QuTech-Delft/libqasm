#include "libqasm/v3x/antlr_scanner.hpp"

#include <antlr4-runtime.h>
#include <fmt/format.h>

#include <filesystem>

#include "libqasm/v3x/CqasmLexer.h"
#include "libqasm/v3x/CqasmParser.h"
#include "libqasm/v3x/antlr_custom_error_listener.hpp"
#include "libqasm/v3x/parse_result.hpp"
#include "libqasm/v3x/syntactic.hpp"
#include "libqasm/v3x/syntactic_analyzer.hpp"

namespace fs = std::filesystem;

namespace cqasm::v3x::parser {

ScannerAdaptor::~ScannerAdaptor() = default;

AntlrScanner::AntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
    std::unique_ptr<AntlrCustomErrorListener> error_listener_up)
: build_visitor_up_{ std::move(build_visitor_up) }
, error_listener_up_{ std::move(error_listener_up) } {}

AntlrScanner::~AntlrScanner() = default;

cqasm::v3x::parser::ParseResult AntlrScanner::parse_(antlr4::ANTLRInputStream& is) {
    CqasmLexer lexer{ &is };
    lexer.removeErrorListeners();
    lexer.addErrorListener(error_listener_up_.get());
    antlr4::CommonTokenStream tokens{ &lexer };

    CqasmParser parser{ &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(error_listener_up_.get());
    auto ast = parser.program();

    build_visitor_up_->addErrorListener(error_listener_up_.get());
    auto custom_ast = build_visitor_up_->visitProgram(ast);
    return cqasm::v3x::parser::ParseResult{
        std::any_cast<cqasm::v3x::syntactic::One<cqasm::v3x::syntactic::Program>>(custom_ast),  // root
        {}  // error
    };
}

FileAntlrScanner::FileAntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
    std::unique_ptr<AntlrCustomErrorListener> error_listener_up, std::string file_path)
: AntlrScanner{ std::move(build_visitor_up), std::move(error_listener_up) }
, file_path_{ std::move(file_path) } {
    if (!fs::exists(file_path_) || !fs::is_regular_file(file_path_)) {
        throw cqasm::error::ParseError{ fmt::format("FileAntlrScanner couldn't access file '{}'.", file_path_) };
    }
}

FileAntlrScanner::~FileAntlrScanner() = default;

cqasm::v3x::parser::ParseResult FileAntlrScanner::parse() {
    antlr4::ANTLRFileStream ifs{};
    ifs.loadFromFile(file_path_);
    return parse_(ifs);
}

StringAntlrScanner::StringAntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
    std::unique_ptr<AntlrCustomErrorListener> error_listener_up, std::string data)
: AntlrScanner{ std::move(build_visitor_up), std::move(error_listener_up) }
, data_{ std::move(data) } {}

StringAntlrScanner::~StringAntlrScanner() = default;

cqasm::v3x::parser::ParseResult StringAntlrScanner::parse() {
    antlr4::ANTLRInputStream is{ data_ };
    return parse_(is);
}

}  // namespace cqasm::v3x::parser
