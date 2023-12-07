wrapper = require("../build/Release/emscripten/cqasm_emscripten.js");

wrapper().then(function(result) {
    var cqasm = new result["EmscriptenWrapper"]()

    console.log(
        "Hola from JavaScript!",
        "\n\nThe version of cqasm compiled with emscripten is:", cqasm.get_version(), "\n"
    );

    var program_2 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "\n\nExample 2:", program_2,
        "\n\nCalling parse_string_to_json...",
        "\n\nOutput:", cqasm.parse_string_to_json(program_2, "<unknown>"), "\n"
    )

    var program_4 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
    console.log(
        "\n\nExample 4:", program_4,
        "\n\nCalling analyze_string_to_json...",
        "\n\nOutput:", cqasm.analyze_string_to_json(program_4, "spin_q.cq"), "\n"
    )

    // TODO: fix tests 1 and 3.
    //       Both parse_string_to_json and analyze_string_to_json cause C++ exceptions to be thrown/catched
    //       when the program string contains parsing and analysis errors, respectively.
    //       And we haven't managed yet to enable C++ exceptions support in the wasm binary
    var program_1 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure"
    console.log(
        "\n\nExample 1:", program_1,
        "\n\nCalling parse_string_to_json...",
        "\n\nOutput:", cqasm.parse_string_to_json(program_1, "shor.cq"), "\n"
    )

    var program_3 = "version 3;qubit[3] q;x q[3]"
    console.log(
        "\n\nExample 3:", program_3,
        "\n\nCalling analyze_string_to_json...",
        "\n\nOutput:", cqasm.analyze_string_to_json(program_3, "q_gym.cq"), "\n"
    )

    cqasm.delete()
});
