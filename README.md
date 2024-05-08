# libQASM: Library to parse cQASM v3.0 files

[![CI](https://github.com/QuTech-Delft/libqasm/workflows/Test/badge.svg)](https://github.com/qutech-delft/libqasm/actions)
[![PyPI](https://badgen.net/pypi/v/libqasm)](https://pypi.org/project/libqasm/)

## File organization

For development, see:

- `include`: public headers.
- `src`: source files.
- `test`: test files.
- `python`: SWIG interface.
- `res`: resource files, for testing.

For build process, continuous integration, and documentation:

- `conan`: Conan profiles.
- `emscripten`: bindings and test for the emscripten binaries.
- `scripts`: helper scripts used during the build process.
- `.github`: GitHub Actions configuration files.
- `doc`: documentation.

Build outputs may go into:

- `build/<build type>`: the C++ library output files generated by Conan.
- `pybuild`: the Python library output files generated by `setup.py`.

## Dependencies

* C++ compiler with C++20 support (gcc 11, clang 14, msvc 17)
* `CMake` >= 3.12
* `git`
* `Python` 3.x plus `pip`, with the following package:
  * `conan` >= 2.0
  
### ARM specific dependencies

We are having problems when using the `zulu-opendjk` Conan package on an ARMv8 architecture.
`zulu-openjdk` provides the Java JRE required by the ANTLR generator.
So, for the time being, we are installing Java manually for this platform.

* `Java JRE` >= 11

## Build

This version of `libqasm` can only be compiled via the Conan package manager.
You'll need to create a default profile before using it for the first time.

The installation of dependencies, as well as the compilation, can be done in one go.

```
git clone https://github.com/QuTech-Delft/libqasm.git
cd libqasm
conan profile detect
conan build . -pr:a=conan/profiles/tests-debug -b missing
```

Notice:
- the `conan profile` command only has to be run only once, and not before every build.
- the `conan build` command is building `libqasm` in Debug mode with tests using the `tests-debug` profile.
- the `-b missing` parameter asks `conan` to build packages from sources
in case it cannot find the binary packages for the current configuration (platform, OS, compiler, build type...). 

### Build profiles

A group of predefined profiles is provided under the `conan/profiles` folder.<br/>
They follow the `[tests](-build_type)(-compiler)(-os)(-arch)[-shared]` naming convention:
  - `tests`: if tests are being built.
  - `build_type`: can be `debug` or `release`.
  - `compiler`: `apple-clang`, `clang`, `gcc`, `msvc`.
  - `os`: `emscripten`, `linux`, `macos`, `windows`.
  - `arch`: `arm64`, `wasm`, `x64`.
  - `shared`: if the library is being built in shared mode.

All the profiles set the C++ standard to 20.<br/>
All the `tests`, except for `linux-x64` profiles, enable Address Sanitizer.

### Build options

Profiles are a shorthand for command line options. The command above could be written as well as: 

```
conan build . -s:a compiler.cppstd=20 -s:a libqasm/*:build_type=Debug -o libqasm/*:build_tests=True -o libqasm/*:asan_enabled=True -b missing
```

These are the list of options that could be specified whether in a profile or in the command line:

- `libqasm/*:asan_enabled={True,False}`: enables Address Sanitizer.
- `libqasm/*:build_tests={True,False}`: builds tests or not.
- `libqasm/*:build_type={Debug,Release}`: builds in debug or release mode.
- `libqasm/*:shared={True,False}`: builds a shared object library instead of a static library, if applicable.

## Install

### From Python

Install from the project root directory as follows:

```
python3 -m pip install --verbose .
```

You can test if it works by running:

```
python3 -m pytest
```

### From C++

The `CMakeLists.txt` file in the root directory includes install targets:

```
conan create --version 0.6.5 . -pr:a=tests-debug -b missing
```

You can test if it works by doing:

```
cd test/Debug
ctest -C Debug --output-on-failure
```

## Use from another project

### From Python

After installation, you should be able to use the bindings for the original API by just `import libQasm`.<br/>
The new API doesn't have Python bindings yet.

### From C++

The easiest way to use `libqasm` in a CMake project is to request the Conan package from a `conanfile.py`.

```
def build_requirements(self):
    self.tool_requires("libqasm/0.6.5")
```

And then link against the `libqasm` library from a `CMakeLists.txt`: 

```
target_link_libraries(<your target> PUBLIC libqasm::libqasm)
```

Another option is to fetch the library and then link against it from the `CMakeLists.txt`. 

```
include(FetchContent)
FetchContent_Declare(libqasm
    GIT_REPOSITORY https://github.com/QuTech-Delft/libqasm.git
    GIT_TAG "<a given libqasm git tag>"
)
FetchContent_MakeAvailable(libqasm)
target_include_directories(<your target> SYSTEM PRIVATE "${libqasm_SOURCE_DIR}/include")
target_link_libraries(<your target> PUBLIC cqasm)
```

Note that, when using `FetchContent`, the library name is `cqasm`.

Note also that the following dependency is required for `libqasm` to build:

* `Java JRE` >= 11

## Docker

This tests the library in a container with the bare minimum requirements for `libqasm`.

```
docker build .
```

**Note for Windows users:** The above might fail on Windows to the `autocrlf` transformation that git does.
If you are having trouble with this just create new clone of this repository using:

```
git clone --config core.autocrlf=input git@github.com:QuTech-Delft/libqasm.git
```

## Emscripten

The generation of emscripten binaries has been tested as a cross-compilation from an ubuntu/x64 platform.

```
conan build . -pr=conan/profiles/release-clang-emscripten-wasm -pr:b=conan/profiles/release -b missing
```

The output of this build lives in `build/Release/emscripten`:
- `cqasm_emscripten.js`.
- `cqasm_emscripten.wasm`.

Note that `cqasm_emscripten.js` is an ES6 module. An example of how to use it would be:

```
cd build/Release/emscripten
mv cqasm_emscripten.js cqasm_emscripten.mjs
cd ../../../emscripten
deno run -A test_cqasm.ts
```