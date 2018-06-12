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
    class SemanticChecker
    {
        public:
            SemanticChecker(FILE *myfile)
            {
                // set lex to read from it instead of defaulting to STDIN:
                yyin = myfile;

                // parse through the input until there is no more:
                int result = 0;
                
                do {
                    result += yyparse();
                } while (!feof(yyin));
                if (result)
                    throw std::runtime_error(std::string("Could not parse qasm file!\n"));

                parse_result_ = result;
                maxNumQubit_ = qasm_representation.numQubits();
                qasm_ = qasm_representation;
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

            
            // void qubitRegisterCheck(const compiler::Qubits& qubits)
            // {

            // }



    }; // class SemanticChecker

} // namespace compiler

#endif