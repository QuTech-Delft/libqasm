#ifndef QASM_AST_REP
#define QASM_AST_REP


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>

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

    class Operations
    {
    };

    template<class QuBitType>
    class MapOperation : Operations
    {
        public:
            MapOperation() = default;
    };

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

            const int numberIterations() const
            {
                return number_iterations_;
            }

            void numberIterations(int iterations)
            {
                number_iterations_ = iterations;
            }

            void printMembers() const
            {
                std::cout << "Subcircuit Name = " << name_ <<  " , Rank = " << subcircuit_number_ << std::endl;
                std::cout << name_ << " has " << number_iterations_ << " iterations." << std::endl;
            }

        protected:
            int number_iterations_; // This member is the number of iterations the subcircuit is supposed to run
            std::string name_; // This member is the name of the subcircuit
            size_t subcircuit_number_; // This member provides the order of the subcircuits when it is found in the qasm file
            std::vector<Operations*> operations_;
    }; //class SubCircuit

    class SubCircuits
    // This class groups all the subcircuits found in the cqasm file
    {
        public:
            SubCircuits() = default;

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
                subcircuits_.back();
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

        protected:
            SubCircuits subcircuits_;
            int qubit_register_;
    }; // class QasmRepresentation
} //namespace compiler


#endif