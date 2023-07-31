#pragma once

#ifndef QASM_AST_REP
#define QASM_AST_REP

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Namespace used for most of the original API.
 *
 * \deprecated Please use the \ref cqasm "new API" instead!
 */
namespace compiler {

// This class takes a list of identifiers used by both bits and qubits to internal int vectors
class NumericalIdentifiers {
public:
    NumericalIdentifiers() = default;

    // This is used when a single integer is encountered
    void addToVector(const int index) {
        indices_.push_back( static_cast<size_t>(index) );
    }

    // This is used for when a range of qubits is being defined
    void addToVector(const int index_min, const int index_max) {
        for (int index = index_min; index <= index_max; ++index) {
            indices_.push_back(static_cast<size_t>(index));
        }
    }

    const std::vector<size_t>& getIndices() const {
        return indices_;
    }

    void removeDuplicates() {
        std::sort( indices_.begin(), indices_.end() );
        indices_.erase( std::unique( indices_.begin(), indices_.end() ), indices_.end() );
    }

    void clear() {
        indices_.clear();
    }

    void printMembers() const {
        std::cout << "Indices: ";
        for (size_t elems : getIndices()) {
            std::cout << elems << " ";
        }
        std::cout << "\n";
    }

protected:
    std::vector<size_t> indices_;
}; // class NumericalIdentifiers


// This class encapsulates the participating qubits in the specified operation
class Qubits {
public:
    Qubits() = default;
    Qubits(NumericalIdentifiers indices) : selected_qubits_(indices) {}

    void setSelectedQubits(NumericalIdentifiers indices) {
        selected_qubits_ = indices;
    }

    const NumericalIdentifiers& getSelectedQubits() const {
        return selected_qubits_;
    }

    void printMembers() const {
        std::cout << "Selected Qubits - ";
        getSelectedQubits().printMembers();
    }

protected:
    NumericalIdentifiers selected_qubits_;
};


// This class encapsulates the participating bit in the specified operation
class Bits {
public:
    Bits() = default;
    Bits(NumericalIdentifiers indices) : selected_bits_(indices) {}

    void setSelectedBits(NumericalIdentifiers indices) {
        selected_bits_ = indices;
    }

    const NumericalIdentifiers& getSelectedBits() const {
        return selected_bits_;
    }

    void printMembers() const {
        std::cout << "Selected Bits - ";
        getSelectedBits().printMembers();
    }

protected:
    NumericalIdentifiers selected_bits_;
};


class Operation {
public:
    // This is the most common operation, the single qubit operation, or reset_averaging
    Operation(const std::string type, Qubits qubits_involved)
    : qubits_(qubits_involved)
    , rotation_angle_(std::numeric_limits<double>::max()),
      bit_controlled_(false) {
        type_ = toLowerCase(type);
    }

    // Single qubit rotations
    Operation(const std::string type, Qubits qubits_involved, const double rotation_angle)
    : qubits_(qubits_involved)
    , rotation_angle_(rotation_angle)
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
    }

