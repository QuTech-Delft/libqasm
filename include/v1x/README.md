# New API

This folder contains the new libqasm API.

This API is a complete overhaul of the previous, located in `include/v10`.
The reason for the overhaul was to improve code quality and
to future-proof the API for things like (high-level) classical logic in cQASM files.
Hopefully, no significant breaking changes will have to be made as such things are added,
reducing maintenance effort in the future.

There are no Python bindings for the new API *yet*.
There is also no proper (generated) documentation yet,
but I tried to do a decent job providing a walkthrough for the API in test/v1x/tutorial.cpp.

For now, especially since there are no Python bindings yet,
the old API is still available through a compatibility layer on top of the new one.
The files in there and the build system should be 100% binary and build-system compatible
with the state of the repository as it was before the new API was introduced,
to ease the probable eventual transition.

## Build instructions

There are several ways in which you can link against the new API of libqasm:

 - **System installation**<br/>
   Have your users install libqasm into their system and link against that.
   In this case they need to either compile and install from source here, or be provided with a prebuilt package.
   You should then be able to include the headers using `#include <cqasm.hpp>` and link using `-lcqasm`.
   Depending on CMake's `BUILD_SHARED_LIBS` variable, shared or static linking will be used.<br/><br/>

- **FetchContent**<br/>
  Retrieve the library via FetchContent, by adding the lines below to the CMakeLists.txt.

   ```
   include(FetchContent)
   FetchContent_Declare(cqasm
      GIT_REPOSITORY https://github.com/QuTech-Delft/libqasm.git
      GIT_TAG "<commit ID>"
   )
   FetchContent_MakeAvailable(cqasm)
   ```

   You can then link your targets against libqasm using `target_link_libraries(<your-target> <PRIVATE|PUBLIC> cqasm`.
   Again, depending on CMake's `BUILD_SHARED_LIBS` variable, shared or static linking will be used.
   You can also link the objects directly into your target;
   in this case `$<TARGET_OBJECTS:cqasm-lib-obj>` expands to the objects (add it to your target as if it's a source file).
   You will need the following two lines in addition to copy over the public include directories and
   any dependencies of libqasm (currently there are none, this is for the sake of future-proofing) into your target:
   
   ```cmake
   target_include_directories(<your-target> <PRIVATE|PUBLIC> $<TARGET_PROPERTY:cqasm-lib-obj,INTERFACE_INCLUDE_DIRECTORIES>)
   target_link_libraries(<your-target> <PRIVATE|PUBLIC> $<TARGET_PROPERTY:cqasm-lib-obj,LINK_LIBRARIES>)
   ```

 - **git submodule**<br/>
   Include a specific version of libqasm into your project as a submodule, and
   include this directory into your `CMakeLists.txt` using `add_subdirectory`.
   The further linkage steps are the same as the ones described above for FetchContent.<br/><br/> 

 - **Conan**<br/>
   There is an ongoing task to create a libqasm Conan package.
   Once available, one could simply link against the library by:
   1. Adding a `self.requires("libqasm/<version>")` to the `build_requirements` function of the conanfile.py, and
   2. Adding a `target_link_libraries(<your-target>  <PRIVATE|PUBLIC> ${CONAN_LIBS})` to the CMakeLists.txt file.

### System installation via CMake

When configuring the project using the `cmake` command,
you may want to add one or more of the following command line arguments:

 - `-DBUILD_SHARED_LIBS=yes`: builds a shared object library instead of a static library, if applicable.
 - `-DLIBQASM_BUILD_TESTS=yes`: builds the tests in addition to the library itself.
   Use `cmake --build . --target RUN_TESTS` to run the tests (in a cross-platform way) after building.
 - `-DCMAKE_INSTALL_PREFIX=<directory>`: specifies the directory that the library will be installed into when you run
   `cmake --build . --target install`, in case you want to override whatever the default is for your OS.

To be complete, here's an example for how to build a shared library, test it, and install it on Linux.

```
mkdir cbuild
cd cbuild
cmake .. -DBUILD_SHARED_LIBS=yes -DLIBQASM_BUILD_TESTS=yes
cmake --build .
make -j
make test
sudo make install
```

### Conan build

You can also use Conan for the build process. The command line arguments, in this case, would be:

- `-o libqasm/*:shared=True`.
- `-o libqasm/*:build_tests=True`.

For example:

```
conan build . -s:h compiler.cppstd=20 -o libqasm/*:shared=True -o libqasm/*:build_tests=True -b missing
```

Then, you would still need the `sudo make install` to do the system installation. 

## File list

 - `src/v1`: contains the C++ source files for the library.
   May also contain private header files, but doesn't at the time of writing.
 - `include/v1`: contains the public header files for the library.
 - `test/v1`: contains the test suite for the library.
 - `src/func-gen`: contains a simple code generator for the repetitive logic of
   generating the operators and constant propagation functions exposed to
   cQASM.
 - `CMakeLists.txt`, `cmake`: build system entry point and support modules.
