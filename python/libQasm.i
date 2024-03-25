/* libQasm_swig.i */
%module libQasm

%begin %{
#ifdef _MSC_VER
#define SWIG_PYTHON_INTERPRETER_NO_DEBUG
#endif
%}

%{
#include "v3x/cqasm-py.hpp"
%}

%include "exception.i"
%include "std_except.i"
%include "std_iostream.i"
%include "std_string.i"
%include "std_vector.i"

namespace std {
    %template(vectori) vector<int>;
    %template(vectorui) vector<size_t>;
    %template(vectorf) vector<float>;
    %template(vectord) vector<double>;
    %template(vectors) vector<string>;
}

%exception {
    try {
        $action
        if (PyErr_Occurred()) SWIG_fail;
    }
    SWIG_CATCH_STDEXCEPT
    catch (...) {
        SWIG_exception(SWIG_RuntimeError, "Unknown exception");
    }
}

%include "v3x/cqasm-py.hpp"
