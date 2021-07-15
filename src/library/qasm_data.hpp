#ifndef QASM_DATA_HPP
#define QASM_DATA_HPP

// NOTE: this file is probably no longer needed as it was used by the old
// parser/lexer, but technically it was a public header file.

#include "qasm_ast.hpp"

typedef struct {
    compiler::Bits bits_identified;
    compiler::NumericalIdentifiers buffer_indices;
    compiler::SubCircuits subcircuits_object;
    compiler::QasmRepresentation qasm_representation;
    std::string buffer_string;
} qasm_data;

#endif // QASM_DATA_HPP
