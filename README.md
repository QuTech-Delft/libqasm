# libQASM

[![CI](https://github.com/QuTech-Delft/libqasm/workflows/Test/badge.svg)](https://github.com/qutech-delft/libqasm/actions)
[![Conan Center](https://img.shields.io/conan/v/libqasm)](https://conan.io/center/recipes/libqasm)
[![cpp-linter](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml)
[![PyPI](https://badgen.net/pypi/v/libqasm)](https://pypi.org/project/libqasm/)
![OS](https://img.shields.io/badge/os-emscripten%20%7C%20linux%20%7C%20macos%20%7C%20windows-blue?style=flat-square)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

libQASM is a library to parse cQASM programs, developed by QuTech. 
At the moment, libQASM only supports cQASM v3.0 programs
(see [cQASM-spec](https://qutech-delft.github.io/cQASM-spec/latest/) for the language specification).

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

## Getting started

Given a cQASM program `example.cq`.

```cQASM
version 3.0

qubit[2] q
bit[2] b

H q[0]
CNOT q[0], q[1]
b = measure q
```

We can parse or analyze this circuit using libQASM through the following programming language:

### C++

```cpp
#include "libqasm/v3x/cqasm-python.hpp"

int main() {
    auto parse_result = V3xAnalyzer::parse_file("example.cq");
    
    auto analyzer = V3xAnalyzer();
    auto analysis_result = analyzer.analyze_file("example.cq");
}
```

### Emscripten

The emscripten API only allows to input a cQASM program as a string.

```typescript
import { default as wrapper } from 'cqasm_emscripten.mjs';

wrapper().then(function(result: any) {
    let analyzer = new result["EmscriptenWrapper"]()
    let program = `
        version 3
        qubit[2] q
        bit[2] b 
        H q[0]
        CNOT q[0], q[1]
        b = measure q
    `
    let output = analyzer.parse_string_to_json(program)
    analyzer.delete()
}).catch((error: any) => {
    console.error("unhandledRejection", error, "\n");
});
```

### Python

```python
from libqasm import Analyzer

if __name__ == "__main__":
    parse_result = Analyzer.parse_file('example.cq')

    analyzer = Analyzer()
    analysis_result = analyzer.analyze_file('example.cq')
```

## Documentation

The [libQASM documentation](https://QuTech-Delft.github.io/libqasm/) is hosted through GitHub Pages.

## License

libQASM is licensed under the Apache License, Version 2.0.
See [LICENSE](https://github.com/QuTech-Delft/libqasm/blob/master/LICENSE.md) for the full license text.

## Authors

Quantum Inspire: [support@quantum-inspire.com](mailto:"support@quantum-inspire.com")
