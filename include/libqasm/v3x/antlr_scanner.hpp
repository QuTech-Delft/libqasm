#pragma once

#include <memory>  // unique_ptr
#include <string>

#include "libqasm/v3x/parse_result.hpp"
#include "libqasm/v3x/syntactic_analyzer_base.hpp"

namespace antlr4 {
class ANTLRInputStream;
}
namespace cqasm::v3x::parser {
class AntlrCustomErrorListener;
}

namespace cqasm::v3x::parser {

struct ScannerAdaptor {
    virtual ~ScannerAdaptor();

    virtual cqasm::v3x::parser::ParseResult parse() = 0;
};

class AntlrScanner : public ScannerAdaptor {
    std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up_;
    std::unique_ptr<AntlrCustomErrorListener> error_listener_up_;

protected:
    cqasm::v3x::parser::ParseResult parse_(antlr4::ANTLRInputStream& is);

public:
    AntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
        std::unique_ptr<AntlrCustomErrorListener> error_listener_up);

    ~AntlrScanner() override;

    cqasm::v3x::parser::ParseResult parse() override = 0;
};

class FileAntlrScanner : public AntlrScanner {
    std::string file_path_;

public:
    FileAntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
        std::unique_ptr<AntlrCustomErrorListener> error_listener_up, std::string file_path);

    ~FileAntlrScanner() override;

    cqasm::v3x::parser::ParseResult parse() override;
};

class StringAntlrScanner : public AntlrScanner {
    std::string data_;

public:
    StringAntlrScanner(std::unique_ptr<BaseSyntacticAnalyzer> build_visitor_up,
        std::unique_ptr<AntlrCustomErrorListener> error_listener_up, std::string data);

    ~StringAntlrScanner() override;

    cqasm::v3x::parser::ParseResult parse() override;
};

}  // namespace cqasm::v3x::parser
