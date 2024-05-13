# Python bindings sources

This directory contains everything to do with generating the Python bindings, aside from the `setup.py` file living in the root directory.

File/directory list:

 - `CMakeLists.txt`: SWIG and the Python extension build are handled by CMake, so it can link everything together properly.
   Don't use this file directly. It is intended to be added by the root `CMakeLists.txt`, and only when the build is orchestrated by `setup.py`.

 - `libqasm`: represents the Python module excluding the extension. Any Python files in here will be included in the Python install.

 - `libqasm.i`: SWIG directives for the bindings.

 - `python_lib.py`: helper script for CMake to find the Python library to link against, because `FindPYTHON` often picks the wrong one, especially in virtual environments.