/** \file
 * Implementation for the abstract syntax tree node classes.
 */

#include "cqasm-annotations.hpp"
#include "v1/cqasm-ast-gen.hpp"

namespace cqasm {
namespace v1 {
namespace ast {

/**
 * Writes a debug dump of this node to the given stream.
 */
void Node::dump(std::ostream &out, int indent) {
    auto dumper = Dumper(out, indent);
    visit(dumper);
}

/**
 * Alternate debug dump that represents links and node uniqueness via sequence
 * number tags.
 */
void Node::dump_seq(std::ostream &out, int indent) {
    ::tree::base::PointerMap ids;
    ids.enable_exceptions = false;
    ids.add_ref(*this);
    find_reachable(ids);
    auto dumper = Dumper(out, indent, &ids);
    visit(dumper);
}

/**
 * Interprets this node to a node of type Add. Returns null if it has the wrong
 * type.
 */
Add *Node::as_add() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Add. Returns null if it has the wrong
 * type.
 */
const Add *Node::as_add() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Annotated. Returns null if it has the
 * wrong type.
 */
Annotated *Node::as_annotated() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Annotated. Returns null if it has the
 * wrong type.
 */
const Annotated *Node::as_annotated() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type AnnotationData. Returns null if it has
 * the wrong type.
 */
AnnotationData *Node::as_annotation_data() {
    return nullptr;
}

/**
 * Interprets this node to a node of type AnnotationData. Returns null if it has
 * the wrong type.
 */
const AnnotationData *Node::as_annotation_data() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ArithOp. Returns null if it has the
 * wrong type.
 */
ArithOp *Node::as_arith_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ArithOp. Returns null if it has the
 * wrong type.
 */
const ArithOp *Node::as_arith_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Assignment. Returns null if it has the
 * wrong type.
 */
Assignment *Node::as_assignment() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Assignment. Returns null if it has the
 * wrong type.
 */
const Assignment *Node::as_assignment() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BinaryOp. Returns null if it has the
 * wrong type.
 */
BinaryOp *Node::as_binary_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BinaryOp. Returns null if it has the
 * wrong type.
 */
const BinaryOp *Node::as_binary_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseAnd. Returns null if it has the
 * wrong type.
 */
BitwiseAnd *Node::as_bitwise_and() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseAnd. Returns null if it has the
 * wrong type.
 */
const BitwiseAnd *Node::as_bitwise_and() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseNot. Returns null if it has the
 * wrong type.
 */
BitwiseNot *Node::as_bitwise_not() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseNot. Returns null if it has the
 * wrong type.
 */
const BitwiseNot *Node::as_bitwise_not() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseOp. Returns null if it has the
 * wrong type.
 */
BitwiseOp *Node::as_bitwise_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseOp. Returns null if it has the
 * wrong type.
 */
const BitwiseOp *Node::as_bitwise_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseOr. Returns null if it has the
 * wrong type.
 */
BitwiseOr *Node::as_bitwise_or() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseOr. Returns null if it has the
 * wrong type.
 */
const BitwiseOr *Node::as_bitwise_or() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseXor. Returns null if it has the
 * wrong type.
 */
BitwiseXor *Node::as_bitwise_xor() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitwiseXor. Returns null if it has the
 * wrong type.
 */
const BitwiseXor *Node::as_bitwise_xor() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type BreakStatement. Returns null if it has
 * the wrong type.
 */
BreakStatement *Node::as_break_statement() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BreakStatement. Returns null if it has
 * the wrong type.
 */
const BreakStatement *Node::as_break_statement() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Bundle. Returns null if it has the
 * wrong type.
 */
Bundle *Node::as_bundle() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Bundle. Returns null if it has the
 * wrong type.
 */
const Bundle *Node::as_bundle() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpEq. Returns null if it has the
 * wrong type.
 */
CmpEq *Node::as_cmp_eq() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpEq. Returns null if it has the
 * wrong type.
 */
const CmpEq *Node::as_cmp_eq() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpGe. Returns null if it has the
 * wrong type.
 */
CmpGe *Node::as_cmp_ge() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpGe. Returns null if it has the
 * wrong type.
 */
const CmpGe *Node::as_cmp_ge() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpGt. Returns null if it has the
 * wrong type.
 */
CmpGt *Node::as_cmp_gt() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpGt. Returns null if it has the
 * wrong type.
 */
const CmpGt *Node::as_cmp_gt() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpLe. Returns null if it has the
 * wrong type.
 */
CmpLe *Node::as_cmp_le() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpLe. Returns null if it has the
 * wrong type.
 */
const CmpLe *Node::as_cmp_le() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpLt. Returns null if it has the
 * wrong type.
 */
CmpLt *Node::as_cmp_lt() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpLt. Returns null if it has the
 * wrong type.
 */
const CmpLt *Node::as_cmp_lt() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpNe. Returns null if it has the
 * wrong type.
 */
CmpNe *Node::as_cmp_ne() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpNe. Returns null if it has the
 * wrong type.
 */
const CmpNe *Node::as_cmp_ne() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpOp. Returns null if it has the
 * wrong type.
 */
CmpOp *Node::as_cmp_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type CmpOp. Returns null if it has the
 * wrong type.
 */
const CmpOp *Node::as_cmp_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ContinueStatement. Returns null if it
 * has the wrong type.
 */
ContinueStatement *Node::as_continue_statement() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ContinueStatement. Returns null if it
 * has the wrong type.
 */
const ContinueStatement *Node::as_continue_statement() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Divide. Returns null if it has the
 * wrong type.
 */
Divide *Node::as_divide() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Divide. Returns null if it has the
 * wrong type.
 */
const Divide *Node::as_divide() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousExpression. Returns null if
 * it has the wrong type.
 */
ErroneousExpression *Node::as_erroneous_expression() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousExpression. Returns null if
 * it has the wrong type.
 */
const ErroneousExpression *Node::as_erroneous_expression() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousProgram. Returns null if it
 * has the wrong type.
 */
ErroneousProgram *Node::as_erroneous_program() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousProgram. Returns null if it
 * has the wrong type.
 */
const ErroneousProgram *Node::as_erroneous_program() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousStatement. Returns null if it
 * has the wrong type.
 */
ErroneousStatement *Node::as_erroneous_statement() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErroneousStatement. Returns null if it
 * has the wrong type.
 */
const ErroneousStatement *Node::as_erroneous_statement() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Expression. Returns null if it has the
 * wrong type.
 */
Expression *Node::as_expression() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Expression. Returns null if it has the
 * wrong type.
 */
const Expression *Node::as_expression() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ExpressionList. Returns null if it has
 * the wrong type.
 */
ExpressionList *Node::as_expression_list() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ExpressionList. Returns null if it has
 * the wrong type.
 */
const ExpressionList *Node::as_expression_list() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type FloatLiteral. Returns null if it has
 * the wrong type.
 */
FloatLiteral *Node::as_float_literal() {
    return nullptr;
}

/**
 * Interprets this node to a node of type FloatLiteral. Returns null if it has
 * the wrong type.
 */
const FloatLiteral *Node::as_float_literal() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ForLoop. Returns null if it has the
 * wrong type.
 */
ForLoop *Node::as_for_loop() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ForLoop. Returns null if it has the
 * wrong type.
 */
const ForLoop *Node::as_for_loop() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ForeachLoop. Returns null if it has
 * the wrong type.
 */
ForeachLoop *Node::as_foreach_loop() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ForeachLoop. Returns null if it has
 * the wrong type.
 */
const ForeachLoop *Node::as_foreach_loop() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type FunctionCall. Returns null if it has
 * the wrong type.
 */
FunctionCall *Node::as_function_call() {
    return nullptr;
}

/**
 * Interprets this node to a node of type FunctionCall. Returns null if it has
 * the wrong type.
 */
const FunctionCall *Node::as_function_call() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Identifier. Returns null if it has the
 * wrong type.
 */
Identifier *Node::as_identifier() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Identifier. Returns null if it has the
 * wrong type.
 */
const Identifier *Node::as_identifier() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IfElse. Returns null if it has the
 * wrong type.
 */
IfElse *Node::as_if_else() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IfElse. Returns null if it has the
 * wrong type.
 */
const IfElse *Node::as_if_else() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IfElseBranch. Returns null if it has
 * the wrong type.
 */
IfElseBranch *Node::as_if_else_branch() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IfElseBranch. Returns null if it has
 * the wrong type.
 */
const IfElseBranch *Node::as_if_else_branch() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Index. Returns null if it has the
 * wrong type.
 */
Index *Node::as_index() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Index. Returns null if it has the
 * wrong type.
 */
const Index *Node::as_index() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexEntry. Returns null if it has the
 * wrong type.
 */
IndexEntry *Node::as_index_entry() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexEntry. Returns null if it has the
 * wrong type.
 */
const IndexEntry *Node::as_index_entry() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexItem. Returns null if it has the
 * wrong type.
 */
IndexItem *Node::as_index_item() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexItem. Returns null if it has the
 * wrong type.
 */
const IndexItem *Node::as_index_item() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexList. Returns null if it has the
 * wrong type.
 */
IndexList *Node::as_index_list() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexList. Returns null if it has the
 * wrong type.
 */
const IndexList *Node::as_index_list() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexRange. Returns null if it has the
 * wrong type.
 */
IndexRange *Node::as_index_range() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IndexRange. Returns null if it has the
 * wrong type.
 */
const IndexRange *Node::as_index_range() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Instruction. Returns null if it has
 * the wrong type.
 */
Instruction *Node::as_instruction() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Instruction. Returns null if it has
 * the wrong type.
 */
const Instruction *Node::as_instruction() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IntDivide. Returns null if it has the
 * wrong type.
 */
IntDivide *Node::as_int_divide() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IntDivide. Returns null if it has the
 * wrong type.
 */
const IntDivide *Node::as_int_divide() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type IntegerLiteral. Returns null if it has
 * the wrong type.
 */
IntegerLiteral *Node::as_integer_literal() {
    return nullptr;
}

/**
 * Interprets this node to a node of type IntegerLiteral. Returns null if it has
 * the wrong type.
 */
const IntegerLiteral *Node::as_integer_literal() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type JsonLiteral. Returns null if it has
 * the wrong type.
 */
JsonLiteral *Node::as_json_literal() {
    return nullptr;
}

/**
 * Interprets this node to a node of type JsonLiteral. Returns null if it has
 * the wrong type.
 */
const JsonLiteral *Node::as_json_literal() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalAnd. Returns null if it has the
 * wrong type.
 */
LogicalAnd *Node::as_logical_and() {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalAnd. Returns null if it has the
 * wrong type.
 */
const LogicalAnd *Node::as_logical_and() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalNot. Returns null if it has the
 * wrong type.
 */
LogicalNot *Node::as_logical_not() {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalNot. Returns null if it has the
 * wrong type.
 */
const LogicalNot *Node::as_logical_not() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalOp. Returns null if it has the
 * wrong type.
 */
LogicalOp *Node::as_logical_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalOp. Returns null if it has the
 * wrong type.
 */
const LogicalOp *Node::as_logical_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalOr. Returns null if it has the
 * wrong type.
 */
LogicalOr *Node::as_logical_or() {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalOr. Returns null if it has the
 * wrong type.
 */
const LogicalOr *Node::as_logical_or() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalXor. Returns null if it has the
 * wrong type.
 */
LogicalXor *Node::as_logical_xor() {
    return nullptr;
}

/**
 * Interprets this node to a node of type LogicalXor. Returns null if it has the
 * wrong type.
 */
const LogicalXor *Node::as_logical_xor() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Mapping. Returns null if it has the
 * wrong type.
 */
Mapping *Node::as_mapping() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Mapping. Returns null if it has the
 * wrong type.
 */
const Mapping *Node::as_mapping() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type MatrixLiteral. Returns null if it has
 * the wrong type.
 */
MatrixLiteral *Node::as_matrix_literal() {
    return nullptr;
}

/**
 * Interprets this node to a node of type MatrixLiteral. Returns null if it has
 * the wrong type.
 */
const MatrixLiteral *Node::as_matrix_literal() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Modulo. Returns null if it has the
 * wrong type.
 */
Modulo *Node::as_modulo() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Modulo. Returns null if it has the
 * wrong type.
 */
const Modulo *Node::as_modulo() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Multiply. Returns null if it has the
 * wrong type.
 */
Multiply *Node::as_multiply() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Multiply. Returns null if it has the
 * wrong type.
 */
const Multiply *Node::as_multiply() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Negate. Returns null if it has the
 * wrong type.
 */
Negate *Node::as_negate() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Negate. Returns null if it has the
 * wrong type.
 */
const Negate *Node::as_negate() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Power. Returns null if it has the
 * wrong type.
 */
Power *Node::as_power() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Power. Returns null if it has the
 * wrong type.
 */
const Power *Node::as_power() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Program. Returns null if it has the
 * wrong type.
 */
Program *Node::as_program() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Program. Returns null if it has the
 * wrong type.
 */
const Program *Node::as_program() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type RepeatUntilLoop. Returns null if it
 * has the wrong type.
 */
RepeatUntilLoop *Node::as_repeat_until_loop() {
    return nullptr;
}

/**
 * Interprets this node to a node of type RepeatUntilLoop. Returns null if it
 * has the wrong type.
 */
const RepeatUntilLoop *Node::as_repeat_until_loop() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Root. Returns null if it has the wrong
 * type.
 */
Root *Node::as_root() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Root. Returns null if it has the wrong
 * type.
 */
const Root *Node::as_root() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftLeft. Returns null if it has the
 * wrong type.
 */
ShiftLeft *Node::as_shift_left() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftLeft. Returns null if it has the
 * wrong type.
 */
const ShiftLeft *Node::as_shift_left() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftOp. Returns null if it has the
 * wrong type.
 */
ShiftOp *Node::as_shift_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftOp. Returns null if it has the
 * wrong type.
 */
const ShiftOp *Node::as_shift_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftRightArith. Returns null if it
 * has the wrong type.
 */
ShiftRightArith *Node::as_shift_right_arith() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftRightArith. Returns null if it
 * has the wrong type.
 */
const ShiftRightArith *Node::as_shift_right_arith() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftRightLogic. Returns null if it
 * has the wrong type.
 */
ShiftRightLogic *Node::as_shift_right_logic() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ShiftRightLogic. Returns null if it
 * has the wrong type.
 */
const ShiftRightLogic *Node::as_shift_right_logic() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Statement. Returns null if it has the
 * wrong type.
 */
Statement *Node::as_statement() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Statement. Returns null if it has the
 * wrong type.
 */
const Statement *Node::as_statement() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type StatementList. Returns null if it has
 * the wrong type.
 */
StatementList *Node::as_statement_list() {
    return nullptr;
}

/**
 * Interprets this node to a node of type StatementList. Returns null if it has
 * the wrong type.
 */
const StatementList *Node::as_statement_list() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type StringLiteral. Returns null if it has
 * the wrong type.
 */
StringLiteral *Node::as_string_literal() {
    return nullptr;
}

/**
 * Interprets this node to a node of type StringLiteral. Returns null if it has
 * the wrong type.
 */
const StringLiteral *Node::as_string_literal() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Structured. Returns null if it has the
 * wrong type.
 */
Structured *Node::as_structured() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Structured. Returns null if it has the
 * wrong type.
 */
const Structured *Node::as_structured() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Subcircuit. Returns null if it has the
 * wrong type.
 */
Subcircuit *Node::as_subcircuit() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Subcircuit. Returns null if it has the
 * wrong type.
 */
const Subcircuit *Node::as_subcircuit() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Subtract. Returns null if it has the
 * wrong type.
 */
Subtract *Node::as_subtract() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Subtract. Returns null if it has the
 * wrong type.
 */
const Subtract *Node::as_subtract() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type TernaryCond. Returns null if it has
 * the wrong type.
 */
TernaryCond *Node::as_ternary_cond() {
    return nullptr;
}

/**
 * Interprets this node to a node of type TernaryCond. Returns null if it has
 * the wrong type.
 */
const TernaryCond *Node::as_ternary_cond() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type UnaryOp. Returns null if it has the
 * wrong type.
 */
UnaryOp *Node::as_unary_op() {
    return nullptr;
}

/**
 * Interprets this node to a node of type UnaryOp. Returns null if it has the
 * wrong type.
 */
const UnaryOp *Node::as_unary_op() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Variables. Returns null if it has the
 * wrong type.
 */
Variables *Node::as_variables() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Variables. Returns null if it has the
 * wrong type.
 */
const Variables *Node::as_variables() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Version. Returns null if it has the
 * wrong type.
 */
Version *Node::as_version() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Version. Returns null if it has the
 * wrong type.
 */
const Version *Node::as_version() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type WhileLoop. Returns null if it has the
 * wrong type.
 */
WhileLoop *Node::as_while_loop() {
    return nullptr;
}

/**
 * Interprets this node to a node of type WhileLoop. Returns null if it has the
 * wrong type.
 */
const WhileLoop *Node::as_while_loop() const {
    return nullptr;
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Node> Node::deserialize(
    const ::tree::cbor::MapReader &map,
    ::tree::base::IdentifierMap &ids
) {
    auto type = map.at("@t").as_string();
    if (type == "Add") return Add::deserialize(map, ids);
    if (type == "AnnotationData") return AnnotationData::deserialize(map, ids);
    if (type == "Assignment") return Assignment::deserialize(map, ids);
    if (type == "BitwiseAnd") return BitwiseAnd::deserialize(map, ids);
    if (type == "BitwiseNot") return BitwiseNot::deserialize(map, ids);
    if (type == "BitwiseOr") return BitwiseOr::deserialize(map, ids);
    if (type == "BitwiseXor") return BitwiseXor::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "Bundle") return Bundle::deserialize(map, ids);
    if (type == "CmpEq") return CmpEq::deserialize(map, ids);
    if (type == "CmpGe") return CmpGe::deserialize(map, ids);
    if (type == "CmpGt") return CmpGt::deserialize(map, ids);
    if (type == "CmpLe") return CmpLe::deserialize(map, ids);
    if (type == "CmpLt") return CmpLt::deserialize(map, ids);
    if (type == "CmpNe") return CmpNe::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    if (type == "Divide") return Divide::deserialize(map, ids);
    if (type == "ErroneousExpression") return ErroneousExpression::deserialize(map, ids);
    if (type == "ErroneousProgram") return ErroneousProgram::deserialize(map, ids);
    if (type == "ErroneousStatement") return ErroneousStatement::deserialize(map, ids);
    if (type == "ExpressionList") return ExpressionList::deserialize(map, ids);
    if (type == "FloatLiteral") return FloatLiteral::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "FunctionCall") return FunctionCall::deserialize(map, ids);
    if (type == "Identifier") return Identifier::deserialize(map, ids);
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "IfElseBranch") return IfElseBranch::deserialize(map, ids);
    if (type == "Index") return Index::deserialize(map, ids);
    if (type == "IndexItem") return IndexItem::deserialize(map, ids);
    if (type == "IndexList") return IndexList::deserialize(map, ids);
    if (type == "IndexRange") return IndexRange::deserialize(map, ids);
    if (type == "Instruction") return Instruction::deserialize(map, ids);
    if (type == "IntDivide") return IntDivide::deserialize(map, ids);
    if (type == "IntegerLiteral") return IntegerLiteral::deserialize(map, ids);
    if (type == "JsonLiteral") return JsonLiteral::deserialize(map, ids);
    if (type == "LogicalAnd") return LogicalAnd::deserialize(map, ids);
    if (type == "LogicalNot") return LogicalNot::deserialize(map, ids);
    if (type == "LogicalOr") return LogicalOr::deserialize(map, ids);
    if (type == "LogicalXor") return LogicalXor::deserialize(map, ids);
    if (type == "Mapping") return Mapping::deserialize(map, ids);
    if (type == "MatrixLiteral") return MatrixLiteral::deserialize(map, ids);
    if (type == "Modulo") return Modulo::deserialize(map, ids);
    if (type == "Multiply") return Multiply::deserialize(map, ids);
    if (type == "Negate") return Negate::deserialize(map, ids);
    if (type == "Power") return Power::deserialize(map, ids);
    if (type == "Program") return Program::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "ShiftLeft") return ShiftLeft::deserialize(map, ids);
    if (type == "ShiftRightArith") return ShiftRightArith::deserialize(map, ids);
    if (type == "ShiftRightLogic") return ShiftRightLogic::deserialize(map, ids);
    if (type == "StatementList") return StatementList::deserialize(map, ids);
    if (type == "StringLiteral") return StringLiteral::deserialize(map, ids);
    if (type == "Subcircuit") return Subcircuit::deserialize(map, ids);
    if (type == "Subtract") return Subtract::deserialize(map, ids);
    if (type == "TernaryCond") return TernaryCond::deserialize(map, ids);
    if (type == "Variables") return Variables::deserialize(map, ids);
    if (type == "Version") return Version::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Interprets this node to a node of type Expression. Returns null if it has the
 * wrong type.
 */
Expression *Expression::as_expression() {
    return dynamic_cast<Expression*>(this);
}

/**
 * Interprets this node to a node of type Expression. Returns null if it has the
 * wrong type.
 */
const Expression *Expression::as_expression() const {
    return dynamic_cast<const Expression*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Expression> Expression::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "IntegerLiteral") return IntegerLiteral::deserialize(map, ids);
    if (type == "FloatLiteral") return FloatLiteral::deserialize(map, ids);
    if (type == "Identifier") return Identifier::deserialize(map, ids);
    if (type == "MatrixLiteral") return MatrixLiteral::deserialize(map, ids);
    if (type == "StringLiteral") return StringLiteral::deserialize(map, ids);
    if (type == "JsonLiteral") return JsonLiteral::deserialize(map, ids);
    if (type == "FunctionCall") return FunctionCall::deserialize(map, ids);
    if (type == "Index") return Index::deserialize(map, ids);
    if (type == "Negate") return Negate::deserialize(map, ids);
    if (type == "BitwiseNot") return BitwiseNot::deserialize(map, ids);
    if (type == "LogicalNot") return LogicalNot::deserialize(map, ids);
    if (type == "Power") return Power::deserialize(map, ids);
    if (type == "Multiply") return Multiply::deserialize(map, ids);
    if (type == "Divide") return Divide::deserialize(map, ids);
    if (type == "IntDivide") return IntDivide::deserialize(map, ids);
    if (type == "Modulo") return Modulo::deserialize(map, ids);
    if (type == "Add") return Add::deserialize(map, ids);
    if (type == "Subtract") return Subtract::deserialize(map, ids);
    if (type == "ShiftLeft") return ShiftLeft::deserialize(map, ids);
    if (type == "ShiftRightArith") return ShiftRightArith::deserialize(map, ids);
    if (type == "ShiftRightLogic") return ShiftRightLogic::deserialize(map, ids);
    if (type == "CmpEq") return CmpEq::deserialize(map, ids);
    if (type == "CmpNe") return CmpNe::deserialize(map, ids);
    if (type == "CmpGt") return CmpGt::deserialize(map, ids);
    if (type == "CmpGe") return CmpGe::deserialize(map, ids);
    if (type == "CmpLt") return CmpLt::deserialize(map, ids);
    if (type == "CmpLe") return CmpLe::deserialize(map, ids);
    if (type == "BitwiseAnd") return BitwiseAnd::deserialize(map, ids);
    if (type == "BitwiseXor") return BitwiseXor::deserialize(map, ids);
    if (type == "BitwiseOr") return BitwiseOr::deserialize(map, ids);
    if (type == "LogicalAnd") return LogicalAnd::deserialize(map, ids);
    if (type == "LogicalXor") return LogicalXor::deserialize(map, ids);
    if (type == "LogicalOr") return LogicalOr::deserialize(map, ids);
    if (type == "TernaryCond") return TernaryCond::deserialize(map, ids);
    if (type == "ErroneousExpression") return ErroneousExpression::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
BinaryOp::BinaryOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : Expression(), lhs(lhs), rhs(rhs)
{}

/**
 * Interprets this node to a node of type BinaryOp. Returns null if it has the
 * wrong type.
 */
BinaryOp *BinaryOp::as_binary_op() {
    return dynamic_cast<BinaryOp*>(this);
}

/**
 * Interprets this node to a node of type BinaryOp. Returns null if it has the
 * wrong type.
 */
const BinaryOp *BinaryOp::as_binary_op() const {
    return dynamic_cast<const BinaryOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BinaryOp> BinaryOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Power") return Power::deserialize(map, ids);
    if (type == "Multiply") return Multiply::deserialize(map, ids);
    if (type == "Divide") return Divide::deserialize(map, ids);
    if (type == "IntDivide") return IntDivide::deserialize(map, ids);
    if (type == "Modulo") return Modulo::deserialize(map, ids);
    if (type == "Add") return Add::deserialize(map, ids);
    if (type == "Subtract") return Subtract::deserialize(map, ids);
    if (type == "ShiftLeft") return ShiftLeft::deserialize(map, ids);
    if (type == "ShiftRightArith") return ShiftRightArith::deserialize(map, ids);
    if (type == "ShiftRightLogic") return ShiftRightLogic::deserialize(map, ids);
    if (type == "CmpEq") return CmpEq::deserialize(map, ids);
    if (type == "CmpNe") return CmpNe::deserialize(map, ids);
    if (type == "CmpGt") return CmpGt::deserialize(map, ids);
    if (type == "CmpGe") return CmpGe::deserialize(map, ids);
    if (type == "CmpLt") return CmpLt::deserialize(map, ids);
    if (type == "CmpLe") return CmpLe::deserialize(map, ids);
    if (type == "BitwiseAnd") return BitwiseAnd::deserialize(map, ids);
    if (type == "BitwiseXor") return BitwiseXor::deserialize(map, ids);
    if (type == "BitwiseOr") return BitwiseOr::deserialize(map, ids);
    if (type == "LogicalAnd") return LogicalAnd::deserialize(map, ids);
    if (type == "LogicalXor") return LogicalXor::deserialize(map, ids);
    if (type == "LogicalOr") return LogicalOr::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
ArithOp::ArithOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : BinaryOp(lhs, rhs)
{}

/**
 * Interprets this node to a node of type ArithOp. Returns null if it has the
 * wrong type.
 */
ArithOp *ArithOp::as_arith_op() {
    return dynamic_cast<ArithOp*>(this);
}

/**
 * Interprets this node to a node of type ArithOp. Returns null if it has the
 * wrong type.
 */
const ArithOp *ArithOp::as_arith_op() const {
    return dynamic_cast<const ArithOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ArithOp> ArithOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Power") return Power::deserialize(map, ids);
    if (type == "Multiply") return Multiply::deserialize(map, ids);
    if (type == "Divide") return Divide::deserialize(map, ids);
    if (type == "IntDivide") return IntDivide::deserialize(map, ids);
    if (type == "Modulo") return Modulo::deserialize(map, ids);
    if (type == "Add") return Add::deserialize(map, ids);
    if (type == "Subtract") return Subtract::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
Add::Add(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Add::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Add` is complete/fully defined.
 */
void Add::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Add::type() const {
    return NodeType::Add;
}

/**
 * Helper method for visiting nodes.
 */
void Add::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_add(*this, retval);
}

/**
 * Interprets this node to a node of type Add. Returns null if it has the wrong
 * type.
 */
Add *Add::as_add() {
    return dynamic_cast<Add*>(this);
}

/**
 * Interprets this node to a node of type Add. Returns null if it has the wrong
 * type.
 */
const Add *Add::as_add() const {
    return dynamic_cast<const Add*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Add::copy() const {
    return cqasm::tree::make<Add>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Add::clone() const {
    auto node = cqasm::tree::make<Add>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Add::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Add) return false;
    auto rhsc = dynamic_cast<const Add&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Add::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Add) return false;
    auto rhsc = dynamic_cast<const Add&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Add::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Add");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Add> Add::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Add") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Add>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Annotated::Annotated(const Any<AnnotationData> &annotations)
    : annotations(annotations)
{}

/**
 * Interprets this node to a node of type Annotated. Returns null if it has the
 * wrong type.
 */
Annotated *Annotated::as_annotated() {
    return dynamic_cast<Annotated*>(this);
}

/**
 * Interprets this node to a node of type Annotated. Returns null if it has the
 * wrong type.
 */
const Annotated *Annotated::as_annotated() const {
    return dynamic_cast<const Annotated*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Annotated> Annotated::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Instruction") return Instruction::deserialize(map, ids);
    if (type == "Bundle") return Bundle::deserialize(map, ids);
    if (type == "Mapping") return Mapping::deserialize(map, ids);
    if (type == "Variables") return Variables::deserialize(map, ids);
    if (type == "Subcircuit") return Subcircuit::deserialize(map, ids);
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    if (type == "ErroneousStatement") return ErroneousStatement::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
AnnotationData::AnnotationData(const One<Identifier> &interface, const One<Identifier> &operation, const One<ExpressionList> &operands)
    : interface(interface), operation(operation), operands(operands)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void AnnotationData::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    interface.find_reachable(map);
    operation.find_reachable(map);
    operands.find_reachable(map);
}

/**
 * Returns whether this `AnnotationData` is complete/fully defined.
 */
void AnnotationData::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    interface.check_complete(map);
    operation.check_complete(map);
    operands.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType AnnotationData::type() const {
    return NodeType::AnnotationData;
}

/**
 * Helper method for visiting nodes.
 */
void AnnotationData::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_annotation_data(*this, retval);
}

/**
 * Interprets this node to a node of type AnnotationData. Returns null if it has
 * the wrong type.
 */
AnnotationData *AnnotationData::as_annotation_data() {
    return dynamic_cast<AnnotationData*>(this);
}

/**
 * Interprets this node to a node of type AnnotationData. Returns null if it has
 * the wrong type.
 */
const AnnotationData *AnnotationData::as_annotation_data() const {
    return dynamic_cast<const AnnotationData*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> AnnotationData::copy() const {
    return cqasm::tree::make<AnnotationData>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> AnnotationData::clone() const {
    auto node = cqasm::tree::make<AnnotationData>(*this);
    node->interface = this->interface.clone();
    node->operation = this->operation.clone();
    node->operands = this->operands.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool AnnotationData::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::AnnotationData) return false;
    auto rhsc = dynamic_cast<const AnnotationData&>(rhs);
    if (!this->interface.equals(rhsc.interface)) return false;
    if (!this->operation.equals(rhsc.operation)) return false;
    if (!this->operands.equals(rhsc.operands)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool AnnotationData::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::AnnotationData) return false;
    auto rhsc = dynamic_cast<const AnnotationData&>(rhs);
    if (this->interface != rhsc.interface) return false;
    if (this->operation != rhsc.operation) return false;
    if (this->operands != rhsc.operands) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void AnnotationData::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "AnnotationData");
    auto submap = map.append_map("interface");
    interface.serialize(submap, ids);
    submap.close();
    submap = map.append_map("operation");
    operation.serialize(submap, ids);
    submap.close();
    submap = map.append_map("operands");
    operands.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<AnnotationData> AnnotationData::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "AnnotationData") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<AnnotationData>(
        One<Identifier>(map.at("interface").as_map(), ids),
        One<Identifier>(map.at("operation").as_map(), ids),
        One<ExpressionList>(map.at("operands").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Assignment::Assignment(const One<Expression> &lhs, const One<Expression> &rhs)
    : lhs(lhs), rhs(rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Assignment::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Assignment` is complete/fully defined.
 */
void Assignment::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Assignment::type() const {
    return NodeType::Assignment;
}

/**
 * Helper method for visiting nodes.
 */
void Assignment::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_assignment(*this, retval);
}

/**
 * Interprets this node to a node of type Assignment. Returns null if it has the
 * wrong type.
 */
Assignment *Assignment::as_assignment() {
    return dynamic_cast<Assignment*>(this);
}

/**
 * Interprets this node to a node of type Assignment. Returns null if it has the
 * wrong type.
 */
const Assignment *Assignment::as_assignment() const {
    return dynamic_cast<const Assignment*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Assignment::copy() const {
    return cqasm::tree::make<Assignment>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Assignment::clone() const {
    auto node = cqasm::tree::make<Assignment>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Assignment::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Assignment) return false;
    auto rhsc = dynamic_cast<const Assignment&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Assignment::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Assignment) return false;
    auto rhsc = dynamic_cast<const Assignment&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Assignment::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Assignment");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Assignment> Assignment::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Assignment") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Assignment>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
BitwiseOp::BitwiseOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : BinaryOp(lhs, rhs)
{}

/**
 * Interprets this node to a node of type BitwiseOp. Returns null if it has the
 * wrong type.
 */
BitwiseOp *BitwiseOp::as_bitwise_op() {
    return dynamic_cast<BitwiseOp*>(this);
}

/**
 * Interprets this node to a node of type BitwiseOp. Returns null if it has the
 * wrong type.
 */
const BitwiseOp *BitwiseOp::as_bitwise_op() const {
    return dynamic_cast<const BitwiseOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitwiseOp> BitwiseOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "BitwiseAnd") return BitwiseAnd::deserialize(map, ids);
    if (type == "BitwiseXor") return BitwiseXor::deserialize(map, ids);
    if (type == "BitwiseOr") return BitwiseOr::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
BitwiseAnd::BitwiseAnd(const One<Expression> &lhs, const One<Expression> &rhs)
    : BitwiseOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BitwiseAnd::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `BitwiseAnd` is complete/fully defined.
 */
void BitwiseAnd::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BitwiseAnd::type() const {
    return NodeType::BitwiseAnd;
}

/**
 * Helper method for visiting nodes.
 */
void BitwiseAnd::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bitwise_and(*this, retval);
}

/**
 * Interprets this node to a node of type BitwiseAnd. Returns null if it has the
 * wrong type.
 */
BitwiseAnd *BitwiseAnd::as_bitwise_and() {
    return dynamic_cast<BitwiseAnd*>(this);
}

/**
 * Interprets this node to a node of type BitwiseAnd. Returns null if it has the
 * wrong type.
 */
const BitwiseAnd *BitwiseAnd::as_bitwise_and() const {
    return dynamic_cast<const BitwiseAnd*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BitwiseAnd::copy() const {
    return cqasm::tree::make<BitwiseAnd>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BitwiseAnd::clone() const {
    auto node = cqasm::tree::make<BitwiseAnd>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BitwiseAnd::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseAnd) return false;
    auto rhsc = dynamic_cast<const BitwiseAnd&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BitwiseAnd::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseAnd) return false;
    auto rhsc = dynamic_cast<const BitwiseAnd&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BitwiseAnd::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BitwiseAnd");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitwiseAnd> BitwiseAnd::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BitwiseAnd") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BitwiseAnd>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
UnaryOp::UnaryOp(const One<Expression> &expr)
    : Expression(), expr(expr)
{}

/**
 * Interprets this node to a node of type UnaryOp. Returns null if it has the
 * wrong type.
 */
UnaryOp *UnaryOp::as_unary_op() {
    return dynamic_cast<UnaryOp*>(this);
}

/**
 * Interprets this node to a node of type UnaryOp. Returns null if it has the
 * wrong type.
 */
const UnaryOp *UnaryOp::as_unary_op() const {
    return dynamic_cast<const UnaryOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<UnaryOp> UnaryOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Negate") return Negate::deserialize(map, ids);
    if (type == "BitwiseNot") return BitwiseNot::deserialize(map, ids);
    if (type == "LogicalNot") return LogicalNot::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
BitwiseNot::BitwiseNot(const One<Expression> &expr)
    : UnaryOp(expr)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BitwiseNot::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    expr.find_reachable(map);
}

/**
 * Returns whether this `BitwiseNot` is complete/fully defined.
 */
void BitwiseNot::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    expr.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BitwiseNot::type() const {
    return NodeType::BitwiseNot;
}

/**
 * Helper method for visiting nodes.
 */
void BitwiseNot::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bitwise_not(*this, retval);
}

/**
 * Interprets this node to a node of type BitwiseNot. Returns null if it has the
 * wrong type.
 */
BitwiseNot *BitwiseNot::as_bitwise_not() {
    return dynamic_cast<BitwiseNot*>(this);
}

/**
 * Interprets this node to a node of type BitwiseNot. Returns null if it has the
 * wrong type.
 */
const BitwiseNot *BitwiseNot::as_bitwise_not() const {
    return dynamic_cast<const BitwiseNot*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BitwiseNot::copy() const {
    return cqasm::tree::make<BitwiseNot>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BitwiseNot::clone() const {
    auto node = cqasm::tree::make<BitwiseNot>(*this);
    node->expr = this->expr.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BitwiseNot::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseNot) return false;
    auto rhsc = dynamic_cast<const BitwiseNot&>(rhs);
    if (!this->expr.equals(rhsc.expr)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BitwiseNot::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseNot) return false;
    auto rhsc = dynamic_cast<const BitwiseNot&>(rhs);
    if (this->expr != rhsc.expr) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BitwiseNot::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BitwiseNot");
    auto submap = map.append_map("expr");
    expr.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitwiseNot> BitwiseNot::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BitwiseNot") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BitwiseNot>(
        One<Expression>(map.at("expr").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
BitwiseOr::BitwiseOr(const One<Expression> &lhs, const One<Expression> &rhs)
    : BitwiseOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BitwiseOr::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `BitwiseOr` is complete/fully defined.
 */
void BitwiseOr::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BitwiseOr::type() const {
    return NodeType::BitwiseOr;
}

/**
 * Helper method for visiting nodes.
 */
void BitwiseOr::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bitwise_or(*this, retval);
}

/**
 * Interprets this node to a node of type BitwiseOr. Returns null if it has the
 * wrong type.
 */
BitwiseOr *BitwiseOr::as_bitwise_or() {
    return dynamic_cast<BitwiseOr*>(this);
}

/**
 * Interprets this node to a node of type BitwiseOr. Returns null if it has the
 * wrong type.
 */
const BitwiseOr *BitwiseOr::as_bitwise_or() const {
    return dynamic_cast<const BitwiseOr*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BitwiseOr::copy() const {
    return cqasm::tree::make<BitwiseOr>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BitwiseOr::clone() const {
    auto node = cqasm::tree::make<BitwiseOr>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BitwiseOr::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseOr) return false;
    auto rhsc = dynamic_cast<const BitwiseOr&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BitwiseOr::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseOr) return false;
    auto rhsc = dynamic_cast<const BitwiseOr&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BitwiseOr::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BitwiseOr");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitwiseOr> BitwiseOr::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BitwiseOr") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BitwiseOr>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
BitwiseXor::BitwiseXor(const One<Expression> &lhs, const One<Expression> &rhs)
    : BitwiseOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BitwiseXor::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `BitwiseXor` is complete/fully defined.
 */
void BitwiseXor::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BitwiseXor::type() const {
    return NodeType::BitwiseXor;
}

/**
 * Helper method for visiting nodes.
 */
void BitwiseXor::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bitwise_xor(*this, retval);
}

/**
 * Interprets this node to a node of type BitwiseXor. Returns null if it has the
 * wrong type.
 */
BitwiseXor *BitwiseXor::as_bitwise_xor() {
    return dynamic_cast<BitwiseXor*>(this);
}

/**
 * Interprets this node to a node of type BitwiseXor. Returns null if it has the
 * wrong type.
 */
const BitwiseXor *BitwiseXor::as_bitwise_xor() const {
    return dynamic_cast<const BitwiseXor*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BitwiseXor::copy() const {
    return cqasm::tree::make<BitwiseXor>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BitwiseXor::clone() const {
    auto node = cqasm::tree::make<BitwiseXor>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BitwiseXor::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseXor) return false;
    auto rhsc = dynamic_cast<const BitwiseXor&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BitwiseXor::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BitwiseXor) return false;
    auto rhsc = dynamic_cast<const BitwiseXor&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BitwiseXor::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BitwiseXor");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitwiseXor> BitwiseXor::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BitwiseXor") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BitwiseXor>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Statement::Statement(const Any<AnnotationData> &annotations)
    : Annotated(annotations)
{}

/**
 * Interprets this node to a node of type Statement. Returns null if it has the
 * wrong type.
 */
Statement *Statement::as_statement() {
    return dynamic_cast<Statement*>(this);
}

/**
 * Interprets this node to a node of type Statement. Returns null if it has the
 * wrong type.
 */
const Statement *Statement::as_statement() const {
    return dynamic_cast<const Statement*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Statement> Statement::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Bundle") return Bundle::deserialize(map, ids);
    if (type == "Mapping") return Mapping::deserialize(map, ids);
    if (type == "Variables") return Variables::deserialize(map, ids);
    if (type == "Subcircuit") return Subcircuit::deserialize(map, ids);
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    if (type == "ErroneousStatement") return ErroneousStatement::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
Structured::Structured(const Any<AnnotationData> &annotations)
    : Statement(annotations)
{}

/**
 * Interprets this node to a node of type Structured. Returns null if it has the
 * wrong type.
 */
Structured *Structured::as_structured() {
    return dynamic_cast<Structured*>(this);
}

/**
 * Interprets this node to a node of type Structured. Returns null if it has the
 * wrong type.
 */
const Structured *Structured::as_structured() const {
    return dynamic_cast<const Structured*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Structured> Structured::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
BreakStatement::BreakStatement(const Any<AnnotationData> &annotations)
    : Structured(annotations)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BreakStatement::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    annotations.find_reachable(map);
}

/**
 * Returns whether this `BreakStatement` is complete/fully defined.
 */
void BreakStatement::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BreakStatement::type() const {
    return NodeType::BreakStatement;
}

/**
 * Helper method for visiting nodes.
 */
void BreakStatement::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_break_statement(*this, retval);
}

/**
 * Interprets this node to a node of type BreakStatement. Returns null if it has
 * the wrong type.
 */
BreakStatement *BreakStatement::as_break_statement() {
    return dynamic_cast<BreakStatement*>(this);
}

/**
 * Interprets this node to a node of type BreakStatement. Returns null if it has
 * the wrong type.
 */
const BreakStatement *BreakStatement::as_break_statement() const {
    return dynamic_cast<const BreakStatement*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BreakStatement::copy() const {
    return cqasm::tree::make<BreakStatement>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BreakStatement::clone() const {
    auto node = cqasm::tree::make<BreakStatement>(*this);
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BreakStatement::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BreakStatement) return false;
    auto rhsc = dynamic_cast<const BreakStatement&>(rhs);
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BreakStatement::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BreakStatement) return false;
    auto rhsc = dynamic_cast<const BreakStatement&>(rhs);
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BreakStatement::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BreakStatement");
    auto submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BreakStatement> BreakStatement::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BreakStatement") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BreakStatement>(
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Bundle::Bundle(const Many<Instruction> &items, const Any<AnnotationData> &annotations)
    : Statement(annotations), items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Bundle::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    items.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Bundle` is complete/fully defined.
 */
void Bundle::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    items.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Bundle::type() const {
    return NodeType::Bundle;
}

/**
 * Helper method for visiting nodes.
 */
void Bundle::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bundle(*this, retval);
}

/**
 * Interprets this node to a node of type Bundle. Returns null if it has the
 * wrong type.
 */
Bundle *Bundle::as_bundle() {
    return dynamic_cast<Bundle*>(this);
}

/**
 * Interprets this node to a node of type Bundle. Returns null if it has the
 * wrong type.
 */
const Bundle *Bundle::as_bundle() const {
    return dynamic_cast<const Bundle*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Bundle::copy() const {
    return cqasm::tree::make<Bundle>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Bundle::clone() const {
    auto node = cqasm::tree::make<Bundle>(*this);
    node->items = this->items.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Bundle::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Bundle) return false;
    auto rhsc = dynamic_cast<const Bundle&>(rhs);
    if (!this->items.equals(rhsc.items)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Bundle::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Bundle) return false;
    auto rhsc = dynamic_cast<const Bundle&>(rhs);
    if (this->items != rhsc.items) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Bundle::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Bundle");
    auto submap = map.append_map("items");
    items.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Bundle> Bundle::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Bundle") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Bundle>(
        Many<Instruction>(map.at("items").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpOp::CmpOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : BinaryOp(lhs, rhs)
{}

/**
 * Interprets this node to a node of type CmpOp. Returns null if it has the
 * wrong type.
 */
CmpOp *CmpOp::as_cmp_op() {
    return dynamic_cast<CmpOp*>(this);
}

/**
 * Interprets this node to a node of type CmpOp. Returns null if it has the
 * wrong type.
 */
const CmpOp *CmpOp::as_cmp_op() const {
    return dynamic_cast<const CmpOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpOp> CmpOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "CmpEq") return CmpEq::deserialize(map, ids);
    if (type == "CmpNe") return CmpNe::deserialize(map, ids);
    if (type == "CmpGt") return CmpGt::deserialize(map, ids);
    if (type == "CmpGe") return CmpGe::deserialize(map, ids);
    if (type == "CmpLt") return CmpLt::deserialize(map, ids);
    if (type == "CmpLe") return CmpLe::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
CmpEq::CmpEq(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpEq::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpEq` is complete/fully defined.
 */
void CmpEq::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpEq::type() const {
    return NodeType::CmpEq;
}

/**
 * Helper method for visiting nodes.
 */
void CmpEq::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_eq(*this, retval);
}

/**
 * Interprets this node to a node of type CmpEq. Returns null if it has the
 * wrong type.
 */
CmpEq *CmpEq::as_cmp_eq() {
    return dynamic_cast<CmpEq*>(this);
}

/**
 * Interprets this node to a node of type CmpEq. Returns null if it has the
 * wrong type.
 */
const CmpEq *CmpEq::as_cmp_eq() const {
    return dynamic_cast<const CmpEq*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpEq::copy() const {
    return cqasm::tree::make<CmpEq>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpEq::clone() const {
    auto node = cqasm::tree::make<CmpEq>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpEq::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpEq) return false;
    auto rhsc = dynamic_cast<const CmpEq&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpEq::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpEq) return false;
    auto rhsc = dynamic_cast<const CmpEq&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpEq::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpEq");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpEq> CmpEq::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpEq") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpEq>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpGe::CmpGe(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpGe::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpGe` is complete/fully defined.
 */
void CmpGe::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpGe::type() const {
    return NodeType::CmpGe;
}

/**
 * Helper method for visiting nodes.
 */
void CmpGe::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_ge(*this, retval);
}

/**
 * Interprets this node to a node of type CmpGe. Returns null if it has the
 * wrong type.
 */
CmpGe *CmpGe::as_cmp_ge() {
    return dynamic_cast<CmpGe*>(this);
}

/**
 * Interprets this node to a node of type CmpGe. Returns null if it has the
 * wrong type.
 */
const CmpGe *CmpGe::as_cmp_ge() const {
    return dynamic_cast<const CmpGe*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpGe::copy() const {
    return cqasm::tree::make<CmpGe>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpGe::clone() const {
    auto node = cqasm::tree::make<CmpGe>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpGe::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpGe) return false;
    auto rhsc = dynamic_cast<const CmpGe&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpGe::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpGe) return false;
    auto rhsc = dynamic_cast<const CmpGe&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpGe::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpGe");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpGe> CmpGe::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpGe") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpGe>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpGt::CmpGt(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpGt::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpGt` is complete/fully defined.
 */
void CmpGt::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpGt::type() const {
    return NodeType::CmpGt;
}

/**
 * Helper method for visiting nodes.
 */
void CmpGt::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_gt(*this, retval);
}

/**
 * Interprets this node to a node of type CmpGt. Returns null if it has the
 * wrong type.
 */
CmpGt *CmpGt::as_cmp_gt() {
    return dynamic_cast<CmpGt*>(this);
}

/**
 * Interprets this node to a node of type CmpGt. Returns null if it has the
 * wrong type.
 */
const CmpGt *CmpGt::as_cmp_gt() const {
    return dynamic_cast<const CmpGt*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpGt::copy() const {
    return cqasm::tree::make<CmpGt>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpGt::clone() const {
    auto node = cqasm::tree::make<CmpGt>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpGt::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpGt) return false;
    auto rhsc = dynamic_cast<const CmpGt&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpGt::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpGt) return false;
    auto rhsc = dynamic_cast<const CmpGt&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpGt::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpGt");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpGt> CmpGt::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpGt") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpGt>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpLe::CmpLe(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpLe::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpLe` is complete/fully defined.
 */
void CmpLe::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpLe::type() const {
    return NodeType::CmpLe;
}

/**
 * Helper method for visiting nodes.
 */
void CmpLe::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_le(*this, retval);
}

/**
 * Interprets this node to a node of type CmpLe. Returns null if it has the
 * wrong type.
 */
CmpLe *CmpLe::as_cmp_le() {
    return dynamic_cast<CmpLe*>(this);
}

/**
 * Interprets this node to a node of type CmpLe. Returns null if it has the
 * wrong type.
 */
const CmpLe *CmpLe::as_cmp_le() const {
    return dynamic_cast<const CmpLe*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpLe::copy() const {
    return cqasm::tree::make<CmpLe>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpLe::clone() const {
    auto node = cqasm::tree::make<CmpLe>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpLe::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpLe) return false;
    auto rhsc = dynamic_cast<const CmpLe&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpLe::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpLe) return false;
    auto rhsc = dynamic_cast<const CmpLe&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpLe::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpLe");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpLe> CmpLe::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpLe") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpLe>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpLt::CmpLt(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpLt::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpLt` is complete/fully defined.
 */
void CmpLt::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpLt::type() const {
    return NodeType::CmpLt;
}

/**
 * Helper method for visiting nodes.
 */
void CmpLt::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_lt(*this, retval);
}

/**
 * Interprets this node to a node of type CmpLt. Returns null if it has the
 * wrong type.
 */
CmpLt *CmpLt::as_cmp_lt() {
    return dynamic_cast<CmpLt*>(this);
}

/**
 * Interprets this node to a node of type CmpLt. Returns null if it has the
 * wrong type.
 */
const CmpLt *CmpLt::as_cmp_lt() const {
    return dynamic_cast<const CmpLt*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpLt::copy() const {
    return cqasm::tree::make<CmpLt>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpLt::clone() const {
    auto node = cqasm::tree::make<CmpLt>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpLt::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpLt) return false;
    auto rhsc = dynamic_cast<const CmpLt&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpLt::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpLt) return false;
    auto rhsc = dynamic_cast<const CmpLt&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpLt::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpLt");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpLt> CmpLt::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpLt") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpLt>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
CmpNe::CmpNe(const One<Expression> &lhs, const One<Expression> &rhs)
    : CmpOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void CmpNe::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `CmpNe` is complete/fully defined.
 */
void CmpNe::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType CmpNe::type() const {
    return NodeType::CmpNe;
}

/**
 * Helper method for visiting nodes.
 */
void CmpNe::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_cmp_ne(*this, retval);
}

/**
 * Interprets this node to a node of type CmpNe. Returns null if it has the
 * wrong type.
 */
CmpNe *CmpNe::as_cmp_ne() {
    return dynamic_cast<CmpNe*>(this);
}

/**
 * Interprets this node to a node of type CmpNe. Returns null if it has the
 * wrong type.
 */
const CmpNe *CmpNe::as_cmp_ne() const {
    return dynamic_cast<const CmpNe*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> CmpNe::copy() const {
    return cqasm::tree::make<CmpNe>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> CmpNe::clone() const {
    auto node = cqasm::tree::make<CmpNe>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool CmpNe::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpNe) return false;
    auto rhsc = dynamic_cast<const CmpNe&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool CmpNe::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::CmpNe) return false;
    auto rhsc = dynamic_cast<const CmpNe&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void CmpNe::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "CmpNe");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<CmpNe> CmpNe::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "CmpNe") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<CmpNe>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ContinueStatement::ContinueStatement(const Any<AnnotationData> &annotations)
    : Structured(annotations)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ContinueStatement::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ContinueStatement` is complete/fully defined.
 */
void ContinueStatement::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ContinueStatement::type() const {
    return NodeType::ContinueStatement;
}

/**
 * Helper method for visiting nodes.
 */
void ContinueStatement::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_continue_statement(*this, retval);
}

/**
 * Interprets this node to a node of type ContinueStatement. Returns null if it
 * has the wrong type.
 */
ContinueStatement *ContinueStatement::as_continue_statement() {
    return dynamic_cast<ContinueStatement*>(this);
}

/**
 * Interprets this node to a node of type ContinueStatement. Returns null if it
 * has the wrong type.
 */
const ContinueStatement *ContinueStatement::as_continue_statement() const {
    return dynamic_cast<const ContinueStatement*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ContinueStatement::copy() const {
    return cqasm::tree::make<ContinueStatement>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ContinueStatement::clone() const {
    auto node = cqasm::tree::make<ContinueStatement>(*this);
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ContinueStatement::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ContinueStatement) return false;
    auto rhsc = dynamic_cast<const ContinueStatement&>(rhs);
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ContinueStatement::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ContinueStatement) return false;
    auto rhsc = dynamic_cast<const ContinueStatement&>(rhs);
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ContinueStatement::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ContinueStatement");
    auto submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ContinueStatement> ContinueStatement::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ContinueStatement") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ContinueStatement>(
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Divide::Divide(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Divide::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Divide` is complete/fully defined.
 */
void Divide::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Divide::type() const {
    return NodeType::Divide;
}

/**
 * Helper method for visiting nodes.
 */
void Divide::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_divide(*this, retval);
}

/**
 * Interprets this node to a node of type Divide. Returns null if it has the
 * wrong type.
 */
Divide *Divide::as_divide() {
    return dynamic_cast<Divide*>(this);
}

/**
 * Interprets this node to a node of type Divide. Returns null if it has the
 * wrong type.
 */
const Divide *Divide::as_divide() const {
    return dynamic_cast<const Divide*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Divide::copy() const {
    return cqasm::tree::make<Divide>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Divide::clone() const {
    auto node = cqasm::tree::make<Divide>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Divide::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Divide) return false;
    auto rhsc = dynamic_cast<const Divide&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Divide::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Divide) return false;
    auto rhsc = dynamic_cast<const Divide&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Divide::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Divide");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Divide> Divide::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Divide") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Divide>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ErroneousExpression::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ErroneousExpression` is complete/fully defined.
 */
void ErroneousExpression::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    throw ::tree::base::NotWellFormed("ErroneousExpression error node in tree");
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ErroneousExpression::type() const {
    return NodeType::ErroneousExpression;
}

/**
 * Helper method for visiting nodes.
 */
void ErroneousExpression::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_erroneous_expression(*this, retval);
}

/**
 * Interprets this node to a node of type ErroneousExpression. Returns null if
 * it has the wrong type.
 */
ErroneousExpression *ErroneousExpression::as_erroneous_expression() {
    return dynamic_cast<ErroneousExpression*>(this);
}

/**
 * Interprets this node to a node of type ErroneousExpression. Returns null if
 * it has the wrong type.
 */
const ErroneousExpression *ErroneousExpression::as_erroneous_expression() const {
    return dynamic_cast<const ErroneousExpression*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ErroneousExpression::copy() const {
    return cqasm::tree::make<ErroneousExpression>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ErroneousExpression::clone() const {
    auto node = cqasm::tree::make<ErroneousExpression>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ErroneousExpression::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousExpression) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ErroneousExpression::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousExpression) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ErroneousExpression::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ErroneousExpression");
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ErroneousExpression> ErroneousExpression::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ErroneousExpression") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ErroneousExpression>(

    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Interprets this node to a node of type Root. Returns null if it has the wrong
 * type.
 */
Root *Root::as_root() {
    return dynamic_cast<Root*>(this);
}

/**
 * Interprets this node to a node of type Root. Returns null if it has the wrong
 * type.
 */
const Root *Root::as_root() const {
    return dynamic_cast<const Root*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Root> Root::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Program") return Program::deserialize(map, ids);
    if (type == "ErroneousProgram") return ErroneousProgram::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ErroneousProgram::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ErroneousProgram` is complete/fully defined.
 */
void ErroneousProgram::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    throw ::tree::base::NotWellFormed("ErroneousProgram error node in tree");
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ErroneousProgram::type() const {
    return NodeType::ErroneousProgram;
}

/**
 * Helper method for visiting nodes.
 */
void ErroneousProgram::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_erroneous_program(*this, retval);
}

/**
 * Interprets this node to a node of type ErroneousProgram. Returns null if it
 * has the wrong type.
 */
ErroneousProgram *ErroneousProgram::as_erroneous_program() {
    return dynamic_cast<ErroneousProgram*>(this);
}

/**
 * Interprets this node to a node of type ErroneousProgram. Returns null if it
 * has the wrong type.
 */
const ErroneousProgram *ErroneousProgram::as_erroneous_program() const {
    return dynamic_cast<const ErroneousProgram*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ErroneousProgram::copy() const {
    return cqasm::tree::make<ErroneousProgram>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ErroneousProgram::clone() const {
    auto node = cqasm::tree::make<ErroneousProgram>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ErroneousProgram::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousProgram) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ErroneousProgram::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousProgram) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ErroneousProgram::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ErroneousProgram");
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ErroneousProgram> ErroneousProgram::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ErroneousProgram") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ErroneousProgram>(

    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ErroneousStatement::ErroneousStatement(const Any<AnnotationData> &annotations)
    : Statement(annotations)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ErroneousStatement::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ErroneousStatement` is complete/fully defined.
 */
void ErroneousStatement::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    throw ::tree::base::NotWellFormed("ErroneousStatement error node in tree");
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ErroneousStatement::type() const {
    return NodeType::ErroneousStatement;
}

/**
 * Helper method for visiting nodes.
 */
void ErroneousStatement::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_erroneous_statement(*this, retval);
}

/**
 * Interprets this node to a node of type ErroneousStatement. Returns null if it
 * has the wrong type.
 */
ErroneousStatement *ErroneousStatement::as_erroneous_statement() {
    return dynamic_cast<ErroneousStatement*>(this);
}

/**
 * Interprets this node to a node of type ErroneousStatement. Returns null if it
 * has the wrong type.
 */
const ErroneousStatement *ErroneousStatement::as_erroneous_statement() const {
    return dynamic_cast<const ErroneousStatement*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ErroneousStatement::copy() const {
    return cqasm::tree::make<ErroneousStatement>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ErroneousStatement::clone() const {
    auto node = cqasm::tree::make<ErroneousStatement>(*this);
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ErroneousStatement::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousStatement) return false;
    auto rhsc = dynamic_cast<const ErroneousStatement&>(rhs);
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ErroneousStatement::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ErroneousStatement) return false;
    auto rhsc = dynamic_cast<const ErroneousStatement&>(rhs);
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ErroneousStatement::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ErroneousStatement");
    auto submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ErroneousStatement> ErroneousStatement::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ErroneousStatement") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ErroneousStatement>(
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ExpressionList::ExpressionList(const Any<Expression> &items)
    : items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ExpressionList::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    items.find_reachable(map);
}

/**
 * Returns whether this `ExpressionList` is complete/fully defined.
 */
void ExpressionList::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    items.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ExpressionList::type() const {
    return NodeType::ExpressionList;
}

/**
 * Helper method for visiting nodes.
 */
void ExpressionList::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_expression_list(*this, retval);
}

/**
 * Interprets this node to a node of type ExpressionList. Returns null if it has
 * the wrong type.
 */
ExpressionList *ExpressionList::as_expression_list() {
    return dynamic_cast<ExpressionList*>(this);
}

/**
 * Interprets this node to a node of type ExpressionList. Returns null if it has
 * the wrong type.
 */
const ExpressionList *ExpressionList::as_expression_list() const {
    return dynamic_cast<const ExpressionList*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ExpressionList::copy() const {
    return cqasm::tree::make<ExpressionList>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ExpressionList::clone() const {
    auto node = cqasm::tree::make<ExpressionList>(*this);
    node->items = this->items.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ExpressionList::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ExpressionList) return false;
    auto rhsc = dynamic_cast<const ExpressionList&>(rhs);
    if (!this->items.equals(rhsc.items)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ExpressionList::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ExpressionList) return false;
    auto rhsc = dynamic_cast<const ExpressionList&>(rhs);
    if (this->items != rhsc.items) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ExpressionList::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ExpressionList");
    auto submap = map.append_map("items");
    items.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ExpressionList> ExpressionList::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ExpressionList") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ExpressionList>(
        Any<Expression>(map.at("items").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
FloatLiteral::FloatLiteral(const cqasm::v1::primitives::Real &value)
    : Expression(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void FloatLiteral::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `FloatLiteral` is complete/fully defined.
 */
void FloatLiteral::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType FloatLiteral::type() const {
    return NodeType::FloatLiteral;
}

/**
 * Helper method for visiting nodes.
 */
void FloatLiteral::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_float_literal(*this, retval);
}

/**
 * Interprets this node to a node of type FloatLiteral. Returns null if it has
 * the wrong type.
 */
FloatLiteral *FloatLiteral::as_float_literal() {
    return dynamic_cast<FloatLiteral*>(this);
}

/**
 * Interprets this node to a node of type FloatLiteral. Returns null if it has
 * the wrong type.
 */
const FloatLiteral *FloatLiteral::as_float_literal() const {
    return dynamic_cast<const FloatLiteral*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> FloatLiteral::copy() const {
    return cqasm::tree::make<FloatLiteral>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> FloatLiteral::clone() const {
    auto node = cqasm::tree::make<FloatLiteral>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool FloatLiteral::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::FloatLiteral) return false;
    auto rhsc = dynamic_cast<const FloatLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool FloatLiteral::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::FloatLiteral) return false;
    auto rhsc = dynamic_cast<const FloatLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void FloatLiteral::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "FloatLiteral");
    auto submap = map.append_map("value");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Real>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<FloatLiteral> FloatLiteral::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "FloatLiteral") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<FloatLiteral>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Real>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ForLoop::ForLoop(const Maybe<Assignment> &initialize, const One<Expression> &condition, const Maybe<Assignment> &update, const One<StatementList> &body, const Any<AnnotationData> &annotations)
    : Structured(annotations), initialize(initialize), condition(condition), update(update), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ForLoop::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    initialize.find_reachable(map);
    condition.find_reachable(map);
    update.find_reachable(map);
    body.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ForLoop` is complete/fully defined.
 */
void ForLoop::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    initialize.check_complete(map);
    condition.check_complete(map);
    update.check_complete(map);
    body.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ForLoop::type() const {
    return NodeType::ForLoop;
}

/**
 * Helper method for visiting nodes.
 */
void ForLoop::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_for_loop(*this, retval);
}

/**
 * Interprets this node to a node of type ForLoop. Returns null if it has the
 * wrong type.
 */
ForLoop *ForLoop::as_for_loop() {
    return dynamic_cast<ForLoop*>(this);
}

/**
 * Interprets this node to a node of type ForLoop. Returns null if it has the
 * wrong type.
 */
const ForLoop *ForLoop::as_for_loop() const {
    return dynamic_cast<const ForLoop*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ForLoop::copy() const {
    return cqasm::tree::make<ForLoop>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ForLoop::clone() const {
    auto node = cqasm::tree::make<ForLoop>(*this);
    node->initialize = this->initialize.clone();
    node->condition = this->condition.clone();
    node->update = this->update.clone();
    node->body = this->body.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ForLoop::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ForLoop) return false;
    auto rhsc = dynamic_cast<const ForLoop&>(rhs);
    if (!this->initialize.equals(rhsc.initialize)) return false;
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->update.equals(rhsc.update)) return false;
    if (!this->body.equals(rhsc.body)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ForLoop::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ForLoop) return false;
    auto rhsc = dynamic_cast<const ForLoop&>(rhs);
    if (this->initialize != rhsc.initialize) return false;
    if (this->condition != rhsc.condition) return false;
    if (this->update != rhsc.update) return false;
    if (this->body != rhsc.body) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ForLoop::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ForLoop");
    auto submap = map.append_map("initialize");
    initialize.serialize(submap, ids);
    submap.close();
    submap = map.append_map("condition");
    condition.serialize(submap, ids);
    submap.close();
    submap = map.append_map("update");
    update.serialize(submap, ids);
    submap.close();
    submap = map.append_map("body");
    body.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ForLoop> ForLoop::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ForLoop") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ForLoop>(
        Maybe<Assignment>(map.at("initialize").as_map(), ids),
        One<Expression>(map.at("condition").as_map(), ids),
        Maybe<Assignment>(map.at("update").as_map(), ids),
        One<StatementList>(map.at("body").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ForeachLoop::ForeachLoop(const One<Expression> &lhs, const One<Expression> &frm, const One<Expression> &to, const One<StatementList> &body, const Any<AnnotationData> &annotations)
    : Structured(annotations), lhs(lhs), frm(frm), to(to), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ForeachLoop::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    frm.find_reachable(map);
    to.find_reachable(map);
    body.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ForeachLoop` is complete/fully defined.
 */
void ForeachLoop::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    frm.check_complete(map);
    to.check_complete(map);
    body.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ForeachLoop::type() const {
    return NodeType::ForeachLoop;
}

/**
 * Helper method for visiting nodes.
 */
void ForeachLoop::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_foreach_loop(*this, retval);
}

/**
 * Interprets this node to a node of type ForeachLoop. Returns null if it has
 * the wrong type.
 */
ForeachLoop *ForeachLoop::as_foreach_loop() {
    return dynamic_cast<ForeachLoop*>(this);
}

/**
 * Interprets this node to a node of type ForeachLoop. Returns null if it has
 * the wrong type.
 */
const ForeachLoop *ForeachLoop::as_foreach_loop() const {
    return dynamic_cast<const ForeachLoop*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ForeachLoop::copy() const {
    return cqasm::tree::make<ForeachLoop>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ForeachLoop::clone() const {
    auto node = cqasm::tree::make<ForeachLoop>(*this);
    node->lhs = this->lhs.clone();
    node->frm = this->frm.clone();
    node->to = this->to.clone();
    node->body = this->body.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ForeachLoop::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ForeachLoop) return false;
    auto rhsc = dynamic_cast<const ForeachLoop&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->frm.equals(rhsc.frm)) return false;
    if (!this->to.equals(rhsc.to)) return false;
    if (!this->body.equals(rhsc.body)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ForeachLoop::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ForeachLoop) return false;
    auto rhsc = dynamic_cast<const ForeachLoop&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->frm != rhsc.frm) return false;
    if (this->to != rhsc.to) return false;
    if (this->body != rhsc.body) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ForeachLoop::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ForeachLoop");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("frm");
    frm.serialize(submap, ids);
    submap.close();
    submap = map.append_map("to");
    to.serialize(submap, ids);
    submap.close();
    submap = map.append_map("body");
    body.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ForeachLoop> ForeachLoop::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ForeachLoop") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ForeachLoop>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("frm").as_map(), ids),
        One<Expression>(map.at("to").as_map(), ids),
        One<StatementList>(map.at("body").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
FunctionCall::FunctionCall(const One<Identifier> &name, const One<ExpressionList> &arguments)
    : Expression(), name(name), arguments(arguments)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void FunctionCall::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    name.find_reachable(map);
    arguments.find_reachable(map);
}

/**
 * Returns whether this `FunctionCall` is complete/fully defined.
 */
void FunctionCall::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    name.check_complete(map);
    arguments.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType FunctionCall::type() const {
    return NodeType::FunctionCall;
}

/**
 * Helper method for visiting nodes.
 */
void FunctionCall::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_function_call(*this, retval);
}

/**
 * Interprets this node to a node of type FunctionCall. Returns null if it has
 * the wrong type.
 */
FunctionCall *FunctionCall::as_function_call() {
    return dynamic_cast<FunctionCall*>(this);
}

/**
 * Interprets this node to a node of type FunctionCall. Returns null if it has
 * the wrong type.
 */
const FunctionCall *FunctionCall::as_function_call() const {
    return dynamic_cast<const FunctionCall*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> FunctionCall::copy() const {
    return cqasm::tree::make<FunctionCall>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> FunctionCall::clone() const {
    auto node = cqasm::tree::make<FunctionCall>(*this);
    node->name = this->name.clone();
    node->arguments = this->arguments.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool FunctionCall::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::FunctionCall) return false;
    auto rhsc = dynamic_cast<const FunctionCall&>(rhs);
    if (!this->name.equals(rhsc.name)) return false;
    if (!this->arguments.equals(rhsc.arguments)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool FunctionCall::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::FunctionCall) return false;
    auto rhsc = dynamic_cast<const FunctionCall&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->arguments != rhsc.arguments) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void FunctionCall::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "FunctionCall");
    auto submap = map.append_map("name");
    name.serialize(submap, ids);
    submap.close();
    submap = map.append_map("arguments");
    arguments.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<FunctionCall> FunctionCall::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "FunctionCall") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<FunctionCall>(
        One<Identifier>(map.at("name").as_map(), ids),
        One<ExpressionList>(map.at("arguments").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Identifier::Identifier(const cqasm::v1::primitives::Str &name)
    : Expression(), name(name)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Identifier::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Identifier` is complete/fully defined.
 */
void Identifier::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Identifier::type() const {
    return NodeType::Identifier;
}

/**
 * Helper method for visiting nodes.
 */
void Identifier::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_identifier(*this, retval);
}

/**
 * Interprets this node to a node of type Identifier. Returns null if it has the
 * wrong type.
 */
Identifier *Identifier::as_identifier() {
    return dynamic_cast<Identifier*>(this);
}

/**
 * Interprets this node to a node of type Identifier. Returns null if it has the
 * wrong type.
 */
const Identifier *Identifier::as_identifier() const {
    return dynamic_cast<const Identifier*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Identifier::copy() const {
    return cqasm::tree::make<Identifier>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Identifier::clone() const {
    auto node = cqasm::tree::make<Identifier>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Identifier::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Identifier) return false;
    auto rhsc = dynamic_cast<const Identifier&>(rhs);
    if (this->name != rhsc.name) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Identifier::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Identifier) return false;
    auto rhsc = dynamic_cast<const Identifier&>(rhs);
    if (this->name != rhsc.name) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Identifier::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Identifier");
    auto submap = map.append_map("name");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Str>(name, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Identifier> Identifier::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Identifier") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Identifier>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Str>(map.at("name").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IfElse::IfElse(const Many<IfElseBranch> &branches, const Maybe<StatementList> &otherwise, const Any<AnnotationData> &annotations)
    : Structured(annotations), branches(branches), otherwise(otherwise)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IfElse::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    branches.find_reachable(map);
    otherwise.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `IfElse` is complete/fully defined.
 */
void IfElse::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    branches.check_complete(map);
    otherwise.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IfElse::type() const {
    return NodeType::IfElse;
}

/**
 * Helper method for visiting nodes.
 */
void IfElse::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_if_else(*this, retval);
}

/**
 * Interprets this node to a node of type IfElse. Returns null if it has the
 * wrong type.
 */
IfElse *IfElse::as_if_else() {
    return dynamic_cast<IfElse*>(this);
}

/**
 * Interprets this node to a node of type IfElse. Returns null if it has the
 * wrong type.
 */
const IfElse *IfElse::as_if_else() const {
    return dynamic_cast<const IfElse*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IfElse::copy() const {
    return cqasm::tree::make<IfElse>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IfElse::clone() const {
    auto node = cqasm::tree::make<IfElse>(*this);
    node->branches = this->branches.clone();
    node->otherwise = this->otherwise.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IfElse::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IfElse) return false;
    auto rhsc = dynamic_cast<const IfElse&>(rhs);
    if (!this->branches.equals(rhsc.branches)) return false;
    if (!this->otherwise.equals(rhsc.otherwise)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IfElse::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IfElse) return false;
    auto rhsc = dynamic_cast<const IfElse&>(rhs);
    if (this->branches != rhsc.branches) return false;
    if (this->otherwise != rhsc.otherwise) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IfElse::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IfElse");
    auto submap = map.append_map("branches");
    branches.serialize(submap, ids);
    submap.close();
    submap = map.append_map("otherwise");
    otherwise.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IfElse> IfElse::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IfElse") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IfElse>(
        Many<IfElseBranch>(map.at("branches").as_map(), ids),
        Maybe<StatementList>(map.at("otherwise").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IfElseBranch::IfElseBranch(const One<Expression> &condition, const One<StatementList> &body)
    : condition(condition), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IfElseBranch::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    condition.find_reachable(map);
    body.find_reachable(map);
}

/**
 * Returns whether this `IfElseBranch` is complete/fully defined.
 */
void IfElseBranch::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    condition.check_complete(map);
    body.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IfElseBranch::type() const {
    return NodeType::IfElseBranch;
}

/**
 * Helper method for visiting nodes.
 */
void IfElseBranch::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_if_else_branch(*this, retval);
}

/**
 * Interprets this node to a node of type IfElseBranch. Returns null if it has
 * the wrong type.
 */
IfElseBranch *IfElseBranch::as_if_else_branch() {
    return dynamic_cast<IfElseBranch*>(this);
}

/**
 * Interprets this node to a node of type IfElseBranch. Returns null if it has
 * the wrong type.
 */
const IfElseBranch *IfElseBranch::as_if_else_branch() const {
    return dynamic_cast<const IfElseBranch*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IfElseBranch::copy() const {
    return cqasm::tree::make<IfElseBranch>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IfElseBranch::clone() const {
    auto node = cqasm::tree::make<IfElseBranch>(*this);
    node->condition = this->condition.clone();
    node->body = this->body.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IfElseBranch::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IfElseBranch) return false;
    auto rhsc = dynamic_cast<const IfElseBranch&>(rhs);
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->body.equals(rhsc.body)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IfElseBranch::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IfElseBranch) return false;
    auto rhsc = dynamic_cast<const IfElseBranch&>(rhs);
    if (this->condition != rhsc.condition) return false;
    if (this->body != rhsc.body) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IfElseBranch::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IfElseBranch");
    auto submap = map.append_map("condition");
    condition.serialize(submap, ids);
    submap.close();
    submap = map.append_map("body");
    body.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IfElseBranch> IfElseBranch::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IfElseBranch") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IfElseBranch>(
        One<Expression>(map.at("condition").as_map(), ids),
        One<StatementList>(map.at("body").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Index::Index(const One<Expression> &expr, const One<IndexList> &indices)
    : Expression(), expr(expr), indices(indices)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Index::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    expr.find_reachable(map);
    indices.find_reachable(map);
}

/**
 * Returns whether this `Index` is complete/fully defined.
 */
void Index::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    expr.check_complete(map);
    indices.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Index::type() const {
    return NodeType::Index;
}

/**
 * Helper method for visiting nodes.
 */
void Index::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_index(*this, retval);
}

/**
 * Interprets this node to a node of type Index. Returns null if it has the
 * wrong type.
 */
Index *Index::as_index() {
    return dynamic_cast<Index*>(this);
}

/**
 * Interprets this node to a node of type Index. Returns null if it has the
 * wrong type.
 */
const Index *Index::as_index() const {
    return dynamic_cast<const Index*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Index::copy() const {
    return cqasm::tree::make<Index>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Index::clone() const {
    auto node = cqasm::tree::make<Index>(*this);
    node->expr = this->expr.clone();
    node->indices = this->indices.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Index::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Index) return false;
    auto rhsc = dynamic_cast<const Index&>(rhs);
    if (!this->expr.equals(rhsc.expr)) return false;
    if (!this->indices.equals(rhsc.indices)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Index::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Index) return false;
    auto rhsc = dynamic_cast<const Index&>(rhs);
    if (this->expr != rhsc.expr) return false;
    if (this->indices != rhsc.indices) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Index::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Index");
    auto submap = map.append_map("expr");
    expr.serialize(submap, ids);
    submap.close();
    submap = map.append_map("indices");
    indices.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Index> Index::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Index") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Index>(
        One<Expression>(map.at("expr").as_map(), ids),
        One<IndexList>(map.at("indices").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Interprets this node to a node of type IndexEntry. Returns null if it has the
 * wrong type.
 */
IndexEntry *IndexEntry::as_index_entry() {
    return dynamic_cast<IndexEntry*>(this);
}

/**
 * Interprets this node to a node of type IndexEntry. Returns null if it has the
 * wrong type.
 */
const IndexEntry *IndexEntry::as_index_entry() const {
    return dynamic_cast<const IndexEntry*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IndexEntry> IndexEntry::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "IndexItem") return IndexItem::deserialize(map, ids);
    if (type == "IndexRange") return IndexRange::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
IndexItem::IndexItem(const One<Expression> &index)
    : IndexEntry(), index(index)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IndexItem::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    index.find_reachable(map);
}

/**
 * Returns whether this `IndexItem` is complete/fully defined.
 */
void IndexItem::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    index.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IndexItem::type() const {
    return NodeType::IndexItem;
}

/**
 * Helper method for visiting nodes.
 */
void IndexItem::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_index_item(*this, retval);
}

/**
 * Interprets this node to a node of type IndexItem. Returns null if it has the
 * wrong type.
 */
IndexItem *IndexItem::as_index_item() {
    return dynamic_cast<IndexItem*>(this);
}

/**
 * Interprets this node to a node of type IndexItem. Returns null if it has the
 * wrong type.
 */
const IndexItem *IndexItem::as_index_item() const {
    return dynamic_cast<const IndexItem*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IndexItem::copy() const {
    return cqasm::tree::make<IndexItem>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IndexItem::clone() const {
    auto node = cqasm::tree::make<IndexItem>(*this);
    node->index = this->index.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IndexItem::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexItem) return false;
    auto rhsc = dynamic_cast<const IndexItem&>(rhs);
    if (!this->index.equals(rhsc.index)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IndexItem::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexItem) return false;
    auto rhsc = dynamic_cast<const IndexItem&>(rhs);
    if (this->index != rhsc.index) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IndexItem::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IndexItem");
    auto submap = map.append_map("index");
    index.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IndexItem> IndexItem::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IndexItem") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IndexItem>(
        One<Expression>(map.at("index").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IndexList::IndexList(const Many<IndexEntry> &items)
    : items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IndexList::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    items.find_reachable(map);
}

/**
 * Returns whether this `IndexList` is complete/fully defined.
 */
void IndexList::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    items.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IndexList::type() const {
    return NodeType::IndexList;
}

/**
 * Helper method for visiting nodes.
 */
void IndexList::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_index_list(*this, retval);
}

/**
 * Interprets this node to a node of type IndexList. Returns null if it has the
 * wrong type.
 */
IndexList *IndexList::as_index_list() {
    return dynamic_cast<IndexList*>(this);
}

/**
 * Interprets this node to a node of type IndexList. Returns null if it has the
 * wrong type.
 */
const IndexList *IndexList::as_index_list() const {
    return dynamic_cast<const IndexList*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IndexList::copy() const {
    return cqasm::tree::make<IndexList>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IndexList::clone() const {
    auto node = cqasm::tree::make<IndexList>(*this);
    node->items = this->items.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IndexList::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexList) return false;
    auto rhsc = dynamic_cast<const IndexList&>(rhs);
    if (!this->items.equals(rhsc.items)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IndexList::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexList) return false;
    auto rhsc = dynamic_cast<const IndexList&>(rhs);
    if (this->items != rhsc.items) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IndexList::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IndexList");
    auto submap = map.append_map("items");
    items.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IndexList> IndexList::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IndexList") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IndexList>(
        Many<IndexEntry>(map.at("items").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IndexRange::IndexRange(const One<Expression> &first, const One<Expression> &last)
    : IndexEntry(), first(first), last(last)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IndexRange::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    first.find_reachable(map);
    last.find_reachable(map);
}

/**
 * Returns whether this `IndexRange` is complete/fully defined.
 */
void IndexRange::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    first.check_complete(map);
    last.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IndexRange::type() const {
    return NodeType::IndexRange;
}

/**
 * Helper method for visiting nodes.
 */
void IndexRange::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_index_range(*this, retval);
}

/**
 * Interprets this node to a node of type IndexRange. Returns null if it has the
 * wrong type.
 */
IndexRange *IndexRange::as_index_range() {
    return dynamic_cast<IndexRange*>(this);
}

/**
 * Interprets this node to a node of type IndexRange. Returns null if it has the
 * wrong type.
 */
const IndexRange *IndexRange::as_index_range() const {
    return dynamic_cast<const IndexRange*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IndexRange::copy() const {
    return cqasm::tree::make<IndexRange>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IndexRange::clone() const {
    auto node = cqasm::tree::make<IndexRange>(*this);
    node->first = this->first.clone();
    node->last = this->last.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IndexRange::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexRange) return false;
    auto rhsc = dynamic_cast<const IndexRange&>(rhs);
    if (!this->first.equals(rhsc.first)) return false;
    if (!this->last.equals(rhsc.last)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IndexRange::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IndexRange) return false;
    auto rhsc = dynamic_cast<const IndexRange&>(rhs);
    if (this->first != rhsc.first) return false;
    if (this->last != rhsc.last) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IndexRange::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IndexRange");
    auto submap = map.append_map("first");
    first.serialize(submap, ids);
    submap.close();
    submap = map.append_map("last");
    last.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IndexRange> IndexRange::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IndexRange") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IndexRange>(
        One<Expression>(map.at("first").as_map(), ids),
        One<Expression>(map.at("last").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Instruction::Instruction(const One<Identifier> &name, const Maybe<Expression> &condition, const One<ExpressionList> &operands, const Any<AnnotationData> &annotations)
    : Annotated(annotations), name(name), condition(condition), operands(operands)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Instruction::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    name.find_reachable(map);
    condition.find_reachable(map);
    operands.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Instruction` is complete/fully defined.
 */
void Instruction::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    name.check_complete(map);
    condition.check_complete(map);
    operands.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Instruction::type() const {
    return NodeType::Instruction;
}

/**
 * Helper method for visiting nodes.
 */
void Instruction::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_instruction(*this, retval);
}

/**
 * Interprets this node to a node of type Instruction. Returns null if it has
 * the wrong type.
 */
Instruction *Instruction::as_instruction() {
    return dynamic_cast<Instruction*>(this);
}

/**
 * Interprets this node to a node of type Instruction. Returns null if it has
 * the wrong type.
 */
const Instruction *Instruction::as_instruction() const {
    return dynamic_cast<const Instruction*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Instruction::copy() const {
    return cqasm::tree::make<Instruction>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Instruction::clone() const {
    auto node = cqasm::tree::make<Instruction>(*this);
    node->name = this->name.clone();
    node->condition = this->condition.clone();
    node->operands = this->operands.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Instruction::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Instruction) return false;
    auto rhsc = dynamic_cast<const Instruction&>(rhs);
    if (!this->name.equals(rhsc.name)) return false;
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->operands.equals(rhsc.operands)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Instruction::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Instruction) return false;
    auto rhsc = dynamic_cast<const Instruction&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->condition != rhsc.condition) return false;
    if (this->operands != rhsc.operands) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Instruction::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Instruction");
    auto submap = map.append_map("name");
    name.serialize(submap, ids);
    submap.close();
    submap = map.append_map("condition");
    condition.serialize(submap, ids);
    submap.close();
    submap = map.append_map("operands");
    operands.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Instruction> Instruction::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Instruction") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Instruction>(
        One<Identifier>(map.at("name").as_map(), ids),
        Maybe<Expression>(map.at("condition").as_map(), ids),
        One<ExpressionList>(map.at("operands").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IntDivide::IntDivide(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IntDivide::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `IntDivide` is complete/fully defined.
 */
void IntDivide::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IntDivide::type() const {
    return NodeType::IntDivide;
}

/**
 * Helper method for visiting nodes.
 */
void IntDivide::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_int_divide(*this, retval);
}

/**
 * Interprets this node to a node of type IntDivide. Returns null if it has the
 * wrong type.
 */
IntDivide *IntDivide::as_int_divide() {
    return dynamic_cast<IntDivide*>(this);
}

/**
 * Interprets this node to a node of type IntDivide. Returns null if it has the
 * wrong type.
 */
const IntDivide *IntDivide::as_int_divide() const {
    return dynamic_cast<const IntDivide*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IntDivide::copy() const {
    return cqasm::tree::make<IntDivide>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IntDivide::clone() const {
    auto node = cqasm::tree::make<IntDivide>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IntDivide::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IntDivide) return false;
    auto rhsc = dynamic_cast<const IntDivide&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IntDivide::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IntDivide) return false;
    auto rhsc = dynamic_cast<const IntDivide&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IntDivide::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IntDivide");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IntDivide> IntDivide::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IntDivide") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IntDivide>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IntegerLiteral::IntegerLiteral(const cqasm::v1::primitives::Int &value)
    : Expression(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void IntegerLiteral::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `IntegerLiteral` is complete/fully defined.
 */
void IntegerLiteral::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType IntegerLiteral::type() const {
    return NodeType::IntegerLiteral;
}

/**
 * Helper method for visiting nodes.
 */
void IntegerLiteral::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_integer_literal(*this, retval);
}

/**
 * Interprets this node to a node of type IntegerLiteral. Returns null if it has
 * the wrong type.
 */
IntegerLiteral *IntegerLiteral::as_integer_literal() {
    return dynamic_cast<IntegerLiteral*>(this);
}

/**
 * Interprets this node to a node of type IntegerLiteral. Returns null if it has
 * the wrong type.
 */
const IntegerLiteral *IntegerLiteral::as_integer_literal() const {
    return dynamic_cast<const IntegerLiteral*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> IntegerLiteral::copy() const {
    return cqasm::tree::make<IntegerLiteral>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> IntegerLiteral::clone() const {
    auto node = cqasm::tree::make<IntegerLiteral>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool IntegerLiteral::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::IntegerLiteral) return false;
    auto rhsc = dynamic_cast<const IntegerLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool IntegerLiteral::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::IntegerLiteral) return false;
    auto rhsc = dynamic_cast<const IntegerLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void IntegerLiteral::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "IntegerLiteral");
    auto submap = map.append_map("value");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Int>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<IntegerLiteral> IntegerLiteral::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "IntegerLiteral") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<IntegerLiteral>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Int>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
JsonLiteral::JsonLiteral(const cqasm::v1::primitives::Str &value)
    : Expression(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void JsonLiteral::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `JsonLiteral` is complete/fully defined.
 */
void JsonLiteral::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType JsonLiteral::type() const {
    return NodeType::JsonLiteral;
}

/**
 * Helper method for visiting nodes.
 */
void JsonLiteral::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_json_literal(*this, retval);
}

/**
 * Interprets this node to a node of type JsonLiteral. Returns null if it has
 * the wrong type.
 */
JsonLiteral *JsonLiteral::as_json_literal() {
    return dynamic_cast<JsonLiteral*>(this);
}

/**
 * Interprets this node to a node of type JsonLiteral. Returns null if it has
 * the wrong type.
 */
const JsonLiteral *JsonLiteral::as_json_literal() const {
    return dynamic_cast<const JsonLiteral*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> JsonLiteral::copy() const {
    return cqasm::tree::make<JsonLiteral>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> JsonLiteral::clone() const {
    auto node = cqasm::tree::make<JsonLiteral>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool JsonLiteral::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::JsonLiteral) return false;
    auto rhsc = dynamic_cast<const JsonLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool JsonLiteral::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::JsonLiteral) return false;
    auto rhsc = dynamic_cast<const JsonLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void JsonLiteral::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "JsonLiteral");
    auto submap = map.append_map("value");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Str>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<JsonLiteral> JsonLiteral::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "JsonLiteral") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<JsonLiteral>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Str>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
LogicalOp::LogicalOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : BinaryOp(lhs, rhs)
{}

/**
 * Interprets this node to a node of type LogicalOp. Returns null if it has the
 * wrong type.
 */
LogicalOp *LogicalOp::as_logical_op() {
    return dynamic_cast<LogicalOp*>(this);
}

/**
 * Interprets this node to a node of type LogicalOp. Returns null if it has the
 * wrong type.
 */
const LogicalOp *LogicalOp::as_logical_op() const {
    return dynamic_cast<const LogicalOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<LogicalOp> LogicalOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "LogicalAnd") return LogicalAnd::deserialize(map, ids);
    if (type == "LogicalXor") return LogicalXor::deserialize(map, ids);
    if (type == "LogicalOr") return LogicalOr::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
LogicalAnd::LogicalAnd(const One<Expression> &lhs, const One<Expression> &rhs)
    : LogicalOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void LogicalAnd::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `LogicalAnd` is complete/fully defined.
 */
void LogicalAnd::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType LogicalAnd::type() const {
    return NodeType::LogicalAnd;
}

/**
 * Helper method for visiting nodes.
 */
void LogicalAnd::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_logical_and(*this, retval);
}

/**
 * Interprets this node to a node of type LogicalAnd. Returns null if it has the
 * wrong type.
 */
LogicalAnd *LogicalAnd::as_logical_and() {
    return dynamic_cast<LogicalAnd*>(this);
}

/**
 * Interprets this node to a node of type LogicalAnd. Returns null if it has the
 * wrong type.
 */
const LogicalAnd *LogicalAnd::as_logical_and() const {
    return dynamic_cast<const LogicalAnd*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> LogicalAnd::copy() const {
    return cqasm::tree::make<LogicalAnd>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> LogicalAnd::clone() const {
    auto node = cqasm::tree::make<LogicalAnd>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool LogicalAnd::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalAnd) return false;
    auto rhsc = dynamic_cast<const LogicalAnd&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool LogicalAnd::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalAnd) return false;
    auto rhsc = dynamic_cast<const LogicalAnd&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void LogicalAnd::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "LogicalAnd");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<LogicalAnd> LogicalAnd::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "LogicalAnd") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<LogicalAnd>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
LogicalNot::LogicalNot(const One<Expression> &expr)
    : UnaryOp(expr)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void LogicalNot::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    expr.find_reachable(map);
}

/**
 * Returns whether this `LogicalNot` is complete/fully defined.
 */
void LogicalNot::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    expr.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType LogicalNot::type() const {
    return NodeType::LogicalNot;
}

/**
 * Helper method for visiting nodes.
 */
void LogicalNot::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_logical_not(*this, retval);
}

/**
 * Interprets this node to a node of type LogicalNot. Returns null if it has the
 * wrong type.
 */
LogicalNot *LogicalNot::as_logical_not() {
    return dynamic_cast<LogicalNot*>(this);
}

/**
 * Interprets this node to a node of type LogicalNot. Returns null if it has the
 * wrong type.
 */
const LogicalNot *LogicalNot::as_logical_not() const {
    return dynamic_cast<const LogicalNot*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> LogicalNot::copy() const {
    return cqasm::tree::make<LogicalNot>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> LogicalNot::clone() const {
    auto node = cqasm::tree::make<LogicalNot>(*this);
    node->expr = this->expr.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool LogicalNot::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalNot) return false;
    auto rhsc = dynamic_cast<const LogicalNot&>(rhs);
    if (!this->expr.equals(rhsc.expr)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool LogicalNot::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalNot) return false;
    auto rhsc = dynamic_cast<const LogicalNot&>(rhs);
    if (this->expr != rhsc.expr) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void LogicalNot::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "LogicalNot");
    auto submap = map.append_map("expr");
    expr.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<LogicalNot> LogicalNot::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "LogicalNot") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<LogicalNot>(
        One<Expression>(map.at("expr").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
LogicalOr::LogicalOr(const One<Expression> &lhs, const One<Expression> &rhs)
    : LogicalOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void LogicalOr::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `LogicalOr` is complete/fully defined.
 */
void LogicalOr::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType LogicalOr::type() const {
    return NodeType::LogicalOr;
}

/**
 * Helper method for visiting nodes.
 */
void LogicalOr::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_logical_or(*this, retval);
}

/**
 * Interprets this node to a node of type LogicalOr. Returns null if it has the
 * wrong type.
 */
LogicalOr *LogicalOr::as_logical_or() {
    return dynamic_cast<LogicalOr*>(this);
}

/**
 * Interprets this node to a node of type LogicalOr. Returns null if it has the
 * wrong type.
 */
const LogicalOr *LogicalOr::as_logical_or() const {
    return dynamic_cast<const LogicalOr*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> LogicalOr::copy() const {
    return cqasm::tree::make<LogicalOr>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> LogicalOr::clone() const {
    auto node = cqasm::tree::make<LogicalOr>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool LogicalOr::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalOr) return false;
    auto rhsc = dynamic_cast<const LogicalOr&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool LogicalOr::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalOr) return false;
    auto rhsc = dynamic_cast<const LogicalOr&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void LogicalOr::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "LogicalOr");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<LogicalOr> LogicalOr::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "LogicalOr") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<LogicalOr>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
LogicalXor::LogicalXor(const One<Expression> &lhs, const One<Expression> &rhs)
    : LogicalOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void LogicalXor::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `LogicalXor` is complete/fully defined.
 */
void LogicalXor::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType LogicalXor::type() const {
    return NodeType::LogicalXor;
}

/**
 * Helper method for visiting nodes.
 */
void LogicalXor::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_logical_xor(*this, retval);
}

/**
 * Interprets this node to a node of type LogicalXor. Returns null if it has the
 * wrong type.
 */
LogicalXor *LogicalXor::as_logical_xor() {
    return dynamic_cast<LogicalXor*>(this);
}

/**
 * Interprets this node to a node of type LogicalXor. Returns null if it has the
 * wrong type.
 */
const LogicalXor *LogicalXor::as_logical_xor() const {
    return dynamic_cast<const LogicalXor*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> LogicalXor::copy() const {
    return cqasm::tree::make<LogicalXor>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> LogicalXor::clone() const {
    auto node = cqasm::tree::make<LogicalXor>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool LogicalXor::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalXor) return false;
    auto rhsc = dynamic_cast<const LogicalXor&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool LogicalXor::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::LogicalXor) return false;
    auto rhsc = dynamic_cast<const LogicalXor&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void LogicalXor::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "LogicalXor");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<LogicalXor> LogicalXor::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "LogicalXor") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<LogicalXor>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Mapping::Mapping(const One<Identifier> &alias, const One<Expression> &expr, const Any<AnnotationData> &annotations)
    : Statement(annotations), alias(alias), expr(expr)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Mapping::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    alias.find_reachable(map);
    expr.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Mapping` is complete/fully defined.
 */
void Mapping::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    alias.check_complete(map);
    expr.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Mapping::type() const {
    return NodeType::Mapping;
}

/**
 * Helper method for visiting nodes.
 */
void Mapping::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_mapping(*this, retval);
}

/**
 * Interprets this node to a node of type Mapping. Returns null if it has the
 * wrong type.
 */
Mapping *Mapping::as_mapping() {
    return dynamic_cast<Mapping*>(this);
}

/**
 * Interprets this node to a node of type Mapping. Returns null if it has the
 * wrong type.
 */
const Mapping *Mapping::as_mapping() const {
    return dynamic_cast<const Mapping*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Mapping::copy() const {
    return cqasm::tree::make<Mapping>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Mapping::clone() const {
    auto node = cqasm::tree::make<Mapping>(*this);
    node->alias = this->alias.clone();
    node->expr = this->expr.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Mapping::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Mapping) return false;
    auto rhsc = dynamic_cast<const Mapping&>(rhs);
    if (!this->alias.equals(rhsc.alias)) return false;
    if (!this->expr.equals(rhsc.expr)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Mapping::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Mapping) return false;
    auto rhsc = dynamic_cast<const Mapping&>(rhs);
    if (this->alias != rhsc.alias) return false;
    if (this->expr != rhsc.expr) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Mapping::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Mapping");
    auto submap = map.append_map("alias");
    alias.serialize(submap, ids);
    submap.close();
    submap = map.append_map("expr");
    expr.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Mapping> Mapping::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Mapping") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Mapping>(
        One<Identifier>(map.at("alias").as_map(), ids),
        One<Expression>(map.at("expr").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
MatrixLiteral::MatrixLiteral(const Many<ExpressionList> &rows)
    : Expression(), rows(rows)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void MatrixLiteral::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    rows.find_reachable(map);
}

/**
 * Returns whether this `MatrixLiteral` is complete/fully defined.
 */
void MatrixLiteral::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    rows.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType MatrixLiteral::type() const {
    return NodeType::MatrixLiteral;
}

/**
 * Helper method for visiting nodes.
 */
void MatrixLiteral::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_matrix_literal(*this, retval);
}

/**
 * Interprets this node to a node of type MatrixLiteral. Returns null if it has
 * the wrong type.
 */
MatrixLiteral *MatrixLiteral::as_matrix_literal() {
    return dynamic_cast<MatrixLiteral*>(this);
}

/**
 * Interprets this node to a node of type MatrixLiteral. Returns null if it has
 * the wrong type.
 */
const MatrixLiteral *MatrixLiteral::as_matrix_literal() const {
    return dynamic_cast<const MatrixLiteral*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> MatrixLiteral::copy() const {
    return cqasm::tree::make<MatrixLiteral>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> MatrixLiteral::clone() const {
    auto node = cqasm::tree::make<MatrixLiteral>(*this);
    node->rows = this->rows.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool MatrixLiteral::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::MatrixLiteral) return false;
    auto rhsc = dynamic_cast<const MatrixLiteral&>(rhs);
    if (!this->rows.equals(rhsc.rows)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool MatrixLiteral::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::MatrixLiteral) return false;
    auto rhsc = dynamic_cast<const MatrixLiteral&>(rhs);
    if (this->rows != rhsc.rows) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void MatrixLiteral::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "MatrixLiteral");
    auto submap = map.append_map("rows");
    rows.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<MatrixLiteral> MatrixLiteral::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "MatrixLiteral") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<MatrixLiteral>(
        Many<ExpressionList>(map.at("rows").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Modulo::Modulo(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Modulo::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Modulo` is complete/fully defined.
 */
void Modulo::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Modulo::type() const {
    return NodeType::Modulo;
}

/**
 * Helper method for visiting nodes.
 */
void Modulo::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_modulo(*this, retval);
}

/**
 * Interprets this node to a node of type Modulo. Returns null if it has the
 * wrong type.
 */
Modulo *Modulo::as_modulo() {
    return dynamic_cast<Modulo*>(this);
}

/**
 * Interprets this node to a node of type Modulo. Returns null if it has the
 * wrong type.
 */
const Modulo *Modulo::as_modulo() const {
    return dynamic_cast<const Modulo*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Modulo::copy() const {
    return cqasm::tree::make<Modulo>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Modulo::clone() const {
    auto node = cqasm::tree::make<Modulo>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Modulo::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Modulo) return false;
    auto rhsc = dynamic_cast<const Modulo&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Modulo::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Modulo) return false;
    auto rhsc = dynamic_cast<const Modulo&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Modulo::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Modulo");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Modulo> Modulo::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Modulo") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Modulo>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Multiply::Multiply(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Multiply::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Multiply` is complete/fully defined.
 */
void Multiply::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Multiply::type() const {
    return NodeType::Multiply;
}

/**
 * Helper method for visiting nodes.
 */
void Multiply::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_multiply(*this, retval);
}

/**
 * Interprets this node to a node of type Multiply. Returns null if it has the
 * wrong type.
 */
Multiply *Multiply::as_multiply() {
    return dynamic_cast<Multiply*>(this);
}

/**
 * Interprets this node to a node of type Multiply. Returns null if it has the
 * wrong type.
 */
const Multiply *Multiply::as_multiply() const {
    return dynamic_cast<const Multiply*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Multiply::copy() const {
    return cqasm::tree::make<Multiply>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Multiply::clone() const {
    auto node = cqasm::tree::make<Multiply>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Multiply::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Multiply) return false;
    auto rhsc = dynamic_cast<const Multiply&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Multiply::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Multiply) return false;
    auto rhsc = dynamic_cast<const Multiply&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Multiply::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Multiply");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Multiply> Multiply::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Multiply") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Multiply>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Negate::Negate(const One<Expression> &expr)
    : UnaryOp(expr)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Negate::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    expr.find_reachable(map);
}

/**
 * Returns whether this `Negate` is complete/fully defined.
 */
void Negate::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    expr.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Negate::type() const {
    return NodeType::Negate;
}

/**
 * Helper method for visiting nodes.
 */
void Negate::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_negate(*this, retval);
}

/**
 * Interprets this node to a node of type Negate. Returns null if it has the
 * wrong type.
 */
Negate *Negate::as_negate() {
    return dynamic_cast<Negate*>(this);
}

/**
 * Interprets this node to a node of type Negate. Returns null if it has the
 * wrong type.
 */
const Negate *Negate::as_negate() const {
    return dynamic_cast<const Negate*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Negate::copy() const {
    return cqasm::tree::make<Negate>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Negate::clone() const {
    auto node = cqasm::tree::make<Negate>(*this);
    node->expr = this->expr.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Negate::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Negate) return false;
    auto rhsc = dynamic_cast<const Negate&>(rhs);
    if (!this->expr.equals(rhsc.expr)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Negate::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Negate) return false;
    auto rhsc = dynamic_cast<const Negate&>(rhs);
    if (this->expr != rhsc.expr) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Negate::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Negate");
    auto submap = map.append_map("expr");
    expr.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Negate> Negate::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Negate") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Negate>(
        One<Expression>(map.at("expr").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Power::Power(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Power::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Power` is complete/fully defined.
 */
void Power::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Power::type() const {
    return NodeType::Power;
}

/**
 * Helper method for visiting nodes.
 */
void Power::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_power(*this, retval);
}

/**
 * Interprets this node to a node of type Power. Returns null if it has the
 * wrong type.
 */
Power *Power::as_power() {
    return dynamic_cast<Power*>(this);
}

/**
 * Interprets this node to a node of type Power. Returns null if it has the
 * wrong type.
 */
const Power *Power::as_power() const {
    return dynamic_cast<const Power*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Power::copy() const {
    return cqasm::tree::make<Power>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Power::clone() const {
    auto node = cqasm::tree::make<Power>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Power::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Power) return false;
    auto rhsc = dynamic_cast<const Power&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Power::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Power) return false;
    auto rhsc = dynamic_cast<const Power&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Power::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Power");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Power> Power::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Power") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Power>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Program::Program(const One<Version> &version, const Maybe<Expression> &num_qubits, const One<StatementList> &statements)
    : Root(), version(version), num_qubits(num_qubits), statements(statements)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Program::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    version.find_reachable(map);
    num_qubits.find_reachable(map);
    statements.find_reachable(map);
}

/**
 * Returns whether this `Program` is complete/fully defined.
 */
void Program::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    version.check_complete(map);
    num_qubits.check_complete(map);
    statements.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Program::type() const {
    return NodeType::Program;
}

/**
 * Helper method for visiting nodes.
 */
void Program::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_program(*this, retval);
}

/**
 * Interprets this node to a node of type Program. Returns null if it has the
 * wrong type.
 */
Program *Program::as_program() {
    return dynamic_cast<Program*>(this);
}

/**
 * Interprets this node to a node of type Program. Returns null if it has the
 * wrong type.
 */
const Program *Program::as_program() const {
    return dynamic_cast<const Program*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Program::copy() const {
    return cqasm::tree::make<Program>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Program::clone() const {
    auto node = cqasm::tree::make<Program>(*this);
    node->version = this->version.clone();
    node->num_qubits = this->num_qubits.clone();
    node->statements = this->statements.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Program::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Program) return false;
    auto rhsc = dynamic_cast<const Program&>(rhs);
    if (!this->version.equals(rhsc.version)) return false;
    if (!this->num_qubits.equals(rhsc.num_qubits)) return false;
    if (!this->statements.equals(rhsc.statements)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Program::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Program) return false;
    auto rhsc = dynamic_cast<const Program&>(rhs);
    if (this->version != rhsc.version) return false;
    if (this->num_qubits != rhsc.num_qubits) return false;
    if (this->statements != rhsc.statements) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Program::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Program");
    auto submap = map.append_map("version");
    version.serialize(submap, ids);
    submap.close();
    submap = map.append_map("num_qubits");
    num_qubits.serialize(submap, ids);
    submap.close();
    submap = map.append_map("statements");
    statements.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Program> Program::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Program") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Program>(
        One<Version>(map.at("version").as_map(), ids),
        Maybe<Expression>(map.at("num_qubits").as_map(), ids),
        One<StatementList>(map.at("statements").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
RepeatUntilLoop::RepeatUntilLoop(const One<StatementList> &body, const One<Expression> &condition, const Any<AnnotationData> &annotations)
    : Structured(annotations), body(body), condition(condition)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void RepeatUntilLoop::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    body.find_reachable(map);
    condition.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `RepeatUntilLoop` is complete/fully defined.
 */
void RepeatUntilLoop::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    body.check_complete(map);
    condition.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType RepeatUntilLoop::type() const {
    return NodeType::RepeatUntilLoop;
}

/**
 * Helper method for visiting nodes.
 */
void RepeatUntilLoop::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_repeat_until_loop(*this, retval);
}

/**
 * Interprets this node to a node of type RepeatUntilLoop. Returns null if it
 * has the wrong type.
 */
RepeatUntilLoop *RepeatUntilLoop::as_repeat_until_loop() {
    return dynamic_cast<RepeatUntilLoop*>(this);
}

/**
 * Interprets this node to a node of type RepeatUntilLoop. Returns null if it
 * has the wrong type.
 */
const RepeatUntilLoop *RepeatUntilLoop::as_repeat_until_loop() const {
    return dynamic_cast<const RepeatUntilLoop*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> RepeatUntilLoop::copy() const {
    return cqasm::tree::make<RepeatUntilLoop>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> RepeatUntilLoop::clone() const {
    auto node = cqasm::tree::make<RepeatUntilLoop>(*this);
    node->body = this->body.clone();
    node->condition = this->condition.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool RepeatUntilLoop::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::RepeatUntilLoop) return false;
    auto rhsc = dynamic_cast<const RepeatUntilLoop&>(rhs);
    if (!this->body.equals(rhsc.body)) return false;
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool RepeatUntilLoop::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::RepeatUntilLoop) return false;
    auto rhsc = dynamic_cast<const RepeatUntilLoop&>(rhs);
    if (this->body != rhsc.body) return false;
    if (this->condition != rhsc.condition) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void RepeatUntilLoop::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "RepeatUntilLoop");
    auto submap = map.append_map("body");
    body.serialize(submap, ids);
    submap.close();
    submap = map.append_map("condition");
    condition.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<RepeatUntilLoop> RepeatUntilLoop::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "RepeatUntilLoop") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<RepeatUntilLoop>(
        One<StatementList>(map.at("body").as_map(), ids),
        One<Expression>(map.at("condition").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ShiftOp::ShiftOp(const One<Expression> &lhs, const One<Expression> &rhs)
    : BinaryOp(lhs, rhs)
{}

/**
 * Interprets this node to a node of type ShiftOp. Returns null if it has the
 * wrong type.
 */
ShiftOp *ShiftOp::as_shift_op() {
    return dynamic_cast<ShiftOp*>(this);
}

/**
 * Interprets this node to a node of type ShiftOp. Returns null if it has the
 * wrong type.
 */
const ShiftOp *ShiftOp::as_shift_op() const {
    return dynamic_cast<const ShiftOp*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ShiftOp> ShiftOp::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "ShiftLeft") return ShiftLeft::deserialize(map, ids);
    if (type == "ShiftRightArith") return ShiftRightArith::deserialize(map, ids);
    if (type == "ShiftRightLogic") return ShiftRightLogic::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
ShiftLeft::ShiftLeft(const One<Expression> &lhs, const One<Expression> &rhs)
    : ShiftOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ShiftLeft::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `ShiftLeft` is complete/fully defined.
 */
void ShiftLeft::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ShiftLeft::type() const {
    return NodeType::ShiftLeft;
}

/**
 * Helper method for visiting nodes.
 */
void ShiftLeft::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_shift_left(*this, retval);
}

/**
 * Interprets this node to a node of type ShiftLeft. Returns null if it has the
 * wrong type.
 */
ShiftLeft *ShiftLeft::as_shift_left() {
    return dynamic_cast<ShiftLeft*>(this);
}

/**
 * Interprets this node to a node of type ShiftLeft. Returns null if it has the
 * wrong type.
 */
const ShiftLeft *ShiftLeft::as_shift_left() const {
    return dynamic_cast<const ShiftLeft*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ShiftLeft::copy() const {
    return cqasm::tree::make<ShiftLeft>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ShiftLeft::clone() const {
    auto node = cqasm::tree::make<ShiftLeft>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ShiftLeft::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftLeft) return false;
    auto rhsc = dynamic_cast<const ShiftLeft&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ShiftLeft::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftLeft) return false;
    auto rhsc = dynamic_cast<const ShiftLeft&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ShiftLeft::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ShiftLeft");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ShiftLeft> ShiftLeft::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ShiftLeft") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ShiftLeft>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ShiftRightArith::ShiftRightArith(const One<Expression> &lhs, const One<Expression> &rhs)
    : ShiftOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ShiftRightArith::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `ShiftRightArith` is complete/fully defined.
 */
void ShiftRightArith::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ShiftRightArith::type() const {
    return NodeType::ShiftRightArith;
}

/**
 * Helper method for visiting nodes.
 */
void ShiftRightArith::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_shift_right_arith(*this, retval);
}

/**
 * Interprets this node to a node of type ShiftRightArith. Returns null if it
 * has the wrong type.
 */
ShiftRightArith *ShiftRightArith::as_shift_right_arith() {
    return dynamic_cast<ShiftRightArith*>(this);
}

/**
 * Interprets this node to a node of type ShiftRightArith. Returns null if it
 * has the wrong type.
 */
const ShiftRightArith *ShiftRightArith::as_shift_right_arith() const {
    return dynamic_cast<const ShiftRightArith*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ShiftRightArith::copy() const {
    return cqasm::tree::make<ShiftRightArith>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ShiftRightArith::clone() const {
    auto node = cqasm::tree::make<ShiftRightArith>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ShiftRightArith::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftRightArith) return false;
    auto rhsc = dynamic_cast<const ShiftRightArith&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ShiftRightArith::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftRightArith) return false;
    auto rhsc = dynamic_cast<const ShiftRightArith&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ShiftRightArith::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ShiftRightArith");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ShiftRightArith> ShiftRightArith::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ShiftRightArith") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ShiftRightArith>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ShiftRightLogic::ShiftRightLogic(const One<Expression> &lhs, const One<Expression> &rhs)
    : ShiftOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ShiftRightLogic::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `ShiftRightLogic` is complete/fully defined.
 */
void ShiftRightLogic::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ShiftRightLogic::type() const {
    return NodeType::ShiftRightLogic;
}

/**
 * Helper method for visiting nodes.
 */
void ShiftRightLogic::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_shift_right_logic(*this, retval);
}

/**
 * Interprets this node to a node of type ShiftRightLogic. Returns null if it
 * has the wrong type.
 */
ShiftRightLogic *ShiftRightLogic::as_shift_right_logic() {
    return dynamic_cast<ShiftRightLogic*>(this);
}

/**
 * Interprets this node to a node of type ShiftRightLogic. Returns null if it
 * has the wrong type.
 */
const ShiftRightLogic *ShiftRightLogic::as_shift_right_logic() const {
    return dynamic_cast<const ShiftRightLogic*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ShiftRightLogic::copy() const {
    return cqasm::tree::make<ShiftRightLogic>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ShiftRightLogic::clone() const {
    auto node = cqasm::tree::make<ShiftRightLogic>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ShiftRightLogic::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftRightLogic) return false;
    auto rhsc = dynamic_cast<const ShiftRightLogic&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ShiftRightLogic::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ShiftRightLogic) return false;
    auto rhsc = dynamic_cast<const ShiftRightLogic&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ShiftRightLogic::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ShiftRightLogic");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ShiftRightLogic> ShiftRightLogic::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ShiftRightLogic") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ShiftRightLogic>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
StatementList::StatementList(const Any<Statement> &items)
    : items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void StatementList::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    items.find_reachable(map);
}

/**
 * Returns whether this `StatementList` is complete/fully defined.
 */
void StatementList::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    items.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType StatementList::type() const {
    return NodeType::StatementList;
}

/**
 * Helper method for visiting nodes.
 */
void StatementList::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_statement_list(*this, retval);
}

/**
 * Interprets this node to a node of type StatementList. Returns null if it has
 * the wrong type.
 */
StatementList *StatementList::as_statement_list() {
    return dynamic_cast<StatementList*>(this);
}

/**
 * Interprets this node to a node of type StatementList. Returns null if it has
 * the wrong type.
 */
const StatementList *StatementList::as_statement_list() const {
    return dynamic_cast<const StatementList*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> StatementList::copy() const {
    return cqasm::tree::make<StatementList>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> StatementList::clone() const {
    auto node = cqasm::tree::make<StatementList>(*this);
    node->items = this->items.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool StatementList::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::StatementList) return false;
    auto rhsc = dynamic_cast<const StatementList&>(rhs);
    if (!this->items.equals(rhsc.items)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool StatementList::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::StatementList) return false;
    auto rhsc = dynamic_cast<const StatementList&>(rhs);
    if (this->items != rhsc.items) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void StatementList::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "StatementList");
    auto submap = map.append_map("items");
    items.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<StatementList> StatementList::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "StatementList") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<StatementList>(
        Any<Statement>(map.at("items").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
StringLiteral::StringLiteral(const cqasm::v1::primitives::Str &value)
    : Expression(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void StringLiteral::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `StringLiteral` is complete/fully defined.
 */
void StringLiteral::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType StringLiteral::type() const {
    return NodeType::StringLiteral;
}

/**
 * Helper method for visiting nodes.
 */
void StringLiteral::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_string_literal(*this, retval);
}

/**
 * Interprets this node to a node of type StringLiteral. Returns null if it has
 * the wrong type.
 */
StringLiteral *StringLiteral::as_string_literal() {
    return dynamic_cast<StringLiteral*>(this);
}

/**
 * Interprets this node to a node of type StringLiteral. Returns null if it has
 * the wrong type.
 */
const StringLiteral *StringLiteral::as_string_literal() const {
    return dynamic_cast<const StringLiteral*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> StringLiteral::copy() const {
    return cqasm::tree::make<StringLiteral>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> StringLiteral::clone() const {
    auto node = cqasm::tree::make<StringLiteral>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool StringLiteral::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::StringLiteral) return false;
    auto rhsc = dynamic_cast<const StringLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool StringLiteral::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::StringLiteral) return false;
    auto rhsc = dynamic_cast<const StringLiteral&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void StringLiteral::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "StringLiteral");
    auto submap = map.append_map("value");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Str>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<StringLiteral> StringLiteral::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "StringLiteral") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<StringLiteral>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Str>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Subcircuit::Subcircuit(const One<Identifier> &name, const Maybe<Expression> &iterations, const Any<AnnotationData> &annotations)
    : Statement(annotations), name(name), iterations(iterations)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Subcircuit::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    name.find_reachable(map);
    iterations.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Subcircuit` is complete/fully defined.
 */
void Subcircuit::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    name.check_complete(map);
    iterations.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Subcircuit::type() const {
    return NodeType::Subcircuit;
}

/**
 * Helper method for visiting nodes.
 */
void Subcircuit::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_subcircuit(*this, retval);
}

/**
 * Interprets this node to a node of type Subcircuit. Returns null if it has the
 * wrong type.
 */
Subcircuit *Subcircuit::as_subcircuit() {
    return dynamic_cast<Subcircuit*>(this);
}

/**
 * Interprets this node to a node of type Subcircuit. Returns null if it has the
 * wrong type.
 */
const Subcircuit *Subcircuit::as_subcircuit() const {
    return dynamic_cast<const Subcircuit*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Subcircuit::copy() const {
    return cqasm::tree::make<Subcircuit>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Subcircuit::clone() const {
    auto node = cqasm::tree::make<Subcircuit>(*this);
    node->name = this->name.clone();
    node->iterations = this->iterations.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Subcircuit::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Subcircuit) return false;
    auto rhsc = dynamic_cast<const Subcircuit&>(rhs);
    if (!this->name.equals(rhsc.name)) return false;
    if (!this->iterations.equals(rhsc.iterations)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Subcircuit::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Subcircuit) return false;
    auto rhsc = dynamic_cast<const Subcircuit&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->iterations != rhsc.iterations) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Subcircuit::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Subcircuit");
    auto submap = map.append_map("name");
    name.serialize(submap, ids);
    submap.close();
    submap = map.append_map("iterations");
    iterations.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Subcircuit> Subcircuit::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Subcircuit") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Subcircuit>(
        One<Identifier>(map.at("name").as_map(), ids),
        Maybe<Expression>(map.at("iterations").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Subtract::Subtract(const One<Expression> &lhs, const One<Expression> &rhs)
    : ArithOp(lhs, rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Subtract::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
}

/**
 * Returns whether this `Subtract` is complete/fully defined.
 */
void Subtract::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Subtract::type() const {
    return NodeType::Subtract;
}

/**
 * Helper method for visiting nodes.
 */
void Subtract::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_subtract(*this, retval);
}

/**
 * Interprets this node to a node of type Subtract. Returns null if it has the
 * wrong type.
 */
Subtract *Subtract::as_subtract() {
    return dynamic_cast<Subtract*>(this);
}

/**
 * Interprets this node to a node of type Subtract. Returns null if it has the
 * wrong type.
 */
const Subtract *Subtract::as_subtract() const {
    return dynamic_cast<const Subtract*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Subtract::copy() const {
    return cqasm::tree::make<Subtract>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Subtract::clone() const {
    auto node = cqasm::tree::make<Subtract>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Subtract::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Subtract) return false;
    auto rhsc = dynamic_cast<const Subtract&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Subtract::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Subtract) return false;
    auto rhsc = dynamic_cast<const Subtract&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Subtract::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Subtract");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Subtract> Subtract::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Subtract") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Subtract>(
        One<Expression>(map.at("lhs").as_map(), ids),
        One<Expression>(map.at("rhs").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
TernaryCond::TernaryCond(const One<Expression> &cond, const One<Expression> &if_true, const One<Expression> &if_false)
    : Expression(), cond(cond), if_true(if_true), if_false(if_false)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void TernaryCond::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    cond.find_reachable(map);
    if_true.find_reachable(map);
    if_false.find_reachable(map);
}

/**
 * Returns whether this `TernaryCond` is complete/fully defined.
 */
void TernaryCond::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    cond.check_complete(map);
    if_true.check_complete(map);
    if_false.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType TernaryCond::type() const {
    return NodeType::TernaryCond;
}

/**
 * Helper method for visiting nodes.
 */
void TernaryCond::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_ternary_cond(*this, retval);
}

/**
 * Interprets this node to a node of type TernaryCond. Returns null if it has
 * the wrong type.
 */
TernaryCond *TernaryCond::as_ternary_cond() {
    return dynamic_cast<TernaryCond*>(this);
}

/**
 * Interprets this node to a node of type TernaryCond. Returns null if it has
 * the wrong type.
 */
const TernaryCond *TernaryCond::as_ternary_cond() const {
    return dynamic_cast<const TernaryCond*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> TernaryCond::copy() const {
    return cqasm::tree::make<TernaryCond>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> TernaryCond::clone() const {
    auto node = cqasm::tree::make<TernaryCond>(*this);
    node->cond = this->cond.clone();
    node->if_true = this->if_true.clone();
    node->if_false = this->if_false.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool TernaryCond::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::TernaryCond) return false;
    auto rhsc = dynamic_cast<const TernaryCond&>(rhs);
    if (!this->cond.equals(rhsc.cond)) return false;
    if (!this->if_true.equals(rhsc.if_true)) return false;
    if (!this->if_false.equals(rhsc.if_false)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool TernaryCond::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::TernaryCond) return false;
    auto rhsc = dynamic_cast<const TernaryCond&>(rhs);
    if (this->cond != rhsc.cond) return false;
    if (this->if_true != rhsc.if_true) return false;
    if (this->if_false != rhsc.if_false) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void TernaryCond::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "TernaryCond");
    auto submap = map.append_map("cond");
    cond.serialize(submap, ids);
    submap.close();
    submap = map.append_map("if_true");
    if_true.serialize(submap, ids);
    submap.close();
    submap = map.append_map("if_false");
    if_false.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<TernaryCond> TernaryCond::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "TernaryCond") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<TernaryCond>(
        One<Expression>(map.at("cond").as_map(), ids),
        One<Expression>(map.at("if_true").as_map(), ids),
        One<Expression>(map.at("if_false").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Variables::Variables(const Many<Identifier> &names, const One<Identifier> &typ, const Any<AnnotationData> &annotations)
    : Statement(annotations), names(names), typ(typ)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Variables::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    names.find_reachable(map);
    typ.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Variables` is complete/fully defined.
 */
void Variables::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    names.check_complete(map);
    typ.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Variables::type() const {
    return NodeType::Variables;
}

/**
 * Helper method for visiting nodes.
 */
void Variables::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_variables(*this, retval);
}

/**
 * Interprets this node to a node of type Variables. Returns null if it has the
 * wrong type.
 */
Variables *Variables::as_variables() {
    return dynamic_cast<Variables*>(this);
}

/**
 * Interprets this node to a node of type Variables. Returns null if it has the
 * wrong type.
 */
const Variables *Variables::as_variables() const {
    return dynamic_cast<const Variables*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Variables::copy() const {
    return cqasm::tree::make<Variables>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Variables::clone() const {
    auto node = cqasm::tree::make<Variables>(*this);
    node->names = this->names.clone();
    node->typ = this->typ.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Variables::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Variables) return false;
    auto rhsc = dynamic_cast<const Variables&>(rhs);
    if (!this->names.equals(rhsc.names)) return false;
    if (!this->typ.equals(rhsc.typ)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Variables::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Variables) return false;
    auto rhsc = dynamic_cast<const Variables&>(rhs);
    if (this->names != rhsc.names) return false;
    if (this->typ != rhsc.typ) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Variables::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Variables");
    auto submap = map.append_map("names");
    names.serialize(submap, ids);
    submap.close();
    submap = map.append_map("typ");
    typ.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Variables> Variables::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Variables") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Variables>(
        Many<Identifier>(map.at("names").as_map(), ids),
        One<Identifier>(map.at("typ").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Version::Version(const cqasm::v1::primitives::Version &items)
    : items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Version::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Version` is complete/fully defined.
 */
void Version::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Version::type() const {
    return NodeType::Version;
}

/**
 * Helper method for visiting nodes.
 */
void Version::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_version(*this, retval);
}

/**
 * Interprets this node to a node of type Version. Returns null if it has the
 * wrong type.
 */
Version *Version::as_version() {
    return dynamic_cast<Version*>(this);
}

/**
 * Interprets this node to a node of type Version. Returns null if it has the
 * wrong type.
 */
const Version *Version::as_version() const {
    return dynamic_cast<const Version*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Version::copy() const {
    return cqasm::tree::make<Version>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Version::clone() const {
    auto node = cqasm::tree::make<Version>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Version::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Version) return false;
    auto rhsc = dynamic_cast<const Version&>(rhs);
    if (this->items != rhsc.items) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Version::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Version) return false;
    auto rhsc = dynamic_cast<const Version&>(rhs);
    if (this->items != rhsc.items) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Version::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Version");
    auto submap = map.append_map("items");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Version>(items, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Version> Version::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Version") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Version>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Version>(map.at("items").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
WhileLoop::WhileLoop(const One<Expression> &condition, const One<StatementList> &body, const Any<AnnotationData> &annotations)
    : Structured(annotations), condition(condition), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void WhileLoop::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    condition.find_reachable(map);
    body.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `WhileLoop` is complete/fully defined.
 */
void WhileLoop::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    condition.check_complete(map);
    body.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType WhileLoop::type() const {
    return NodeType::WhileLoop;
}

/**
 * Helper method for visiting nodes.
 */
void WhileLoop::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_while_loop(*this, retval);
}

/**
 * Interprets this node to a node of type WhileLoop. Returns null if it has the
 * wrong type.
 */
WhileLoop *WhileLoop::as_while_loop() {
    return dynamic_cast<WhileLoop*>(this);
}

/**
 * Interprets this node to a node of type WhileLoop. Returns null if it has the
 * wrong type.
 */
const WhileLoop *WhileLoop::as_while_loop() const {
    return dynamic_cast<const WhileLoop*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> WhileLoop::copy() const {
    return cqasm::tree::make<WhileLoop>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> WhileLoop::clone() const {
    auto node = cqasm::tree::make<WhileLoop>(*this);
    node->condition = this->condition.clone();
    node->body = this->body.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool WhileLoop::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::WhileLoop) return false;
    auto rhsc = dynamic_cast<const WhileLoop&>(rhs);
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->body.equals(rhsc.body)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool WhileLoop::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::WhileLoop) return false;
    auto rhsc = dynamic_cast<const WhileLoop&>(rhs);
    if (this->condition != rhsc.condition) return false;
    if (this->body != rhsc.body) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void WhileLoop::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "WhileLoop");
    auto submap = map.append_map("condition");
    condition.serialize(submap, ids);
    submap.close();
    submap = map.append_map("body");
    body.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<WhileLoop> WhileLoop::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "WhileLoop") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<WhileLoop>(
        One<Expression>(map.at("condition").as_map(), ids),
        One<StatementList>(map.at("body").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Internal visitor function for nodes of any type.
 */
template <>
void Visitor<void>::raw_visit_node(Node &node, void *retval) {
    (void)retval;
    this->visit_node(node);
}

/**
 * Internal visitor function for `Add` nodes.
 */
template <>
void Visitor<void>::raw_visit_add(Add &node, void *retval) {
    (void)retval;
    this->visit_add(node);
}

/**
 * Internal visitor function for `Annotated` nodes.
 */
template <>
void Visitor<void>::raw_visit_annotated(Annotated &node, void *retval) {
    (void)retval;
    this->visit_annotated(node);
}

/**
 * Internal visitor function for `AnnotationData` nodes.
 */
template <>
void Visitor<void>::raw_visit_annotation_data(AnnotationData &node, void *retval) {
    (void)retval;
    this->visit_annotation_data(node);
}

/**
 * Internal visitor function for `ArithOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_arith_op(ArithOp &node, void *retval) {
    (void)retval;
    this->visit_arith_op(node);
}

/**
 * Internal visitor function for `Assignment` nodes.
 */
template <>
void Visitor<void>::raw_visit_assignment(Assignment &node, void *retval) {
    (void)retval;
    this->visit_assignment(node);
}

/**
 * Internal visitor function for `BinaryOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_binary_op(BinaryOp &node, void *retval) {
    (void)retval;
    this->visit_binary_op(node);
}

/**
 * Internal visitor function for `BitwiseAnd` nodes.
 */
template <>
void Visitor<void>::raw_visit_bitwise_and(BitwiseAnd &node, void *retval) {
    (void)retval;
    this->visit_bitwise_and(node);
}

/**
 * Internal visitor function for `BitwiseNot` nodes.
 */
template <>
void Visitor<void>::raw_visit_bitwise_not(BitwiseNot &node, void *retval) {
    (void)retval;
    this->visit_bitwise_not(node);
}

/**
 * Internal visitor function for `BitwiseOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_bitwise_op(BitwiseOp &node, void *retval) {
    (void)retval;
    this->visit_bitwise_op(node);
}

/**
 * Internal visitor function for `BitwiseOr` nodes.
 */
template <>
void Visitor<void>::raw_visit_bitwise_or(BitwiseOr &node, void *retval) {
    (void)retval;
    this->visit_bitwise_or(node);
}

/**
 * Internal visitor function for `BitwiseXor` nodes.
 */
template <>
void Visitor<void>::raw_visit_bitwise_xor(BitwiseXor &node, void *retval) {
    (void)retval;
    this->visit_bitwise_xor(node);
}

/**
 * Internal visitor function for `BreakStatement` nodes.
 */
template <>
void Visitor<void>::raw_visit_break_statement(BreakStatement &node, void *retval) {
    (void)retval;
    this->visit_break_statement(node);
}

/**
 * Internal visitor function for `Bundle` nodes.
 */
template <>
void Visitor<void>::raw_visit_bundle(Bundle &node, void *retval) {
    (void)retval;
    this->visit_bundle(node);
}

/**
 * Internal visitor function for `CmpEq` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_eq(CmpEq &node, void *retval) {
    (void)retval;
    this->visit_cmp_eq(node);
}

/**
 * Internal visitor function for `CmpGe` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_ge(CmpGe &node, void *retval) {
    (void)retval;
    this->visit_cmp_ge(node);
}

/**
 * Internal visitor function for `CmpGt` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_gt(CmpGt &node, void *retval) {
    (void)retval;
    this->visit_cmp_gt(node);
}

/**
 * Internal visitor function for `CmpLe` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_le(CmpLe &node, void *retval) {
    (void)retval;
    this->visit_cmp_le(node);
}

/**
 * Internal visitor function for `CmpLt` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_lt(CmpLt &node, void *retval) {
    (void)retval;
    this->visit_cmp_lt(node);
}

/**
 * Internal visitor function for `CmpNe` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_ne(CmpNe &node, void *retval) {
    (void)retval;
    this->visit_cmp_ne(node);
}

/**
 * Internal visitor function for `CmpOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_cmp_op(CmpOp &node, void *retval) {
    (void)retval;
    this->visit_cmp_op(node);
}

/**
 * Internal visitor function for `ContinueStatement` nodes.
 */
template <>
void Visitor<void>::raw_visit_continue_statement(ContinueStatement &node, void *retval) {
    (void)retval;
    this->visit_continue_statement(node);
}

/**
 * Internal visitor function for `Divide` nodes.
 */
template <>
void Visitor<void>::raw_visit_divide(Divide &node, void *retval) {
    (void)retval;
    this->visit_divide(node);
}

/**
 * Internal visitor function for `ErroneousExpression` nodes.
 */
template <>
void Visitor<void>::raw_visit_erroneous_expression(ErroneousExpression &node, void *retval) {
    (void)retval;
    this->visit_erroneous_expression(node);
}

/**
 * Internal visitor function for `ErroneousProgram` nodes.
 */
template <>
void Visitor<void>::raw_visit_erroneous_program(ErroneousProgram &node, void *retval) {
    (void)retval;
    this->visit_erroneous_program(node);
}

/**
 * Internal visitor function for `ErroneousStatement` nodes.
 */
template <>
void Visitor<void>::raw_visit_erroneous_statement(ErroneousStatement &node, void *retval) {
    (void)retval;
    this->visit_erroneous_statement(node);
}

/**
 * Internal visitor function for `Expression` nodes.
 */
template <>
void Visitor<void>::raw_visit_expression(Expression &node, void *retval) {
    (void)retval;
    this->visit_expression(node);
}

/**
 * Internal visitor function for `ExpressionList` nodes.
 */
template <>
void Visitor<void>::raw_visit_expression_list(ExpressionList &node, void *retval) {
    (void)retval;
    this->visit_expression_list(node);
}

/**
 * Internal visitor function for `FloatLiteral` nodes.
 */
template <>
void Visitor<void>::raw_visit_float_literal(FloatLiteral &node, void *retval) {
    (void)retval;
    this->visit_float_literal(node);
}

/**
 * Internal visitor function for `ForLoop` nodes.
 */
template <>
void Visitor<void>::raw_visit_for_loop(ForLoop &node, void *retval) {
    (void)retval;
    this->visit_for_loop(node);
}

/**
 * Internal visitor function for `ForeachLoop` nodes.
 */
template <>
void Visitor<void>::raw_visit_foreach_loop(ForeachLoop &node, void *retval) {
    (void)retval;
    this->visit_foreach_loop(node);
}

/**
 * Internal visitor function for `FunctionCall` nodes.
 */
template <>
void Visitor<void>::raw_visit_function_call(FunctionCall &node, void *retval) {
    (void)retval;
    this->visit_function_call(node);
}

/**
 * Internal visitor function for `Identifier` nodes.
 */
template <>
void Visitor<void>::raw_visit_identifier(Identifier &node, void *retval) {
    (void)retval;
    this->visit_identifier(node);
}

/**
 * Internal visitor function for `IfElse` nodes.
 */
template <>
void Visitor<void>::raw_visit_if_else(IfElse &node, void *retval) {
    (void)retval;
    this->visit_if_else(node);
}

/**
 * Internal visitor function for `IfElseBranch` nodes.
 */
template <>
void Visitor<void>::raw_visit_if_else_branch(IfElseBranch &node, void *retval) {
    (void)retval;
    this->visit_if_else_branch(node);
}

/**
 * Internal visitor function for `Index` nodes.
 */
template <>
void Visitor<void>::raw_visit_index(Index &node, void *retval) {
    (void)retval;
    this->visit_index(node);
}

/**
 * Internal visitor function for `IndexEntry` nodes.
 */
template <>
void Visitor<void>::raw_visit_index_entry(IndexEntry &node, void *retval) {
    (void)retval;
    this->visit_index_entry(node);
}

/**
 * Internal visitor function for `IndexItem` nodes.
 */
template <>
void Visitor<void>::raw_visit_index_item(IndexItem &node, void *retval) {
    (void)retval;
    this->visit_index_item(node);
}

/**
 * Internal visitor function for `IndexList` nodes.
 */
template <>
void Visitor<void>::raw_visit_index_list(IndexList &node, void *retval) {
    (void)retval;
    this->visit_index_list(node);
}

/**
 * Internal visitor function for `IndexRange` nodes.
 */
template <>
void Visitor<void>::raw_visit_index_range(IndexRange &node, void *retval) {
    (void)retval;
    this->visit_index_range(node);
}

/**
 * Internal visitor function for `Instruction` nodes.
 */
template <>
void Visitor<void>::raw_visit_instruction(Instruction &node, void *retval) {
    (void)retval;
    this->visit_instruction(node);
}

/**
 * Internal visitor function for `IntDivide` nodes.
 */
template <>
void Visitor<void>::raw_visit_int_divide(IntDivide &node, void *retval) {
    (void)retval;
    this->visit_int_divide(node);
}

/**
 * Internal visitor function for `IntegerLiteral` nodes.
 */
template <>
void Visitor<void>::raw_visit_integer_literal(IntegerLiteral &node, void *retval) {
    (void)retval;
    this->visit_integer_literal(node);
}

/**
 * Internal visitor function for `JsonLiteral` nodes.
 */
template <>
void Visitor<void>::raw_visit_json_literal(JsonLiteral &node, void *retval) {
    (void)retval;
    this->visit_json_literal(node);
}

/**
 * Internal visitor function for `LogicalAnd` nodes.
 */
template <>
void Visitor<void>::raw_visit_logical_and(LogicalAnd &node, void *retval) {
    (void)retval;
    this->visit_logical_and(node);
}

/**
 * Internal visitor function for `LogicalNot` nodes.
 */
template <>
void Visitor<void>::raw_visit_logical_not(LogicalNot &node, void *retval) {
    (void)retval;
    this->visit_logical_not(node);
}

/**
 * Internal visitor function for `LogicalOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_logical_op(LogicalOp &node, void *retval) {
    (void)retval;
    this->visit_logical_op(node);
}

/**
 * Internal visitor function for `LogicalOr` nodes.
 */
template <>
void Visitor<void>::raw_visit_logical_or(LogicalOr &node, void *retval) {
    (void)retval;
    this->visit_logical_or(node);
}

/**
 * Internal visitor function for `LogicalXor` nodes.
 */
template <>
void Visitor<void>::raw_visit_logical_xor(LogicalXor &node, void *retval) {
    (void)retval;
    this->visit_logical_xor(node);
}

/**
 * Internal visitor function for `Mapping` nodes.
 */
template <>
void Visitor<void>::raw_visit_mapping(Mapping &node, void *retval) {
    (void)retval;
    this->visit_mapping(node);
}

/**
 * Internal visitor function for `MatrixLiteral` nodes.
 */
template <>
void Visitor<void>::raw_visit_matrix_literal(MatrixLiteral &node, void *retval) {
    (void)retval;
    this->visit_matrix_literal(node);
}

/**
 * Internal visitor function for `Modulo` nodes.
 */
template <>
void Visitor<void>::raw_visit_modulo(Modulo &node, void *retval) {
    (void)retval;
    this->visit_modulo(node);
}

/**
 * Internal visitor function for `Multiply` nodes.
 */
template <>
void Visitor<void>::raw_visit_multiply(Multiply &node, void *retval) {
    (void)retval;
    this->visit_multiply(node);
}

/**
 * Internal visitor function for `Negate` nodes.
 */
template <>
void Visitor<void>::raw_visit_negate(Negate &node, void *retval) {
    (void)retval;
    this->visit_negate(node);
}

/**
 * Internal visitor function for `Power` nodes.
 */
template <>
void Visitor<void>::raw_visit_power(Power &node, void *retval) {
    (void)retval;
    this->visit_power(node);
}

/**
 * Internal visitor function for `Program` nodes.
 */
template <>
void Visitor<void>::raw_visit_program(Program &node, void *retval) {
    (void)retval;
    this->visit_program(node);
}

/**
 * Internal visitor function for `RepeatUntilLoop` nodes.
 */
template <>
void Visitor<void>::raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) {
    (void)retval;
    this->visit_repeat_until_loop(node);
}

/**
 * Internal visitor function for `Root` nodes.
 */
template <>
void Visitor<void>::raw_visit_root(Root &node, void *retval) {
    (void)retval;
    this->visit_root(node);
}

/**
 * Internal visitor function for `ShiftLeft` nodes.
 */
template <>
void Visitor<void>::raw_visit_shift_left(ShiftLeft &node, void *retval) {
    (void)retval;
    this->visit_shift_left(node);
}

/**
 * Internal visitor function for `ShiftOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_shift_op(ShiftOp &node, void *retval) {
    (void)retval;
    this->visit_shift_op(node);
}

/**
 * Internal visitor function for `ShiftRightArith` nodes.
 */
template <>
void Visitor<void>::raw_visit_shift_right_arith(ShiftRightArith &node, void *retval) {
    (void)retval;
    this->visit_shift_right_arith(node);
}

/**
 * Internal visitor function for `ShiftRightLogic` nodes.
 */
template <>
void Visitor<void>::raw_visit_shift_right_logic(ShiftRightLogic &node, void *retval) {
    (void)retval;
    this->visit_shift_right_logic(node);
}

/**
 * Internal visitor function for `Statement` nodes.
 */
template <>
void Visitor<void>::raw_visit_statement(Statement &node, void *retval) {
    (void)retval;
    this->visit_statement(node);
}

/**
 * Internal visitor function for `StatementList` nodes.
 */
template <>
void Visitor<void>::raw_visit_statement_list(StatementList &node, void *retval) {
    (void)retval;
    this->visit_statement_list(node);
}

/**
 * Internal visitor function for `StringLiteral` nodes.
 */
template <>
void Visitor<void>::raw_visit_string_literal(StringLiteral &node, void *retval) {
    (void)retval;
    this->visit_string_literal(node);
}

/**
 * Internal visitor function for `Structured` nodes.
 */
template <>
void Visitor<void>::raw_visit_structured(Structured &node, void *retval) {
    (void)retval;
    this->visit_structured(node);
}

/**
 * Internal visitor function for `Subcircuit` nodes.
 */
template <>
void Visitor<void>::raw_visit_subcircuit(Subcircuit &node, void *retval) {
    (void)retval;
    this->visit_subcircuit(node);
}

/**
 * Internal visitor function for `Subtract` nodes.
 */
template <>
void Visitor<void>::raw_visit_subtract(Subtract &node, void *retval) {
    (void)retval;
    this->visit_subtract(node);
}

/**
 * Internal visitor function for `TernaryCond` nodes.
 */
template <>
void Visitor<void>::raw_visit_ternary_cond(TernaryCond &node, void *retval) {
    (void)retval;
    this->visit_ternary_cond(node);
}

/**
 * Internal visitor function for `UnaryOp` nodes.
 */
template <>
void Visitor<void>::raw_visit_unary_op(UnaryOp &node, void *retval) {
    (void)retval;
    this->visit_unary_op(node);
}

/**
 * Internal visitor function for `Variables` nodes.
 */
template <>
void Visitor<void>::raw_visit_variables(Variables &node, void *retval) {
    (void)retval;
    this->visit_variables(node);
}

/**
 * Internal visitor function for `Version` nodes.
 */
template <>
void Visitor<void>::raw_visit_version(Version &node, void *retval) {
    (void)retval;
    this->visit_version(node);
}

/**
 * Internal visitor function for `WhileLoop` nodes.
 */
template <>
void Visitor<void>::raw_visit_while_loop(WhileLoop &node, void *retval) {
    (void)retval;
    this->visit_while_loop(node);
}

/**
 * Recursive traversal for `Add` nodes.
 */
void RecursiveVisitor::visit_add(Add &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `Annotated` nodes.
 */
void RecursiveVisitor::visit_annotated(Annotated &node) {
    visit_node(node);
    node.annotations.visit(*this);
}

/**
 * Recursive traversal for `AnnotationData` nodes.
 */
void RecursiveVisitor::visit_annotation_data(AnnotationData &node) {
    visit_node(node);
    node.interface.visit(*this);
    node.operation.visit(*this);
    node.operands.visit(*this);
}

/**
 * Recursive traversal for `ArithOp` nodes.
 */
void RecursiveVisitor::visit_arith_op(ArithOp &node) {
    visit_binary_op(node);
}

/**
 * Recursive traversal for `Assignment` nodes.
 */
void RecursiveVisitor::visit_assignment(Assignment &node) {
    visit_node(node);
    node.lhs.visit(*this);
    node.rhs.visit(*this);
}

/**
 * Recursive traversal for `BinaryOp` nodes.
 */
void RecursiveVisitor::visit_binary_op(BinaryOp &node) {
    visit_expression(node);
    node.lhs.visit(*this);
    node.rhs.visit(*this);
}

/**
 * Recursive traversal for `BitwiseAnd` nodes.
 */
void RecursiveVisitor::visit_bitwise_and(BitwiseAnd &node) {
    visit_bitwise_op(node);
}

/**
 * Recursive traversal for `BitwiseNot` nodes.
 */
void RecursiveVisitor::visit_bitwise_not(BitwiseNot &node) {
    visit_unary_op(node);
}

/**
 * Recursive traversal for `BitwiseOp` nodes.
 */
void RecursiveVisitor::visit_bitwise_op(BitwiseOp &node) {
    visit_binary_op(node);
}

/**
 * Recursive traversal for `BitwiseOr` nodes.
 */
void RecursiveVisitor::visit_bitwise_or(BitwiseOr &node) {
    visit_bitwise_op(node);
}

/**
 * Recursive traversal for `BitwiseXor` nodes.
 */
void RecursiveVisitor::visit_bitwise_xor(BitwiseXor &node) {
    visit_bitwise_op(node);
}

/**
 * Recursive traversal for `BreakStatement` nodes.
 */
void RecursiveVisitor::visit_break_statement(BreakStatement &node) {
    visit_structured(node);
}

/**
 * Recursive traversal for `Bundle` nodes.
 */
void RecursiveVisitor::visit_bundle(Bundle &node) {
    visit_statement(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `CmpEq` nodes.
 */
void RecursiveVisitor::visit_cmp_eq(CmpEq &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpGe` nodes.
 */
void RecursiveVisitor::visit_cmp_ge(CmpGe &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpGt` nodes.
 */
void RecursiveVisitor::visit_cmp_gt(CmpGt &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpLe` nodes.
 */
void RecursiveVisitor::visit_cmp_le(CmpLe &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpLt` nodes.
 */
void RecursiveVisitor::visit_cmp_lt(CmpLt &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpNe` nodes.
 */
void RecursiveVisitor::visit_cmp_ne(CmpNe &node) {
    visit_cmp_op(node);
}

/**
 * Recursive traversal for `CmpOp` nodes.
 */
void RecursiveVisitor::visit_cmp_op(CmpOp &node) {
    visit_binary_op(node);
}

/**
 * Recursive traversal for `ContinueStatement` nodes.
 */
void RecursiveVisitor::visit_continue_statement(ContinueStatement &node) {
    visit_structured(node);
}

/**
 * Recursive traversal for `Divide` nodes.
 */
void RecursiveVisitor::visit_divide(Divide &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `ErroneousExpression` nodes.
 */
void RecursiveVisitor::visit_erroneous_expression(ErroneousExpression &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `ErroneousProgram` nodes.
 */
void RecursiveVisitor::visit_erroneous_program(ErroneousProgram &node) {
    visit_root(node);
}

/**
 * Recursive traversal for `ErroneousStatement` nodes.
 */
void RecursiveVisitor::visit_erroneous_statement(ErroneousStatement &node) {
    visit_statement(node);
}

/**
 * Recursive traversal for `Expression` nodes.
 */
void RecursiveVisitor::visit_expression(Expression &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `ExpressionList` nodes.
 */
void RecursiveVisitor::visit_expression_list(ExpressionList &node) {
    visit_node(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `FloatLiteral` nodes.
 */
void RecursiveVisitor::visit_float_literal(FloatLiteral &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `ForLoop` nodes.
 */
void RecursiveVisitor::visit_for_loop(ForLoop &node) {
    visit_structured(node);
    node.initialize.visit(*this);
    node.condition.visit(*this);
    node.update.visit(*this);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `ForeachLoop` nodes.
 */
void RecursiveVisitor::visit_foreach_loop(ForeachLoop &node) {
    visit_structured(node);
    node.lhs.visit(*this);
    node.frm.visit(*this);
    node.to.visit(*this);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `FunctionCall` nodes.
 */
void RecursiveVisitor::visit_function_call(FunctionCall &node) {
    visit_expression(node);
    node.name.visit(*this);
    node.arguments.visit(*this);
}

/**
 * Recursive traversal for `Identifier` nodes.
 */
void RecursiveVisitor::visit_identifier(Identifier &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `IfElse` nodes.
 */
void RecursiveVisitor::visit_if_else(IfElse &node) {
    visit_structured(node);
    node.branches.visit(*this);
    node.otherwise.visit(*this);
}

/**
 * Recursive traversal for `IfElseBranch` nodes.
 */
void RecursiveVisitor::visit_if_else_branch(IfElseBranch &node) {
    visit_node(node);
    node.condition.visit(*this);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `Index` nodes.
 */
void RecursiveVisitor::visit_index(Index &node) {
    visit_expression(node);
    node.expr.visit(*this);
    node.indices.visit(*this);
}

/**
 * Recursive traversal for `IndexEntry` nodes.
 */
void RecursiveVisitor::visit_index_entry(IndexEntry &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `IndexItem` nodes.
 */
void RecursiveVisitor::visit_index_item(IndexItem &node) {
    visit_index_entry(node);
    node.index.visit(*this);
}

/**
 * Recursive traversal for `IndexList` nodes.
 */
void RecursiveVisitor::visit_index_list(IndexList &node) {
    visit_node(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `IndexRange` nodes.
 */
void RecursiveVisitor::visit_index_range(IndexRange &node) {
    visit_index_entry(node);
    node.first.visit(*this);
    node.last.visit(*this);
}

/**
 * Recursive traversal for `Instruction` nodes.
 */
void RecursiveVisitor::visit_instruction(Instruction &node) {
    visit_annotated(node);
    node.name.visit(*this);
    node.condition.visit(*this);
    node.operands.visit(*this);
}

/**
 * Recursive traversal for `IntDivide` nodes.
 */
void RecursiveVisitor::visit_int_divide(IntDivide &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `IntegerLiteral` nodes.
 */
void RecursiveVisitor::visit_integer_literal(IntegerLiteral &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `JsonLiteral` nodes.
 */
void RecursiveVisitor::visit_json_literal(JsonLiteral &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `LogicalAnd` nodes.
 */
void RecursiveVisitor::visit_logical_and(LogicalAnd &node) {
    visit_logical_op(node);
}

/**
 * Recursive traversal for `LogicalNot` nodes.
 */
void RecursiveVisitor::visit_logical_not(LogicalNot &node) {
    visit_unary_op(node);
}

/**
 * Recursive traversal for `LogicalOp` nodes.
 */
void RecursiveVisitor::visit_logical_op(LogicalOp &node) {
    visit_binary_op(node);
}

/**
 * Recursive traversal for `LogicalOr` nodes.
 */
void RecursiveVisitor::visit_logical_or(LogicalOr &node) {
    visit_logical_op(node);
}

/**
 * Recursive traversal for `LogicalXor` nodes.
 */
void RecursiveVisitor::visit_logical_xor(LogicalXor &node) {
    visit_logical_op(node);
}

/**
 * Recursive traversal for `Mapping` nodes.
 */
void RecursiveVisitor::visit_mapping(Mapping &node) {
    visit_statement(node);
    node.alias.visit(*this);
    node.expr.visit(*this);
}

/**
 * Recursive traversal for `MatrixLiteral` nodes.
 */
void RecursiveVisitor::visit_matrix_literal(MatrixLiteral &node) {
    visit_expression(node);
    node.rows.visit(*this);
}

/**
 * Recursive traversal for `Modulo` nodes.
 */
void RecursiveVisitor::visit_modulo(Modulo &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `Multiply` nodes.
 */
void RecursiveVisitor::visit_multiply(Multiply &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `Negate` nodes.
 */
void RecursiveVisitor::visit_negate(Negate &node) {
    visit_unary_op(node);
}

/**
 * Recursive traversal for `Power` nodes.
 */
void RecursiveVisitor::visit_power(Power &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `Program` nodes.
 */
void RecursiveVisitor::visit_program(Program &node) {
    visit_root(node);
    node.version.visit(*this);
    node.num_qubits.visit(*this);
    node.statements.visit(*this);
}

/**
 * Recursive traversal for `RepeatUntilLoop` nodes.
 */
void RecursiveVisitor::visit_repeat_until_loop(RepeatUntilLoop &node) {
    visit_structured(node);
    node.body.visit(*this);
    node.condition.visit(*this);
}

/**
 * Recursive traversal for `Root` nodes.
 */
void RecursiveVisitor::visit_root(Root &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `ShiftLeft` nodes.
 */
void RecursiveVisitor::visit_shift_left(ShiftLeft &node) {
    visit_shift_op(node);
}

/**
 * Recursive traversal for `ShiftOp` nodes.
 */
void RecursiveVisitor::visit_shift_op(ShiftOp &node) {
    visit_binary_op(node);
}

/**
 * Recursive traversal for `ShiftRightArith` nodes.
 */
void RecursiveVisitor::visit_shift_right_arith(ShiftRightArith &node) {
    visit_shift_op(node);
}

/**
 * Recursive traversal for `ShiftRightLogic` nodes.
 */
void RecursiveVisitor::visit_shift_right_logic(ShiftRightLogic &node) {
    visit_shift_op(node);
}

/**
 * Recursive traversal for `Statement` nodes.
 */
void RecursiveVisitor::visit_statement(Statement &node) {
    visit_annotated(node);
}

/**
 * Recursive traversal for `StatementList` nodes.
 */
void RecursiveVisitor::visit_statement_list(StatementList &node) {
    visit_node(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `StringLiteral` nodes.
 */
void RecursiveVisitor::visit_string_literal(StringLiteral &node) {
    visit_expression(node);
}

/**
 * Recursive traversal for `Structured` nodes.
 */
void RecursiveVisitor::visit_structured(Structured &node) {
    visit_statement(node);
}

/**
 * Recursive traversal for `Subcircuit` nodes.
 */
void RecursiveVisitor::visit_subcircuit(Subcircuit &node) {
    visit_statement(node);
    node.name.visit(*this);
    node.iterations.visit(*this);
}

/**
 * Recursive traversal for `Subtract` nodes.
 */
void RecursiveVisitor::visit_subtract(Subtract &node) {
    visit_arith_op(node);
}

/**
 * Recursive traversal for `TernaryCond` nodes.
 */
void RecursiveVisitor::visit_ternary_cond(TernaryCond &node) {
    visit_expression(node);
    node.cond.visit(*this);
    node.if_true.visit(*this);
    node.if_false.visit(*this);
}

/**
 * Recursive traversal for `UnaryOp` nodes.
 */
void RecursiveVisitor::visit_unary_op(UnaryOp &node) {
    visit_expression(node);
    node.expr.visit(*this);
}

/**
 * Recursive traversal for `Variables` nodes.
 */
void RecursiveVisitor::visit_variables(Variables &node) {
    visit_statement(node);
    node.names.visit(*this);
    node.typ.visit(*this);
}

/**
 * Recursive traversal for `Version` nodes.
 */
void RecursiveVisitor::visit_version(Version &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `WhileLoop` nodes.
 */
void RecursiveVisitor::visit_while_loop(WhileLoop &node) {
    visit_structured(node);
    node.condition.visit(*this);
    node.body.visit(*this);
}

/**
 * Writes the current indentation level's worth of spaces.
 */
void Dumper::write_indent() {
    for (int i = 0; i < indent; i++) {
        out << "  ";
    }
}

/**
 * Dumps a `Node`.
 */
void Dumper::visit_node(Node &node) {
    (void)node;
    write_indent();
    out << "!Node()" << std::endl;
}

/**
 * Dumps a `Add` node.
 */
void Dumper::visit_add(Add &node) {
    write_indent();
    out << "Add";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Annotated` node.
 */
void Dumper::visit_annotated(Annotated &node) {
    write_indent();
    out << "Annotated";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `AnnotationData` node.
 */
void Dumper::visit_annotation_data(AnnotationData &node) {
    write_indent();
    out << "AnnotationData";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "interface: ";
    if (node.interface.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.interface.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "operation: ";
    if (node.operation.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.operation.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "operands: ";
    if (node.operands.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.operands.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ArithOp` node.
 */
void Dumper::visit_arith_op(ArithOp &node) {
    write_indent();
    out << "ArithOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Assignment` node.
 */
void Dumper::visit_assignment(Assignment &node) {
    write_indent();
    out << "Assignment";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BinaryOp` node.
 */
void Dumper::visit_binary_op(BinaryOp &node) {
    write_indent();
    out << "BinaryOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BitwiseAnd` node.
 */
void Dumper::visit_bitwise_and(BitwiseAnd &node) {
    write_indent();
    out << "BitwiseAnd";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BitwiseNot` node.
 */
void Dumper::visit_bitwise_not(BitwiseNot &node) {
    write_indent();
    out << "BitwiseNot";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BitwiseOp` node.
 */
void Dumper::visit_bitwise_op(BitwiseOp &node) {
    write_indent();
    out << "BitwiseOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BitwiseOr` node.
 */
void Dumper::visit_bitwise_or(BitwiseOr &node) {
    write_indent();
    out << "BitwiseOr";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BitwiseXor` node.
 */
void Dumper::visit_bitwise_xor(BitwiseXor &node) {
    write_indent();
    out << "BitwiseXor";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `BreakStatement` node.
 */
void Dumper::visit_break_statement(BreakStatement &node) {
    write_indent();
    out << "BreakStatement";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Bundle` node.
 */
void Dumper::visit_bundle(Bundle &node) {
    write_indent();
    out << "Bundle";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "items: ";
    if (node.items.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.items) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpEq` node.
 */
void Dumper::visit_cmp_eq(CmpEq &node) {
    write_indent();
    out << "CmpEq";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpGe` node.
 */
void Dumper::visit_cmp_ge(CmpGe &node) {
    write_indent();
    out << "CmpGe";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpGt` node.
 */
void Dumper::visit_cmp_gt(CmpGt &node) {
    write_indent();
    out << "CmpGt";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpLe` node.
 */
void Dumper::visit_cmp_le(CmpLe &node) {
    write_indent();
    out << "CmpLe";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpLt` node.
 */
void Dumper::visit_cmp_lt(CmpLt &node) {
    write_indent();
    out << "CmpLt";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpNe` node.
 */
void Dumper::visit_cmp_ne(CmpNe &node) {
    write_indent();
    out << "CmpNe";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `CmpOp` node.
 */
void Dumper::visit_cmp_op(CmpOp &node) {
    write_indent();
    out << "CmpOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ContinueStatement` node.
 */
void Dumper::visit_continue_statement(ContinueStatement &node) {
    write_indent();
    out << "ContinueStatement";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Divide` node.
 */
void Dumper::visit_divide(Divide &node) {
    write_indent();
    out << "Divide";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ErroneousExpression` node.
 */
void Dumper::visit_erroneous_expression(ErroneousExpression &node) {
    write_indent();
    out << "ErroneousExpression";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    out << ")" << std::endl;
}

/**
 * Dumps a `ErroneousProgram` node.
 */
void Dumper::visit_erroneous_program(ErroneousProgram &node) {
    write_indent();
    out << "ErroneousProgram";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    out << ")" << std::endl;
}

/**
 * Dumps a `ErroneousStatement` node.
 */
void Dumper::visit_erroneous_statement(ErroneousStatement &node) {
    write_indent();
    out << "ErroneousStatement";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Expression` node.
 */
void Dumper::visit_expression(Expression &node) {
    write_indent();
    out << "Expression";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    out << ")" << std::endl;
}

/**
 * Dumps a `ExpressionList` node.
 */
void Dumper::visit_expression_list(ExpressionList &node) {
    write_indent();
    out << "ExpressionList";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "items: ";
    if (node.items.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.items) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `FloatLiteral` node.
 */
void Dumper::visit_float_literal(FloatLiteral &node) {
    write_indent();
    out << "FloatLiteral";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "value: ";
    std::stringstream ss;
    size_t pos;
    ss << node.value;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Real<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ForLoop` node.
 */
void Dumper::visit_for_loop(ForLoop &node) {
    write_indent();
    out << "ForLoop";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "initialize: ";
    if (node.initialize.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.initialize.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "condition: ";
    if (node.condition.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.condition.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "update: ";
    if (node.update.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.update.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.body.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ForeachLoop` node.
 */
void Dumper::visit_foreach_loop(ForeachLoop &node) {
    write_indent();
    out << "ForeachLoop";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "frm: ";
    if (node.frm.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.frm.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "to: ";
    if (node.to.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.to.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.body.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `FunctionCall` node.
 */
void Dumper::visit_function_call(FunctionCall &node) {
    write_indent();
    out << "FunctionCall";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "name: ";
    if (node.name.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.name.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "arguments: ";
    if (node.arguments.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.arguments.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Identifier` node.
 */
void Dumper::visit_identifier(Identifier &node) {
    write_indent();
    out << "Identifier";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "name: ";
    std::stringstream ss;
    size_t pos;
    ss << node.name;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Str<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IfElse` node.
 */
void Dumper::visit_if_else(IfElse &node) {
    write_indent();
    out << "IfElse";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "branches: ";
    if (node.branches.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.branches) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    write_indent();
    out << "otherwise: ";
    if (node.otherwise.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.otherwise.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IfElseBranch` node.
 */
void Dumper::visit_if_else_branch(IfElseBranch &node) {
    write_indent();
    out << "IfElseBranch";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "condition: ";
    if (node.condition.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.condition.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.body.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Index` node.
 */
void Dumper::visit_index(Index &node) {
    write_indent();
    out << "Index";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "indices: ";
    if (node.indices.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.indices.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IndexEntry` node.
 */
void Dumper::visit_index_entry(IndexEntry &node) {
    write_indent();
    out << "IndexEntry";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    out << ")" << std::endl;
}

/**
 * Dumps a `IndexItem` node.
 */
void Dumper::visit_index_item(IndexItem &node) {
    write_indent();
    out << "IndexItem";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "index: ";
    if (node.index.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.index.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IndexList` node.
 */
void Dumper::visit_index_list(IndexList &node) {
    write_indent();
    out << "IndexList";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "items: ";
    if (node.items.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.items) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IndexRange` node.
 */
void Dumper::visit_index_range(IndexRange &node) {
    write_indent();
    out << "IndexRange";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "first: ";
    if (node.first.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.first.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "last: ";
    if (node.last.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.last.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Instruction` node.
 */
void Dumper::visit_instruction(Instruction &node) {
    write_indent();
    out << "Instruction";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "name: ";
    if (node.name.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.name.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "condition: ";
    if (node.condition.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.condition.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "operands: ";
    if (node.operands.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.operands.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IntDivide` node.
 */
void Dumper::visit_int_divide(IntDivide &node) {
    write_indent();
    out << "IntDivide";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `IntegerLiteral` node.
 */
void Dumper::visit_integer_literal(IntegerLiteral &node) {
    write_indent();
    out << "IntegerLiteral";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "value: ";
    std::stringstream ss;
    size_t pos;
    ss << node.value;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Int<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `JsonLiteral` node.
 */
void Dumper::visit_json_literal(JsonLiteral &node) {
    write_indent();
    out << "JsonLiteral";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "value: ";
    std::stringstream ss;
    size_t pos;
    ss << node.value;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Str<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `LogicalAnd` node.
 */
void Dumper::visit_logical_and(LogicalAnd &node) {
    write_indent();
    out << "LogicalAnd";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `LogicalNot` node.
 */
void Dumper::visit_logical_not(LogicalNot &node) {
    write_indent();
    out << "LogicalNot";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `LogicalOp` node.
 */
void Dumper::visit_logical_op(LogicalOp &node) {
    write_indent();
    out << "LogicalOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `LogicalOr` node.
 */
void Dumper::visit_logical_or(LogicalOr &node) {
    write_indent();
    out << "LogicalOr";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `LogicalXor` node.
 */
void Dumper::visit_logical_xor(LogicalXor &node) {
    write_indent();
    out << "LogicalXor";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Mapping` node.
 */
void Dumper::visit_mapping(Mapping &node) {
    write_indent();
    out << "Mapping";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "alias: ";
    if (node.alias.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.alias.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `MatrixLiteral` node.
 */
void Dumper::visit_matrix_literal(MatrixLiteral &node) {
    write_indent();
    out << "MatrixLiteral";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "rows: ";
    if (node.rows.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.rows) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Modulo` node.
 */
void Dumper::visit_modulo(Modulo &node) {
    write_indent();
    out << "Modulo";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Multiply` node.
 */
void Dumper::visit_multiply(Multiply &node) {
    write_indent();
    out << "Multiply";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Negate` node.
 */
void Dumper::visit_negate(Negate &node) {
    write_indent();
    out << "Negate";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Power` node.
 */
void Dumper::visit_power(Power &node) {
    write_indent();
    out << "Power";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Program` node.
 */
void Dumper::visit_program(Program &node) {
    write_indent();
    out << "Program";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "version: ";
    if (node.version.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.version.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "num_qubits: ";
    if (node.num_qubits.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.num_qubits.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "statements: ";
    if (node.statements.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.statements.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `RepeatUntilLoop` node.
 */
void Dumper::visit_repeat_until_loop(RepeatUntilLoop &node) {
    write_indent();
    out << "RepeatUntilLoop";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.body.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "condition: ";
    if (node.condition.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.condition.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Root` node.
 */
void Dumper::visit_root(Root &node) {
    write_indent();
    out << "Root";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    out << ")" << std::endl;
}

/**
 * Dumps a `ShiftLeft` node.
 */
void Dumper::visit_shift_left(ShiftLeft &node) {
    write_indent();
    out << "ShiftLeft";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ShiftOp` node.
 */
void Dumper::visit_shift_op(ShiftOp &node) {
    write_indent();
    out << "ShiftOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ShiftRightArith` node.
 */
void Dumper::visit_shift_right_arith(ShiftRightArith &node) {
    write_indent();
    out << "ShiftRightArith";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `ShiftRightLogic` node.
 */
void Dumper::visit_shift_right_logic(ShiftRightLogic &node) {
    write_indent();
    out << "ShiftRightLogic";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Statement` node.
 */
void Dumper::visit_statement(Statement &node) {
    write_indent();
    out << "Statement";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `StatementList` node.
 */
void Dumper::visit_statement_list(StatementList &node) {
    write_indent();
    out << "StatementList";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "items: ";
    if (node.items.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.items) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `StringLiteral` node.
 */
void Dumper::visit_string_literal(StringLiteral &node) {
    write_indent();
    out << "StringLiteral";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "value: ";
    std::stringstream ss;
    size_t pos;
    ss << node.value;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Str<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Structured` node.
 */
void Dumper::visit_structured(Structured &node) {
    write_indent();
    out << "Structured";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Subcircuit` node.
 */
void Dumper::visit_subcircuit(Subcircuit &node) {
    write_indent();
    out << "Subcircuit";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "name: ";
    if (node.name.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.name.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "iterations: ";
    if (node.iterations.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.iterations.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Subtract` node.
 */
void Dumper::visit_subtract(Subtract &node) {
    write_indent();
    out << "Subtract";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "lhs: ";
    if (node.lhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.lhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "rhs: ";
    if (node.rhs.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.rhs.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `TernaryCond` node.
 */
void Dumper::visit_ternary_cond(TernaryCond &node) {
    write_indent();
    out << "TernaryCond";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "cond: ";
    if (node.cond.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.cond.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "if_true: ";
    if (node.if_true.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.if_true.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "if_false: ";
    if (node.if_false.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.if_false.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `UnaryOp` node.
 */
void Dumper::visit_unary_op(UnaryOp &node) {
    write_indent();
    out << "UnaryOp";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "expr: ";
    if (node.expr.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.expr.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Variables` node.
 */
void Dumper::visit_variables(Variables &node) {
    write_indent();
    out << "Variables";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "names: ";
    if (node.names.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.names) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    write_indent();
    out << "typ: ";
    if (node.typ.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.typ.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Version` node.
 */
void Dumper::visit_version(Version &node) {
    write_indent();
    out << "Version";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "items: ";
    std::stringstream ss;
    size_t pos;
    ss << node.items;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Version<<" << std::endl;
        indent++;
        std::string s;
        while (!ss.eof()) {
            std::getline(ss, s);
            write_indent();
            out << s << std::endl;
        }
        indent--;
        write_indent();
        out << ">>" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `WhileLoop` node.
 */
void Dumper::visit_while_loop(WhileLoop &node) {
    write_indent();
    out << "WhileLoop";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    if (auto loc = node.get_annotation_ptr<cqasm::annotations::SourceLocation>()) {
        out << " # " << *loc;
    }
    out << std::endl;
    indent++;
    write_indent();
    out << "condition: ";
    if (node.condition.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.condition.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.body.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "annotations: ";
    if (node.annotations.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.annotations) {
            if (!sptr.empty()) {
                sptr->visit(*this);
            } else {
                write_indent();
                out << "!NULL" << std::endl;
            }
        }
        indent--;
        write_indent();
        out << "]" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Visit this object.
 */
template <>
void Node::visit(Visitor<void> &visitor) {
    this->visit_internal(visitor);
}

/**
 * Stream << overload for tree nodes (writes debug dump).
 */
std::ostream &operator<<(std::ostream &os, const Node &object) {
    const_cast<Node&>(object).dump(os);
    return os;
}

} // namespace ast
} // namespace v1
} // namespace cqasm

