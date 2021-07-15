# Original API tests

This directory contains the testing framework for the original API, effectively also testing the new API now through the compatibility layer.

 - `cpp`: contains the C++ test files for the original API using [doctest](https://github.com/onqtam/doctest), which should be checked out as a submodule in `cpp/doctest`.
 - `python`: contains the Python test files, as discovered automatically by [unittest](https://docs.python.org/3/library/unittest.html).
 - `test_data`: contains the cQASM files used for testing.