    // Single qubit wait
    Operation(const std::string type, Qubits qubits_involved, const int wait_int)
    : qubits_(qubits_involved)
    , rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        wait_time_ = wait_int;
    }

    // Measure parity operation
    Operation(const std::string type, Qubits qubit_pair1, std::string axis1, Qubits qubit_pair2, std::string axis2)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        measure_parity_qubits_ = std::pair<Qubits, Qubits>(qubit_pair1, qubit_pair2);
        measure_parity_axis_ = std::pair<std::string, std::string>(toLowerCase(axis1), toLowerCase(axis2));
    }

    // Measure all operation
    Operation(const std::string type)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        all_qubits_bits_ = true;
    }

    // Skip command
    Operation(const std::string type, const int wait_int)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        wait_time_ = wait_int;
    }

    // Display command
    Operation(const std::string type, const Bits display_bits)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        bits_ = display_bits;
    }

    // Two qubit gate operations
    Operation(const std::string type, Qubits qubit_pair1, Qubits qubit_pair2)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        two_qubit_pairs_ = std::pair<Qubits, Qubits>(qubit_pair1, qubit_pair2);
    }

    // Two qubit gate operations
    Operation(const std::string type, Qubits qubit_pair1, Qubits qubit_pair2, double rotations)
    : rotation_angle_(rotations)
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        two_qubit_pairs_ = std::pair<Qubits, Qubits>(qubit_pair1, qubit_pair2);
    }

    // Toffoli operations
    Operation(const std::string type, Qubits qubit_pair1, Qubits qubit_pair2, Qubits qubit_pair3)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        toffoli_qubit_pairs_ = std::pair<Qubits, std::pair<Qubits, Qubits>>(
            qubit_pair1,
            std::pair<Qubits, Qubits>(qubit_pair2, qubit_pair3)
        );
    }

    // load_state command (this one is special because it should be case sensitive for the filename)
    Operation(const std::string type, const std::string state_filename)
    : rotation_angle_(std::numeric_limits<double>::max())
    , bit_controlled_(false) {
        type_ = toLowerCase(type);
        state_filename_ = removeQuotes(state_filename);
    }

    std::string getType() const {
        return type_;
    }

    std::string getStateFilename() const {
        return state_filename_;
    }

    const Qubits& getQubitsInvolved() const {
        return qubits_;
    }

    const Qubits& getQubitsInvolved(const int qubit_pair_index) const {
        if (type_ == "toffoli") {
            switch (qubit_pair_index) {
                case 1: return toffoli_qubit_pairs_.first;
                case 2: return toffoli_qubit_pairs_.second.first;
                case 3: return toffoli_qubit_pairs_.second.second;
                default: throw std::runtime_error( std::string("Accessing qubit pair ")
                            + std::to_string(qubit_pair_index)
                            + std::string(" on operation ") + type_ );
            }
        } else if (type_ == "cnot" || type_ == "cz" || type_ == "swap" || type_ == "cr" || type_ == "crk") {
            switch(qubit_pair_index){
                case 1: return two_qubit_pairs_.first;
                case 2: return two_qubit_pairs_.second;
                default: throw std::runtime_error( std::string("Accessing qubit pair ")
                            + std::to_string(qubit_pair_index)
                            + std::string(" on operation ") + type_ );
            }
        } else {
            throw std::runtime_error(std::string("Accessing qubit pair ")
                + std::to_string(qubit_pair_index)
                + std::string(" on operation ") + type_ );
        }
    }

    double getRotationAngle() const {
        return rotation_angle_;
    }

    const std::pair<std::pair<Qubits, Qubits>, std::pair<std::string, std::string>> getMeasureParityQubitsAndAxis() const {
        std::pair<std::pair<Qubits, Qubits>, std::pair<std::string, std::string>> pair_result(measure_parity_qubits_, measure_parity_axis_);
        return pair_result;
    }

    const std::pair <Qubits, Qubits>& getTwoQubitPairs() const {
        return two_qubit_pairs_;
    }

    const std::pair<Qubits, std::pair<Qubits, Qubits>>& getToffoliQubitPairs() const {
        return toffoli_qubit_pairs_;
    }

    bool isBitControlled() const {
        return bit_controlled_;
    }

    bool allQubitsBits() const {
        return all_qubits_bits_;
    }

    void setControlBits(Bits control_bits) {
        bits_ = control_bits;
        bit_controlled_ = true;
    }

    const Bits& getControlBits() const {
        return bits_;
    }

    const Bits& getDisplayBits() const {
        return bits_;
    }

    int getWaitTime() const {
        return wait_time_;
    }

    void setUMatrixElements(const std::vector<double> input) {
        unitary_matrix_elements_ = input;
    }

    const std::vector<double>& getUMatrixElements() const {
        return unitary_matrix_elements_;
    }

    void printOperation() const {
        std::cout << "Operation " << type_ << ": ";
        if (type_ == "rx" || type_ == "ry" || type_ == "rz") {
            getQubitsInvolved().printMembers();
            std::cout << "Rotations = " << getRotationAngle() << "\n";
        } else if (type_ == "measure_parity") {
            std::cout << "\n";
            auto measureParityProperties = getMeasureParityQubitsAndAxis();
            measureParityProperties.first.first.printMembers();
            std::cout << "With axis " << measureParityProperties.second.first << "\n";
            measureParityProperties.first.second.printMembers();
            std::cout << "With axis " << measureParityProperties.second.second << "\n";
        } else if (type_ == "cnot" || type_ == "cz" || type_ == "swap") {
            std::cout << "\n";
            std::cout << "Qubit Pair 1: ";
            getTwoQubitPairs().first.printMembers();
            std::cout << "Qubit Pair 2: ";
            getTwoQubitPairs().second.printMembers();
        } else if (type_ == "cr") {
            std::cout << "\n";
            std::cout << "Qubit Pair 1: ";
            getTwoQubitPairs().first.printMembers();
            std::cout << "Qubit Pair 2: ";
            getTwoQubitPairs().second.printMembers();
            std::cout << "Rotation = " << getRotationAngle() << "\n";
        } else if (type_ == "toffoli") {
            std::cout << "\n";
            std::cout << "Qubit Pair 1: ";
            getToffoliQubitPairs().first.printMembers();
            std::cout << "Qubit Pair 2: ";
            getToffoliQubitPairs().second.first.printMembers();
            std::cout << "Qubit Pair 3: ";
            getToffoliQubitPairs().second.second.printMembers();
        } else if (type_ == "skip") {
            std::cout << "\n";
            std::cout << "Wait time (integer) = " << getWaitTime() << "\n";
        } else if (type_ == "wait") {
            std::cout << "\n";
            getQubitsInvolved().printMembers();
            std::cout << "Wait time (integer) = " << getWaitTime() << "\n";
        } else if ((type_ == "display") || (type_ == "display_binary")) {
            std::cout << "Display bits: ";
            getDisplayBits().printMembers();
        } else {
            getQubitsInvolved().printMembers();
        }

        if (isBitControlled()){
            std::cout << "Bit controlled with bits: ";
            getControlBits().printMembers();
        }
    }

