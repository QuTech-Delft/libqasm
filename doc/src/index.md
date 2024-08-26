libQASM is a library to parse cQASM programs, developed by QuTech.

It performs lexical, syntactic, and semantic analysis of an input program received via a file or a string.
It produces one of the following results:

- A syntactic or semantic AST (Abstract Syntax Tree) object. Depending on if we are parsing or analysing.
- A list of parsing or analysing errors. In case the input program was malformed.
- A JSON representation of either the AST or the list of errors.

It can be used from:

- C++ projects (as a [Conan package](https://conan.io/center/recipes/libqasm)).
- Python projects (as a [Python package](https://pypi.org/project/libqasm/)).
- Emscripten projects (via a Typescript frontend).

Check out [QX simulator](https://github.com/QuTech-Delft/qx-simulator)
and [OpenSquirrel](https://github.com/QuTech-Delft/OpenSquirrel) compiler
for an example of use in a C++ and a Python project, respectively.
