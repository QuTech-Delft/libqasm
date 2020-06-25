
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
    * Linux: 3.0.12
    * Windows: 4.0.0

## Organization
* The accompanying paper and BNF specification for the cQASM v1.0 are located in the **./doc** directory
* The library itself is located in the **./src** directory

## Installation

### As a standalone C++ library
In order to install (assuming the present working directory is at **./src**):
```
git submodule update --init --recursive
cd src
mkdir cbuild
cd cbuild
cmake ../library
make or make -j \<Number of processors\>
make test or make test -j \<Number of processors\>
```

### As a standalone C++ library (Windows + MinGW)
Same as the above, except we'll use the MinGW toolchain. You should first get [mingw-w64](https://sourceforge.net/projects/mingw-w64/) and install it.
```
git submodule update --init --recursive
cd src
mkdir cbuild
cd cbuild
cmake -G "MinGW Makefiles" ..\library\
mingw32-make.exe
mingw32-make.exe test
```

This will output `_libQasm.pyd`, `liblexgram.dll` and a static library `liblexgramstatic.a`

### As a standalone C++ library (Windows + MSVC)
This should work with the environment described for [building OpenQL from sources on Windows](https://openql.readthedocs.io/en/latest/installation.html#notes-for-windows-users). Summarized, you need the following tools installed and registered into your user or system PATH environment variable:

 - [git](https://gitforwindows.org/)
 - MSVC 2015 with update 3 or above
 - [Python 3.5+](https://www.python.org/downloads/)
 - [win_flex_bison 2.5.20](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-2.5.20.zip/download)
 - [cmake 3.15.3](https://github.com/Kitware/CMake/releases/download/v3.15.3/cmake-3.15.3-win64-x64.msi)
 - [swigwin 4.0.0](https://sourceforge.net/projects/swig/files/swigwin/swigwin-4.0.0/swigwin-4.0.0.zip/download)

You can then run the following in powershell:

```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned
Install-Module -AllowClobber -Name Pscx -RequiredVersion 3.2.2
Invoke-BatchFile "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
# or equivalent of the above, depending on your version and install location of MSVC.
git submodule update --init --recursive
cd src
mkdir cbuild
cd cbuild
cmake ..\library\
cmake --build .
cmake --build . --target RUN_TESTS
```

This was last tested using [one of these VMs](https://developer.microsoft.com/en-us/microsoft-edge/tools/vms/) on 2020-06-25.

### As a python3 module
Install from the project root directory:
```
python -m pip install .
python -m unittest discover -s src/tests -v
```

If you ran setup.py manually or built with CMake manually before, you may need to run cleanme.py due to CMake cache conflicts for some reason.

### Cleaning up the src directory (not Windows)
In order to clean up the entire source directory when a user installs using the setup.py python script:
```
make clean
```
