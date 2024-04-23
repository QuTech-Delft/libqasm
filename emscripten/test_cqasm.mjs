import * as wrapper from '../build/Release/emscripten/cqasm_emscripten.mjs';

process.on("unhandledRejection", error => {
    console.error("unhandledRejection", error, "\n");
});

wrapper.default().then(function(result) {
    var cqasm = new result["EmscriptenWrapper"]()
    var ret_code = 0

    console.log("Hola from JavaScript!")

    var cqasm = new result["EmscriptenWrapper"]();
    try {
        var output = cqasm.get_version()
        var expected_output = "0.5.2"
        console.log("\nThe version of libqasm compiled with emscripten is:", output);
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 1
        }
    } catch (e) {
        console.log(e.stack);
    }

    if (ret_code === 0) {
        console.log("\nOK\n")
    } else {
        console.log("\nError: returning", ret_code.toString(16), "\n")
    }
    return process.exit(ret_code)
});
