#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <any>


namespace cqasm::v3x::analyzer {

using IndexT = values::ConstInt;
using IndexListT = tree::Many<IndexT>;

class AnalyzeTreeGenAstVisitor : public ast::Visitor<std::any> {
    Analyzer &analyzer_;
    AnalysisResult result_;

public:
    explicit AnalyzeTreeGenAstVisitor(Analyzer &analyzer);

    std::any visit_node(ast::Node &node) override;
    std::any visit_program(ast::Program &node) override;
    std::any visit_version(ast::Version &node) override;
    std::any visit_annotated(ast::Annotated &node) override;
    std::any visit_annotation_data(ast::AnnotationData &node) override;
    std::any visit_statement_list(ast::StatementList &node) override;
    std::any visit_variable(ast::Variable &node) override;
    std::any visit_initialization(ast::Initialization &node) override;
    std::any visit_assignment_instruction(ast::AssignmentInstruction &node) override;
    std::any visit_measure_instruction(ast::MeasureInstruction &node) override;
    std::any visit_instruction(ast::Instruction &node) override;
    std::any visit_expression(ast::Expression &node) override;
    std::any visit_boolean_literal(ast::BooleanLiteral &node) override;
    std::any visit_integer_literal(ast::IntegerLiteral &node) override;
    std::any visit_float_literal(ast::FloatLiteral &node) override;
    std::any visit_identifier(ast::Identifier &node) override;
    std::any visit_index(ast::Index &node) override;
    std::any visit_index_list(ast::IndexList &node) override;
    std::any visit_index_item(ast::IndexItem &node) override;
    std::any visit_index_range(ast::IndexRange &ast) override;
    std::any visit_initialization_list(ast::InitializationList &node) override;

private:
    /*
     * Build a semantic type
     * It can be a simple type T, of size 1,
     * or an array type TArray, which size is given by the syntactic type
     */
    template <typename T, typename TArray>
    [[nodiscard]] types::Type visit_variable_type(const ast::Variable &variable_ast, std::string_view type_name) const;

    /*
     * Transform an input array of values into an array of a given Type
     * Pre condition: all the values in the input array can be promoted to Type
     */
    template <typename ConstTypeArray>
    [[nodiscard]] static tree::One<ConstTypeArray> build_array_value_from_promoted_values(
        const values::Values &values, const types::Type &type);

    /*
     * Transform an input array into a const array of Type
     * Pre conditions:
     *   Type can only be Bool, Int, or Real
     *   All the values in the input array can be promoted to Type
     */
    [[nodiscard]] static values::Value build_value_from_promoted_values(
        const values::Values &values, const types::Type &type);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack.
     * Returns empty when the cast fails.
     */
    template<class Type, class... TypeArgs>
    values::Value analyze_as(ast::Expression &expression, TypeArgs... type_args);

    /**
     * Shorthand for parsing an expression to a constant integer.
     */
    primitives::Int visit_const_int(ast::Expression &expression);
};

}  // namespace cqasm::v3x::analyzer
