import libQasm;

class Analyzer(libQasm.V1xAnalyzer):

    @staticmethod
    def parse_file(*args):
        retval = libQasm.V1xAnalyzer.parse_file(*args)
        if len(retval) == 1:
            import cqasm.v1x.ast as ast
            return ast.Root.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    @staticmethod
    def parse_string(*args):
        retval = libQasm.V1xAnalyzer.parse_string(*args)
        if len(retval) == 1:
            import cqasm.v1x.ast as ast
            return ast.Root.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    def analyze_file(self, *args):
        retval = super().analyze_file(*args)
        if len(retval) == 1:
            import cqasm.v1x.semantic as semantic
            print(retval[0].encode("utf-8", errors="surrogateescape"))
            return semantic.Program.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    def analyze_string(self, *args):
        retval = super().analyze_string(*args)
        if len(retval) == 1:
            import cqasm.v1x.semantic as semantic
            return semantic.Program.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])
