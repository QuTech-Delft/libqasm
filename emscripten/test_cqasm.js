wrapper = require("../build/Release/emscripten/cqasm_emscripten.js");

wrapper().then(function(result) {
    var cqasm = new result["EmscriptenWrapper"]()

    console.log(
        "Hola from JavaScript!",
        "\nThe version of cqasm compiled with emscripten is:", cqasm.get_version(), "\n"
    );

    var program_1 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "\nExample 1:", program_1,
        "\nCalling parse_string_to_json...",
        "\nOutput:", cqasm.parse_string_to_json(program_1, "<unknown>"), "\n"
    )

    // TODO: fix cases 2 and 4, which test the parser and the analyser returning an error string, respectively

    // var program_2 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure"
    // console.log(
    //     "\nExample 2:", program_2,
    //     "\nCalling parse_string_to_json...",
    //     "\nOutput:", cqasm.parse_string_to_json(program_2, "shor.cq"), "\n"
    // )

    var program_3 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "\nExample 3:", program_3,
        "\nCalling analyze_string_to_json...",
        "\nOutput:", cqasm.analyze_string_to_json(program_3, "spin_q.cq"), "\n"
    )

    // var program_4 = "version 3;qubit[3] q;x q[3]"
    // console.log(
    //     "\nExample 4:", program_4,
    //     "\nCalling analyze_string_to_json...",
    //     "\nOutput:", cqasm.analyze_string_to_json.(program_4, "qgym.cq"), "\n"
    // )
});
