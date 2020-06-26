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
