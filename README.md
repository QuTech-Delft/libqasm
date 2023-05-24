
# libQASM: Library to parse cQASM v1.0 files

[![CI](https://github.com/QuTech-Delft/libqasm/workflows/Test/badge.svg)](https://github.com/qutech-delft/libqasm/actions)
[![PyPI](https://badgen.net/pypi/v/libqasm)](https://pypi.org/project/libqasm/)
[![Anaconda](https://anaconda.org/qutech/libqasm/badges/version.svg)](https://anaconda.org/qutech/libqasm/)
[![API docs](https://readthedocs.org/projects/libqasm/badge/?version=latest)](https://libqasm.readthedocs.io/en/latest/)

## Dependencies
* cmake (>= 3.12)
* gcc and g++ capable of C++23 standard
* Python3 (Tested on Python v3.6)
* conan: 2.0
* Swig
    * Linux: 3.0.12
    * Windows: 4.0.0

## Installation

### Using conan

`conan` is a package manager that is very convenient for managing dependencies. It is installed via `pip`.

```
pip install conan
```

The installation of `libqasm` dependencies, as well as the compilation, can be done in one go.<br/>
Notice the command below is building `libqasm` in Debug mode, and passing the build options `build_tests` and `compat`.

```
$ projects> git clone https://github.com/QuTech-Delft/libqasm.git
$ projects/libqasm> git checkout v3
$ projects/libqasm> conan build . -s:h libqasm/*:build_type=Debug -o libqasm/*:build_tests=True -o libqasm/*:compat=True -b missing
```

### Using CMake

#### Windows environment using MSVC (Visual C++)
libqasm should work with the environment described for [building OpenQL from sources on Windows](https://openql.readthedocs.io/en/latest/installation.html#notes-for-windows-users). In case that link breaks, here's a summary.

Download and install the following tools. When prompted whether you want to add them to your PATH, do so.

 - [git](https://gitforwindows.org/)
 - MSVC 2015 with update 3 or above
 - [Python 3.5+](https://www.python.org/downloads/) (make sure to get a 64-bit version!)
 - [cmake 3.15.3](https://github.com/Kitware/CMake/releases/download/v3.15.3/cmake-3.15.3-win64-x64.msi)

Download an extract the following tools, and then add the directories containing the executables to your PATH manually:

 - [win_flex_bison 2.5.20](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-2.5.20.zip/download)
 - [swigwin 4.0.0](https://sourceforge.net/projects/swig/files/swigwin/swigwin-4.0.0/swigwin-4.0.0.zip/download)

You should also find out where and how Python installed, and link its `include` directory and the `pythonXX.dll` file using the `PYTHON_INCLUDE` and `PYTHON_LIB` environment variables. For me, these are:

 - `PYTHON_INCLUDE` -> `C:\Users\IEUser\AppData\Local\Programs\Python\Python38\include`
 - `PYTHON_LIB` -> `C:\Users\IEUser\AppData\Local\Programs\Python\Python38\python38.dll`

Next, in an *elevated* Powershell (that is, "run as administrator"), run:

```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned
Install-Module -AllowClobber -Name Pscx -RequiredVersion 3.2.2
```

Finally, whenever you open a Powershell to build, first run *ONE* of the following, depending on your MSVC version (you may need to modify version numbers as well...):

```
Invoke-BatchFile "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
Invoke-BatchFile "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
```

## Organization
* The original paper and BNF specification for the cQASM v1.0 are located in the **./doc** directory. Please note however that the language has changed to some degree since then, and the qasm.bnf has never fully corresponded with the flex/bison grammar. New, up-to-date documentation is a work in progress.
* There are currently two C++ API versions contained in this repository, a new API still being worked on and the original soon-to-be deprecated API maintained for backward compatibility purposes. The new API is fully self-contained in the **./src/cqasm** directory. The older API is where it used to be, with its sources residing in **./src/library** and its tests in **./src/tests**.

## Installing/using the library

### From Python
Install from the project root directory as follows:

```
python -m pip install .
```

or if you'd rather use conda:

```
conda build python/conda
conda install libqasm --use-local
```

You can test if it works using

```
python -m unittest discover -s src/tests -v
```

or, with `pytest` if you prefer,

```
python -m pytest
```

Now you should be able to `import libQasm` to use the bindings for the original API. The new API doesn't have Python bindings yet.

### From C++ using CMake, without installation
The easiest way to use libqasm in a CMake project is to clone this repository into your repository as a submodule, and then use

```
add_subdirectory(libqasm) # or wherever your submodule is
target_link_libraries(<your target> cqasm)
```

The `target_link_libraries()` directive should automatically add the public include directories for libqasm's new API to your target's include path. You need to do this for each target.

The original API headers are *not* included by default. To enable those, add `option(LIBQASM_COMPAT "" ON)` before the `add_subdirectory()` directive.

### From C++ by manually installing (only tested on Linux)
The `CMakeLists.txt` file in the root directory includes install targets:

```
mkdir cbuild
cd cbuild
cmake .. <directives, see below>
make -j <Number of processors>
make install
```

You may want to add one or more directives to the `cmake` command:

 - `-DLIBQASM_COMPAT=yes`: enables installation of the headers for the original API, on top of the ones for the new API.
 - `-DBUILD_SHARED_LIBS=yes`: builds a shared object library instead of a static library, if applicable.
 - `-DCMAKE_INSTALL_PREFIX=<directory>`: specifies the directory that the library will be installed into, in case you want to override whatever the default is for your OS.

## Testing the library

### Linux and MacOS
```
git submodule update --init --recursive
mkdir cbuild
cd cbuild
cmake .. -DLIBQASM_BUILD_TESTS=yes -DLIBQASM_COMPAT=yes
make -j <Number of processors>
make test
```

### Windows + MinGW
Same as the above, except we'll use the MinGW toolchain. You should first get [mingw-w64](https://sourceforge.net/projects/mingw-w64/) and install it.
```
git submodule update --init --recursive
mkdir cbuild
cd cbuild
cmake -G "MinGW Makefiles" .. -DLIBQASM_BUILD_TESTS=yes -DLIBQASM_COMPAT=yes
mingw32-make.exe
mingw32-make.exe test
```

This will output `_libQasm.pyd`, `liblexgram.dll` and a static library `liblexgramstatic.a`

### Windows + MSVC
Continuing from the Windows environment setup instructions (don't forget to run `vcvarsall.bat` first):

```
git submodule update --init --recursive
mkdir cbuild
cd cbuild
cmake .. -DLIBQASM_BUILD_TESTS=yes -DLIBQASM_COMPAT=yes
cmake --build .
cmake --build . --target RUN_TESTS
```

### Docker
This tests the library in a container with the bare minimum requirements for libqasm.

```
docker build .
```

**Note for Windows users:** The above might fail on Windows to the autocrlf transformation that git does. If you are having trouble with this just create new clone of this repository using:

```
git clone --config core.autocrlf=input git@github.com:QuTech-Delft/libqasm.git
```

