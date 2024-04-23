import { default as wrapper } from '../build/Release/emscripten/cqasm_emscripten.mjs';

wrapper().then(function(result: any) {
    var cqasm = new result["EmscriptenWrapper"]()
    var ret_code = 0

    console.log("Hola from JavaScript!")

    try {
        var output = cqasm.get_version()
        var expected_output = "0.6.5"
        console.log("\nThe version of libqasm compiled with emscripten is:", output);
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 1
        }
    } catch (e) {
        console.log(e.stack);
    }

    try {
        var program_1 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure"
        output = cqasm.parse_string_to_json(program_1, "shor.cq")
        expected_output = String.raw`{"errors":[{"range":{"start":{"line":1,"character":51},"end":{"line":1,"character":56}},"message":"mismatched input '<EOF>' expecting {'[', '{', '(', '+', '-', '~', '!', BOOLEAN_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, IDENTIFIER}","severity":1,"relatedInformation":[{"location":{"uri":"file:///shor.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}`
        console.log( "\nExample 1:", program_1, "\n\tCalling parse_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 2
        }
    } catch (e) { console.log(e.stack); }

    try {
        var program_2 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        output = cqasm.parse_string_to_json(program_2, "<unknown>")
        expected_output = String.raw`{"Program":{"version":{"Version":{"items":"3","source_location":"<unknown>:1:9..10"}},"block":{"GlobalBlock":{"statements":[{"Variable":{"name":{"Identifier":{"name":"q"}},"typ":{"Type":{"name":{"Keyword":{"name":"qubit"}},"size":{"IntegerLiteral":{"value":"5"}},"source_location":"<unknown>:1:11..19"}},"annotations":"[]","source_location":"<unknown>:1:20..21"}},{"Variable":{"name":{"Identifier":{"name":"b"}},"typ":{"Type":{"name":{"Keyword":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"5"}},"source_location":"<unknown>:1:22..28"}},"annotations":"[]","source_location":"<unknown>:1:29..30"}},{"Gate":{"name":{"Identifier":{"name":"h"}},"operands":{"ExpressionList":{"items":[{"Index":{"expr":{"Identifier":{"name":"q"}},"indices":{"IndexList":{"items":[{"IndexRange":{"first":{"IntegerLiteral":{"value":"0","source_location":"<unknown>:1:35..36"}},"last":{"IntegerLiteral":{"value":"4","source_location":"<unknown>:1:37..38"}}}}]}},"source_location":"<unknown>:1:33..34"}}]}},"annotations":"[]","source_location":"<unknown>:1:31..32"}},{"MeasureInstruction":{"name":{"Identifier":{"name":"measure"}},"lhs":{"Identifier":{"name":"b","source_location":"<unknown>:1:40..41"}},"rhs":{"Identifier":{"name":"q","source_location":"<unknown>:1:52..53"}},"annotations":"[]","source_location":"<unknown>:1:44..51"}}]}}}}`
        console.log("\nExample 2:", program_2, "\n\tCalling parse_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 4
        }
    } catch (e) {
        console.log(e.stack);
    }

    try {
        var program_3 = "version 3;qubit[3] q;x q[3]"
        output = cqasm.analyze_string_to_json(program_3, "q_gym.cq")
        expected_output = String.raw`{"errors":[{"range":{"start":{"line":1,"character":24},"end":{"line":1,"character":25}},"message":"index 3 out of range (size 3)","severity":1,"relatedInformation":[{"location":{"uri":"file:///q_gym.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}`
        console.log("\nExample 3:", program_3, "\n\tCalling analyze_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 8
        }
    } catch (e) { console.log(e.stack); }

    try {
        var program_4 = "version 3;qubit[5] q;bit[5] b;h q[0:4];b = measure q"
        output = cqasm.analyze_string_to_json(program_4, "spin_q.cq")
        expected_output = String.raw`{"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"block":{"Block":{"statements":[{"Instruction":{"instruction_ref":"h(qubit array)","name":"h","operands":[{"IndexRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},"indices":[{"ConstInt":{"value":"0"}},{"ConstInt":{"value":"1"}},{"ConstInt":{"value":"2"}},{"ConstInt":{"value":"3"}},{"ConstInt":{"value":"4"}}]}}],"annotations":"[]"}},{"Instruction":{"instruction_ref":"measure(bit array, qubit array)","name":"measure","operands":[{"VariableRef":{"variable":{"Variable":{"name":"b","typ":{"BitArray":{"size":"5"}},"annotations":"[]"}}}},{"VariableRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}}}}],"annotations":"[]"}}]}},"functions":"[]","variables":[{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},{"Variable":{"name":"b","typ":{"BitArray":{"size":"5"}},"annotations":"[]"}}]}}`
        console.log("\nExample 4:", program_4, "\n\tCalling analyze_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 16
        }
    } catch (e) { console.log(e.stack); }

    cqasm.delete()

    if (ret_code === 0) {
        console.log("\nOK\n")
    } else {
        console.log("\nError: returning", ret_code.toString(16), "\n")
    }
    Deno.exit(ret_code);
}).catch((error: any) => {
    console.error("unhandledRejection", error, "\n");
});
