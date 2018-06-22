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
                int checkResult = 0;
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

                if (!checkResult)
                    std::cout << "Semantic check complete. Qasm file is valid." << std::endl;
                else
                    throw std::runtime_error(std::string("Qasm file invalid\n"));
                return checkResult;
            }

            void checkQubits(compiler::Operation& op, int & result)
            {
                std::string type_ = op.getType();
                if (type_ == "measure_parity")
                {
                    result = checkMeasureParity(op);
                }
                else if (type_ == "cnot" || type_ == "cz" || type_ == "swap" || type_ == "cr" || type_ == "crk")
                {
                    result = checkTwoQubits(op);
                }
                else if (type_ == "toffoli")
                {
                    result = checkToffoli(op);
                }
                else if (type_ == "measure_all")
                {
                    result = checkMeasureAll(op);
                }
                else if (type_ == "reset-averaging")
                {
                    result = checkResetAveraging(op);
                }
                else if (type_ == "wait" || type_ == "display" || type_ == "not")
                {
                    result = checkWaitDisplayNot(op);
                }
                else 
                // No other special operations. Left with single qubits
                {
                    result = checkSingleQubit(op);
                }
                if (result > 0)
                    throw std::runtime_error(std::string("Operation invalid\n"));

            }            

            int checkQubitList(const compiler::Qubits& qubits) const
            {
                auto indices = qubits.getSelectedQubits().getIndices();
                if (indices.back() < maxNumQubit_)
                    return 0;
                else
                    throw std::runtime_error(std::string("Qubit indices exceed the number in qubit register\n"));
            }

            int checkQubitListLength(const compiler::Operation& op __attribute__((unused))) const
            // This function ensures that the lengths of the qubit lists are the same for the different pairs involved in the operation
            {
                return 0;
            }

            int checkSingleQubit(const compiler::Operation& op) const
            {
                return checkQubitList(op.getQubitsInvolved());
            }

            int checkWaitDisplayNot(const compiler::Operation& op __attribute__((unused))) const
            {
                return 0;
            }

            int checkResetAveraging(const compiler::Operation& op) const
            {
                int result = 1;
                if (op.allQubitsBits())
                    result = 0;
                else
                    result = checkQubitList(op.getQubitsInvolved());
                return result;
            }

            int checkMeasureAll(const compiler::Operation& op __attribute__((unused))) const
            {
                return 0;
            }

            int checkToffoli(const compiler::Operation& op) const
            {
                int result = 1;
                int resultlist = checkQubitList(op.getToffoliQubitPairs().first);
                resultlist += checkQubitList(op.getToffoliQubitPairs().second.first);
                resultlist += checkQubitList(op.getToffoliQubitPairs().second.second); 
                result *= resultlist;
                return result;
            }

            int checkTwoQubits(const compiler::Operation& op) const
            {
                int result = 1;
                int resultlist = checkQubitList(op.getTwoQubitPairs().first);
                resultlist += checkQubitList(op.getTwoQubitPairs().second); 
                result *= resultlist;
                return result;
            }

            int checkMeasureParity(const compiler::Operation& op) const
            {
                int result = 1;
                auto measureParityProperties = op.getMeasureParityQubitsAndAxis();
                int resultlist = checkQubitList(measureParityProperties.first.first);
                resultlist += checkQubitList(measureParityProperties.first.second); 
                result *= resultlist;
                return result;
            }


    }; // class QasmSemanticChecker

} // namespace compiler

#endif