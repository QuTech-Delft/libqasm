#pragma once

#include "v3x/cqasm-analyzer.hpp"
#include "v3x/cqasm-ast-gen.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <any>
#include <string_view>
#include <tuple>
#include <utility>  // pair


namespace cqasm::v3x::analyzer {

using IndexT = values::ConstInt;
using IndexListT = tree::Many<IndexT>;

using GlobalBlockReturnT = std::tuple<
    tree::One<semantic::Block>,
    const tree::Any<semantic::Variable> &,
    const tree::Any<semantic::Function> &>;

using LocalBlockReturnT = std::pair<
    tree::One<semantic::Block>,
    const tree::Any<semantic::Variable> &>;

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
    std::any visit_global_block(ast::GlobalBlock &node) override;
    std::any visit_local_block(ast::LocalBlock &node) override;
    std::any visit_variable(ast::Variable &node) override;
    std::any visit_initialization(ast::Initialization &node) override;
    std::any visit_assignment_statement(ast::AssignmentStatement &node) override;
    std::any visit_measure_instruction(ast::MeasureInstruction &node) override;
    std::any visit_gate(ast::Gate &node) override;
    std::any visit_function(ast::Function &node) override;
    std::any visit_return_statement(ast::ReturnStatement &node) override;
    std::any visit_expression(ast::Expression &node) override;
    std::any visit_unary_minus_expression(ast::UnaryMinusExpression &node) override;
    std::any visit_bitwise_not_expression(ast::BitwiseNotExpression &node) override;
    std::any visit_logical_not_expression(ast::LogicalNotExpression &node) override;
    std::any visit_power_expression(ast::PowerExpression &node) override;
    std::any visit_product_expression(ast::ProductExpression &node) override;
    std::any visit_division_expression(ast::DivisionExpression &node) override;
    std::any visit_modulo_expression(ast::ModuloExpression &node) override;
    std::any visit_addition_expression(ast::AdditionExpression &node) override;
    std::any visit_subtraction_expression(ast::SubtractionExpression &node) override;
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
    bool current_block_has_return_statement();
    void current_block_return_statements_promote_or_error(const tree::Maybe<types::Node> &return_type);

    /**
     * Build a semantic type
     * It can be a simple type SemanticT, of size 1,
     * or an array type SemanticTArray, which size is given by the syntactic type
     */
    template <typename SemanticT, typename SemanticTArray, typename SyntacticT>
    types::Type build_semantic_type(const SyntacticT &type, std::string_view type_name) const {
        if (type.size.empty()) {
            return tree::make<SemanticT>(1);
        } else if (type.size->value > 0) {
            return tree::make<SemanticTArray>(type.size->value);
        } else {
            throw error::AnalysisError{ fmt::format("found {} array of size <= 0", type_name) };
        }
    }

    /**
     * Build a semantic type
     */
    template <typename SyntacticT>
    types::Type build_semantic_type(const SyntacticT &type) const {
        assert(!type.empty() && !type->name.empty());
        types::Type ret;
        auto type_name = type->name->name;
        if (type_name == types::qubit_type_name) {
            ret = build_semantic_type<types::Qubit, types::QubitArray>(*type, types::qubit_type_name);
        } else if (type_name == types::bit_type_name) {
            ret = build_semantic_type<types::Bit, types::BitArray>(*type, types::bit_type_name);
        } else if (type_name == types::axis_type_name) {
            ret = tree::make<types::Axis>(3);
        } else if (type_name == types::bool_type_name) {
            ret = build_semantic_type<types::Bool, types::BoolArray>(*type, types::bool_type_name);
        } else if (type_name == types::integer_type_name) {
            ret = build_semantic_type<types::Int, types::IntArray>(*type, types::integer_type_name);
        } else if (type_name == types::float_type_name) {
            ret = build_semantic_type<types::Float, types::FloatArray>(*type, types::float_type_name);
        } else {
            throw error::AnalysisError("unknown type \"" + type_name + "\"");
        }
        return ret;
    }

    /**
     * Transform an input array of values into an array of a given Type
     * Pre condition: all the values in the input array can be promoted to Type
     */
    template <typename ConstTypeArray>
    static tree::One<ConstTypeArray> build_array_value_from_promoted_values(
        const values::Values &values, const types::Type &type) {

        auto ret = tree::make<ConstTypeArray>();
        ret->value.get_vec().resize(values.size());
        std::transform(values.begin(), values.end(), ret->value.begin(),
           [&type](const auto const_value) {
                return values::promote(const_value, type);
        });
        return ret;
    }

    /**
     * Transform an input array into a const array of Type
     * Pre conditions:
     *   Type can only be Bool, Int, or Real
     *   All the values in the input array can be promoted to Type
     */
    [[nodiscard]] static values::Value build_value_from_promoted_values(
        const values::Values &values, const types::Type &type);

    /**
     * Convenience function for visiting a global or a local block
     */
    template <typename Block>
    void visit_block(Block &block) {
        for (const auto &statement_ast : block.statements) {
            try {
                statement_ast->visit(*this);
            } catch (error::AnalysisError &err) {
                err.context(block);
                result_.errors.push_back(std::move(err));
            }
        }
    }

    /**
     * Convenience function for visiting a function call given the function's name and arguments
     */
    values::Value visit_function_call(
        const tree::One<ast::Identifier> &name,
        const tree::One<ast::ExpressionList> &arguments);

    /**
     * Convenience function for visiting unary operators
     */
    std::any visit_unary_operator(
        const std::string &name,
        const tree::One<ast::Expression> &expression);

    /**
     * Convenience function for visiting binary operators
     */
    std::any visit_binary_operator(
        const std::string &name,
        const tree::One<ast::Expression> &lhs,
        const tree::One<ast::Expression> &rhs);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack.
     * Returns empty when the cast fails
     */
    template <class Type, class... TypeArgs>
    values::Value visit_as(ast::Expression &expression, TypeArgs... type_args) {
        return values::promote(std::any_cast<values::Value>(expression.visit(*this)), tree::make<Type>(type_args...));
    }

    /**
     * Shorthand for parsing an expression to a constant integer
     */
    primitives::Int visit_const_int(ast::Expression &expression);
};

}  // namespace cqasm::v3x::analyzer