protected:
    std::string toLowerCase(const std::string& string_input) {
        std::string lower_case_input = string_input;
        std::transform(lower_case_input.begin(), lower_case_input.end(), lower_case_input.begin(), ::tolower);
        return lower_case_input;
    }

    std::string removeQuotes(const std::string& string_input) {
        std::string result = string_input;
        result.erase(result.begin());
        result.erase(result.end()-1);
        return result;
    }

    std::string type_;
    std::string state_filename_;
    Qubits qubits_;
    Bits bits_;
    double rotation_angle_;
    bool bit_controlled_;
    bool all_qubits_bits_ = false;
    int wait_time_;
    std::pair<Qubits, Qubits> measure_parity_qubits_;
    std::pair<std::string, std::string> measure_parity_axis_;
    std::pair<Qubits, Qubits> two_qubit_pairs_;
    std::pair<Qubits, std::pair<Qubits, Qubits>> toffoli_qubit_pairs_;
    std::vector<double> unitary_matrix_elements_;
}; // class Operation


// This class enables parallel operation support
class OperationsCluster {
public:
    OperationsCluster() {
        isParallel_ = false;
    }

    OperationsCluster(std::shared_ptr<Operation> valid_op, int linenumber) {
        operations_.push_back(std::move(valid_op));
        isParallel_ = false;
        linenumber_ = linenumber;
    }

    std::shared_ptr<Operation> lastOperation() {
        return operations_.back();
    }

    void addOperation(std::shared_ptr<Operation> valid_op) {
        operations_.push_back(std::move(valid_op));
    }

    void addParallelOperation(std::shared_ptr<Operation> valid_op) {
        operations_.push_back(std::move(valid_op));
        isParallel_ = true;
    }

    bool isParallel() const {
        return isParallel_;
    }

    const std::vector<std::shared_ptr<Operation>>& getOperations() const {
        return operations_;
    }

    void setLineNumber(int linenumber) {
        linenumber_ = linenumber;
    }

    int getLineNumber() const {
        return linenumber_;
    }

    void printOperations() {
        if (isParallel()) {
            std::cout << "Parallel operations cluster: " << "\n";
            for (auto elem : getOperations()) {
                elem->printOperation();
            }
            std::cout << "End Parallel operations \n\n";
        } else {
            std::cout << "Serial operation: " << "\n";
            for (auto elem : getOperations()) {
                elem->printOperation();
            }
            std::cout << "End Serial operation \n\n";
        }
    }

protected:
    std::vector<std::shared_ptr<Operation>> operations_;
    bool isParallel_;
    int linenumber_;
}; // class Operations


// This class encapsulates the subcircuit with the number of iterations and also the statements contained in it.
class SubCircuit {
public:
    SubCircuit() {}

    SubCircuit(const char* name, const int subcircuit_number, const int linenumber)
    : name_(std::string(name))
    , number_iterations_(1)  // By default, we run the subcircuit at least once
    , subcircuit_number_(subcircuit_number)
    , linenumber_(linenumber) {}

    int numberIterations() const {
        return number_iterations_;
    }

    void numberIterations(int iterations) {
        number_iterations_ = iterations;
    }

    int getLineNumber() const {
        return linenumber_;
    }

    size_t rankSubCircuit() const {
        return subcircuit_number_;
    }

    const std::string& nameSubCircuit() const {
        return name_;
    }

    void addOperationsCluster(std::shared_ptr<OperationsCluster> opclus) {
        operations_cluster_.push_back(std::move(opclus));
    }

    std::shared_ptr<OperationsCluster> lastOperationsCluster() {
        return operations_cluster_.back();
    }

