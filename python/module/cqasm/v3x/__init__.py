import libQasm


class Analyzer(libQasm.V3xAnalyzer):

    @staticmethod
    def parse_file(*args):
        ret = libQasm.V3xAnalyzer.parse_file(*args)
        if len(ret) == 1:
            import cqasm.v3x.ast as ast
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return list(ret[1:])

    @staticmethod
    def parse_string(*args):
        ret = libQasm.V3xAnalyzer.parse_string(*args)
        if len(ret) == 1:
            import cqasm.v3x.ast as ast
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return list(ret[1:])

    def analyze_file(self, *args):
        ret = super().analyze_file(*args)
        if len(ret) == 1:
            import cqasm.v3x.semantic as semantic
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return list(ret[1:])

    def analyze_string(self, *args):
        ret = super().analyze_string(*args)
        if len(ret) == 1:
            import cqasm.v3x.semantic as semantic
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return list(ret[1:])
