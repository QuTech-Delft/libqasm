# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [ 0.5.2 ] - [ 2024-01-18 ]

### Added
- Variable declaration and initialization in cqasm v3 parser
- New types in v3: bool, int, float, axis
- Arrays in v3

### Changed

- v3x::AnalyzeTreeGenAstVisitor is an instance of tree-gen generated visitor base class

### Removed

- msvc-runtime pip dependency on Windows

## [ 0.5.1 ] - [ 2024-01-08 ]

### Added

### Changed

- Fixed `upload-release-asset` steps in `.github/workflow/assets.yml`.
- Updated `RELEASE.md`.

### Removed

## [ 0.5.0 ] - [ 2023-12-16 ]

### Added

- cQasm 3.0 parser. MVP (Minimum Viable Product) implemented. Extended v3 is a WIP.
- Conan as package manager.
- res folder for resources.

### Changed

- Different upgrades: C++20, CMake 3.12, Python 3.8 to 3.12.
- GitHub workflows and jobs.
- File reorganization to accommodate v3 parser: v10, v1x, and v3x folders. tests folder is now called test.

### Removed

- Git submodules.
- GitHub Conda job.
- CMake folder.
