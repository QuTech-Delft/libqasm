Foo

# libQASM: Library to parse cQASM v1.0 files

## Dependencies
* Flex (> 2.6)
* Bison (> 3.0)
* cmake (> 2.8)
* gcc and g++ capable of C++11 standard
* doctest (As a git submodule)

### If python module is required
* Python3 (Tested on Python v3.5)
* Swig

## Organization
* The accompanying paper and BNF specification for the cQASM v1.0 are located in the **./doc** directory
* The library itself is located in the **./qasm_flex_bison** directory

## Installation

### As a standalone library
As the library uses doctest as part of the testing procedure. In order to install (assuming the present working directory is at **./qasm_flex_bison**):
1. git submodule update --init --recursive
2. mkdir **cbuild**
3. cd **cbuild**
4. cmake **../library**
5. make or make -j \<Number of processors\>
6. make test or make test -j \<Number of processors\>

### As a python3 module
1. git submodule update --init --recursive
2. python3 setup.py --install --user

### Cleaning up the **qasm\_flex\_bison** directory
In order to clean up the entire source directory when a user installs using the setup.py python script:
1. make clean
