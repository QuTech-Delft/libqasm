# libQASM: Library to parse cQASM v1.0 files

## Dependencies
* Flex (> 2.6)
* Bison (> 3.0)
* cmake (> 2.8)
* gcc and g++ capable of C++11 standard
* doctest (As a git submodule)

### If python module is required
* Python3 (Tested on Python v3.6)
* Swig

## Organization
* The accompanying paper and BNF specification for the cQASM v1.0 are located in the **./doc** directory
* The library itself is located in the **./qasm_flex_bison** directory

## Installation

### As a standalone c++ library
As the library uses doctest as part of the testing procedure. In order to install (assuming the present working directory is at **./qasm_flex_bison**):
```
git submodule update --init --recursive
mkdir cbuild
cd cbuild
cmake ../library
make or make -j \<Number of processors\>
make test or make test -j \<Number of processors\>
```

### As a python3 module
In order to install (assuming the present working directory is at **./qasm_flex_bison**):
```
git submodule update --init --recursive
python3 -m venv env
. ./env/bin/activate
python setup.py install --user
python -m unittest discover -s tests/python -v
```

### Cleaning up the **qasm\_flex\_bison** directory
In order to clean up the entire source directory when a user installs using the setup.py python script:
```
make clean
```
