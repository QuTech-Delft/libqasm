# Changelog

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

### Types of changes:
- **Added** for new features.
- **Changed** for changes in existing functionality.
- **Fixed** for any bug fixes.
- **Removed** for now removed features.


## [ 1.2.0 ] - [ 2025-03-27 ]

### Added
- Semantic analyzer check for same number of indices in each qubit operand.


## [ 1.1.0 ] - [ 2025-03-13 ]

### Added
- Assembly declaration.
- `Rn` unitary instruction.
- Publish WASM binaries to npm.

### Changed
- Update to `mkdocs-material/9.6.5` and `mkdocstrings/0.28.2` versions in `requirements.txt`.
- Do not hardcode `CMAKE_CXX_STANDARD`.
- Rename `ast` to `syntactic`.

### Fixed
- Setting of `build_type` in Conan profiles.
- `cibw-wheels` GitHub Actions job was building wheels only for Linux/ARM64.

## Removed
- Upload WASM binaries as release assets.


## [ 1.0.0 ] - [ 2025-01-30 ]

### Added
- `.clang-tidy`.
- Python Linux/ARM64 GitHub Actions job.

### Changed
- Error when redeclaring a variable.
- Change Linux/ARM64 jobs to use GitHub-hosted runners.

### Fixed
- clang/Linux/x64 GitHub Actions jobs (which use Ubuntu 24.04.1 since 2025).


## [ 0.6.9 ] - [ 2024-11-13 ]

### Added
- `SWAP` unitary instruction.
- `init`, `barrier`, and `wait` non-unitary instructions.

### Removed
- `reset` without qubit operands form.


## [ 0.6.8 ] - [ 2024-10-16 ]

### Added
- Gate modifiers: `inv`, `pow`, and `ctrl`.
- C++ linters GitHub Actions job (just running clang-format at the moment).

### Changed
- More descriptive and consistent (snake case) file names. 
- Minor fixes and tweaks to documentation.

### Fixed
- Empty lists are written out to JSON as `"[]"` instead of `[]`.
- `CRk` and `CZ` parameter types were interchanged in the instruction set.


## [ 0.6.7 ] - [ 2024-05-30 ]

### Added
- `pyproject.toml`.
- GitHub pages.

### Fixed
- Building of tests in Conan profiles and `setup.py`.
- Python MacOS/x64 (with Python > 3.11) packages.
- Deserialization of CBOR strings.

### Removed
- Python 3.8 packages generation.


## [ 0.6.6 ] - [ 2024-05-14 ]

### Added
- Allow multiple qubit/bit (register) definitions and mid-circuit measurements.
- Python MacOS/ARM64 jobs.
- `python/test_libqasm.py` to test Python jobs.

### Changed
- MacOS/x64 jobs now run in macos-13, and MacOS/ARM64 jobs in macos-14, both GitHub runners.
- Python module name from `libQasm` to `libqasm`.

## Fixed
- Python MacOS/ARM64 packages.
- `scripts/generate_antlr_parser.py`.

### Removed
- `python/module/libQasm` folder.


## [ 0.6.5 ] - [ 2024-04-23 ]

### Changed
- Update emscripten compilation flags.
- GitHub Actions `js-emscripten-wasm` job to work with `deno` instead of `node`.


## [ 0.6.4 ] - [ 2024-04-15 ]

### Changed
- `generate_antlr_parser.py` writes output include files in a given `include` folder. 

### Fixed
- CMake install.


## [ 0.6.3 ] - [ 2024-04-12 ]

### Added
- `cpp-linux-x64-release-shared` test job.

### Changed
- Allow multiple `measure` instructions at the end of a program.


## [ 0.6.2 ] - [ 2024-04-09 ]

### Added
- `.clang-format`.
- `emscripten-wasm` assets job.

### Fixed
- Shared library build.

### Removed
- `TREEN_GEN_BUILD_TESTS` option.


## [ 0.6.1 ] - [ 2024-04-08 ]

### Changed
- Update GitHub Actions versions.

### Fixed
- Python interface.

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
