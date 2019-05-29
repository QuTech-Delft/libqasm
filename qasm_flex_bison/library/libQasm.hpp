// This is a simple interface for the python checker

#include <stdio.h>
#include <string>
#include <vector>
#include "qasm_ast.hpp"
#include "qasm_semantic.hpp"

class libQasm
{
    public:
        libQasm(char* qasm_file_path)
        {
            FILE* myfile = fopen(qasm_file_path, "r");
            if (!myfile) {
                std::string file_not_found = std::string("File ")
                                              + qasm_file_path
                                              + std::string(" not found!\n");
                throw std::runtime_error(file_not_found);
            }
            sm_ = new compiler::QasmSemanticChecker(myfile);
            qasm_rep_ = sm_->getQasmRepresentation();
            parse_result_ = sm_->parseResult();
        }

        int getParseResult() const
        {
            return parse_result_;
        }

        const compiler::QasmRepresentation& getQasmRepresentation() const
        {
            return qasm_rep_;
        }

    private:
        compiler::QasmSemanticChecker* sm_;
        compiler::QasmRepresentation qasm_rep_;
        int parse_result_;
};
