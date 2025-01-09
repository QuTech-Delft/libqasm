# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).


## [ 1.0.0 ] - [ xxxx-yy-zz ]

### Added
- `.clang-tidy`.

### Changed
- Error when redeclaring a variable.
- Fix clang/Linux/x64 GitHub Actions jobs (which use Ubuntu 24.04.1 since 2025).

### Removed


## [ 0.6.9 ] - [ 2024-11-13 ]

### Added
- `SWAP` unitary instruction.
- `init`, `barrier`, and `wait` non-unitary instructions.

### Changed

### Removed
- `reset` without qubit operands form.


## [ 0.6.8 ] - [ 2024-10-16 ]

### Added
- Gate modifiers: `inv`, `pow`, and `ctrl`.
- C++ linters GitHub Actions job (just running clang-format at the moment).

### Changed
- Fix bug: empty lists are written out to JSON as `"[]"` instead of `[]`.
- Fix bug: CRk and CZ parameter types were interchanged in the instruction set.
- More descriptive and consistent (snake case) file names. 
- Minor fixes and tweaks to documentation.

### Removed


## [ 0.6.7 ] - [ 2024-05-30 ]

### Added
- `pyproject.toml`.
- GitHub pages.

### Changed
- Fix building of tests in Conan profiles and `setup.py`.
- Fix Python MacOS/x64 (with Python > 3.11) packages.
- Fix deserialization of CBOR strings.

### Removed
- Python 3.8 packages generation.


## [ 0.6.6 ] - [ 2024-05-14 ]

### Added
- Allow multiple qubit/bit (register) definitions and mid-circuit measurements.
- Python MacOS/arm64 jobs.
- `python/test_libqasm.py` to test Python jobs.

### Changed
- Fix Python MacOS/arm64 packages.
- Fix `scripts/generate_antlr_parser.py`.
- MacOS/x64 jobs now run in macos-13, and MacOS/arm64 jobs in macos-14, both GitHub runners.
- Python module name from `libQasm` to `libqasm`.

### Removed
- `python/module/libQasm` folder.


## [ 0.6.5 ] - [ 2024-04-23 ]

### Added

### Changed
- Update emscripten compilation flags.
- Change GitHub Actions `js-emscripten-wasm` job to work with `deno` instead of `node`.

### Removed


## [ 0.6.4 ] - [ 2024-04-15 ]

### Added

### Changed
- Fix CMake install.
- `generate_antlr_parser.py` writes output include files in a given `include` folder. 

### Removed


## [ 0.6.3 ] - [ 2024-04-12 ]

### Added
- `cpp-linux-x64-release-shared` test job.

### Changed
- Allow multiple `measure` instructions at the end of a program.

### Removed


## [ 0.6.2 ] - [ 2024-04-09 ]

### Added
- `.clang-format`.
- `emscripten-wasm` assets job.

### Changed
- Fix shared library build.

### Removed
- `TREEN_GEN_BUILD_TESTS` option.


## [ 0.6.1 ] - [ 2024-04-08 ]

### Added

### Changed
- Fix Python interface.
- Update GitHub Actions versions.

### Removed
- `m4` dependency.


## [ 0.6.0 ] - [ 2024-03-28 ]

### Added
- cQASM 3.0 parser. MVP (Minimum Viable Product) implemented.
- Conan as package manager.

### Changed
- Different upgrades: C++20, CMake 3.12, Python 3.8 to 3.12.

### Removed
- cQASM 1.x support.
- Git submodules.
