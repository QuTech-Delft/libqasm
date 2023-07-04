#ifndef LIBQASM_HPP
#define LIBQASM_HPP
// This is a simple interface for the python checker

#include <stdio.h>
#include <string>
#include <vector>
#include "qasm_ast.hpp"
#include "qasm_semantic.hpp"

class libQasm
{
    public:
        libQasm()
        {
            sm_ = nullptr;
        }

        ~libQasm()
        {
            delete sm_;
        }

        void parse_string(const char* qasm_str)
        {
            delete sm_;
            sm_ = new compiler::QasmSemanticChecker(qasm_str);
        }

        void parse_file(const char* qasm_file_path)
        {
            FILE* qasm_file = fopen(qasm_file_path, "r");
            if (!qasm_file) {
                std::string file_not_found = std::string("File ")
                    + qasm_file_path
                    + std::string(" not found!\n");
                throw std::runtime_error(file_not_found);
            }
            delete sm_;
            sm_ = new compiler::QasmSemanticChecker(qasm_file);
        }

        int getParseResult() const
        {
            return sm_->parseResult();
        }

        const compiler::QasmRepresentation& getQasmRepresentation() const
        {
            return sm_->getQasmRepresentation();
        }

    private:
        compiler::QasmSemanticChecker* sm_;
};

#endif  // LIBQASM_HPP
