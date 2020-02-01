/* libQasm_swig.i */
%module libQasm

%{
#include "qasm_ast.hpp"
#include "libQasm.hpp"
%}

%include "std_vector.i"
%include "std_string.i"
%include "std_map.i"
%include "std_except.i"
%include "std_iostream.i"
%include "exception.i"

namespace std {
    %template(vectori) vector<int>;
    %template(vectorui) vector<size_t>;
    %template(vectorf) vector<float>;
    %template(vectord) vector<double>;
}

%exception {
    try
    {
        $action
        if (PyErr_Occurred()) SWIG_fail;
    }
    SWIG_CATCH_STDEXCEPT
    catch (...)
    {
        SWIG_exception(SWIG_RuntimeError, "Unknown exception");
    }
}

%include "qasm_ast.hpp"
%include "libQasm.hpp"

namespace std {
    %template(subcircuit_vector) vector<compiler::SubCircuit>;
    %template(operationscluster_vector) vector<compiler::OperationsCluster*>;
    %template(operation_vector) vector<compiler::Operation*>;
    %template(twoqubitgatequbits_pair) pair<compiler::Qubits, compiler::Qubits>;
    %template(toffoliqubits_pair) pair<compiler::Qubits, pair<compiler::Qubits, compiler::Qubits> >;
}
