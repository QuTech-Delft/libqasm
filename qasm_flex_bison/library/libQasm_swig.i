/* libQasm_swig.i */
%module libQasm

%include "std_vector.i"
%include "std_string.i"
%include "std_map.i"
%include "std_except.i"
%include "std_iostream.i"

namespace std {
   %template(vectori) vector<int>;
   %template(vectorui) vector<size_t>;
   %template(vectorf) vector<float>;
   %template(vectord) vector<double>;
};

%{
#include "qasm_ast.hpp"
#include "libQasm.hpp"
%}

%include "qasm_ast.hpp"
%include "libQasm.hpp"

namespace std {
    %template(subcircuit_vector) vector<compiler::SubCircuit>;
    %template(operationscluster_vector) vector<compiler::OperationsCluster*>;
    %template(operation_vector) vector<compiler::Operation*>;
}
