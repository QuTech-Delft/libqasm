#pragma once
#ifndef AST_SEMANTIC_HPP

#include "qasm_ast.hpp"
#include <stdio.h>

extern int yyparse();
extern int yylex();
extern FILE* yyin;
extern compiler::QasmRepresentation qasm_representation;

namespace compiler
{
    class QasmSemanticChecker
    {
        public:
            QasmSemanticChecker(FILE *qasm_file)
            {
                // set lex to read from it instead of defaulting to STDIN:
                yyin = qasm_file;

                // parse through the input until there is no more:
                int result = 0;
                
                do {
                    result += yyparse();
                } while (!feof(yyin));
                if (result)
                    throw std::runtime_error(std::string("Could not parse qasm file!\n"));

                maxNumQubit_ = qasm_representation.numQubits();
                qasm_ = qasm_representation;
                result = doChecks();
                parse_result_ = result;
            }

            int parseResult() const
            {
                return parse_result_;
            }

            const compiler::QasmRepresentation& getQasmRepresentation() const
            {
                return qasm_;
            } 

        protected:
            compiler::QasmRepresentation qasm_;
            size_t maxNumQubit_;
            int parse_result_;

            
            int doChecks() const
            {
                int checkResult = 0;
                std::cout << "Semantic check complete. Qasm file is valid." << std::endl;
                return checkResult;
            }



    }; // class QasmSemanticChecker

} // namespace compiler

#endif