wrapper = require("../build/Release/emscripten/cqasm_emscripten.js");

process.on("unhandledRejection", error => {
    console.error("unhandledRejection", error, "\n");
});

wrapper().then(function(result) {
    var cqasm = new result["EmscriptenWrapper"]()

    try {
        console.log(
            "Hola from JavaScript!",
            "\n\tThe version of cqasm compiled with emscripten is:", cqasm.get_version()
        );
    } catch (e) { console.log(e.stack); }

    try {
        var program_1 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure"
        console.log(
            "\nExample 1:", program_1,
            "\n\tCalling parse_string_to_json...",
            "\n\tOutput:", cqasm.parse_string_to_json(program_1, "shor.cq")
        )
    } catch (e) { console.log(e.stack); }

    try {
        var program_2 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        console.log(
            "\nExample 2:", program_2,
            "\n\tCalling parse_string_to_json...",
            "\n\tOutput:", cqasm.parse_string_to_json(program_2, "<unknown>")
        )
    } catch (e) {
        console.log(e.stack);
    }

    try {
        var program_3 = "version 3;qubit[3] q;x q[3]"
        console.log(
            "\nExample 3:", program_3,
            "\n\tCalling analyze_string_to_json...",
            "\n\tOutput:", cqasm.analyze_string_to_json(program_3, "q_gym.cq")
        )
    } catch (e) { console.log(e.stack); }

    try {
        var program_4 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        console.log(
            "\nExample 4:", program_4,
            "\n\tCalling analyze_string_to_json...",
            "\n\tOutput:", cqasm.analyze_string_to_json(program_4, "spin_q.cq"), "\n"
        )
    } catch (e) { console.log(e.stack); }

    cqasm.delete()
});
