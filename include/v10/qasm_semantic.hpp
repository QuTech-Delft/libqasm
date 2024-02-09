#pragma once
#ifndef AST_SEMANTIC_HPP

#include "qasm_ast.hpp"
#include "qasm_data.hpp"
#ifndef SWIG
#include "qasm_new_to_old.hpp"
#endif

#include <exception>
#include <fmt/format.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace compiler {

namespace cq1x = cqasm::v1x;

class QasmSemanticChecker {
public:
    explicit QasmSemanticChecker(const std::string &qasm_str_input) {
        new_to_old::handle_parse_result(
            qasm_, cq1x::parser::parse_string(qasm_str_input, std::nullopt));
        maxNumQubit_ = qasm_.numQubits();
        parse_result_ = doChecks();
    }

    explicit QasmSemanticChecker(FILE* qasm_file) {
        if (!qasm_file) {
            throw std::invalid_argument("received null file pointer");
        }
        new_to_old::handle_parse_result(qasm_, cq1x::parser::parse_file(qasm_file, std::nullopt));
        maxNumQubit_ = qasm_.numQubits();
        parse_result_ = doChecks();
    }

    [[nodiscard]] int parseResult() const {
        return parse_result_;
    }

    [[nodiscard]] const compiler::QasmRepresentation& getQasmRepresentation() const {
        return qasm_;
    }

protected:
    compiler::QasmRepresentation qasm_;
    size_t maxNumQubit_;
    int parse_result_;

    int doChecks() {
        int checkResult = 0;
        for (const auto &subcircuit : qasm_.getSubCircuits().getAllSubCircuits()) {
            if (subcircuit->numberIterations() < 1) {
                throw std::runtime_error(std::string("Iteration count invalid for subcircuit ") +
                    subcircuit->nameSubCircuit() + " on Line: " + std::to_string(subcircuit->getLineNumber()));
            }
            for (auto &ops_cluster : subcircuit->getOperationsCluster()) {
                int line_number = ops_cluster->getLineNumber();
                for (auto &ops : ops_cluster->getOperations()) {
                    checkQubits(*ops, checkResult, line_number);
                }
            }
        }

        if (checkResult) {
            throw std::runtime_error(std::string("Qasm file invalid\n"));
        }
        return checkResult;
    }

    void checkQubits(compiler::Operation& op, int& result, int line_number) {
        std::string type_ = op.getType();
        if (type_ == "measure_parity") {
            result = checkMeasureParity(op, line_number);
        } else if (type_ == "u") {
            result = checkUnitaryGate(op, line_number);
        } else if (type_ == "cnot" || type_ == "cz" || type_ == "swap" || type_ == "cr" || type_ == "crk") {
            result = checkTwoQubits(op, line_number);
        } else if (type_ == "toffoli") {
            result = checkToffoli(op, line_number);
        } else if (type_ == "measure_all") {
            result = checkMeasureAll(op, line_number);
        } else if (type_ == "reset-averaging") {
            result = checkResetAveraging(op, line_number);
        } else if (type_ == "skip" || type_ == "display" || type_ == "display_binary" || type_ == "not" || type_ == "load_state") {
            result = checkSkipDisplayNot(op, line_number);
        } else {  // No other special operations. Left with single qubits
            try {
                result = checkSingleQubit(op, line_number);
            } catch (...) {
                throw std::runtime_error(std::string("Operation invalid. Line: ") + std::to_string(line_number));
            }
        }
        if (result > 0) {
            throw std::runtime_error(std::string("Operation invalid. Line: ") + std::to_string(line_number));
        }
    }

    [[nodiscard]] int checkQubitList(const compiler::Qubits& qubits, int line_number) const {
        if (auto indices = qubits.getSelectedQubits().getIndices(); indices.back() >= maxNumQubit_) {
            throw std::runtime_error{ fmt::format(
                "Qubit indices exceed the number in qubit register. Line: {}", line_number) };
        }
        return 0;
    }

    // This function ensures that the lengths of the qubit lists are the same for the different pairs involved in the operation
    [[nodiscard]] int checkQubitListLength(
        const compiler::Qubits& qubits1, const compiler::Qubits& qubits2, int line_number) const {

        (void) line_number;
        return static_cast<int>(
            qubits1.getSelectedQubits().getIndices().size() != qubits2.getSelectedQubits().getIndices().size());
    }

    [[nodiscard]] int checkUnitaryGate(const compiler::Operation& op, int line_number) const {
        if (int result_list = checkQubitList(op.getQubitsInvolved(), line_number); result_list <= 0) {
            return result_list;
        }
        throw std::runtime_error(std::string("Matrix is not unitary. Line: ") + std::to_string(line_number));
    }

    [[nodiscard]] int checkSingleQubit(const compiler::Operation& op, int line_number) const {
        return checkQubitList(op.getQubitsInvolved(), line_number);
    }

    [[nodiscard]] int checkSkipDisplayNot(const compiler::Operation& op, int line_number) const {
        (void) op;
        (void) line_number;
        return 0;
    }

    [[nodiscard]] int checkResetAveraging(const compiler::Operation& op, int line_number) const {
        return op.allQubitsBits()
            ? 0
            : checkQubitList(op.getQubitsInvolved(), line_number);
    }

    [[nodiscard]] int checkMeasureAll(const compiler::Operation& op, int line_number) const {
        (void) op;
        (void) line_number;
        return 0;
    }

    [[nodiscard]] int checkToffoli(const compiler::Operation& op, int line_number) const {
        int result_list = checkQubitList(op.getToffoliQubitPairs().first, line_number);
        result_list += checkQubitList(op.getToffoliQubitPairs().second.first, line_number);
        result_list += checkQubitList(op.getToffoliQubitPairs().second.second, line_number);
        result_list += checkQubitListLength(op.getQubitsInvolved(1), op.getQubitsInvolved(2), line_number);
        result_list += checkQubitListLength(op.getQubitsInvolved(2), op.getQubitsInvolved(3), line_number);
        if (result_list > 0) {
            throw std::runtime_error{ fmt::format("Mismatch in the qubit pair sizes. Line: {}", line_number) };
        }
        return result_list;
    }

    [[nodiscard]] int checkTwoQubits(const compiler::Operation& op, int line_number) const {
        int result_list = checkQubitList(op.getTwoQubitPairs().first, line_number);
        result_list += checkQubitList(op.getTwoQubitPairs().second, line_number);
        result_list += checkQubitListLength(op.getQubitsInvolved(1), op.getQubitsInvolved(2), line_number);
        if (result_list > 0) {
            throw std::runtime_error{ fmt::format("Mismatch in the qubit pair sizes. Line: {}", line_number) };
        }
        return result_list;
    }

    [[nodiscard]] int checkMeasureParity(const compiler::Operation& op, int line_number) const {
        auto measureParityProperties = op.getMeasureParityQubitsAndAxis();
        int result_list = checkQubitList(measureParityProperties.first.first, line_number);
        result_list += checkQubitList(measureParityProperties.first.second, line_number);
        return result_list;
    }

}; // class QasmSemanticChecker

} // namespace compiler

#endif
