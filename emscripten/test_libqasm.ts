import { default as wrapper } from '../build/Release/emscripten/cqasm_emscripten.mjs';

wrapper().then(function(result: any) {
    let cqasm = new result["EmscriptenWrapper"]()
    let ret_code = 0

    console.log("Hola from JavaScript!")

    try {
        let output = cqasm.get_version()
        let expected_output = "0.6.5"
        console.log("\nThe version of libqasm compiled with emscripten is:", output);
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 1
        }
    } catch (e) {
        console.log(e.stack);
    }

    try {
        let program_1 = "version 3;qubit[5] q;H q[0:4];measure"
        let output = cqasm.parse_string_to_json(program_1, "shor.cq")
        let expected_output = String.raw`{"errors":[{"range":{"start":{"line":1,"character":38},"end":{"line":1,"character":38}},"message":"mismatched input '<EOF>' expecting {'(', '+', '-', '~', '!', BOOLEAN_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, IDENTIFIER}","severity":1,"relatedInformation":[{"location":{"uri":"file:///shor.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}`
        console.log( "\nExample 1:", program_1, "\n\tCalling parse_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 2
        }
    } catch (e) { console.log(e.stack); }

    try {
        let program_2 = "version 3;qubit[5] q;H q[0:4];measure q"
        let output = cqasm.parse_string_to_json(program_2, "<unknown>")
        let expected_output = String.raw`{"Program":{"version":{"Version":{"items":"3","source_location":"<unknown>:1:9..10"}},"block":{"GlobalBlock":{"qubit_variable_declaration":{"Variable":{"name":{"Identifier":{"name":"q"}},"typ":{"Type":{"name":{"Keyword":{"name":"qubit"}},"size":{"IntegerLiteral":{"value":"5"}},"source_location":"<unknown>:1:11..19"}},"annotations":"[]","source_location":"<unknown>:1:20..21"}},"gates":[{"Gate":{"name":{"Identifier":{"name":"H"}},"operands":{"ExpressionList":{"items":[{"Index":{"expr":{"Identifier":{"name":"q"}},"indices":{"IndexList":{"items":[{"IndexRange":{"first":{"IntegerLiteral":{"value":"0","source_location":"<unknown>:1:26..27"}},"last":{"IntegerLiteral":{"value":"4","source_location":"<unknown>:1:28..29"}}}}]}},"source_location":"<unknown>:1:24..25"}}]}},"annotations":"[]","source_location":"<unknown>:1:22..23"}}],"measure_instructions":[{"MeasureInstruction":{"name":{"Identifier":{"name":"measure"}},"operand":{"Identifier":{"name":"q","source_location":"<unknown>:1:39..40"}},"annotations":"[]","source_location":"<unknown>:1:31..38"}}]}}}}`
        console.log("\nExample 2:", program_2, "\n\tCalling parse_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 4
        }
    } catch (e) {
        console.log(e.stack);
    }

    try {
        let program_3 = "version 3;qubit[3] q;X q[3]"
        let output = cqasm.analyze_string_to_json(program_3, "q_gym.cq")
        let expected_output = String.raw`{"errors":[{"range":{"start":{"line":1,"character":24},"end":{"line":1,"character":25}},"message":"index 3 out of range (size 3)","severity":1,"relatedInformation":[{"location":{"uri":"file:///q_gym.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}`
        console.log("\nExample 3:", program_3, "\n\tCalling analyze_string_to_json...", "\n\tOutput:", output)
        if (output !== expected_output) {
            console.log("\tExpected output:", expected_output)
            ret_code |= 8
        }
    } catch (e) { console.log(e.stack); }

    try {
        let program_4 = "version 3;qubit[5] q;H q[0:4];measure q"
        let output = cqasm.analyze_string_to_json(program_4, "spin_q.cq")
        let expected_output = String.raw`{"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"qubit_variable_declaration":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},"block":{"Block":{"statements":[{"Instruction":{"instruction_ref":"H(qubit array)","name":"H","operands":[{"IndexRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},"indices":[{"ConstInt":{"value":"0"}},{"ConstInt":{"value":"1"}},{"ConstInt":{"value":"2"}},{"ConstInt":{"value":"3"}},{"ConstInt":{"value":"4"}}]}}],"annotations":"[]"}},{"Instruction":{"instruction_ref":"measure(qubit array)","name":"measure","operands":[{"VariableRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}}}}],"annotations":"[]"}}]}}}}`
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
