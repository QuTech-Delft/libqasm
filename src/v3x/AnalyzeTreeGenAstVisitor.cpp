#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-analyzer.hpp"


namespace cqasm::v3x::analyzer {


AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(const Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{} {}

AnalysisResult AnalyzeTreeGenAstVisitor::visitProgram(const ast::Program &ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.get_api_version();
    visitVersion(*ast.version);
    visitStatementList(*ast.statements);
    return result_;
}

void AnalyzeTreeGenAstVisitor::visitVersion(const ast::Version &ast) {
    try {
        // Default to API version in case the version in the AST is broken
        result_.root->version = tree::make<semantic::Version>();
        result_.root->version->items = analyzer_.get_api_version();

        // Check API version
        for (auto item : ast.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (ast.items.more_than(analyzer_.get_api_version())) {
            throw error::AnalysisError(fmt::format(
                "the maximum cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.get_api_version(),
                ast.items
            ));
        }

        // Save the file version
        result_.root->version->items = ast.items;
    } catch (error::AnalysisError &err) {
        err.context(ast);
        result_.errors.push_back(err.get_message());
    }
    result_.root->version->copy_annotation<parser::SourceLocation>(ast);
}

void AnalyzeTreeGenAstVisitor::visitStatementList(const ast::StatementList &ast) {
    for (const auto &statement : ast.items) {
        (void) statement;
        throw std::runtime_error{ "Unimplemented"};
    }
}

void AnalyzeTreeGenAstVisitor::visitVariables(const ast::Variables &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

void AnalyzeTreeGenAstVisitor::visitMeasureStatement(const ast::MeasureStatement &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

tree::Maybe<semantic::Instruction> AnalyzeTreeGenAstVisitor::visitInstruction(const ast::Instruction &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

tree::Maybe<values::Value> AnalyzeTreeGenAstVisitor::visitExpressionList(const ast::ExpressionList &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

tree::Many<values::ConstInt> AnalyzeTreeGenAstVisitor::visitIndexList(const ast::IndexList &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

values::ConstInt AnalyzeTreeGenAstVisitor::visitIndexItem(const ast::IndexItem &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

tree::Many<values::ConstInt> AnalyzeTreeGenAstVisitor::visitIndexRange(const ast::IndexRange &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

values::ConstInt AnalyzeTreeGenAstVisitor::visitIntegerLiteral(const ast::IntegerLiteral &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

values::ConstReal AnalyzeTreeGenAstVisitor::visitFloatLiteral(const ast::FloatLiteral &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

values::Value AnalyzeTreeGenAstVisitor::visitIdentifier(const ast::Identifier &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

values::Value AnalyzeTreeGenAstVisitor::visitIndex(const ast::Index &ast) {
    (void) ast;
    throw std::runtime_error{ "Unimplemented"};
}

}  // namespace cqasm::v3x::analyzer
