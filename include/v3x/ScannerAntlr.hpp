#pragma once

#include "v3x/BuildCustomAstVisitor.hpp"
#include "v3x/CustomErrorListener.hpp"

#include <memory>  // unique_ptr
#include <string>


namespace cqasm::v3x::parser {

struct ScannerAdaptor {
    virtual ~ScannerAdaptor();

    virtual cqasm::v1x::parser::ParseResult parse() = 0;
};

class ScannerAntlr : public ScannerAdaptor {
    std::unique_ptr<BuildCustomAstVisitor> build_visitor_up_;
    std::unique_ptr<CustomErrorListener> error_listener_up_;
protected:
    cqasm::v1x::parser::ParseResult parse_(antlr4::ANTLRInputStream &is);

public:
    explicit ScannerAntlr(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
                          std::unique_ptr<CustomErrorListener> error_listener_up);

    ~ScannerAntlr() override;

    cqasm::v1x::parser::ParseResult parse() override = 0;
};

class ScannerAntlrFile : public ScannerAntlr {
    std::string file_path_;
public:
    ScannerAntlrFile(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
                     std::unique_ptr<CustomErrorListener> error_listener_up,
                     const std::string &file_path);

    ~ScannerAntlrFile() override;

    cqasm::v1x::parser::ParseResult parse() override;
};

class ScannerAntlrString : public ScannerAntlr {
    std::string data_;
public:
    ScannerAntlrString(std::unique_ptr<BuildCustomAstVisitor> build_visitor_up,
                       std::unique_ptr<CustomErrorListener> error_listener_up,
                       const std::string &data);

    ~ScannerAntlrString() override;

    cqasm::v1x::parser::ParseResult parse() override;
};

}  // namespace cqasm::v3x::parser
