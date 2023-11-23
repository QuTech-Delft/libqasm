import libQasm

import cqasm.v1x.ast as ast
import cqasm.v1x.semantic as semantic
import cqasm.v1x.parser as parser
import cqasm.v1x.analyzer as analyzer


class Analyzer(libQasm.V1xAnalyzer):

    # parse_file and parse_string are static methods because they do not change the status of the analyzer
    # Instead, they just invoke free functions that create a temporary instance of a parser
    # analyze_file and analyze_string are not static methods because they change the status of the analyzer

    # parse_file, parse_string, analyze_file, and analyze_string return a vector of strings
    # If the length of the vector is 1, the string is a serialization of the AST
    # Otherwise, it is a list of errors

    @staticmethod
    def parse_file(*args):
        retval = libQasm.V1xAnalyzer.parse_file(*args)
        if len(retval) == 1:
            return ast.Root.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    @staticmethod
    def parse_string(*args):
        retval = libQasm.V1xAnalyzer.parse_string(*args)
        if len(retval) == 1:
            return ast.Root.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    def analyze_file(self, *args):
        retval = super().analyze_file(*args)
        if len(retval) == 1:
            print(retval[0].encode("utf-8", errors="surrogateescape"))
            return semantic.Program.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])

    def analyze_string(self, *args):
        retval = super().analyze_string(*args)
        if len(retval) == 1:
            return semantic.Program.deserialize(retval[0].encode("utf-8", errors="surrogateescape"))
        return list(retval[1:])


class ParseResult(libQasm.V1xParseResult):
    @staticmethod
    def to_json(parse_result: parser.ParseResult) -> str:
        return parse_result.to_json()


class AnalysisResult(libQasm.V1xAnalysisResult):
    @staticmethod
    def to_json(analysis_result: analyzer.AnalysisResult) -> str:
        return analysis_result.to_json()
