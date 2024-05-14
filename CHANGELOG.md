# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).


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


## [ 0.6.5 ] - [ 2024-04-23 ]

### Added

### Changed
- Updated emscripten compilation flags.
- Changed GitHub Actions `js-emscripten-wasm` job to work with `deno` instead of `node`.

### Removed


## [ 0.6.4 ] - [ 2024-04-15 ]

### Added

### Changed
- Fixed CMake install.
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
- cQasm 3.0 parser. MVP (Minimum Viable Product) implemented.
- Conan as package manager.

### Changed
- Different upgrades: C++20, CMake 3.12, Python 3.8 to 3.12.

### Removed
- cQasm 1.x support.
- Git submodules.
