#pragma once
#ifndef QASM_AST_REP
#define QASM_AST_REP


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace compiler
{

    class NumericalIdentifiers
    // This class takes a list of identifiers used by both bits and qubits to internal int vectors
    {
        public:
            NumericalIdentifiers() = default;

            void addToVector (const int index)
            {
                indices_.push_back( static_cast<size_t> (index) );
            }

            void addToVector (const int index_min, const int index_max)
            {
                for (int index = index_min; index <= index_max; ++index)
                    indices_.push_back( static_cast<size_t> (index) );
            }

            const std::vector<size_t> & getIndices() const
            {
                return indices_;
            }

            void removeDuplicates()
            {
                std::sort( indices_.begin(), indices_.end() );
                indices_.erase( std::unique( indices_.begin(), indices_.end() ), indices_.end() );
            }

            void clear()
            {
                indices_.clear();
            }

            void printMembers() const
            {
                std::cout << "Indices: ";
                for (size_t elems : indices_)
                {
                    std::cout << elems << " ";
                }
                std::cout << std::endl;
            }

        protected:
            std::vector<size_t> indices_;
    }; // class NumericalIdentifiers

    class Qubits
    // This class encapsulates the participating qubits in the specified operation
    {
        public:

            Qubits() = default;
            Qubits(NumericalIdentifiers indices) : selected_qubits_ (indices) {}

            void setSelectedQubits(NumericalIdentifiers indices)
            {
                selected_qubits_ = indices;
            }

            const NumericalIdentifiers& getSelectedQubits() const
            {
                return selected_qubits_;
            }

            // Check for map also

        protected:
            NumericalIdentifiers selected_qubits_;
    };

    class Bits
    // This class encapsulates the participating bit in the specified operation
    {
        public:

            Bits() = default;
            Bits(NumericalIdentifiers indices) : selected_bits_ (indices) {}

            void setSelectedBits(NumericalIdentifiers indices)
            {
                selected_bits_ = indices;
            }

            const NumericalIdentifiers& getSelectedBits() const
            {
                return selected_bits_;
            }

            // Check for map also

        protected:
            NumericalIdentifiers selected_bits_;
    };

    class Operation
    {
        public:
            Operation(const std::string type, Qubits qubits_involved)
            : qubits_ (qubits_involved), 
              rotation_angle_ (std::numeric_limits<double>::max()), bit_controlled_(false)
            // This is the most common operation, the single qubit operation
            {
                type_ = toLowerCase(type);
            }

            Operation(const std::string type, Qubits qubits_involved, const double rotation_angle)
            : qubits_ (qubits_involved), 
              rotation_angle_ (rotation_angle), bit_controlled_(false)
            // Single qubit rotations
            {
                type_ = toLowerCase(type);
            }

            Operation(const std::string type, Qubits qubit_pair1, std::string axis1, Qubits qubit_pair2, std::string axis2)
            : rotation_angle_ (std::numeric_limits<double>::max()), bit_controlled_(false)
            // Measure parity operation
            {
                type_ = toLowerCase(type);
                measure_parity_qubits_ = std::pair<Qubits,Qubits> (qubit_pair1,qubit_pair2);
                measure_parity_axis_ = std::pair<std::string,std::string> (toLowerCase(axis1),toLowerCase(axis2));
            }

            Operation(const std::string type)
            : rotation_angle_ (std::numeric_limits<double>::max()), bit_controlled_(false)
            // Measure all operation
            {
                type_ = toLowerCase(type);
            }

            Operation(const std::string type, Qubits qubit_pair1, Qubits qubit_pair2)
            : rotation_angle_ (std::numeric_limits<double>::max()), bit_controlled_(false)
            // Two qubit gate operations
            {
                type_ = toLowerCase(type);
                two_qubit_pairs_ = std::pair<Qubits,Qubits> (qubit_pair1,qubit_pair2);
            }

            Operation (   
                          const std::string type, 
                          Qubits qubits_involved,
                          const double rotation_angle,
                          const bool bit_controlled
                      )
            : qubits_ (qubits_involved), 
              rotation_angle_ (rotation_angle), bit_controlled_(bit_controlled)
            // bit controlled operations
            {
                type_ = toLowerCase(type);
            }

            std::string getType() const
            {
                return type_;
            }

            const Qubits& getQubitsInvolved() const
            {
                return qubits_;
            }

            double getRotationAngle() const
            {
                return rotation_angle_;
            }

            const std::pair< std::pair<Qubits,Qubits>, std::pair<std::string,std::string> > getMeasureParityQubitsAndAxis() const
            {
                std::pair< std::pair<Qubits,Qubits>, std::pair<std::string,std::string> > pair_result (measure_parity_qubits_, measure_parity_axis_);
                return pair_result;
            }

            const std::pair <Qubits,Qubits>& getTwoQubitPairs() const
            {
                return two_qubit_pairs_;
            }

            bool isBitControlled() const
            {
                return bit_controlled_;
            }

            void setControlBits(Bits control_bits)
            {
                control_bits_ = control_bits;
                bit_controlled_ = true;
            }

            const Bits& getControlBits() const
            {
                return control_bits_;
            }

            void printOperation() const
            {
                if (isBitControlled()){
                    std::cout << "Bit controlled with bits: ";
                    getControlBits().getSelectedBits().printMembers();
                }
                std::cout << "Gate " << type_ << ": " << "with rotations = " << rotation_angle_ << " involving qubits ";
                if (type_ == "measure_parity")
                {
                    std::cout << std::endl;
                    auto measureParityProperties = getMeasureParityQubitsAndAxis();
                    measureParityProperties.first.first.getSelectedQubits().printMembers();
                    std::cout << "With axis " << measureParityProperties.second.first << std::endl;
                    measureParityProperties.first.second.getSelectedQubits().printMembers();
                    std::cout << "With axis " << measureParityProperties.second.second << std::endl;
                }
                else if (type_ == "cnot" | type_ == "cz" | type_ == "swap")
                {
                    std::cout << std::endl;
                    std::cout << "Qubit Pair 1: ";
                    getTwoQubitPairs().first.getSelectedQubits().printMembers();
                    std::cout << "Qubit Pair 2: "; 
                    getTwoQubitPairs().second.getSelectedQubits().printMembers();
                }
                else qubits_.getSelectedQubits().printMembers();
            }

        protected:

            std::string toLowerCase(const std::string& string_input)
            {
                std::string lower_case_input = string_input;
                std::transform(lower_case_input.begin(), lower_case_input.end(), lower_case_input.begin(), ::tolower);
                return lower_case_input;
            }

            std::string type_;
            Qubits qubits_;
            Bits control_bits_;
            bool bit_controlled_;
            double rotation_angle_;
            std::pair<Qubits,Qubits> measure_parity_qubits_;
            std::pair<std::string,std::string> measure_parity_axis_;
            std::pair<Qubits,Qubits> two_qubit_pairs_;
    }; // class Operation

    class SubCircuit
    // This class encapsulates the subcircuit with the number of iterations and also the statements contained in it.
    {
        public:
            SubCircuit(const char *name, const int subcircuit_number):
            name_ ( std::string(name) ),
            number_iterations_ ( 1 ), // By default, we run the subcircuit at least once
            subcircuit_number_ ( subcircuit_number )
            {
            }

            int numberIterations() const
            {
                return number_iterations_;
            }

            void numberIterations(int iterations)
            {
                number_iterations_ = iterations;
            }

            void addOperation(Operation *opline)
            {
                operations_.push_back(opline);
            }

            Operation* lastOperation()
            {
                return operations_.back();
            }

            void printMembers() const
            {
                std::cout << "Subcircuit Name = " << name_ <<  " , Rank = " << subcircuit_number_ << std::endl;
                std::cout << name_ << " has " << number_iterations_ << " iterations." << std::endl;
                std::cout << "Contains these operations:" << std::endl;
                for (auto elem : operations_)
                    elem->printOperation();
            }

        protected:
            std::string name_; // This member is the name of the subcircuit
            int number_iterations_; // This member is the number of iterations the subcircuit is supposed to run
            size_t subcircuit_number_; // This member provides the order of the subcircuits when it is found in the qasm file
            std::vector< Operation* > operations_;
            std::vector< Operation* > parallel_operations_;
    }; //class SubCircuit

    class SubCircuits
    // This class groups all the subcircuits found in the cqasm file
    {
        public:
            SubCircuits()
            {
                SubCircuit default_circuit("default", 0);
                subcircuits_.push_back ( default_circuit );
            }

            void addSubCircuit(SubCircuit subcircuit)
            {
                subcircuits_.push_back(subcircuit);
            }

            size_t numberOfSubCircuits() const
            {
                return subcircuits_.size();
            }

            SubCircuit& lastSubCircuit()
            {
                return subcircuits_.back();
            }

            const std::vector<SubCircuit>& getAllSubCircuits() const
            {
                return subcircuits_;
            }

        protected:
            std::vector<SubCircuit> subcircuits_;
    }; //class SubCircuits

    class QasmRepresentation
    // This class is supposed to provide an encapsulation of all the objects within the qasm file
    {
        public:
            QasmRepresentation() = default;

            void qubitRegister(int participating_number)
            {
                qubit_register_ = participating_number;
            }

            int numQubits()
            {
                return qubit_register_;
            }

            SubCircuits& getSubCircuits()
            {
                return subcircuits_;
            }

            void addMappings(std::string name_key, NumericalIdentifiers indices, bool isQubit)
            {
                // Make sure they are all lowercase
                std::transform(name_key.begin(), name_key.end(), name_key.begin(), ::tolower);
                // Make pair between the indices and whether or not it is a qubit
                std::pair<NumericalIdentifiers, bool> map_value (indices, isQubit);
                mappings_[name_key] = map_value;
            }

            const NumericalIdentifiers& getMappedIndices(std::string name_key, bool isQubit) const
            {
                // Make sure they are all lowercase
                std::transform(name_key.begin(), name_key.end(), name_key.begin(), ::tolower);
                if( mappings_.find(name_key)->second.second == isQubit)
                    return mappings_.find(name_key)->second.first;
                else
                    throw std::runtime_error(std::string("Could not get wanted mapping ") + name_key);
            }

            void printMappings() const
            // This is just for debugging purposes
            {
                for (auto elem : mappings_)
                {
                    std::cout << elem.first << ": ";
                    elem.second.first.printMembers(); std::cout << elem.second.second << std::endl;
                }
            }

        protected:
            SubCircuits subcircuits_;
            int qubit_register_;
            std::map<std::string , std::pair<NumericalIdentifiers,bool> > mappings_;
    }; // class QasmRepresentation
} //namespace compiler


#endif