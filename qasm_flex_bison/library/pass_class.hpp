#ifndef QASM_REP
#define QASM_REP


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

namespace compiler
{
    // class QasmRepresentation 
    // // This class provides the cQASM representation of the parsed source
    // {
    //     public:
    //         QasmRepresentation();

    //         virtual ~QasmRepresentation();

    //     protected:
    //         std::vector<QasmLine> qasm_representation_;
    // }; // class QasmRepresentation

    class NumericalIdentifiers
    // This class takes a list of identifiers used by both bits and qubits to internal int vectors
    {
        public:
            NumericalIdentifiers():
            {
                
            }



        protected:
            std::vector<size_t> indices_;
    };

    // class Qubits
    // {
    //     public:
    //         Qubits(std::vector<int> indices)
    //         {

    //         }



    //     protected:
    //         std::vector<int> qubits_; //

    // }; // class Qubits

} //namespace compiler


#endif