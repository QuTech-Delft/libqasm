#pragma once

#include <any>
#include <string_view>
#include <tuple>
#include <utility>  // pair

#include "libqasm/v3x/analyzer.hpp"
#include "libqasm/v3x/ast_generated.hpp"
#include "libqasm/v3x/semantic_generated.hpp"

namespace cqasm::v3x::analyzer {

using IndexT = values::ConstInt;
using IndexListT = tree::Many<IndexT>;

using GlobalBlockReturnT = std::tuple<tree::One<semantic::Block>, const tree::Any<semantic::Variable>&>;

class SemanticAnalyzer : public ast::Visitor<std::any> {
protected:
    Analyzer& analyzer_;
    AnalysisResult result_;

public:
    explicit SemanticAnalyzer(Analyzer& analyzer);

    std::any visit_node(ast::Node& node) override;
    std::any visit_program(ast::Program& node) override;
    std::any visit_version(ast::Version& node) override;
    std::any visit_annotated(ast::Annotated& node) override;
    std::any visit_annotation_data(ast::AnnotationData& node) override;
    std::any visit_global_block(ast::GlobalBlock& node) override;
    std::any visit_variable(ast::Variable& node) override;
    std::any visit_gate_instruction(ast::GateInstruction& node) override;
    std::any visit_gate(ast::Gate& node) override;
    std::any visit_non_gate_instruction(ast::NonGateInstruction& node) override;
    std::any visit_expression_list(ast::ExpressionList& node) override;
    std::any visit_expression(ast::Expression& node) override;
    std::any visit_unary_minus_expression(ast::UnaryMinusExpression& node) override;
    std::any visit_bitwise_not_expression(ast::BitwiseNotExpression& node) override;
    std::any visit_logical_not_expression(ast::LogicalNotExpression& node) override;
    std::any visit_power_expression(ast::PowerExpression& node) override;
    std::any visit_product_expression(ast::ProductExpression& node) override;
    std::any visit_division_expression(ast::DivisionExpression& node) override;
    std::any visit_modulo_expression(ast::ModuloExpression& node) override;
    std::any visit_addition_expression(ast::AdditionExpression& node) override;
    std::any visit_subtraction_expression(ast::SubtractionExpression& node) override;
    std::any visit_shift_left_expression(ast::ShiftLeftExpression& node) override;
    std::any visit_shift_right_expression(ast::ShiftRightExpression& node) override;
    std::any visit_cmp_gt_expression(ast::CmpGtExpression& node) override;
    std::any visit_cmp_lt_expression(ast::CmpLtExpression& node) override;
    std::any visit_cmp_ge_expression(ast::CmpGeExpression& node) override;
    std::any visit_cmp_le_expression(ast::CmpLeExpression& node) override;
    std::any visit_cmp_eq_expression(ast::CmpEqExpression& node) override;
    std::any visit_cmp_ne_expression(ast::CmpNeExpression& node) override;
    std::any visit_bitwise_and_expression(ast::BitwiseAndExpression& node) override;
    std::any visit_bitwise_xor_expression(ast::BitwiseXorExpression& node) override;
    std::any visit_bitwise_or_expression(ast::BitwiseOrExpression& node) override;
    std::any visit_logical_and_expression(ast::LogicalAndExpression& node) override;
    std::any visit_logical_xor_expression(ast::LogicalXorExpression& node) override;
    std::any visit_logical_or_expression(ast::LogicalOrExpression& node) override;
    std::any visit_ternary_conditional_expression(ast::TernaryConditionalExpression& node) override;
    std::any visit_function_call(ast::FunctionCall& node) override;
    std::any visit_index(ast::Index& node) override;
    std::any visit_index_list(ast::IndexList& node) override;
    std::any visit_index_item(ast::IndexItem& node) override;
    std::any visit_index_range(ast::IndexRange& ast) override;
    std::any visit_identifier(ast::Identifier& node) override;
    std::any visit_boolean_literal(ast::BooleanLiteral& node) override;
    std::any visit_integer_literal(ast::IntegerLiteral& node) override;
    std::any visit_float_literal(ast::FloatLiteral& node) override;

private:
    /**
     * Build a semantic type
     * It can be a simple type SemanticT, of size 1,
     * or an array type SemanticTArray, which size is given by the syntactic type
     */
    template <typename SemanticT, typename SemanticTArray, typename SyntacticT>
    types::Type build_semantic_type(const SyntacticT& type, std::string_view type_name) const {
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
    types::Type build_semantic_type(const SyntacticT& type) const {
        assert(!type.empty() && !type->name.empty());
        auto type_name = type->name->name;
        if (type_name == types::qubit_type_name) {
            return build_semantic_type<types::Qubit, types::QubitArray>(*type, types::qubit_type_name);
        }
        if (type_name == types::bit_type_name) {
            return build_semantic_type<types::Bit, types::BitArray>(*type, types::bit_type_name);
        }
        throw error::AnalysisError("unknown type \"" + type_name + "\"");
    }

    /**
     * Convenience function for visiting a global or a local block
     */
    template <typename Block>
    void visit_block(Block& block) {
        for (const auto& statement_ast : block.statements) {
            try {
                statement_ast->visit(*this);
            } catch (error::AnalysisError& err) {
                err.context(block);
                result_.errors.push_back(std::move(err));
            }
        }
    }

    /**
     * Convenience function for visiting a function call given the function's name and arguments
     */
    values::Value visit_function_call(
        const tree::One<ast::Identifier>& name, const tree::Maybe<ast::ExpressionList>& arguments);

    /**
     * Convenience function for visiting unary operators
     */
    std::any visit_unary_operator(const std::string& name, const tree::One<ast::Expression>& expression);

    /**
     * Convenience function for visiting binary operators
     */
    std::any visit_binary_operator(
        const std::string& name, const tree::One<ast::Expression>& lhs, const tree::One<ast::Expression>& rhs);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack.
     * Returns empty when the cast fails
     */
    template <class Type, class... TypeArgs>
    values::Value visit_as(ast::Expression& expression, TypeArgs... type_args) {
        return values::promote(std::any_cast<values::Value>(expression.visit(*this)), tree::make<Type>(type_args...));
    }

    /**
     * Shorthand for parsing an expression to a constant integer
     */
    primitives::Int visit_const_int(ast::Expression& expression);
};

}  // namespace cqasm::v3x::analyzer
