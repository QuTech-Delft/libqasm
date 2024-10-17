import cqasm.v3x.ast as ast
import cqasm.v3x.semantic as semantic
import libqasm


class Analyzer(libqasm.V3xAnalyzer):
    """!
    Main class for parsing and analyzing cQASM v3.0 files.

    This class works as a binding for accessing C++ code from Python.

    The parsing methods are static because they do not change the status of the analyzer.
    Instead, they just invoke free functions that create a temporary instance of a parser.

    None of the parsing or the analyzing methods perform any version check.

    `parse_file`, `parse_string`, `analyze_file`, and `analyze_string`:

      - return a vector of strings.
        If the length of the vector is 1, the string is a serialization
        of the syntactic/semantic (in the case of parsing/analyzing) Abstract Syntax Tree (AST)
        of the input program.
        Otherwise, it is a list of errors.

    `parse_file_to_json`, `parse_string_to_json`, `analyze_file_to_json`, and `analyze_string_to_json`:

      - return a string in JSON format.
        If the parsing was successful, the string contains a JSON representation
        of the AST of the input program.
        Otherwise, it will contain a list of errors.
        The JSON representation of each error follows the Language Server Protocol (LSP) specification.
        Every error is mapped to an LSP Diagnostic structure:
        `severity` is hardcoded to 1 at the moment (value corresponding to an Error level).

    `parse_string`, `parse_string_to_json`, `analyze_string, and `analyze_string_to_json`:

      - have an optional second argument: `file_name`. It is only used when reporting errors.

     **Example**:

         result = libqasm.analyze_file("grover.cq")

     **Example**:

         program = r'''
             version 3.0
             qubit[2] q
             bit[2] b
             H q[0]
             CNOT q[0], q[1]
             b = measure q
         '''
         result = libqasm.parse_string_to_json(program, "bell.cq")
    """

    @staticmethod
    def parse_file(*args) -> list[str]:
        """! Parses a file containing a cQASM v3.0 program."""
        ret = libqasm.V3xAnalyzer.parse_file(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    @staticmethod
    def parse_file_to_json(*args) -> str:
        """! Parses a file containing a cQASM v3.0 program."""
        return libqasm.V3xAnalyzer.parse_file_to_json(*args)

    @staticmethod
    def parse_string(*args) -> list[str]:
        """! Parses a string containing a cQASM v3.0 program."""
        ret = libqasm.V3xAnalyzer.parse_string(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    @staticmethod
    def parse_string_to_json(*args) -> str:
        """! Parses a string containing a cQASM v3.0 program."""
        return libqasm.V3xAnalyzer.parse_string_to_json(*args)

    def analyze_file(self, *args) -> list[str]:
        """! Parses and analyzes a file containing a cQASM v3.0 program."""
        ret = super().analyze_file(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Program.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_file_to_json(self, *args) -> str:
        """! Parses and analyzes a file containing a cQASM v3.0 program."""
        return super().analyze_file_to_json(*args)

    def analyze_string(self, *args) -> list[str]:
        """! Parses and analyzes a string containing a cQASM v3.0 program."""
        ret = super().analyze_string(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Program.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_string_to_json(self, *args) -> str:
        """! Parses and analyzes a string containing a cQASM v3.0 program."""
        return super().analyze_string_to_json(*args)
