import libQasm


class Analyzer(libQasm.V1xAnalyzer):

    @staticmethod
    def parse_file(*args):
        ret = libQasm.V1xAnalyzer.parse_file(*args)
        if len(ret) == 1:
            import cqasm.v1x.ast as ast
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    @staticmethod
    def parse_string(*args):
        ret = libQasm.V1xAnalyzer.parse_string(*args)
        if len(ret) == 1:
            import cqasm.v1x.ast as ast
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = ast.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_file(self, *args):
        ret = super().analyze_file(*args)
        if len(ret) == 1:
            import cqasm.v1x.semantic as semantic
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]

    def analyze_string(self, *args):
        ret = super().analyze_string(*args)
        if len(ret) == 1:
            import cqasm.v1x.semantic as semantic
            serialized_ast_str = str(ret[0])
            serialized_ast_bytes = serialized_ast_str.encode(encoding='utf-8', errors="surrogateescape")
            deserialized_ast = semantic.Root.deserialize(serialized_ast_bytes)
            return deserialized_ast
        return [str(error) for error in ret[1:]]