    const std::vector<std::shared_ptr<OperationsCluster>>& getOperationsCluster() const {
        return operations_cluster_;
    }

    void printMembers() const {
        std::cout << "Subcircuit Name = " << nameSubCircuit() <<  " , Rank = " << rankSubCircuit() << "\n";
        std::cout << nameSubCircuit() << " has " << numberIterations() << " iterations." << "\n";
        std::cout << "Contains these operations clusters:" << "\n";
        for (auto elem : getOperationsCluster()) {
            elem->printOperations();
        }
        std::cout << "End of subcircuit " << nameSubCircuit() << "\n\n";
    }

protected:
    std::string name_; // This member is the name of the subcircuit
    int number_iterations_; // This member is the number of iterations the subcircuit is supposed to run
    size_t subcircuit_number_; // This member provides the order of the subcircuits when it is found in the qasm file
    int linenumber_;
    std::vector<std::shared_ptr<OperationsCluster>> operations_cluster_;
}; //class SubCircuit


// This class groups all the subcircuits found in the cqasm file
class SubCircuits {
public:
    SubCircuits() {
        auto default_circuit{ std::make_shared<SubCircuit>("default", 0, 1) };
        subcircuits_.push_back(std::move(default_circuit));
    }

    void addSubCircuit(std::shared_ptr<SubCircuit> subcircuit) {
        subcircuits_.push_back(std::move(subcircuit));
    }

    size_t numberOfSubCircuits() const {
        return subcircuits_.size();
    }

    std::shared_ptr<SubCircuit> lastSubCircuit() {
        return subcircuits_.back();
    }

    const std::vector<std::shared_ptr<SubCircuit>>& getAllSubCircuits() const {
        return subcircuits_;
    }

    void clearSubCircuits() {
        subcircuits_.clear();
    }

protected:
    std::vector<std::shared_ptr<SubCircuit>> subcircuits_;
}; //class SubCircuits


// This class is supposed to provide an encapsulation of all the objects within the qasm file
class QasmRepresentation {
public:
    QasmRepresentation() {
        std::vector<double> default_param(1, 0.);
        setErrorModel("None", default_param);
    }

    void qubitRegister(int participating_number) {
        qubit_register_ = participating_number;
    }

    int numQubits() {
        return qubit_register_;
    }

    double versionNumber() const {
        return version_number_;
    }

    void versionNumber(double version) {
        version_number_ = version;
    }

    SubCircuits& getSubCircuits() {
        return subcircuits_;
    }

    void addMappings(std::string name_key, NumericalIdentifiers indices, bool isQubit) {
        // Make sure they are all lowercase
        std::transform(name_key.begin(), name_key.end(), name_key.begin(), ::tolower);
        // Make pair between the indices and whether or not it is a qubit
        std::pair<NumericalIdentifiers, bool> map_value(indices, isQubit);
        mappings_[name_key] = map_value;
    }

    const NumericalIdentifiers& getMappedIndices(std::string name_key, bool isQubit, int linenumber) const {
        // Make sure they are all lowercase
        std::transform(name_key.begin(), name_key.end(), name_key.begin(), ::tolower);

        if (mappings_.find(name_key) != mappings_.end() &&
            mappings_.find(name_key)->second.second == isQubit) {
            return mappings_.find(name_key)->second.first;
        } else {
            throw std::runtime_error(std::string("Could not get wanted mapping ") +
                                     name_key + ": Line " + std::to_string(linenumber));
        }
    }

    void setErrorModel(std::string error_model_type, std::vector<double> error_model_num_params) {
        error_model_.first = error_model_type;
        error_model_.second = error_model_num_params;
    }

    const std::string getErrorModelType() const {
        return error_model_.first;
    }

    std::vector<double> getErrorModelParameters() const {
        return error_model_.second;
    }

    // This is just for debugging purposes
    void printMappings() const {
        for (auto elem : mappings_) {
            std::cout << elem.first << ": ";
            elem.second.first.printMembers(); std::cout << elem.second.second << "\n";
        }
        printErrorModel();
    }

    // This is just for debugging purposes
    void printErrorModel() const {
        std::cout << "Current error model: " << error_model_.first << "\nError Probability = " ;
        for (auto i : getErrorModelParameters()) {
            std::cout << i << "\n";
        }
    }

protected:
    SubCircuits subcircuits_;
    int qubit_register_;
    double version_number_;
    std::map<std::string , std::pair<NumericalIdentifiers, bool>> mappings_;
    std::pair<std::string, std::vector<double>> error_model_;
}; // class QasmRepresentation

} //namespace compiler

#endif
