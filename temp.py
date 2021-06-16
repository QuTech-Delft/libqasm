import sys
import cqasm.v2

cq = """
cQASM 2.0

1..2
"""

result = cqasm.v2.Analyzer().parse_string(cq)

if isinstance(result, list):
    for msg in result:
        print(msg)
    sys.exit(1)
else:
    print(result)
    sys.exit(0)
