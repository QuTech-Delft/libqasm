`libqasm` can be used from a web environment via a Typescript frontend.

Emscripten API only allows to input a cQASM program via a string
and always returns a JSON string output. 

**Example**:

```typescript
import { default as wrapper } from 'cqasm_emscripten.mjs';

wrapper().then(function(result: any) {
    let cqasm = new result["EmscriptenWrapper"]()
    let program = `version 3.0        
                   qubit[2] q 
                   bit[2] b        
                   H q[0]
                   CNOT q[0], q[1]
                   b = measure q`
    let output = cqasm.parse_string_to_json(program, "bell.cq")
    cqasm.delete()
}).catch((error: any) => {
    console.error("unhandledRejection", error, "\n");
});
```
