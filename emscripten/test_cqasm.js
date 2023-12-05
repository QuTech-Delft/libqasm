// TODO: this 'build_wasm' hardcoded path has to go
// TODO: maybe I can use Pablo's Superpositeur/wasm/demo.js as an example

wrapper = require("../build_wasm/main.js");

wrapper().then(function(result) {
    cqasm_wrapper = new result.EmscriptenWrapper()

    console.log(
        "Hola from JavaScript!\n\n",
        "The version of cqasm compiled with emscripten is: ", cqasm_wrapper.get_version(), "\n"
    );

    program_1 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "Example 1: '", program_1, "'\n",
        "Calling parse_file...\n",
        "Output: '", cqasm_wrapper.parse_file_to_json(program_1), "'\n"
    )

    program_2 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure"
    console.log(
        "Example 2: '", program_2, "'\n",
        "Calling parse_file...\n",
        "Output: '", cqasm_wrapper.parse_file_to_json(program_2), "'\n"
    )

    program_3 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "Example 3: '", program_3, "'\n",
        "Calling parse_file...\n",
        "Output: '", cqasm_wrapper.analyze_file_to_json(program_3), "'\n"
    )

    program_4 = "version 3;qubit[3] q;x q[3]"
    console.log(
        "Example 4: '", program_4, "'\n",
        "Calling parse_file...\n",
        "Output: '", cqasm_wrapper.analyze_file_to_json(program_4), "'\n"
    )
});
