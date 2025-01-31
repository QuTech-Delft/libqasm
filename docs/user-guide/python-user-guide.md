libQASM can be installed as a Python package:

```shell
pip install libqasm
```

And then imported from a Python program:

```python
from libqasm import Analyzer

program = r'''
    version 3.0
    qubit[2] q
    bit[2] b
    H q[0]
    CNOT q[0], q[1]
    b = measure q
'''

parse_result = Analyzer.parse_string(program, "bell.cq")

analyzer = Analyzer()
analysis_result = analyzer.analyze_string(program, "bell.cq")
```
