#pragma once
#ifndef AST_SEMANTIC_HPP

#include <stdio.h>
#include <string.h>
#include <vector>
#include <exception>
#include "qasm_ast.hpp"
#include "qasm_data.hpp"
#ifndef SWIG
#include "qasm_new_to_old.hpp"
#endif

namespace compiler
{
    class QasmSemanticChecker
    {
        public:
            QasmSemanticChecker(const char* qasm_str_input)
            {
                if (!qasm_str_input) {
                    throw std::invalid_argument("received null string");
                }
                new_to_old::handle_parse_result(
                    qasm_, cqasm::parser::parse_string(qasm_str_input));
                maxNumQubit_ = qasm_.numQubits();
                parse_result_ = doChecks();
            }

            QasmSemanticChecker(FILE* qasm_file)
            {
                if (!qasm_file) {
                    throw std::invalid_argument("received null file pointer");
                }
                new_to_old::handle_parse_result(
                    qasm_, cqasm::parser::parse_file(qasm_file));
                maxNumQubit_ = qasm_.numQubits();
                parse_result_ = doChecks();
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
                    if (subcircuit->numberIterations() < 1)
                    {
                        std::string base_error_message("Iteration count invalid for subcircuit");
                        std::string entire_error_message = base_error_message +
                                                           " " + subcircuit->nameSubCircuit() +
                                                           " on Line: " +
                                                           std::to_string(subcircuit->getLineNumber());
                        throw std::runtime_error(entire_error_message);
                    }
                    for (auto ops_cluster : subcircuit->getOperationsCluster())
                    {
                        int linenumber = ops_cluster->getLineNumber();
                        for (auto ops : ops_cluster->getOperations())
                        {
                            checkQubits(*ops, checkResult, linenumber);
                        }
                    }
                }

                if (checkResult)
                    throw std::runtime_error(std::string("Qasm file invalid\n"));
                return checkResult;
            }

            void checkQubits(compiler::Operation& op, int& result, int linenumber)
            {
                std::string type_ = op.getType();
                if (type_ == "measure_parity")
                {
                    result = checkMeasureParity(op, linenumber);
                }
                else if (type_ == "u")
                {
                    result = checkUnitaryGate(op, linenumber);
                }
                else if (type_ == "cnot" || type_ == "cz" || type_ == "swap" || type_ == "cr" || type_ == "crk")
                {
                    result = checkTwoQubits(op, linenumber);
                }
                else if (type_ == "toffoli")
                {
                    result = checkToffoli(op, linenumber);
                }
                else if (type_ == "measure_all")
                {
                    result = checkMeasureAll(op, linenumber);
                }
                else if (type_ == "reset-averaging")
                {
                    result = checkResetAveraging(op, linenumber);
                }
                else if (type_ == "skip" || type_ == "display" || type_ == "display_binary" || type_ == "not" || type_ == "load_state")
                {
                    result = checkSkipDisplayNot(op, linenumber);
                }
                else
                // No other special operations. Left with single qubits
                {
                    try
                    {
                        result = checkSingleQubit(op, linenumber);
                    }
                    catch (...)
                    {
                        throw std::runtime_error(std::string("Operation invalid.") + std::string(" Line: ") + std::to_string(linenumber));
                    }
                }
                if (result > 0)
                    throw std::runtime_error(std::string("Operation invalid. ") + "Line " + std::to_string(linenumber));

            }

            int checkQubitList(const compiler::Qubits& qubits, int linenumber) const
            {
                auto indices = qubits.getSelectedQubits().getIndices();
                if (indices.back() < maxNumQubit_)
                    return 0;
                else
                    throw std::runtime_error(std::string("Qubit indices exceed the number in qubit register. Line: ")
                                             + std::to_string(linenumber));
            }

            int checkQubitListLength(const compiler::Qubits& qubits1,
                                     const compiler::Qubits& qubits2,
                                     int linenumber) const
            // This function ensures that the lengths of the qubit lists are the same for the different pairs involved in the operation
            {
                (void)linenumber;
                int retnum = 1;
                if (qubits1.getSelectedQubits().getIndices().size() ==
                     qubits2.getSelectedQubits().getIndices().size())
                {
                    retnum = 0;
                }
                return retnum;
            }

            int checkUnitaryGate(const compiler::Operation& op, int linenumber) const
            {
                int result = 1;
                int resultlist = checkQubitList(op.getQubitsInvolved(), linenumber);
                if (resultlist > 0)
                {
                    std::string base_error_message("Matrix is not unitary. Line: ");
                    std::string entire_error_message = base_error_message +
                                                       std::to_string(linenumber);
                    throw std::runtime_error(entire_error_message);
                }
                result *= resultlist;
                return result;
            }

            int checkSingleQubit(const compiler::Operation& op, int linenumber) const
            {
                return checkQubitList(op.getQubitsInvolved(), linenumber);
            }

            int checkSkipDisplayNot(const compiler::Operation& op, int linenumber) const
            {
                (void)op;
                (void)linenumber;
                return 0;
            }

            int checkResetAveraging(const compiler::Operation& op, int linenumber) const
            {
                int result = 1;
                if (op.allQubitsBits())
                    result = 0;
                else
                    result = checkQubitList(op.getQubitsInvolved(), linenumber);
                return result;
            }

            int checkMeasureAll(const compiler::Operation& op, int linenumber) const
            {
                (void)op;
                (void)linenumber;
                return 0;
            }

            int checkToffoli(const compiler::Operation& op, int linenumber) const
            {
                int result = 1;
                int resultlist = checkQubitList(op.getToffoliQubitPairs().first, linenumber);
                resultlist += checkQubitList(op.getToffoliQubitPairs().second.first, linenumber);
                resultlist += checkQubitList(op.getToffoliQubitPairs().second.second, linenumber);
                resultlist += checkQubitListLength(op.getQubitsInvolved(1), op.getQubitsInvolved(2), linenumber);
                resultlist += checkQubitListLength(op.getQubitsInvolved(2), op.getQubitsInvolved(3), linenumber);
                if (resultlist > 0)
                {
                    std::string base_error_message("Mismatch in the qubit pair sizes. Line: ");
                    std::string entire_error_message = base_error_message +
                                                       std::to_string(linenumber);
                    throw std::runtime_error(entire_error_message);
                }
                result *= resultlist;
                return result;
            }

            int checkTwoQubits(const compiler::Operation& op, int linenumber) const
            {
                int result = 1;
                int resultlist = checkQubitList(op.getTwoQubitPairs().first, linenumber);
                resultlist += checkQubitList(op.getTwoQubitPairs().second, linenumber);
                resultlist += checkQubitListLength(op.getQubitsInvolved(1), op.getQubitsInvolved(2), linenumber);
                if (resultlist > 0)
                {
                    std::string base_error_message("Mismatch in the qubit pair sizes. Line: ");
                    std::string entire_error_message = base_error_message +
                                                       std::to_string(linenumber);
                    throw std::runtime_error(entire_error_message);
                }
                result *= resultlist;
                return result;
            }

            int checkMeasureParity(const compiler::Operation& op, int linenumber) const
            {
                int result = 1;
                auto measureParityProperties = op.getMeasureParityQubitsAndAxis();
                int resultlist = checkQubitList(measureParityProperties.first.first, linenumber);
                resultlist += checkQubitList(measureParityProperties.first.second, linenumber);
                result *= resultlist;
                return result;
            }


    }; // class QasmSemanticChecker

} // namespace compiler

#endif
