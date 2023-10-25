#include "v3x/AnalyzeTreeGenAstVisitor.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-analyzer.hpp"


namespace cqasm::v3x::analyzer {


AnalyzeTreeGenAstVisitor::AnalyzeTreeGenAstVisitor(const Analyzer &analyzer)
: analyzer_{ analyzer }
, result_{} {}

AnalysisResult AnalyzeTreeGenAstVisitor::visitProgram(const ast::Program &program_ast) {
    result_.root = tree::make<semantic::Program>();
    result_.root->api_version = analyzer_.get_api_version();
    visitVersion(*program_ast.version);
    visitStatementList(*program_ast.statements);
    return result_;
}

void AnalyzeTreeGenAstVisitor::visitVersion(const ast::Version &version_ast) {
    try {
        // Default to API version in case the version in the AST is broken
        result_.root->version = tree::make<semantic::Version>();
        result_.root->version->items = analyzer_.get_api_version();

        // Check API version
        for (auto item : version_ast.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        if (version_ast.items.more_than(analyzer_.get_api_version())) {
            throw error::AnalysisError(fmt::format(
                "the maximum cQASM version supported is {}, but the cQASM file is version {}",
                analyzer_.get_api_version(),
                version_ast.items
            ));
        }

        // Save the file version
        result_.root->version->items = version_ast.items;
    } catch (error::AnalysisError &err) {
        err.context(version_ast);
        result_.errors.push_back(err.get_message());
    }
    result_.root->version->copy_annotation<parser::SourceLocation>(version_ast);
}

void AnalyzeTreeGenAstVisitor::visitStatementList(const ast::StatementList &statement_list_ast) {
    for (const auto &statement : statement_list_ast.items) {
        try {
            if (auto variables = statement->as_variables()) {
                visitVariables(*variables);
            } else if (auto measure_statement = statement->as_measure_statement()) {
                (void) measure_statement;
                throw std::runtime_error{ "Unimplemented"};
            } else if (auto instruction = statement->as_instruction()) {
                (void) instruction;
                throw std::runtime_error{ "Unimplemented"};
            } else {
                throw std::runtime_error("unexpected statement node");
            }
        } catch (error::AnalysisError &err) {
            err.context(*statement);
            result_.errors.push_back(err.get_message());
        }
    }
}

void AnalyzeTreeGenAstVisitor::visitVariables(const ast::Variables &variables_ast) {
    try {
        auto type_name = variables_ast.typ->name;
        types::Type type{};
        if (type_name == "qubit") {
            if (variables_ast.size.empty()) {
                type = tree::make<types::Qubit>();
            } else if (variables_ast.size->value > 0) {
                type = tree::make<types::QubitArray>(variables_ast.size->value);
            } else {
                throw error::AnalysisError("declaring qubit array of size <= 0");
            }
        } else if (type_name == "bit") {
            if (variables_ast.size.empty()) {
                type = tree::make<types::Bit>();
            } else if (variables_ast.size->value > 0) {
                type = tree::make<types::BitArray>(variables_ast.size->value);
            } else {
                throw error::AnalysisError("declaring bit array of size <= 0");
            }
        } else {
            throw error::AnalysisError("unknown type \"" + type_name + "\"");
        }

        // Construct the variables
        for (const auto &identifier : variables_ast.names) {
            // Construct variable
            // Use the location tag of the identifier to record where the variable was defined
            auto variable = tree::make<semantic::Variable>(identifier->name, type.clone());
            variable->copy_annotation<parser::SourceLocation>(*identifier);
            variable->annotations = visitAnnotations(variables_ast.annotations);
            result_.root->variables.add(variable);
        }
    } catch (error::AnalysisError &err) {
        err.context(variables_ast);
        result_.errors.push_back(err.get_message());
    }
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

values::Value AnalyzeTreeGenAstVisitor::visitExpression(const ast::Expression &ast) {
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

tree::Any<semantic::AnnotationData> AnalyzeTreeGenAstVisitor::visitAnnotations(
    const tree::Any<ast::AnnotationData> &annotations_ast) {

    auto ret = tree::Any<semantic::AnnotationData>();
    for (const auto &annotation_ast : annotations_ast) {
        try {
            auto annotation = tree::make<semantic::AnnotationData>();
            annotation->interface = annotation_ast->interface->name;
            annotation->operation = annotation_ast->operation->name;
            for (const auto &expression_ast : annotation_ast->operands->items) {
                try {
                    annotation->operands.add(visitExpression(*expression_ast));
                } catch (error::AnalysisError &e) {
                    e.context(*annotation_ast);
                    result_.errors.push_back(e.get_message());
                }
            }
            annotation->copy_annotation<parser::SourceLocation>(*annotation_ast);
            ret.add(annotation);
        } catch (error::AnalysisError &err) {
            err.context(*annotation_ast);
            result_.errors.push_back(err.get_message());
        }
    }
    return ret;
}

}  // namespace cqasm::v3x::analyzer
