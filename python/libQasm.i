/* libQasm_swig.i */
%module libQasm

%begin %{
#ifdef _MSC_VER
#define SWIG_PYTHON_INTERPRETER_NO_DEBUG
#endif
%}

%{
#include "v10/qasm_ast.hpp"
#include "v10/libQasm.hpp"
#include "cqasm-py.hpp"
%}

%include "exception.i"
%include "std_except.i"
%include "std_iostream.i"
%include "std_map.i"
%include "std_shared_ptr.i"
%include "std_string.i"
%include "std_vector.i"

%shared_ptr(compiler::Operation)
%shared_ptr(compiler::OperationsCluster)
%shared_ptr(compiler::SubCircuit)

namespace std {
    %template(vectori) vector<int>;
    %template(vectorui) vector<size_t>;
    %template(vectorf) vector<float>;
    %template(vectord) vector<double>;
    %template(vectors) vector<string>;
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

%include "v10/qasm_ast.hpp"
%include "v10/libQasm.hpp"
%include "cqasm-py.hpp"

namespace std {
    %template(subcircuit_vector) vector<shared_ptr<compiler::SubCircuit>>;
    %template(operationscluster_vector) vector<shared_ptr<compiler::OperationsCluster>>;
    %template(operation_vector) vector<shared_ptr<compiler::Operation>>;
    %template(twoqubitgatequbits_pair) pair<compiler::Qubits, compiler::Qubits>;
    %template(toffoliqubits_pair) pair<compiler::Qubits, pair<compiler::Qubits, compiler::Qubits> >;
}
