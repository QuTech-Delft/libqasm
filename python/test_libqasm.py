import sys

import cqasm.v3x as cqasm


class TestDataEntry:
    def __init__(self, file_name: str, program: str, analyzer_api_name: str, expected_output: str):
        self.file_name = file_name
        self.program = program
        self.analyzer_api_name = analyzer_api_name
        self.expected_output = expected_output


test_data_entries = [
    TestDataEntry(
        "shor.cq",
        "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure",
        "parse_string_to_json",
        '''{"errors":[{"range":{"start":{"line":1,"character":51},"end":{"line":1,"character":51}},"message":"mismatched input '<EOF>' expecting {'(', '+', '-', '~', '!', BOOLEAN_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, IDENTIFIER}","severity":1,"relatedInformation":[{"location":{"uri":"file:///shor.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}'''
    ),
    TestDataEntry(
        "<unknown>",
        "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure q",
        "parse_string_to_json",
        '''{"Program":{"version":{"Version":{"items":"3","source_location":"<unknown>:1:9..10"}},"block":{"GlobalBlock":{"statements":[{"Variable":{"name":{"Identifier":{"name":"q"}},"typ":{"Type":{"name":{"Keyword":{"name":"qubit"}},"size":{"IntegerLiteral":{"value":"5"}},"source_location":"<unknown>:1:11..19"}},"annotations":"[]","source_location":"<unknown>:1:20..21"}},{"Variable":{"name":{"Identifier":{"name":"b"}},"typ":{"Type":{"name":{"Keyword":{"name":"bit"}},"size":{"IntegerLiteral":{"value":"5"}},"source_location":"<unknown>:1:22..28"}},"annotations":"[]","source_location":"<unknown>:1:29..30"}},{"Gate":{"name":{"Identifier":{"name":"H"}},"operands":{"ExpressionList":{"items":[{"Index":{"expr":{"Identifier":{"name":"q"}},"indices":{"IndexList":{"items":[{"IndexRange":{"first":{"IntegerLiteral":{"value":"0","source_location":"<unknown>:1:35..36"}},"last":{"IntegerLiteral":{"value":"4","source_location":"<unknown>:1:37..38"}}}}]}},"source_location":"<unknown>:1:33..34"}}]}},"annotations":"[]","source_location":"<unknown>:1:31..32"}},{"MeasureInstruction":{"name":{"Identifier":{"name":"measure"}},"lhs":{"Identifier":{"name":"b","source_location":"<unknown>:1:40..41"}},"rhs":{"Identifier":{"name":"q","source_location":"<unknown>:1:52..53"}},"annotations":"[]","source_location":"<unknown>:1:44..51"}}]}}}}'''
    ),
    TestDataEntry(
        "q_gym.cq",
        "version 3;qubit[3] q;X q[3]",
        "analyze_string_to_json",
        '''{"errors":[{"range":{"start":{"line":1,"character":24},"end":{"line":1,"character":25}},"message":"index 3 out of range (size 3)","severity":1,"relatedInformation":[{"location":{"uri":"file:///q_gym.cq","range":{"start":{"line":0,"character":0},"end":{"line":0,"character":0}}},"message":"<unknown error message>"}]}]}'''
    ),
    TestDataEntry(
        "spin_q.cq",
        "version 3;qubit[5] q;bit[5] b;H q[0:4];b = measure q",
        "analyze_string_to_json",
        '''{"Program":{"api_version":"3.0","version":{"Version":{"items":"3"}},"block":{"Block":{"statements":[{"Instruction":{"instruction_ref":"H(qubit array)","name":"H","operands":[{"IndexRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},"indices":[{"ConstInt":{"value":"0"}},{"ConstInt":{"value":"1"}},{"ConstInt":{"value":"2"}},{"ConstInt":{"value":"3"}},{"ConstInt":{"value":"4"}}]}}],"annotations":"[]"}},{"Instruction":{"instruction_ref":"measure(bit array, qubit array)","name":"measure","operands":[{"VariableRef":{"variable":{"Variable":{"name":"b","typ":{"BitArray":{"size":"5"}},"annotations":"[]"}}}},{"VariableRef":{"variable":{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}}}}],"annotations":"[]"}}]}},"variables":[{"Variable":{"name":"q","typ":{"QubitArray":{"size":"5"}},"annotations":"[]"}},{"Variable":{"name":"b","typ":{"BitArray":{"size":"5"}},"annotations":"[]"}}]}}'''
    )
]


def main():
    ret_code = 0
    for i, entry in enumerate(test_data_entries):
        analyzer = cqasm.Analyzer()
        print(f"\nExample {i+1}: {entry.program}\n\tCalling {entry.analyzer_api_name}...\n")
        analyzer_api = getattr(analyzer, entry.analyzer_api_name)
        output = analyzer_api(entry.program, entry.file_name)
        print(f"\tOutput: {output}")
        if output != entry.expected_output:
            print(f"\tExpected output: {entry.expected_output}\n")
            ret_code |= i*2
    if ret_code == 0:
        print("\nOK\n")
    else:
        print(f"\nError: returning {ret_code:04b}b\n")
        sys.exit(ret_code)


if __name__ == '__main__':
    main()
