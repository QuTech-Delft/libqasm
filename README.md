
# libQASM: Library to parse cQASM v1.0 files

## Dependencies
* Flex (> 2.6)
* Bison (> 3.0)
* cmake (> 2.8)
* gcc and g++ capable of C++11 standard
* MinGW (For Windows builds)
* doctest (As a git submodule)

### If python module is required
* Python3 (Tested on Python v3.6)
* Swig

## Organization
* The accompanying paper and BNF specification for the cQASM v1.0 are located in the **./doc** directory
* The library itself is located in the **./qasm_flex_bison** directory

## Installation

### As a standalone C++ library
As the library uses doctest as part of the testing procedure. In order to install (assuming the present working directory is at **./qasm_flex_bison**):
```
git submodule update --init --recursive
mkdir cbuild
cd cbuild
cmake ../library
make or make -j \<Number of processors\>
make test or make test -j \<Number of processors\>
```

### As a standalone C++ library (Windows)
Same as the above, except we'll use the MinGW toolchain. You shoud first get [mingw-w64](https://sourceforge.net/projects/mingw-w64/) and install it.
```
git submodule update --init --recursive
mkdir cbuild
cmake -G "MinGW Makefiles" ..\library\
mingw32-make.exe
mingw32-make.exe test
```

This will output `_libQasm.pyd` and `liblexgram.dll`

### As a python3 module
Install from the project root directory:
```
pip install .
python -m unittest discover -s src/tests/python -v
```

### Cleaning up the **qasm\_flex\_bison** directory
In order to clean up the entire source directory when a user installs using the setup.py python script:
```
make clean
```
