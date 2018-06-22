// This is a simple interface for the python checker

#include <stdio.h>
#include <string>
#include <vector>
#include "qasm_semantic.hpp"

class libQasmPy
{
    public:
        libQasmPy(std::string qasm_file_path)
        {
            if (!myfile) {
                std::cout << "File " << argv[1] << " not found!" << std::endl;
                return -1;
            }
            FILE *myfile = fopen(qasm_file_path.c_str(), "r");
            sm = new compiler::QasmSemanticChecker(myfile);
            qasm_rep = sm -> getQasmRepresentation();
            parse_result_ = sm -> parseResult();
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
}