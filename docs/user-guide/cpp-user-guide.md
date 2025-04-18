libQASM can be requested as a Conan package from a `conanfile.py`:

```
def build_requirements(self):
    self.tool_requires("libqasm/1.2.0")
def requirements(self):
    self.requires("libqasm/1.2.0")
```

And then linked against from a `CMakeLists.txt`:

```
target_link_libraries(<your target> PUBLIC libqasm::libqasm)
```

!!! note

    You will need to have `Java JRE` >= 11 installed in case Conan needs to build libQASM.

And used from a C++ program:

```cpp
#include "libqasm/v3x/cqasm_python.hpp"

auto program = std::string{ R"(
    version 3.0
    qubit[2] q
    bit[2] b
    H q[0]
    CNOT q[0], q[1]
    b = measure q
)" };

auto parse_result = V3xAnalyzer::parse_string(program);

auto analyzer = V3xAnalyzer();
auto analysis_result = analyzer.analyze_string(program);
```
