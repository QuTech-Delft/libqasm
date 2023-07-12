#ifndef LIBQASM_HPP
#define LIBQASM_HPP
// This is a simple interface for the python checker

#include "qasm_ast.hpp"
#include "qasm_semantic.hpp"

#include <stdio.h>
#include <string>
#include <vector>

class libQasm {
public:
    libQasm() : parse_result_{ 0 }, qasm_rep_{} {}

    void parse_string(const std::string &qasm_str) {
        compiler::QasmSemanticChecker sm(qasm_str);
        qasm_rep_ = sm.getQasmRepresentation();
        parse_result_ = sm.parseResult();
    }

    void parse_file(const std::string &qasm_file_path) {
        FILE* qasm_file = fopen(qasm_file_path.c_str(), "r");
        if (!qasm_file) {
            std::string file_not_found = std::string("File ")
                + qasm_file_path
                + std::string(" not found!\n");
            throw std::runtime_error(file_not_found);
        }
        compiler::QasmSemanticChecker sm(qasm_file);
        qasm_rep_ = sm.getQasmRepresentation();
        parse_result_ = sm.parseResult();
    }

    int getParseResult() const {
        return parse_result_;
    }

    const compiler::QasmRepresentation& getQasmRepresentation() const {
        return qasm_rep_;
    }

private:
    compiler::QasmRepresentation qasm_rep_;
    int parse_result_;
};

#endif  // LIBQASM_HPP
