import cqasm.v3x.ast as ast
import cqasm.v3x.semantic as semantic
import libQasm


class Analyzer(libQasm.V3xAnalyzer):
    # parse_file and parse_string are static methods because they do not change the status of the analyzer
    # Instead, they just invoke free functions that create a temporary instance of a parser
    # analyze_file and analyze_string are not static methods because they change the status of the analyzer

    # parse_file, parse_string, analyze_file, and analyze_string return a vector of strings
    # If the length of the vector is 1, the string is a serialization of the AST
    # Otherwise, it is a list of errors

    @staticmethod
    def parse_file(*args):
        ret = libQasm.V3xAnalyzer.parse_file(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    @staticmethod
    def parse_string(*args):
        ret = libQasm.V3xAnalyzer.parse_string(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_file(self, *args):
        ret = super().analyze_file(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Program.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_string(self, *args):
        ret = super().analyze_string(*args)
        if len(ret) == 1:
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Program.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    @staticmethod
    def parse_file_to_json(*args):
        return libQasm.V3xAnalyzer.parse_file_to_json(*args)

    @staticmethod
    def parse_string_to_json(*args):
        return libQasm.V3xAnalyzer.parse_string_to_json(*args)

    def analyze_file_to_json(self, *args):
        return super().analyze_file_to_json(*args)

    def analyze_string_to_json(self, *args):
        return super().analyze_string_to_json(*args)
