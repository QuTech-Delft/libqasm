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
    std::any visit_unary_minus_expression(ast::UnaryMinusExpression &node) override;
    std::any visit_bitwise_not_expression(ast::BitwiseNotExpression &node) override;
    std::any visit_logical_not_expression(ast::LogicalNotExpression &node) override;
    std::any visit_power_expression(ast::PowerExpression &node) override;
    std::any visit_product_expression(ast::ProductExpression &node) override;
    std::any visit_division_expression(ast::DivisionExpression &node) override;
    std::any visit_modulo_expression(ast::ModuloExpression &node) override;
    std::any visit_shift_left_expression(ast::ShiftLeftExpression &node) override;
    std::any visit_shift_right_expression(ast::ShiftRightExpression &node) override;
    std::any visit_cmp_gt_expression(ast::CmpGtExpression &node) override;
    std::any visit_cmp_lt_expression(ast::CmpLtExpression &node) override;
    std::any visit_cmp_ge_expression(ast::CmpGeExpression &node) override;
    std::any visit_cmp_le_expression(ast::CmpLeExpression &node) override;
    std::any visit_cmp_eq_expression(ast::CmpEqExpression &node) override;
    std::any visit_cmp_ne_expression(ast::CmpNeExpression &node) override;
    std::any visit_bitwise_and_expression(ast::BitwiseAndExpression &node) override;
    std::any visit_bitwise_xor_expression(ast::BitwiseXorExpression &node) override;
    std::any visit_bitwise_or_expression(ast::BitwiseOrExpression &node) override;
    std::any visit_logical_and_expression(ast::LogicalAndExpression &node) override;
    std::any visit_logical_xor_expression(ast::LogicalXorExpression &node) override;
    std::any visit_logical_or_expression(ast::LogicalOrExpression &node) override;
    std::any visit_ternary_conditional_expression(ast::TernaryConditionalExpression &node) override;
    std::any visit_function_call(ast::FunctionCall &node) override;
    std::any visit_index(ast::Index &node) override;
    std::any visit_index_list(ast::IndexList &node) override;
    std::any visit_index_item(ast::IndexItem &node) override;
    std::any visit_index_range(ast::IndexRange &ast) override;
    std::any visit_identifier(ast::Identifier &node) override;
    std::any visit_initialization_list(ast::InitializationList &node) override;
    std::any visit_boolean_literal(ast::BooleanLiteral &node) override;
    std::any visit_integer_literal(ast::IntegerLiteral &node) override;
    std::any visit_float_literal(ast::FloatLiteral &node) override;

private:
    /*
     * Build a semantic type
     * It can be a simple type T, of size 1,
     * or an array type TArray, which size is given by the syntactic type
     */
    template <typename T, typename TArray>
    [[nodiscard]] types::Type visit_variable_type(const ast::Variable &variable_ast, std::string_view type_name) const;

    /*
     * Convenience function for visiting a function call given the function's name and arguments
     */
    values::Value visit_function_call(
        const tree::One<ast::Identifier> &name,
        const tree::One<ast::ExpressionList> &arguments);

    /*
     * Convenience function for visiting unary operators
     */
    std::any visit_unary_operator(
        const std::string &name,
        const tree::One<ast::Expression> &expression);

    /*
     * Convenience function for visiting binary operators
     */
    std::any visit_binary_operator(
        const std::string &name,
        const tree::One<ast::Expression> &lhs,
        const tree::One<ast::Expression> &rhs);

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
