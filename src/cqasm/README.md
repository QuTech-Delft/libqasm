# New API

This folder contains the new libqasm API.

This API is a complete overhaul of the previous, located in the other
subdirectories of `src`. The reason for the overhaul was to improve code
quality and to future-proof the API for things like (high-level) classical
logic in cQASM files. Hopefully, no significant breaking changes will have
to be made as such things are added, reducing maintenance effort in the
future.

There are no Python bindings for the new API *yet*. These are however planned.
There is also no proper (generated) documentation yet, but I tried to do a
decent job providing a walkthrough for the API in tests/tutorial.cpp.

For now, especially since there are no Python bindings yet, the old API is
still available through a compatibility layer on top of the new one. The files
in there and the build system should be 100% binary and build-system compatible
with the state of the repository as it was before the new API was introduced,
to ease the probable eventual transition.

## Build instructions

There are two ways in which you can link against the new API of libqasm:

 - Have your users install libqasm into their system and link against that.
   In this case they need to either compile and install from source here, or
   be provided with a prebuilt package. You should then by able to include
   the headers using `#include <cqasm.hpp>` and link using `-lcqasm`. Depending
   on CMake's `BUILD_SHARED_LIBS` variable, shared or static linking will be
   used.

 - Include a specific version of libqasm into your project as a submodule, and
   include this directory into your `CMakeLists.txt` using `add_subdirectory`.
   You can then link your targets against libqasm using
   `target_link_libraries(<your-target> <PRIVATE|PUBLIC> cqasm`. Again,
   depending on CMake's `BUILD_SHARED_LIBS` variable, shared or static linking
   will be used. You can also link the objects directly into your target; in
   this case `$<TARGET_OBJECTS:cqasm_objlib>` expands to the objects (add it
   to your target as if it's a source file). You will need the following two
   lines in addition to copy over the public include directories and any
   dependencies of libqasm (currently there are none, this is for the sake of
   future-proofing) into your target:

   ```cmake
   target_include_directories(<your-target> <PRIVATE|PUBLIC> $<TARGET_PROPERTY:cqasm_objlib,INTERFACE_INCLUDE_DIRECTORIES>)
   target_link_libraries(<your-target> <PRIVATE|PUBLIC> $<TARGET_PROPERTY:cqasm_objlib,LINK_LIBRARIES>)
   ```

When configuring the project using the `cmake` command, you may want to add one
or more of the following command-line arguments:

 - `-DBUILD_SHARED_LIBS=yes`: builds a shared object library instead of a
   static library, if applicable.

 - `-DBUILD_TESTS=yes`: builds the tests in addition to the library itself. Use
   `cmake --build . --target RUN_TESTS` to run the tests (in a cross-platform
   way) after building.

 - `-DCMAKE_INSTALL_PREFIX=<directory>`: specifies the directory that the
   library will be installed into when you run
   `cmake --build . --target install`, in case you want to override whatever
   the default is for your OS.

To be complete, here's an example for how to build a shared library, test it,
and install it on Linux.

```
mkdir cbuild
cd cbuild
cmake .. -DBUILD_SHARED_LIBS=yes -DBUILD_TESTS=yes
cmake --build .
make -j
make test
sudo make install
```

## File list

 - `src`: contains the C++ source files for the library. May also contain
   private header files, but doesn't at the time of writing.
 - `include`: contains the public header files for the library.
 - `tests`: contains the test suite for the library.
 - `tree-gen`: contains the sources and build logic for the `tree-gen` tool,
   used to generate the repetitive C++ code for a tree structure defined using
   a `*.tree` file (as found in `src`).
 - `func-gen`: contains a simple code generate for the repetitive logic for
   generating the operators and constant propagation functions exposed to
   cQASM.
 - `CMakeLists.txt`/`cmake`: build system entry point and support modules.
 - `Dockerfile`/`.dockerignore`: this docker configuration tests the build on
   the oldest system Linux known to work, CentOS 6 + devtoolset 7. Just run
   `docker build .` (assuming you have Docker installed and are in the `docker`
   group).
