#pragma once
#ifndef AST_SEMANTIC_HPP

#include "qasm_ast.hpp"
#include <stdio.h>
#include <vector>

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

            
            int doChecks()
            {
                int checkResult = 1;
                for (auto subcircuit : qasm_.getSubCircuits().getAllSubCircuits()) 
                {
                    for (auto ops_cluster : subcircuit.getOperationsCluster()) 
                    {
                        for (auto ops : ops_cluster->getOperations()) 
                        {
                            checkQubits(*ops, checkResult);
                        }
                    }
                }

                std::cout << "Semantic check complete. Qasm file is valid." << std::endl;
                return checkResult;
            }

            void checkQubits(compiler::Operation& op, int & result)
            {
                std::string type_ = op.getType();
                if ( type_ == "rx" || type_ == "ry" || type_ == "rz" )
                {
                    result = checkQubitList(op.getQubitsInvolved());
                }
                else if (type_ == "measure_parity")
                {
                    auto measureParityProperties = op.getMeasureParityQubitsAndAxis();
                    result = checkQubitList(measureParityProperties.first.first);
                    result = checkQubitList(measureParityProperties.first.second); 
                }
                else if (type_ == "cnot" || type_ == "cz" || type_ == "swap" || type_ == "cr")
                {
                    result = checkQubitList(op.getTwoQubitPairs().first);
                    result = checkQubitList(op.getTwoQubitPairs().second); 
                }
                else if (type_ == "toffoli")
                {
                    result = checkQubitList(op.getToffoliQubitPairs().first);
                    result = checkQubitList(op.getToffoliQubitPairs().second.first);
                    result = checkQubitList(op.getToffoliQubitPairs().second.second); 
                }
                else if (type_ == "measure_all")
                {
                    result = 0;
                }
                else if (type_ == "reset-averaging")
                {
                    if (op.allQubitsBits())
                        result = 0;
                    else
                        result = checkQubitList(op.getQubitsInvolved());
                }
                else if (type_ == "wait" || type_ == "display" || type_ == "not")
                {
                    result = 0;
                }
                else 
                {
                    result = checkQubitList(op.getQubitsInvolved());
                }

                if (op.isBitControlled()){
                    op.getControlBits().printMembers();
                }
            }            

            int checkQubitList(const compiler::Qubits& qubits) const
            {
                auto indices = qubits.getSelectedQubits().getIndices();
                std::cout << "Max index = " << indices.back() << std::endl;
                if (indices.back() < maxNumQubit_)
                    return 0;
                else
                    throw std::runtime_error(std::string("Qubit indices exceed the number in qubit register\n"));
            }



    }; // class QasmSemanticChecker

} // namespace compiler

#endif