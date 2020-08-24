# Python bindings sources

This directory contains everything to do with generating the Python bindings,
aside from the `setup.py` file living in the root directory.

File/directory list:

 - `libQasm.i`: SWIG directives for the bindings.

 - `libQasm`: represents the Python module excluding the extension. Any Python
   files in here will be included in the Python install.

 - `conda`: contains the conda recipe for libqasm, in case you're using that
   rather than pip to manage your Python packages. It mostly just defers to
   `setup.py`, but will help you set up the build dependencies regardless of
   your platform.

 - `CMakeLists.txt`: SWIG and the Python extension build are handled by CMake,
   so it can make everything's linked together properly. Don't use this file
   directly; it's intended to be added by the root `CMakeLists.txt`, and only
   when the build is orchestrated by `setup.py`.

 - `python_lib.py`: helper script for CMake to find the Python library to link
   against, because FindPYTHON often picks the wrong one, especially in venvs.

 - `compat`: helper scripts for CMake to make old CMake versions work.
