/** \file
 * Header file for the abstract syntax tree node classes.
 */

#pragma once

#include <iostream>
#include "cqasm-tree.hpp"
#include "v1/cqasm-primitives.hpp"

namespace cqasm {
namespace v1 {

/**
 * Namespace for the abstract syntax tree node classes.
 */
/**
 * \dot
 * digraph example {
 *   node [shape=record, fontname=Helvetica, fontsize=10];
 *   Add [ label="Add" URL="\ref cqasm::v1::ast::Add"];
 *   Annotated [ label="Annotated" URL="\ref cqasm::v1::ast::Annotated", style=dotted];
 *   AnnotationData [ label="AnnotationData" URL="\ref cqasm::v1::ast::AnnotationData"];
 *   ArithOp [ label="ArithOp" URL="\ref cqasm::v1::ast::ArithOp", style=dotted];
 *   Assignment [ label="Assignment" URL="\ref cqasm::v1::ast::Assignment"];
 *   BinaryOp [ label="BinaryOp" URL="\ref cqasm::v1::ast::BinaryOp", style=dotted];
 *   BitwiseAnd [ label="BitwiseAnd" URL="\ref cqasm::v1::ast::BitwiseAnd"];
 *   BitwiseNot [ label="BitwiseNot" URL="\ref cqasm::v1::ast::BitwiseNot"];
 *   BitwiseOp [ label="BitwiseOp" URL="\ref cqasm::v1::ast::BitwiseOp", style=dotted];
 *   BitwiseOr [ label="BitwiseOr" URL="\ref cqasm::v1::ast::BitwiseOr"];
 *   BitwiseXor [ label="BitwiseXor" URL="\ref cqasm::v1::ast::BitwiseXor"];
 *   BreakStatement [ label="BreakStatement" URL="\ref cqasm::v1::ast::BreakStatement"];
 *   Bundle [ label="Bundle" URL="\ref cqasm::v1::ast::Bundle"];
 *   CmpEq [ label="CmpEq" URL="\ref cqasm::v1::ast::CmpEq"];
 *   CmpGe [ label="CmpGe" URL="\ref cqasm::v1::ast::CmpGe"];
 *   CmpGt [ label="CmpGt" URL="\ref cqasm::v1::ast::CmpGt"];
 *   CmpLe [ label="CmpLe" URL="\ref cqasm::v1::ast::CmpLe"];
 *   CmpLt [ label="CmpLt" URL="\ref cqasm::v1::ast::CmpLt"];
 *   CmpNe [ label="CmpNe" URL="\ref cqasm::v1::ast::CmpNe"];
 *   CmpOp [ label="CmpOp" URL="\ref cqasm::v1::ast::CmpOp", style=dotted];
 *   ContinueStatement [ label="ContinueStatement" URL="\ref cqasm::v1::ast::ContinueStatement"];
 *   Divide [ label="Divide" URL="\ref cqasm::v1::ast::Divide"];
 *   ErroneousExpression [ label="ErroneousExpression" URL="\ref cqasm::v1::ast::ErroneousExpression"];
 *   ErroneousProgram [ label="ErroneousProgram" URL="\ref cqasm::v1::ast::ErroneousProgram"];
 *   ErroneousStatement [ label="ErroneousStatement" URL="\ref cqasm::v1::ast::ErroneousStatement"];
 *   Expression [ label="Expression" URL="\ref cqasm::v1::ast::Expression", style=dotted];
 *   ExpressionList [ label="ExpressionList" URL="\ref cqasm::v1::ast::ExpressionList"];
 *   FloatLiteral [ label="FloatLiteral" URL="\ref cqasm::v1::ast::FloatLiteral"];
 *   ForLoop [ label="ForLoop" URL="\ref cqasm::v1::ast::ForLoop"];
 *   ForeachLoop [ label="ForeachLoop" URL="\ref cqasm::v1::ast::ForeachLoop"];
 *   FunctionCall [ label="FunctionCall" URL="\ref cqasm::v1::ast::FunctionCall"];
 *   Identifier [ label="Identifier" URL="\ref cqasm::v1::ast::Identifier"];
 *   IfElse [ label="IfElse" URL="\ref cqasm::v1::ast::IfElse"];
 *   IfElseBranch [ label="IfElseBranch" URL="\ref cqasm::v1::ast::IfElseBranch"];
 *   Index [ label="Index" URL="\ref cqasm::v1::ast::Index"];
 *   IndexEntry [ label="IndexEntry" URL="\ref cqasm::v1::ast::IndexEntry", style=dotted];
 *   IndexItem [ label="IndexItem" URL="\ref cqasm::v1::ast::IndexItem"];
 *   IndexList [ label="IndexList" URL="\ref cqasm::v1::ast::IndexList"];
 *   IndexRange [ label="IndexRange" URL="\ref cqasm::v1::ast::IndexRange"];
 *   Instruction [ label="Instruction" URL="\ref cqasm::v1::ast::Instruction"];
 *   IntDivide [ label="IntDivide" URL="\ref cqasm::v1::ast::IntDivide"];
 *   IntegerLiteral [ label="IntegerLiteral" URL="\ref cqasm::v1::ast::IntegerLiteral"];
 *   JsonLiteral [ label="JsonLiteral" URL="\ref cqasm::v1::ast::JsonLiteral"];
 *   LogicalAnd [ label="LogicalAnd" URL="\ref cqasm::v1::ast::LogicalAnd"];
 *   LogicalNot [ label="LogicalNot" URL="\ref cqasm::v1::ast::LogicalNot"];
 *   LogicalOp [ label="LogicalOp" URL="\ref cqasm::v1::ast::LogicalOp", style=dotted];
 *   LogicalOr [ label="LogicalOr" URL="\ref cqasm::v1::ast::LogicalOr"];
 *   LogicalXor [ label="LogicalXor" URL="\ref cqasm::v1::ast::LogicalXor"];
 *   Mapping [ label="Mapping" URL="\ref cqasm::v1::ast::Mapping"];
 *   MatrixLiteral [ label="MatrixLiteral" URL="\ref cqasm::v1::ast::MatrixLiteral"];
 *   Modulo [ label="Modulo" URL="\ref cqasm::v1::ast::Modulo"];
 *   Multiply [ label="Multiply" URL="\ref cqasm::v1::ast::Multiply"];
 *   Negate [ label="Negate" URL="\ref cqasm::v1::ast::Negate"];
 *   Power [ label="Power" URL="\ref cqasm::v1::ast::Power"];
 *   Program [ label="Program" URL="\ref cqasm::v1::ast::Program"];
 *   RepeatUntilLoop [ label="RepeatUntilLoop" URL="\ref cqasm::v1::ast::RepeatUntilLoop"];
 *   Root [ label="Root" URL="\ref cqasm::v1::ast::Root", style=dotted];
 *   ShiftLeft [ label="ShiftLeft" URL="\ref cqasm::v1::ast::ShiftLeft"];
 *   ShiftOp [ label="ShiftOp" URL="\ref cqasm::v1::ast::ShiftOp", style=dotted];
 *   ShiftRightArith [ label="ShiftRightArith" URL="\ref cqasm::v1::ast::ShiftRightArith"];
 *   ShiftRightLogic [ label="ShiftRightLogic" URL="\ref cqasm::v1::ast::ShiftRightLogic"];
 *   Statement [ label="Statement" URL="\ref cqasm::v1::ast::Statement", style=dotted];
 *   StatementList [ label="StatementList" URL="\ref cqasm::v1::ast::StatementList"];
 *   StringLiteral [ label="StringLiteral" URL="\ref cqasm::v1::ast::StringLiteral"];
 *   Structured [ label="Structured" URL="\ref cqasm::v1::ast::Structured", style=dotted];
 *   Subcircuit [ label="Subcircuit" URL="\ref cqasm::v1::ast::Subcircuit"];
 *   Subtract [ label="Subtract" URL="\ref cqasm::v1::ast::Subtract"];
 *   TernaryCond [ label="TernaryCond" URL="\ref cqasm::v1::ast::TernaryCond"];
 *   UnaryOp [ label="UnaryOp" URL="\ref cqasm::v1::ast::UnaryOp", style=dotted];
 *   Variables [ label="Variables" URL="\ref cqasm::v1::ast::Variables"];
 *   Version [ label="Version" URL="\ref cqasm::v1::ast::Version"];
 *   WhileLoop [ label="WhileLoop" URL="\ref cqasm::v1::ast::WhileLoop"];
 *   ArithOp -> Add [ arrowhead=open, style=dotted ];
 *   BinaryOp -> ArithOp [ arrowhead=open, style=dotted ];
 *   Expression -> BinaryOp [ arrowhead=open, style=dotted ];
 *   BitwiseOp -> BitwiseAnd [ arrowhead=open, style=dotted ];
 *   UnaryOp -> BitwiseNot [ arrowhead=open, style=dotted ];
 *   BinaryOp -> BitwiseOp [ arrowhead=open, style=dotted ];
 *   BitwiseOp -> BitwiseOr [ arrowhead=open, style=dotted ];
 *   BitwiseOp -> BitwiseXor [ arrowhead=open, style=dotted ];
 *   Structured -> BreakStatement [ arrowhead=open, style=dotted ];
 *   Statement -> Bundle [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpEq [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpGe [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpGt [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpLe [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpLt [ arrowhead=open, style=dotted ];
 *   CmpOp -> CmpNe [ arrowhead=open, style=dotted ];
 *   BinaryOp -> CmpOp [ arrowhead=open, style=dotted ];
 *   Structured -> ContinueStatement [ arrowhead=open, style=dotted ];
 *   ArithOp -> Divide [ arrowhead=open, style=dotted ];
 *   Expression -> ErroneousExpression [ arrowhead=open, style=dotted ];
 *   Root -> ErroneousProgram [ arrowhead=open, style=dotted ];
 *   Statement -> ErroneousStatement [ arrowhead=open, style=dotted ];
 *   Expression -> FloatLiteral [ arrowhead=open, style=dotted ];
 *   Structured -> ForLoop [ arrowhead=open, style=dotted ];
 *   Structured -> ForeachLoop [ arrowhead=open, style=dotted ];
 *   Expression -> FunctionCall [ arrowhead=open, style=dotted ];
 *   Expression -> Identifier [ arrowhead=open, style=dotted ];
 *   Structured -> IfElse [ arrowhead=open, style=dotted ];
 *   Expression -> Index [ arrowhead=open, style=dotted ];
 *   IndexEntry -> IndexItem [ arrowhead=open, style=dotted ];
 *   IndexEntry -> IndexRange [ arrowhead=open, style=dotted ];
 *   Annotated -> Instruction [ arrowhead=open, style=dotted ];
 *   ArithOp -> IntDivide [ arrowhead=open, style=dotted ];
 *   Expression -> IntegerLiteral [ arrowhead=open, style=dotted ];
 *   Expression -> JsonLiteral [ arrowhead=open, style=dotted ];
 *   LogicalOp -> LogicalAnd [ arrowhead=open, style=dotted ];
 *   UnaryOp -> LogicalNot [ arrowhead=open, style=dotted ];
 *   BinaryOp -> LogicalOp [ arrowhead=open, style=dotted ];
 *   LogicalOp -> LogicalOr [ arrowhead=open, style=dotted ];
 *   LogicalOp -> LogicalXor [ arrowhead=open, style=dotted ];
 *   Statement -> Mapping [ arrowhead=open, style=dotted ];
 *   Expression -> MatrixLiteral [ arrowhead=open, style=dotted ];
 *   ArithOp -> Modulo [ arrowhead=open, style=dotted ];
 *   ArithOp -> Multiply [ arrowhead=open, style=dotted ];
 *   UnaryOp -> Negate [ arrowhead=open, style=dotted ];
 *   ArithOp -> Power [ arrowhead=open, style=dotted ];
 *   Root -> Program [ arrowhead=open, style=dotted ];
 *   Structured -> RepeatUntilLoop [ arrowhead=open, style=dotted ];
 *   ShiftOp -> ShiftLeft [ arrowhead=open, style=dotted ];
 *   BinaryOp -> ShiftOp [ arrowhead=open, style=dotted ];
 *   ShiftOp -> ShiftRightArith [ arrowhead=open, style=dotted ];
 *   ShiftOp -> ShiftRightLogic [ arrowhead=open, style=dotted ];
 *   Annotated -> Statement [ arrowhead=open, style=dotted ];
 *   Expression -> StringLiteral [ arrowhead=open, style=dotted ];
 *   Statement -> Structured [ arrowhead=open, style=dotted ];
 *   Statement -> Subcircuit [ arrowhead=open, style=dotted ];
 *   ArithOp -> Subtract [ arrowhead=open, style=dotted ];
 *   Expression -> TernaryCond [ arrowhead=open, style=dotted ];
 *   Expression -> UnaryOp [ arrowhead=open, style=dotted ];
 *   Statement -> Variables [ arrowhead=open, style=dotted ];
 *   Structured -> WhileLoop [ arrowhead=open, style=dotted ];
 *   Annotated -> AnnotationData [ label="annotations*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   AnnotationData -> Identifier [ label="interface", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   AnnotationData -> Identifier [ label="operation", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   AnnotationData -> ExpressionList [ label="operands", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Assignment -> Expression [ label="lhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Assignment -> Expression [ label="rhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   BinaryOp -> Expression [ label="lhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   BinaryOp -> Expression [ label="rhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Bundle -> Instruction [ label="items+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   ExpressionList -> Expression [ label="items*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim0 [ label="primitives::Real" URL="\ref cqasm::v1::primitives::Real"];
 *   FloatLiteral -> prim0 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> Assignment [ label="initialize?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> Expression [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> Assignment [ label="update?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> StatementList [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForeachLoop -> Expression [ label="lhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForeachLoop -> Expression [ label="frm", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForeachLoop -> Expression [ label="to", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForeachLoop -> StatementList [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   FunctionCall -> Identifier [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   FunctionCall -> ExpressionList [ label="arguments", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim1 [ label="primitives::Str" URL="\ref cqasm::v1::primitives::Str"];
 *   Identifier -> prim1 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IfElse -> IfElseBranch [ label="branches+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   IfElse -> StatementList [ label="otherwise?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   IfElseBranch -> Expression [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IfElseBranch -> StatementList [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Index -> Expression [ label="expr", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Index -> IndexList [ label="indices", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IndexItem -> Expression [ label="index", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IndexList -> IndexEntry [ label="items+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   IndexRange -> Expression [ label="first", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IndexRange -> Expression [ label="last", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Instruction -> Identifier [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Instruction -> Expression [ label="condition?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   Instruction -> ExpressionList [ label="operands", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim2 [ label="primitives::Int" URL="\ref cqasm::v1::primitives::Int"];
 *   IntegerLiteral -> prim2 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim3 [ label="primitives::Str" URL="\ref cqasm::v1::primitives::Str"];
 *   JsonLiteral -> prim3 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Mapping -> Identifier [ label="alias", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Mapping -> Expression [ label="expr", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   MatrixLiteral -> ExpressionList [ label="rows+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   Program -> Version [ label="version", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Program -> Expression [ label="num_qubits?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   Program -> StatementList [ label="statements", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   RepeatUntilLoop -> StatementList [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   RepeatUntilLoop -> Expression [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   StatementList -> Statement [ label="items*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim4 [ label="primitives::Str" URL="\ref cqasm::v1::primitives::Str"];
 *   StringLiteral -> prim4 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Subcircuit -> Identifier [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Subcircuit -> Expression [ label="iterations?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   TernaryCond -> Expression [ label="cond", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   TernaryCond -> Expression [ label="if_true", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   TernaryCond -> Expression [ label="if_false", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   UnaryOp -> Expression [ label="expr", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Variables -> Identifier [ label="names+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   Variables -> Identifier [ label="typ", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim5 [ label="primitives::Version" URL="\ref cqasm::v1::primitives::Version"];
 *   Version -> prim5 [ label="items", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   WhileLoop -> Expression [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   WhileLoop -> StatementList [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 * }
 * \enddot
 */
namespace ast {

// Base classes used to construct the tree.
using Base = cqasm::tree::Base;
template <class T> using Maybe   = cqasm::tree::Maybe<T>;
template <class T> using One     = cqasm::tree::One<T>;
template <class T> using Any     = cqasm::tree::Any<T>;
template <class T> using Many    = cqasm::tree::Many<T>;
template <class T> using OptLink = cqasm::tree::OptLink<T>;
template <class T> using Link    = cqasm::tree::Link<T>;

// Forward declarations for all classes.
class Node;
class Add;
class Annotated;
class AnnotationData;
class ArithOp;
class Assignment;
class BinaryOp;
class BitwiseAnd;
class BitwiseNot;
class BitwiseOp;
class BitwiseOr;
class BitwiseXor;
class BreakStatement;
class Bundle;
class CmpEq;
class CmpGe;
class CmpGt;
class CmpLe;
class CmpLt;
class CmpNe;
class CmpOp;
class ContinueStatement;
class Divide;
class ErroneousExpression;
class ErroneousProgram;
class ErroneousStatement;
class Expression;
class ExpressionList;
class FloatLiteral;
class ForLoop;
class ForeachLoop;
class FunctionCall;
class Identifier;
class IfElse;
class IfElseBranch;
class Index;
class IndexEntry;
class IndexItem;
class IndexList;
class IndexRange;
class Instruction;
class IntDivide;
class IntegerLiteral;
class JsonLiteral;
class LogicalAnd;
class LogicalNot;
class LogicalOp;
class LogicalOr;
class LogicalXor;
class Mapping;
class MatrixLiteral;
class Modulo;
class Multiply;
class Negate;
class Power;
class Program;
class RepeatUntilLoop;
class Root;
class ShiftLeft;
class ShiftOp;
class ShiftRightArith;
class ShiftRightLogic;
class Statement;
class StatementList;
class StringLiteral;
class Structured;
class Subcircuit;
class Subtract;
class TernaryCond;
class UnaryOp;
class Variables;
class Version;
class WhileLoop;
class VisitorBase;
template <typename T = void>
class Visitor;
class RecursiveVisitor;
class Dumper;

/**
 * Enumeration of all node types.
 */
enum class NodeType {
    Add,
    AnnotationData,
    Assignment,
    BitwiseAnd,
    BitwiseNot,
    BitwiseOr,
    BitwiseXor,
    BreakStatement,
    Bundle,
    CmpEq,
    CmpGe,
    CmpGt,
    CmpLe,
    CmpLt,
    CmpNe,
    ContinueStatement,
    Divide,
    ErroneousExpression,
    ErroneousProgram,
    ErroneousStatement,
    ExpressionList,
    FloatLiteral,
    ForLoop,
    ForeachLoop,
    FunctionCall,
    Identifier,
    IfElse,
    IfElseBranch,
    Index,
    IndexItem,
    IndexList,
    IndexRange,
    Instruction,
    IntDivide,
    IntegerLiteral,
    JsonLiteral,
    LogicalAnd,
    LogicalNot,
    LogicalOr,
    LogicalXor,
    Mapping,
    MatrixLiteral,
    Modulo,
    Multiply,
    Negate,
    Power,
    Program,
    RepeatUntilLoop,
    ShiftLeft,
    ShiftRightArith,
    ShiftRightLogic,
    StatementList,
    StringLiteral,
    Subcircuit,
    Subtract,
    TernaryCond,
    Variables,
    Version,
    WhileLoop
};

/**
 * Main class for all nodes.
 */
class Node : public Base {
public:

    /**
     * Returns the `NodeType` of this node.
     */
    virtual NodeType type() const = 0;

    /**
     * Returns a shallow copy of this node.
     */
    virtual One<Node> copy() const = 0;

    /**
     * Returns a deep copy of this node.
     */
    virtual One<Node> clone() const = 0;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    virtual bool equals(const Node& rhs) const = 0;

    /**
     * Pointer-based equality operator.
     */
    virtual bool operator==(const Node& rhs) const = 0;

    /**
     * Pointer-based inequality operator.
     */
    inline bool operator!=(const Node& rhs) const {
        return !(*this == rhs);
    }

protected:

    /**
     * Internal helper method for visiter pattern.
     */
    virtual void visit_internal(VisitorBase &visitor, void *retval=nullptr) = 0;

public:

    /**
     * Visit this object.
     */
    template <typename T>
    T visit(Visitor<T> &visitor);

    /**
     * Writes a debug dump of this node to the given stream.
     */
    void dump(std::ostream &out=std::cout, int indent=0);

    /**
     * Alternate debug dump that represents links and node uniqueness via
     * sequence number tags.
     */
    void dump_seq(std::ostream &out=std::cout, int indent=0);

    /**
     * Interprets this node to a node of type Add. Returns null if it has the
     * wrong type.
     */
    virtual Add *as_add();

    /**
     * Interprets this node to a node of type Add. Returns null if it has the
     * wrong type.
     */
    virtual const Add *as_add() const;

    /**
     * Interprets this node to a node of type Annotated. Returns null if it has
     * the wrong type.
     */
    virtual Annotated *as_annotated();

    /**
     * Interprets this node to a node of type Annotated. Returns null if it has
     * the wrong type.
     */
    virtual const Annotated *as_annotated() const;

    /**
     * Interprets this node to a node of type AnnotationData. Returns null if it
     * has the wrong type.
     */
    virtual AnnotationData *as_annotation_data();

    /**
     * Interprets this node to a node of type AnnotationData. Returns null if it
     * has the wrong type.
     */
    virtual const AnnotationData *as_annotation_data() const;

    /**
     * Interprets this node to a node of type ArithOp. Returns null if it has
     * the wrong type.
     */
    virtual ArithOp *as_arith_op();

    /**
     * Interprets this node to a node of type ArithOp. Returns null if it has
     * the wrong type.
     */
    virtual const ArithOp *as_arith_op() const;

    /**
     * Interprets this node to a node of type Assignment. Returns null if it has
     * the wrong type.
     */
    virtual Assignment *as_assignment();

    /**
     * Interprets this node to a node of type Assignment. Returns null if it has
     * the wrong type.
     */
    virtual const Assignment *as_assignment() const;

    /**
     * Interprets this node to a node of type BinaryOp. Returns null if it has
     * the wrong type.
     */
    virtual BinaryOp *as_binary_op();

    /**
     * Interprets this node to a node of type BinaryOp. Returns null if it has
     * the wrong type.
     */
    virtual const BinaryOp *as_binary_op() const;

    /**
     * Interprets this node to a node of type BitwiseAnd. Returns null if it has
     * the wrong type.
     */
    virtual BitwiseAnd *as_bitwise_and();

    /**
     * Interprets this node to a node of type BitwiseAnd. Returns null if it has
     * the wrong type.
     */
    virtual const BitwiseAnd *as_bitwise_and() const;

    /**
     * Interprets this node to a node of type BitwiseNot. Returns null if it has
     * the wrong type.
     */
    virtual BitwiseNot *as_bitwise_not();

    /**
     * Interprets this node to a node of type BitwiseNot. Returns null if it has
     * the wrong type.
     */
    virtual const BitwiseNot *as_bitwise_not() const;

    /**
     * Interprets this node to a node of type BitwiseOp. Returns null if it has
     * the wrong type.
     */
    virtual BitwiseOp *as_bitwise_op();

    /**
     * Interprets this node to a node of type BitwiseOp. Returns null if it has
     * the wrong type.
     */
    virtual const BitwiseOp *as_bitwise_op() const;

    /**
     * Interprets this node to a node of type BitwiseOr. Returns null if it has
     * the wrong type.
     */
    virtual BitwiseOr *as_bitwise_or();

    /**
     * Interprets this node to a node of type BitwiseOr. Returns null if it has
     * the wrong type.
     */
    virtual const BitwiseOr *as_bitwise_or() const;

    /**
     * Interprets this node to a node of type BitwiseXor. Returns null if it has
     * the wrong type.
     */
    virtual BitwiseXor *as_bitwise_xor();

    /**
     * Interprets this node to a node of type BitwiseXor. Returns null if it has
     * the wrong type.
     */
    virtual const BitwiseXor *as_bitwise_xor() const;

    /**
     * Interprets this node to a node of type BreakStatement. Returns null if it
     * has the wrong type.
     */
    virtual BreakStatement *as_break_statement();

    /**
     * Interprets this node to a node of type BreakStatement. Returns null if it
     * has the wrong type.
     */
    virtual const BreakStatement *as_break_statement() const;

    /**
     * Interprets this node to a node of type Bundle. Returns null if it has the
     * wrong type.
     */
    virtual Bundle *as_bundle();

    /**
     * Interprets this node to a node of type Bundle. Returns null if it has the
     * wrong type.
     */
    virtual const Bundle *as_bundle() const;

    /**
     * Interprets this node to a node of type CmpEq. Returns null if it has the
     * wrong type.
     */
    virtual CmpEq *as_cmp_eq();

    /**
     * Interprets this node to a node of type CmpEq. Returns null if it has the
     * wrong type.
     */
    virtual const CmpEq *as_cmp_eq() const;

    /**
     * Interprets this node to a node of type CmpGe. Returns null if it has the
     * wrong type.
     */
    virtual CmpGe *as_cmp_ge();

    /**
     * Interprets this node to a node of type CmpGe. Returns null if it has the
     * wrong type.
     */
    virtual const CmpGe *as_cmp_ge() const;

    /**
     * Interprets this node to a node of type CmpGt. Returns null if it has the
     * wrong type.
     */
    virtual CmpGt *as_cmp_gt();

    /**
     * Interprets this node to a node of type CmpGt. Returns null if it has the
     * wrong type.
     */
    virtual const CmpGt *as_cmp_gt() const;

    /**
     * Interprets this node to a node of type CmpLe. Returns null if it has the
     * wrong type.
     */
    virtual CmpLe *as_cmp_le();

    /**
     * Interprets this node to a node of type CmpLe. Returns null if it has the
     * wrong type.
     */
    virtual const CmpLe *as_cmp_le() const;

    /**
     * Interprets this node to a node of type CmpLt. Returns null if it has the
     * wrong type.
     */
    virtual CmpLt *as_cmp_lt();

    /**
     * Interprets this node to a node of type CmpLt. Returns null if it has the
     * wrong type.
     */
    virtual const CmpLt *as_cmp_lt() const;

    /**
     * Interprets this node to a node of type CmpNe. Returns null if it has the
     * wrong type.
     */
    virtual CmpNe *as_cmp_ne();

    /**
     * Interprets this node to a node of type CmpNe. Returns null if it has the
     * wrong type.
     */
    virtual const CmpNe *as_cmp_ne() const;

    /**
     * Interprets this node to a node of type CmpOp. Returns null if it has the
     * wrong type.
     */
    virtual CmpOp *as_cmp_op();

    /**
     * Interprets this node to a node of type CmpOp. Returns null if it has the
     * wrong type.
     */
    virtual const CmpOp *as_cmp_op() const;

    /**
     * Interprets this node to a node of type ContinueStatement. Returns null if
     * it has the wrong type.
     */
    virtual ContinueStatement *as_continue_statement();

    /**
     * Interprets this node to a node of type ContinueStatement. Returns null if
     * it has the wrong type.
     */
    virtual const ContinueStatement *as_continue_statement() const;

    /**
     * Interprets this node to a node of type Divide. Returns null if it has the
     * wrong type.
     */
    virtual Divide *as_divide();

    /**
     * Interprets this node to a node of type Divide. Returns null if it has the
     * wrong type.
     */
    virtual const Divide *as_divide() const;

    /**
     * Interprets this node to a node of type ErroneousExpression. Returns null
     * if it has the wrong type.
     */
    virtual ErroneousExpression *as_erroneous_expression();

    /**
     * Interprets this node to a node of type ErroneousExpression. Returns null
     * if it has the wrong type.
     */
    virtual const ErroneousExpression *as_erroneous_expression() const;

    /**
     * Interprets this node to a node of type ErroneousProgram. Returns null if
     * it has the wrong type.
     */
    virtual ErroneousProgram *as_erroneous_program();

    /**
     * Interprets this node to a node of type ErroneousProgram. Returns null if
     * it has the wrong type.
     */
    virtual const ErroneousProgram *as_erroneous_program() const;

    /**
     * Interprets this node to a node of type ErroneousStatement. Returns null
     * if it has the wrong type.
     */
    virtual ErroneousStatement *as_erroneous_statement();

    /**
     * Interprets this node to a node of type ErroneousStatement. Returns null
     * if it has the wrong type.
     */
    virtual const ErroneousStatement *as_erroneous_statement() const;

    /**
     * Interprets this node to a node of type Expression. Returns null if it has
     * the wrong type.
     */
    virtual Expression *as_expression();

    /**
     * Interprets this node to a node of type Expression. Returns null if it has
     * the wrong type.
     */
    virtual const Expression *as_expression() const;

    /**
     * Interprets this node to a node of type ExpressionList. Returns null if it
     * has the wrong type.
     */
    virtual ExpressionList *as_expression_list();

    /**
     * Interprets this node to a node of type ExpressionList. Returns null if it
     * has the wrong type.
     */
    virtual const ExpressionList *as_expression_list() const;

    /**
     * Interprets this node to a node of type FloatLiteral. Returns null if it
     * has the wrong type.
     */
    virtual FloatLiteral *as_float_literal();

    /**
     * Interprets this node to a node of type FloatLiteral. Returns null if it
     * has the wrong type.
     */
    virtual const FloatLiteral *as_float_literal() const;

    /**
     * Interprets this node to a node of type ForLoop. Returns null if it has
     * the wrong type.
     */
    virtual ForLoop *as_for_loop();

    /**
     * Interprets this node to a node of type ForLoop. Returns null if it has
     * the wrong type.
     */
    virtual const ForLoop *as_for_loop() const;

    /**
     * Interprets this node to a node of type ForeachLoop. Returns null if it
     * has the wrong type.
     */
    virtual ForeachLoop *as_foreach_loop();

    /**
     * Interprets this node to a node of type ForeachLoop. Returns null if it
     * has the wrong type.
     */
    virtual const ForeachLoop *as_foreach_loop() const;

    /**
     * Interprets this node to a node of type FunctionCall. Returns null if it
     * has the wrong type.
     */
    virtual FunctionCall *as_function_call();

    /**
     * Interprets this node to a node of type FunctionCall. Returns null if it
     * has the wrong type.
     */
    virtual const FunctionCall *as_function_call() const;

    /**
     * Interprets this node to a node of type Identifier. Returns null if it has
     * the wrong type.
     */
    virtual Identifier *as_identifier();

    /**
     * Interprets this node to a node of type Identifier. Returns null if it has
     * the wrong type.
     */
    virtual const Identifier *as_identifier() const;

    /**
     * Interprets this node to a node of type IfElse. Returns null if it has the
     * wrong type.
     */
    virtual IfElse *as_if_else();

    /**
     * Interprets this node to a node of type IfElse. Returns null if it has the
     * wrong type.
     */
    virtual const IfElse *as_if_else() const;

    /**
     * Interprets this node to a node of type IfElseBranch. Returns null if it
     * has the wrong type.
     */
    virtual IfElseBranch *as_if_else_branch();

    /**
     * Interprets this node to a node of type IfElseBranch. Returns null if it
     * has the wrong type.
     */
    virtual const IfElseBranch *as_if_else_branch() const;

    /**
     * Interprets this node to a node of type Index. Returns null if it has the
     * wrong type.
     */
    virtual Index *as_index();

    /**
     * Interprets this node to a node of type Index. Returns null if it has the
     * wrong type.
     */
    virtual const Index *as_index() const;

    /**
     * Interprets this node to a node of type IndexEntry. Returns null if it has
     * the wrong type.
     */
    virtual IndexEntry *as_index_entry();

    /**
     * Interprets this node to a node of type IndexEntry. Returns null if it has
     * the wrong type.
     */
    virtual const IndexEntry *as_index_entry() const;

    /**
     * Interprets this node to a node of type IndexItem. Returns null if it has
     * the wrong type.
     */
    virtual IndexItem *as_index_item();

    /**
     * Interprets this node to a node of type IndexItem. Returns null if it has
     * the wrong type.
     */
    virtual const IndexItem *as_index_item() const;

    /**
     * Interprets this node to a node of type IndexList. Returns null if it has
     * the wrong type.
     */
    virtual IndexList *as_index_list();

    /**
     * Interprets this node to a node of type IndexList. Returns null if it has
     * the wrong type.
     */
    virtual const IndexList *as_index_list() const;

    /**
     * Interprets this node to a node of type IndexRange. Returns null if it has
     * the wrong type.
     */
    virtual IndexRange *as_index_range();

    /**
     * Interprets this node to a node of type IndexRange. Returns null if it has
     * the wrong type.
     */
    virtual const IndexRange *as_index_range() const;

    /**
     * Interprets this node to a node of type Instruction. Returns null if it
     * has the wrong type.
     */
    virtual Instruction *as_instruction();

    /**
     * Interprets this node to a node of type Instruction. Returns null if it
     * has the wrong type.
     */
    virtual const Instruction *as_instruction() const;

    /**
     * Interprets this node to a node of type IntDivide. Returns null if it has
     * the wrong type.
     */
    virtual IntDivide *as_int_divide();

    /**
     * Interprets this node to a node of type IntDivide. Returns null if it has
     * the wrong type.
     */
    virtual const IntDivide *as_int_divide() const;

    /**
     * Interprets this node to a node of type IntegerLiteral. Returns null if it
     * has the wrong type.
     */
    virtual IntegerLiteral *as_integer_literal();

    /**
     * Interprets this node to a node of type IntegerLiteral. Returns null if it
     * has the wrong type.
     */
    virtual const IntegerLiteral *as_integer_literal() const;

    /**
     * Interprets this node to a node of type JsonLiteral. Returns null if it
     * has the wrong type.
     */
    virtual JsonLiteral *as_json_literal();

    /**
     * Interprets this node to a node of type JsonLiteral. Returns null if it
     * has the wrong type.
     */
    virtual const JsonLiteral *as_json_literal() const;

    /**
     * Interprets this node to a node of type LogicalAnd. Returns null if it has
     * the wrong type.
     */
    virtual LogicalAnd *as_logical_and();

    /**
     * Interprets this node to a node of type LogicalAnd. Returns null if it has
     * the wrong type.
     */
    virtual const LogicalAnd *as_logical_and() const;

    /**
     * Interprets this node to a node of type LogicalNot. Returns null if it has
     * the wrong type.
     */
    virtual LogicalNot *as_logical_not();

    /**
     * Interprets this node to a node of type LogicalNot. Returns null if it has
     * the wrong type.
     */
    virtual const LogicalNot *as_logical_not() const;

    /**
     * Interprets this node to a node of type LogicalOp. Returns null if it has
     * the wrong type.
     */
    virtual LogicalOp *as_logical_op();

    /**
     * Interprets this node to a node of type LogicalOp. Returns null if it has
     * the wrong type.
     */
    virtual const LogicalOp *as_logical_op() const;

    /**
     * Interprets this node to a node of type LogicalOr. Returns null if it has
     * the wrong type.
     */
    virtual LogicalOr *as_logical_or();

    /**
     * Interprets this node to a node of type LogicalOr. Returns null if it has
     * the wrong type.
     */
    virtual const LogicalOr *as_logical_or() const;

    /**
     * Interprets this node to a node of type LogicalXor. Returns null if it has
     * the wrong type.
     */
    virtual LogicalXor *as_logical_xor();

    /**
     * Interprets this node to a node of type LogicalXor. Returns null if it has
     * the wrong type.
     */
    virtual const LogicalXor *as_logical_xor() const;

    /**
     * Interprets this node to a node of type Mapping. Returns null if it has
     * the wrong type.
     */
    virtual Mapping *as_mapping();

    /**
     * Interprets this node to a node of type Mapping. Returns null if it has
     * the wrong type.
     */
    virtual const Mapping *as_mapping() const;

    /**
     * Interprets this node to a node of type MatrixLiteral. Returns null if it
     * has the wrong type.
     */
    virtual MatrixLiteral *as_matrix_literal();

    /**
     * Interprets this node to a node of type MatrixLiteral. Returns null if it
     * has the wrong type.
     */
    virtual const MatrixLiteral *as_matrix_literal() const;

    /**
     * Interprets this node to a node of type Modulo. Returns null if it has the
     * wrong type.
     */
    virtual Modulo *as_modulo();

    /**
     * Interprets this node to a node of type Modulo. Returns null if it has the
     * wrong type.
     */
    virtual const Modulo *as_modulo() const;

    /**
     * Interprets this node to a node of type Multiply. Returns null if it has
     * the wrong type.
     */
    virtual Multiply *as_multiply();

    /**
     * Interprets this node to a node of type Multiply. Returns null if it has
     * the wrong type.
     */
    virtual const Multiply *as_multiply() const;

    /**
     * Interprets this node to a node of type Negate. Returns null if it has the
     * wrong type.
     */
    virtual Negate *as_negate();

    /**
     * Interprets this node to a node of type Negate. Returns null if it has the
     * wrong type.
     */
    virtual const Negate *as_negate() const;

    /**
     * Interprets this node to a node of type Power. Returns null if it has the
     * wrong type.
     */
    virtual Power *as_power();

    /**
     * Interprets this node to a node of type Power. Returns null if it has the
     * wrong type.
     */
    virtual const Power *as_power() const;

    /**
     * Interprets this node to a node of type Program. Returns null if it has
     * the wrong type.
     */
    virtual Program *as_program();

    /**
     * Interprets this node to a node of type Program. Returns null if it has
     * the wrong type.
     */
    virtual const Program *as_program() const;

    /**
     * Interprets this node to a node of type RepeatUntilLoop. Returns null if
     * it has the wrong type.
     */
    virtual RepeatUntilLoop *as_repeat_until_loop();

    /**
     * Interprets this node to a node of type RepeatUntilLoop. Returns null if
     * it has the wrong type.
     */
    virtual const RepeatUntilLoop *as_repeat_until_loop() const;

    /**
     * Interprets this node to a node of type Root. Returns null if it has the
     * wrong type.
     */
    virtual Root *as_root();

    /**
     * Interprets this node to a node of type Root. Returns null if it has the
     * wrong type.
     */
    virtual const Root *as_root() const;

    /**
     * Interprets this node to a node of type ShiftLeft. Returns null if it has
     * the wrong type.
     */
    virtual ShiftLeft *as_shift_left();

    /**
     * Interprets this node to a node of type ShiftLeft. Returns null if it has
     * the wrong type.
     */
    virtual const ShiftLeft *as_shift_left() const;

    /**
     * Interprets this node to a node of type ShiftOp. Returns null if it has
     * the wrong type.
     */
    virtual ShiftOp *as_shift_op();

    /**
     * Interprets this node to a node of type ShiftOp. Returns null if it has
     * the wrong type.
     */
    virtual const ShiftOp *as_shift_op() const;

    /**
     * Interprets this node to a node of type ShiftRightArith. Returns null if
     * it has the wrong type.
     */
    virtual ShiftRightArith *as_shift_right_arith();

    /**
     * Interprets this node to a node of type ShiftRightArith. Returns null if
     * it has the wrong type.
     */
    virtual const ShiftRightArith *as_shift_right_arith() const;

    /**
     * Interprets this node to a node of type ShiftRightLogic. Returns null if
     * it has the wrong type.
     */
    virtual ShiftRightLogic *as_shift_right_logic();

    /**
     * Interprets this node to a node of type ShiftRightLogic. Returns null if
     * it has the wrong type.
     */
    virtual const ShiftRightLogic *as_shift_right_logic() const;

    /**
     * Interprets this node to a node of type Statement. Returns null if it has
     * the wrong type.
     */
    virtual Statement *as_statement();

    /**
     * Interprets this node to a node of type Statement. Returns null if it has
     * the wrong type.
     */
    virtual const Statement *as_statement() const;

    /**
     * Interprets this node to a node of type StatementList. Returns null if it
     * has the wrong type.
     */
    virtual StatementList *as_statement_list();

    /**
     * Interprets this node to a node of type StatementList. Returns null if it
     * has the wrong type.
     */
    virtual const StatementList *as_statement_list() const;

    /**
     * Interprets this node to a node of type StringLiteral. Returns null if it
     * has the wrong type.
     */
    virtual StringLiteral *as_string_literal();

    /**
     * Interprets this node to a node of type StringLiteral. Returns null if it
     * has the wrong type.
     */
    virtual const StringLiteral *as_string_literal() const;

    /**
     * Interprets this node to a node of type Structured. Returns null if it has
     * the wrong type.
     */
    virtual Structured *as_structured();

    /**
     * Interprets this node to a node of type Structured. Returns null if it has
     * the wrong type.
     */
    virtual const Structured *as_structured() const;

    /**
     * Interprets this node to a node of type Subcircuit. Returns null if it has
     * the wrong type.
     */
    virtual Subcircuit *as_subcircuit();

    /**
     * Interprets this node to a node of type Subcircuit. Returns null if it has
     * the wrong type.
     */
    virtual const Subcircuit *as_subcircuit() const;

    /**
     * Interprets this node to a node of type Subtract. Returns null if it has
     * the wrong type.
     */
    virtual Subtract *as_subtract();

    /**
     * Interprets this node to a node of type Subtract. Returns null if it has
     * the wrong type.
     */
    virtual const Subtract *as_subtract() const;

    /**
     * Interprets this node to a node of type TernaryCond. Returns null if it
     * has the wrong type.
     */
    virtual TernaryCond *as_ternary_cond();

    /**
     * Interprets this node to a node of type TernaryCond. Returns null if it
     * has the wrong type.
     */
    virtual const TernaryCond *as_ternary_cond() const;

    /**
     * Interprets this node to a node of type UnaryOp. Returns null if it has
     * the wrong type.
     */
    virtual UnaryOp *as_unary_op();

    /**
     * Interprets this node to a node of type UnaryOp. Returns null if it has
     * the wrong type.
     */
    virtual const UnaryOp *as_unary_op() const;

    /**
     * Interprets this node to a node of type Variables. Returns null if it has
     * the wrong type.
     */
    virtual Variables *as_variables();

    /**
     * Interprets this node to a node of type Variables. Returns null if it has
     * the wrong type.
     */
    virtual const Variables *as_variables() const;

    /**
     * Interprets this node to a node of type Version. Returns null if it has
     * the wrong type.
     */
    virtual Version *as_version();

    /**
     * Interprets this node to a node of type Version. Returns null if it has
     * the wrong type.
     */
    virtual const Version *as_version() const;

    /**
     * Interprets this node to a node of type WhileLoop. Returns null if it has
     * the wrong type.
     */
    virtual WhileLoop *as_while_loop();

    /**
     * Interprets this node to a node of type WhileLoop. Returns null if it has
     * the wrong type.
     */
    virtual const WhileLoop *as_while_loop() const;

    /**
     * Serializes this node to the given map.
     */
    virtual void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const = 0;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Node> deserialize(
         const ::tree::cbor::MapReader &map,
         ::tree::base::IdentifierMap &ids
    );

};

/**
 * Any kind of expression.
 */
class Expression : public Node {
public:

    /**
     * Interprets this node to a node of type Expression. Returns null if it has
     * the wrong type.
     */
    Expression *as_expression() override;

    /**
     * Interprets this node to a node of type Expression. Returns null if it has
     * the wrong type.
     */
    const Expression *as_expression() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Expression> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any binary operator.
 */
class BinaryOp : public Expression {
public:

    /**
     * The left-hand side of the expression.
     */
    One<Expression> lhs;

    /**
     * The right-hand side of the expression.
     */
    One<Expression> rhs;

    /**
     * Constructor.
     */
    BinaryOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type BinaryOp. Returns null if it has
     * the wrong type.
     */
    BinaryOp *as_binary_op() override;

    /**
     * Interprets this node to a node of type BinaryOp. Returns null if it has
     * the wrong type.
     */
    const BinaryOp *as_binary_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BinaryOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Arithmetic operators.
 */
class ArithOp : public BinaryOp {
public:

    /**
     * Constructor.
     */
    ArithOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type ArithOp. Returns null if it has
     * the wrong type.
     */
    ArithOp *as_arith_op() override;

    /**
     * Interprets this node to a node of type ArithOp. Returns null if it has
     * the wrong type.
     */
    const ArithOp *as_arith_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ArithOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Addition operator.
 */
class Add : public ArithOp {
public:

    /**
     * Constructor.
     */
    Add(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Add` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Add. Returns null if it has the
     * wrong type.
     */
    Add *as_add() override;

    /**
     * Interprets this node to a node of type Add. Returns null if it has the
     * wrong type.
     */
    const Add *as_add() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Add> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a node that carries annotation data.
 */
class Annotated : public Node {
public:

    /**
     * Zero or more annotations attached to this object.
     */
    Any<AnnotationData> annotations;

    /**
     * Constructor.
     */
    Annotated(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Interprets this node to a node of type Annotated. Returns null if it has
     * the wrong type.
     */
    Annotated *as_annotated() override;

    /**
     * Interprets this node to a node of type Annotated. Returns null if it has
     * the wrong type.
     */
    const Annotated *as_annotated() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Annotated> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents an annotation.
 */
class AnnotationData : public Node {
public:

    /**
     * The interface this annotation is intended for. If a target doesn't
     * support an interface, it should silently ignore the annotation.
     */
    One<Identifier> interface;

    /**
     * The operation within the interface that this annotation is intended for.
     * If a supports the corresponding interface but not the operation, it
     * should throw an error.
     */
    One<Identifier> operation;

    /**
     * Any operands attached to the annotation.
     */
    One<ExpressionList> operands;

    /**
     * Constructor.
     */
    AnnotationData(const One<Identifier> &interface = One<Identifier>(), const One<Identifier> &operation = One<Identifier>(), const One<ExpressionList> &operands = One<ExpressionList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `AnnotationData` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type AnnotationData. Returns null if it
     * has the wrong type.
     */
    AnnotationData *as_annotation_data() override;

    /**
     * Interprets this node to a node of type AnnotationData. Returns null if it
     * has the wrong type.
     */
    const AnnotationData *as_annotation_data() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<AnnotationData> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An assignment statement within a loop definition.
 */
class Assignment : public Node {
public:

    /**
     * Name of the variable being assigned.
     */
    One<Expression> lhs;

    /**
     * Value being assigned to the variable.
     */
    One<Expression> rhs;

    /**
     * Constructor.
     */
    Assignment(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Assignment` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Assignment. Returns null if it has
     * the wrong type.
     */
    Assignment *as_assignment() override;

    /**
     * Interprets this node to a node of type Assignment. Returns null if it has
     * the wrong type.
     */
    const Assignment *as_assignment() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Assignment> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Bitwise operators.
 */
class BitwiseOp : public BinaryOp {
public:

    /**
     * Constructor.
     */
    BitwiseOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type BitwiseOp. Returns null if it has
     * the wrong type.
     */
    BitwiseOp *as_bitwise_op() override;

    /**
     * Interprets this node to a node of type BitwiseOp. Returns null if it has
     * the wrong type.
     */
    const BitwiseOp *as_bitwise_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BitwiseOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Bitwise AND operator.
 */
class BitwiseAnd : public BitwiseOp {
public:

    /**
     * Constructor.
     */
    BitwiseAnd(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BitwiseAnd` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type BitwiseAnd. Returns null if it has
     * the wrong type.
     */
    BitwiseAnd *as_bitwise_and() override;

    /**
     * Interprets this node to a node of type BitwiseAnd. Returns null if it has
     * the wrong type.
     */
    const BitwiseAnd *as_bitwise_and() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BitwiseAnd> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any unary operator.
 */
class UnaryOp : public Expression {
public:

    /**
     * The expression being operated on.
     */
    One<Expression> expr;

    /**
     * Constructor.
     */
    UnaryOp(const One<Expression> &expr = One<Expression>());

    /**
     * Interprets this node to a node of type UnaryOp. Returns null if it has
     * the wrong type.
     */
    UnaryOp *as_unary_op() override;

    /**
     * Interprets this node to a node of type UnaryOp. Returns null if it has
     * the wrong type.
     */
    const UnaryOp *as_unary_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<UnaryOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Bitwise NOT (one's complement).
 */
class BitwiseNot : public UnaryOp {
public:

    /**
     * Constructor.
     */
    BitwiseNot(const One<Expression> &expr = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BitwiseNot` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type BitwiseNot. Returns null if it has
     * the wrong type.
     */
    BitwiseNot *as_bitwise_not() override;

    /**
     * Interprets this node to a node of type BitwiseNot. Returns null if it has
     * the wrong type.
     */
    const BitwiseNot *as_bitwise_not() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BitwiseNot> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Bitwise OR operator.
 */
class BitwiseOr : public BitwiseOp {
public:

    /**
     * Constructor.
     */
    BitwiseOr(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BitwiseOr` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type BitwiseOr. Returns null if it has
     * the wrong type.
     */
    BitwiseOr *as_bitwise_or() override;

    /**
     * Interprets this node to a node of type BitwiseOr. Returns null if it has
     * the wrong type.
     */
    const BitwiseOr *as_bitwise_or() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BitwiseOr> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Bitwise XOR operator.
 */
class BitwiseXor : public BitwiseOp {
public:

    /**
     * Constructor.
     */
    BitwiseXor(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BitwiseXor` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type BitwiseXor. Returns null if it has
     * the wrong type.
     */
    BitwiseXor *as_bitwise_xor() override;

    /**
     * Interprets this node to a node of type BitwiseXor. Returns null if it has
     * the wrong type.
     */
    const BitwiseXor *as_bitwise_xor() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BitwiseXor> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any kind of statement.
 */
class Statement : public Annotated {
public:

    /**
     * Constructor.
     */
    Statement(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Interprets this node to a node of type Statement. Returns null if it has
     * the wrong type.
     */
    Statement *as_statement() override;

    /**
     * Interprets this node to a node of type Statement. Returns null if it has
     * the wrong type.
     */
    const Statement *as_statement() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Statement> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any version 1.2+ structured control-flow statement.
 */
class Structured : public Statement {
public:

    /**
     * Constructor.
     */
    Structured(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Interprets this node to a node of type Structured. Returns null if it has
     * the wrong type.
     */
    Structured *as_structured() override;

    /**
     * Interprets this node to a node of type Structured. Returns null if it has
     * the wrong type.
     */
    const Structured *as_structured() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Structured> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A break statement.
 */
class BreakStatement : public Structured {
public:

    /**
     * Constructor.
     */
    BreakStatement(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BreakStatement` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type BreakStatement. Returns null if it
     * has the wrong type.
     */
    BreakStatement *as_break_statement() override;

    /**
     * Interprets this node to a node of type BreakStatement. Returns null if it
     * has the wrong type.
     */
    const BreakStatement *as_break_statement() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<BreakStatement> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A list of parallel instructions.
 */
class Bundle : public Statement {
public:

    /**
     * The list of parallel instructions.
     */
    Many<Instruction> items;

    /**
     * Constructor.
     */
    Bundle(const Many<Instruction> &items = Many<Instruction>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Bundle` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Bundle. Returns null if it has the
     * wrong type.
     */
    Bundle *as_bundle() override;

    /**
     * Interprets this node to a node of type Bundle. Returns null if it has the
     * wrong type.
     */
    const Bundle *as_bundle() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Bundle> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Comparison operators.
 */
class CmpOp : public BinaryOp {
public:

    /**
     * Constructor.
     */
    CmpOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type CmpOp. Returns null if it has the
     * wrong type.
     */
    CmpOp *as_cmp_op() override;

    /**
     * Interprets this node to a node of type CmpOp. Returns null if it has the
     * wrong type.
     */
    const CmpOp *as_cmp_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Equality operator.
 */
class CmpEq : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpEq(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpEq` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpEq. Returns null if it has the
     * wrong type.
     */
    CmpEq *as_cmp_eq() override;

    /**
     * Interprets this node to a node of type CmpEq. Returns null if it has the
     * wrong type.
     */
    const CmpEq *as_cmp_eq() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpEq> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Greater-than-or-equal operator.
 */
class CmpGe : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpGe(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpGe` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpGe. Returns null if it has the
     * wrong type.
     */
    CmpGe *as_cmp_ge() override;

    /**
     * Interprets this node to a node of type CmpGe. Returns null if it has the
     * wrong type.
     */
    const CmpGe *as_cmp_ge() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpGe> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Greater-than operator.
 */
class CmpGt : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpGt(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpGt` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpGt. Returns null if it has the
     * wrong type.
     */
    CmpGt *as_cmp_gt() override;

    /**
     * Interprets this node to a node of type CmpGt. Returns null if it has the
     * wrong type.
     */
    const CmpGt *as_cmp_gt() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpGt> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Less-than-or-equal operator.
 */
class CmpLe : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpLe(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpLe` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpLe. Returns null if it has the
     * wrong type.
     */
    CmpLe *as_cmp_le() override;

    /**
     * Interprets this node to a node of type CmpLe. Returns null if it has the
     * wrong type.
     */
    const CmpLe *as_cmp_le() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpLe> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Less-than operator.
 */
class CmpLt : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpLt(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpLt` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpLt. Returns null if it has the
     * wrong type.
     */
    CmpLt *as_cmp_lt() override;

    /**
     * Interprets this node to a node of type CmpLt. Returns null if it has the
     * wrong type.
     */
    const CmpLt *as_cmp_lt() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpLt> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Inequality operator.
 */
class CmpNe : public CmpOp {
public:

    /**
     * Constructor.
     */
    CmpNe(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `CmpNe` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type CmpNe. Returns null if it has the
     * wrong type.
     */
    CmpNe *as_cmp_ne() override;

    /**
     * Interprets this node to a node of type CmpNe. Returns null if it has the
     * wrong type.
     */
    const CmpNe *as_cmp_ne() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<CmpNe> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A continue statement.
 */
class ContinueStatement : public Structured {
public:

    /**
     * Constructor.
     */
    ContinueStatement(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ContinueStatement` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ContinueStatement. Returns null if
     * it has the wrong type.
     */
    ContinueStatement *as_continue_statement() override;

    /**
     * Interprets this node to a node of type ContinueStatement. Returns null if
     * it has the wrong type.
     */
    const ContinueStatement *as_continue_statement() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ContinueStatement> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * True division operator.
 */
class Divide : public ArithOp {
public:

    /**
     * Constructor.
     */
    Divide(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Divide` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Divide. Returns null if it has the
     * wrong type.
     */
    Divide *as_divide() override;

    /**
     * Interprets this node to a node of type Divide. Returns null if it has the
     * wrong type.
     */
    const Divide *as_divide() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Divide> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Placeholder for an expression with a parse error.
 */
class ErroneousExpression : public Expression {
public:

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ErroneousExpression` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ErroneousExpression. Returns null
     * if it has the wrong type.
     */
    ErroneousExpression *as_erroneous_expression() override;

    /**
     * Interprets this node to a node of type ErroneousExpression. Returns null
     * if it has the wrong type.
     */
    const ErroneousExpression *as_erroneous_expression() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ErroneousExpression> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any root node for the AST.
 */
class Root : public Node {
public:

    /**
     * Interprets this node to a node of type Root. Returns null if it has the
     * wrong type.
     */
    Root *as_root() override;

    /**
     * Interprets this node to a node of type Root. Returns null if it has the
     * wrong type.
     */
    const Root *as_root() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Root> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Placeholder for a program with a parse error.
 */
class ErroneousProgram : public Root {
public:

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ErroneousProgram` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ErroneousProgram. Returns null if
     * it has the wrong type.
     */
    ErroneousProgram *as_erroneous_program() override;

    /**
     * Interprets this node to a node of type ErroneousProgram. Returns null if
     * it has the wrong type.
     */
    const ErroneousProgram *as_erroneous_program() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ErroneousProgram> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Placeholder for a statement with a parse error.
 */
class ErroneousStatement : public Statement {
public:

    /**
     * Constructor.
     */
    ErroneousStatement(const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ErroneousStatement` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ErroneousStatement. Returns null
     * if it has the wrong type.
     */
    ErroneousStatement *as_erroneous_statement() override;

    /**
     * Interprets this node to a node of type ErroneousStatement. Returns null
     * if it has the wrong type.
     */
    const ErroneousStatement *as_erroneous_statement() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ErroneousStatement> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a comma-separated list of expressions.
 */
class ExpressionList : public Node {
public:

    /**
     * The list of expressions.
     */
    Any<Expression> items;

    /**
     * Constructor.
     */
    ExpressionList(const Any<Expression> &items = Any<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ExpressionList` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ExpressionList. Returns null if it
     * has the wrong type.
     */
    ExpressionList *as_expression_list() override;

    /**
     * Interprets this node to a node of type ExpressionList. Returns null if it
     * has the wrong type.
     */
    const ExpressionList *as_expression_list() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ExpressionList> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A floating point literal.
 */
class FloatLiteral : public Expression {
public:

    /**
     * The floating point number.
     */
    cqasm::v1::primitives::Real value;

    /**
     * Constructor.
     */
    FloatLiteral(const cqasm::v1::primitives::Real &value = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Real>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `FloatLiteral` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type FloatLiteral. Returns null if it
     * has the wrong type.
     */
    FloatLiteral *as_float_literal() override;

    /**
     * Interprets this node to a node of type FloatLiteral. Returns null if it
     * has the wrong type.
     */
    const FloatLiteral *as_float_literal() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<FloatLiteral> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A C-style for loop.
 */
class ForLoop : public Structured {
public:

    /**
     * The optional initializing assignment, run before the loop starts.
     */
    Maybe<Assignment> initialize;

    /**
     * The condition for starting another iteration.
     */
    One<Expression> condition;

    /**
     * The updating assignment, done at the end of the loop body and upon
     * continue.
     */
    Maybe<Assignment> update;

    /**
     * The loop body.
     */
    One<StatementList> body;

    /**
     * Constructor.
     */
    ForLoop(const Maybe<Assignment> &initialize = Maybe<Assignment>(), const One<Expression> &condition = One<Expression>(), const Maybe<Assignment> &update = Maybe<Assignment>(), const One<StatementList> &body = One<StatementList>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ForLoop` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ForLoop. Returns null if it has
     * the wrong type.
     */
    ForLoop *as_for_loop() override;

    /**
     * Interprets this node to a node of type ForLoop. Returns null if it has
     * the wrong type.
     */
    const ForLoop *as_for_loop() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ForLoop> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A foreach loop. Note that this is rather dumbed-down in cQASM 1.2 compared to
 * the cQASM 2.0 syntax it was inspired from, and is only usable as a
 * range-based loop.
 */
class ForeachLoop : public Structured {
public:

    /**
     * Reference to the variable used for looping.
     */
    One<Expression> lhs;

    /**
     * The first value.
     */
    One<Expression> frm;

    /**
     * The last value.
     */
    One<Expression> to;

    /**
     * The loop body.
     */
    One<StatementList> body;

    /**
     * Constructor.
     */
    ForeachLoop(const One<Expression> &lhs = One<Expression>(), const One<Expression> &frm = One<Expression>(), const One<Expression> &to = One<Expression>(), const One<StatementList> &body = One<StatementList>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ForeachLoop` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ForeachLoop. Returns null if it
     * has the wrong type.
     */
    ForeachLoop *as_foreach_loop() override;

    /**
     * Interprets this node to a node of type ForeachLoop. Returns null if it
     * has the wrong type.
     */
    const ForeachLoop *as_foreach_loop() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ForeachLoop> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A function call.
 */
class FunctionCall : public Expression {
public:

    /**
     * The name of the function.
     */
    One<Identifier> name;

    /**
     * The function arguments.
     */
    One<ExpressionList> arguments;

    /**
     * Constructor.
     */
    FunctionCall(const One<Identifier> &name = One<Identifier>(), const One<ExpressionList> &arguments = One<ExpressionList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `FunctionCall` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type FunctionCall. Returns null if it
     * has the wrong type.
     */
    FunctionCall *as_function_call() override;

    /**
     * Interprets this node to a node of type FunctionCall. Returns null if it
     * has the wrong type.
     */
    const FunctionCall *as_function_call() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<FunctionCall> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An identifier.
 */
class Identifier : public Expression {
public:

    /**
     * The identifier.
     */
    cqasm::v1::primitives::Str name;

    /**
     * Constructor.
     */
    Identifier(const cqasm::v1::primitives::Str &name = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Str>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Identifier` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Identifier. Returns null if it has
     * the wrong type.
     */
    Identifier *as_identifier() override;

    /**
     * Interprets this node to a node of type Identifier. Returns null if it has
     * the wrong type.
     */
    const Identifier *as_identifier() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Identifier> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An if-else chain.
 */
class IfElse : public Structured {
public:

    /**
     * The if-else branches.
     */
    Many<IfElseBranch> branches;

    /**
     * The final else block, if any.
     */
    Maybe<StatementList> otherwise;

    /**
     * Constructor.
     */
    IfElse(const Many<IfElseBranch> &branches = Many<IfElseBranch>(), const Maybe<StatementList> &otherwise = Maybe<StatementList>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IfElse` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IfElse. Returns null if it has the
     * wrong type.
     */
    IfElse *as_if_else() override;

    /**
     * Interprets this node to a node of type IfElse. Returns null if it has the
     * wrong type.
     */
    const IfElse *as_if_else() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IfElse> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A single condition + block for use in an if-else chain.
 */
class IfElseBranch : public Node {
public:

    /**
     * The condition.
     */
    One<Expression> condition;

    /**
     * The body.
     */
    One<StatementList> body;

    /**
     * Constructor.
     */
    IfElseBranch(const One<Expression> &condition = One<Expression>(), const One<StatementList> &body = One<StatementList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IfElseBranch` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IfElseBranch. Returns null if it
     * has the wrong type.
     */
    IfElseBranch *as_if_else_branch() override;

    /**
     * Interprets this node to a node of type IfElseBranch. Returns null if it
     * has the wrong type.
     */
    const IfElseBranch *as_if_else_branch() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IfElseBranch> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Indexation operator.
 */
class Index : public Expression {
public:

    /**
     * The expression being indexed.
     */
    One<Expression> expr;

    /**
     * The list of indices.
     */
    One<IndexList> indices;

    /**
     * Constructor.
     */
    Index(const One<Expression> &expr = One<Expression>(), const One<IndexList> &indices = One<IndexList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Index` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Index. Returns null if it has the
     * wrong type.
     */
    Index *as_index() override;

    /**
     * Interprets this node to a node of type Index. Returns null if it has the
     * wrong type.
     */
    const Index *as_index() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Index> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An entry in an index list. Can be a single index or a range.
 */
class IndexEntry : public Node {
public:

    /**
     * Interprets this node to a node of type IndexEntry. Returns null if it has
     * the wrong type.
     */
    IndexEntry *as_index_entry() override;

    /**
     * Interprets this node to a node of type IndexEntry. Returns null if it has
     * the wrong type.
     */
    const IndexEntry *as_index_entry() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IndexEntry> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A single index in an index list.
 */
class IndexItem : public IndexEntry {
public:

    /**
     * An integer expression representing the index.
     */
    One<Expression> index;

    /**
     * Constructor.
     */
    IndexItem(const One<Expression> &index = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IndexItem` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IndexItem. Returns null if it has
     * the wrong type.
     */
    IndexItem *as_index_item() override;

    /**
     * Interprets this node to a node of type IndexItem. Returns null if it has
     * the wrong type.
     */
    const IndexItem *as_index_item() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IndexItem> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A list of one or more indices.
 */
class IndexList : public Node {
public:

    /**
     * The list of indices.
     */
    Many<IndexEntry> items;

    /**
     * Constructor.
     */
    IndexList(const Many<IndexEntry> &items = Many<IndexEntry>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IndexList` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IndexList. Returns null if it has
     * the wrong type.
     */
    IndexList *as_index_list() override;

    /**
     * Interprets this node to a node of type IndexList. Returns null if it has
     * the wrong type.
     */
    const IndexList *as_index_list() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IndexList> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An inclusive range of indices in an index list.
 */
class IndexRange : public IndexEntry {
public:

    /**
     * An integer expression representing the first index.
     */
    One<Expression> first;

    /**
     * An integer expression representing the last index.
     */
    One<Expression> last;

    /**
     * Constructor.
     */
    IndexRange(const One<Expression> &first = One<Expression>(), const One<Expression> &last = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IndexRange` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IndexRange. Returns null if it has
     * the wrong type.
     */
    IndexRange *as_index_range() override;

    /**
     * Interprets this node to a node of type IndexRange. Returns null if it has
     * the wrong type.
     */
    const IndexRange *as_index_range() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IndexRange> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Any kind of instruction. Note that this is NOT a statement; instructions are
 * always considered part of a parallel bundle, even if they're on their own.
 */
class Instruction : public Annotated {
public:

    /**
     * Name identifying the instruction.
     */
    One<Identifier> name;

    /**
     * Optional conditional expression.
     */
    Maybe<Expression> condition;

    /**
     * Operands for the instruction.
     */
    One<ExpressionList> operands;

    /**
     * Constructor.
     */
    Instruction(const One<Identifier> &name = One<Identifier>(), const Maybe<Expression> &condition = Maybe<Expression>(), const One<ExpressionList> &operands = One<ExpressionList>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Instruction` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Instruction. Returns null if it
     * has the wrong type.
     */
    Instruction *as_instruction() override;

    /**
     * Interprets this node to a node of type Instruction. Returns null if it
     * has the wrong type.
     */
    const Instruction *as_instruction() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Instruction> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Integer division operator.
 */
class IntDivide : public ArithOp {
public:

    /**
     * Constructor.
     */
    IntDivide(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IntDivide` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IntDivide. Returns null if it has
     * the wrong type.
     */
    IntDivide *as_int_divide() override;

    /**
     * Interprets this node to a node of type IntDivide. Returns null if it has
     * the wrong type.
     */
    const IntDivide *as_int_divide() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IntDivide> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * An integer literal.
 */
class IntegerLiteral : public Expression {
public:

    /**
     * The integer.
     */
    cqasm::v1::primitives::Int value;

    /**
     * Constructor.
     */
    IntegerLiteral(const cqasm::v1::primitives::Int &value = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Int>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `IntegerLiteral` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type IntegerLiteral. Returns null if it
     * has the wrong type.
     */
    IntegerLiteral *as_integer_literal() override;

    /**
     * Interprets this node to a node of type IntegerLiteral. Returns null if it
     * has the wrong type.
     */
    const IntegerLiteral *as_integer_literal() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<IntegerLiteral> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a JSON literal.
 */
class JsonLiteral : public Expression {
public:

    /**
     * The JSON literal.
     */
    cqasm::v1::primitives::Str value;

    /**
     * Constructor.
     */
    JsonLiteral(const cqasm::v1::primitives::Str &value = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Str>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `JsonLiteral` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type JsonLiteral. Returns null if it
     * has the wrong type.
     */
    JsonLiteral *as_json_literal() override;

    /**
     * Interprets this node to a node of type JsonLiteral. Returns null if it
     * has the wrong type.
     */
    const JsonLiteral *as_json_literal() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<JsonLiteral> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical operators.
 */
class LogicalOp : public BinaryOp {
public:

    /**
     * Constructor.
     */
    LogicalOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type LogicalOp. Returns null if it has
     * the wrong type.
     */
    LogicalOp *as_logical_op() override;

    /**
     * Interprets this node to a node of type LogicalOp. Returns null if it has
     * the wrong type.
     */
    const LogicalOp *as_logical_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<LogicalOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical AND operator.
 */
class LogicalAnd : public LogicalOp {
public:

    /**
     * Constructor.
     */
    LogicalAnd(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `LogicalAnd` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type LogicalAnd. Returns null if it has
     * the wrong type.
     */
    LogicalAnd *as_logical_and() override;

    /**
     * Interprets this node to a node of type LogicalAnd. Returns null if it has
     * the wrong type.
     */
    const LogicalAnd *as_logical_and() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<LogicalAnd> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical/boolean NOT.
 */
class LogicalNot : public UnaryOp {
public:

    /**
     * Constructor.
     */
    LogicalNot(const One<Expression> &expr = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `LogicalNot` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type LogicalNot. Returns null if it has
     * the wrong type.
     */
    LogicalNot *as_logical_not() override;

    /**
     * Interprets this node to a node of type LogicalNot. Returns null if it has
     * the wrong type.
     */
    const LogicalNot *as_logical_not() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<LogicalNot> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical OR operator.
 */
class LogicalOr : public LogicalOp {
public:

    /**
     * Constructor.
     */
    LogicalOr(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `LogicalOr` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type LogicalOr. Returns null if it has
     * the wrong type.
     */
    LogicalOr *as_logical_or() override;

    /**
     * Interprets this node to a node of type LogicalOr. Returns null if it has
     * the wrong type.
     */
    const LogicalOr *as_logical_or() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<LogicalOr> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical XOR operator.
 */
class LogicalXor : public LogicalOp {
public:

    /**
     * Constructor.
     */
    LogicalXor(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `LogicalXor` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type LogicalXor. Returns null if it has
     * the wrong type.
     */
    LogicalXor *as_logical_xor() override;

    /**
     * Interprets this node to a node of type LogicalXor. Returns null if it has
     * the wrong type.
     */
    const LogicalXor *as_logical_xor() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<LogicalXor> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A mapping (alias) for an expression. Originally just a way of naming a single
 * qubit.
 */
class Mapping : public Statement {
public:

    /**
     * The identifier used to refer to the expression.
     */
    One<Identifier> alias;

    /**
     * The aliased expression.
     */
    One<Expression> expr;

    /**
     * Constructor.
     */
    Mapping(const One<Identifier> &alias = One<Identifier>(), const One<Expression> &expr = One<Expression>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Mapping` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Mapping. Returns null if it has
     * the wrong type.
     */
    Mapping *as_mapping() override;

    /**
     * Interprets this node to a node of type Mapping. Returns null if it has
     * the wrong type.
     */
    const Mapping *as_mapping() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Mapping> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a matrix literal. An empty matrix is defined to have one row with
 * zero elements. Arrays are represented as a matrix with one row. Row vs column
 * vectors are different by syntax, depending on whether comma separation vs
 * newline/semicolon is respectively used. While the AST allows a different
 * number of elements for each row, this is illegal; it is caught at parse-time.
 */
class MatrixLiteral : public Expression {
public:

    /**
     * The list of rows in the matrix.
     */
    Many<ExpressionList> rows;

    /**
     * Constructor.
     */
    MatrixLiteral(const Many<ExpressionList> &rows = Many<ExpressionList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `MatrixLiteral` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type MatrixLiteral. Returns null if it
     * has the wrong type.
     */
    MatrixLiteral *as_matrix_literal() override;

    /**
     * Interprets this node to a node of type MatrixLiteral. Returns null if it
     * has the wrong type.
     */
    const MatrixLiteral *as_matrix_literal() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<MatrixLiteral> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Modulo operator.
 */
class Modulo : public ArithOp {
public:

    /**
     * Constructor.
     */
    Modulo(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Modulo` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Modulo. Returns null if it has the
     * wrong type.
     */
    Modulo *as_modulo() override;

    /**
     * Interprets this node to a node of type Modulo. Returns null if it has the
     * wrong type.
     */
    const Modulo *as_modulo() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Modulo> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Multiplication operator.
 */
class Multiply : public ArithOp {
public:

    /**
     * Constructor.
     */
    Multiply(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Multiply` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Multiply. Returns null if it has
     * the wrong type.
     */
    Multiply *as_multiply() override;

    /**
     * Interprets this node to a node of type Multiply. Returns null if it has
     * the wrong type.
     */
    const Multiply *as_multiply() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Multiply> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Negation operator (two's complement).
 */
class Negate : public UnaryOp {
public:

    /**
     * Constructor.
     */
    Negate(const One<Expression> &expr = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Negate` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Negate. Returns null if it has the
     * wrong type.
     */
    Negate *as_negate() override;

    /**
     * Interprets this node to a node of type Negate. Returns null if it has the
     * wrong type.
     */
    const Negate *as_negate() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Negate> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Exponentiation operator.
 */
class Power : public ArithOp {
public:

    /**
     * Constructor.
     */
    Power(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Power` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Power. Returns null if it has the
     * wrong type.
     */
    Power *as_power() override;

    /**
     * Interprets this node to a node of type Power. Returns null if it has the
     * wrong type.
     */
    const Power *as_power() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Power> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A complete program.
 */
class Program : public Root {
public:

    /**
     * File version.
     */
    One<Version> version;

    /**
     * Integer expression indicating the number of qubits.
     */
    Maybe<Expression> num_qubits;

    /**
     * The statement list.
     */
    One<StatementList> statements;

    /**
     * Constructor.
     */
    Program(const One<Version> &version = One<Version>(), const Maybe<Expression> &num_qubits = Maybe<Expression>(), const One<StatementList> &statements = One<StatementList>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Program` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Program. Returns null if it has
     * the wrong type.
     */
    Program *as_program() override;

    /**
     * Interprets this node to a node of type Program. Returns null if it has
     * the wrong type.
     */
    const Program *as_program() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Program> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A repeat-until loop.
 */
class RepeatUntilLoop : public Structured {
public:

    /**
     * The loop body.
     */
    One<StatementList> body;

    /**
     * The condition for stopping iteration.
     */
    One<Expression> condition;

    /**
     * Constructor.
     */
    RepeatUntilLoop(const One<StatementList> &body = One<StatementList>(), const One<Expression> &condition = One<Expression>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `RepeatUntilLoop` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type RepeatUntilLoop. Returns null if
     * it has the wrong type.
     */
    RepeatUntilLoop *as_repeat_until_loop() override;

    /**
     * Interprets this node to a node of type RepeatUntilLoop. Returns null if
     * it has the wrong type.
     */
    const RepeatUntilLoop *as_repeat_until_loop() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<RepeatUntilLoop> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Shift operators.
 */
class ShiftOp : public BinaryOp {
public:

    /**
     * Constructor.
     */
    ShiftOp(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Interprets this node to a node of type ShiftOp. Returns null if it has
     * the wrong type.
     */
    ShiftOp *as_shift_op() override;

    /**
     * Interprets this node to a node of type ShiftOp. Returns null if it has
     * the wrong type.
     */
    const ShiftOp *as_shift_op() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ShiftOp> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Shift-left operator.
 */
class ShiftLeft : public ShiftOp {
public:

    /**
     * Constructor.
     */
    ShiftLeft(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ShiftLeft` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ShiftLeft. Returns null if it has
     * the wrong type.
     */
    ShiftLeft *as_shift_left() override;

    /**
     * Interprets this node to a node of type ShiftLeft. Returns null if it has
     * the wrong type.
     */
    const ShiftLeft *as_shift_left() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ShiftLeft> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Arithmetic/signed shift-right operator (shifts in sign).
 */
class ShiftRightArith : public ShiftOp {
public:

    /**
     * Constructor.
     */
    ShiftRightArith(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ShiftRightArith` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ShiftRightArith. Returns null if
     * it has the wrong type.
     */
    ShiftRightArith *as_shift_right_arith() override;

    /**
     * Interprets this node to a node of type ShiftRightArith. Returns null if
     * it has the wrong type.
     */
    const ShiftRightArith *as_shift_right_arith() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ShiftRightArith> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Logical/unsigned shift-right operator (shifts in zero).
 */
class ShiftRightLogic : public ShiftOp {
public:

    /**
     * Constructor.
     */
    ShiftRightLogic(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ShiftRightLogic` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type ShiftRightLogic. Returns null if
     * it has the wrong type.
     */
    ShiftRightLogic *as_shift_right_logic() override;

    /**
     * Interprets this node to a node of type ShiftRightLogic. Returns null if
     * it has the wrong type.
     */
    const ShiftRightLogic *as_shift_right_logic() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<ShiftRightLogic> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A list of statements.
 */
class StatementList : public Node {
public:

    /**
     * The list of statements.
     */
    Any<Statement> items;

    /**
     * Constructor.
     */
    StatementList(const Any<Statement> &items = Any<Statement>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `StatementList` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type StatementList. Returns null if it
     * has the wrong type.
     */
    StatementList *as_statement_list() override;

    /**
     * Interprets this node to a node of type StatementList. Returns null if it
     * has the wrong type.
     */
    const StatementList *as_statement_list() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<StatementList> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a string literal.
 */
class StringLiteral : public Expression {
public:

    /**
     * The string literal.
     */
    cqasm::v1::primitives::Str value;

    /**
     * Constructor.
     */
    StringLiteral(const cqasm::v1::primitives::Str &value = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Str>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `StringLiteral` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type StringLiteral. Returns null if it
     * has the wrong type.
     */
    StringLiteral *as_string_literal() override;

    /**
     * Interprets this node to a node of type StringLiteral. Returns null if it
     * has the wrong type.
     */
    const StringLiteral *as_string_literal() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<StringLiteral> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A subcircuit header.
 */
class Subcircuit : public Statement {
public:

    /**
     * The name of the subcircuit.
     */
    One<Identifier> name;

    /**
     * An optional integer expression representing the number of iterations for
     * this subcircuit.
     */
    Maybe<Expression> iterations;

    /**
     * Constructor.
     */
    Subcircuit(const One<Identifier> &name = One<Identifier>(), const Maybe<Expression> &iterations = Maybe<Expression>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Subcircuit` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Subcircuit. Returns null if it has
     * the wrong type.
     */
    Subcircuit *as_subcircuit() override;

    /**
     * Interprets this node to a node of type Subcircuit. Returns null if it has
     * the wrong type.
     */
    const Subcircuit *as_subcircuit() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Subcircuit> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Subtraction operator.
 */
class Subtract : public ArithOp {
public:

    /**
     * Constructor.
     */
    Subtract(const One<Expression> &lhs = One<Expression>(), const One<Expression> &rhs = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Subtract` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Subtract. Returns null if it has
     * the wrong type.
     */
    Subtract *as_subtract() override;

    /**
     * Interprets this node to a node of type Subtract. Returns null if it has
     * the wrong type.
     */
    const Subtract *as_subtract() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Subtract> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Ternary conditional operator.
 */
class TernaryCond : public Expression {
public:

    /**
     * The boolean condition for selection.
     */
    One<Expression> cond;

    /**
     * The result if cond is true.
     */
    One<Expression> if_true;

    /**
     * The result if cond is false.
     */
    One<Expression> if_false;

    /**
     * Constructor.
     */
    TernaryCond(const One<Expression> &cond = One<Expression>(), const One<Expression> &if_true = One<Expression>(), const One<Expression> &if_false = One<Expression>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `TernaryCond` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type TernaryCond. Returns null if it
     * has the wrong type.
     */
    TernaryCond *as_ternary_cond() override;

    /**
     * Interprets this node to a node of type TernaryCond. Returns null if it
     * has the wrong type.
     */
    const TernaryCond *as_ternary_cond() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<TernaryCond> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * One or more variable declaration for some type.
 */
class Variables : public Statement {
public:

    /**
     * Name of the variables. Multiple can be declared on one line.
     */
    Many<Identifier> names;

    /**
     * Name of the type.
     */
    One<Identifier> typ;

    /**
     * Constructor.
     */
    Variables(const Many<Identifier> &names = Many<Identifier>(), const One<Identifier> &typ = One<Identifier>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Variables` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Variables. Returns null if it has
     * the wrong type.
     */
    Variables *as_variables() override;

    /**
     * Interprets this node to a node of type Variables. Returns null if it has
     * the wrong type.
     */
    const Variables *as_variables() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Variables> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * The file version identifier.
 */
class Version : public Node {
public:

    /**
     * The list of version components, ordered major to minor.
     */
    cqasm::v1::primitives::Version items;

    /**
     * Constructor.
     */
    Version(const cqasm::v1::primitives::Version &items = cqasm::v1::primitives::initialize<cqasm::v1::primitives::Version>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Version` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type Version. Returns null if it has
     * the wrong type.
     */
    Version *as_version() override;

    /**
     * Interprets this node to a node of type Version. Returns null if it has
     * the wrong type.
     */
    const Version *as_version() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Version> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A while loop.
 */
class WhileLoop : public Structured {
public:

    /**
     * The condition for starting another iteration.
     */
    One<Expression> condition;

    /**
     * The loop body.
     */
    One<StatementList> body;

    /**
     * Constructor.
     */
    WhileLoop(const One<Expression> &condition = One<Expression>(), const One<StatementList> &body = One<StatementList>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `WhileLoop` is complete/fully defined.
     */
    void check_complete(const ::tree::base::PointerMap &map) const override;

    /**
     * Returns the `NodeType` of this node.
     */
    NodeType type() const override;

protected:

    /**
     * Helper method for visiting nodes.
     */
    void visit_internal(VisitorBase &visitor, void *retval) override;

public:

    /**
     * Interprets this node to a node of type WhileLoop. Returns null if it has
     * the wrong type.
     */
    WhileLoop *as_while_loop() override;

    /**
     * Interprets this node to a node of type WhileLoop. Returns null if it has
     * the wrong type.
     */
    const WhileLoop *as_while_loop() const override;

    /**
     * Returns a shallow copy of this node.
     */
    One<Node> copy() const override;

    /**
     * Returns a deep copy of this node.
     */
    One<Node> clone() const override;

    /**
     * Value-based equality operator. Ignores annotations!
     */
    bool equals(const Node &rhs) const override;

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Node &rhs) const override;

    /**
     * Serializes this node to the given map.
     */
    void serialize(
        ::tree::cbor::MapWriter &map,
        const ::tree::base::PointerMap &ids
    ) const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<WhileLoop> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Internal class for implementing the visitor pattern.
 */
class VisitorBase {
public:

    /**
     * Virtual destructor for proper cleanup.
     */
    virtual ~VisitorBase() = default;

protected:

    friend class Node;
    friend class Add;
    friend class Annotated;
    friend class AnnotationData;
    friend class ArithOp;
    friend class Assignment;
    friend class BinaryOp;
    friend class BitwiseAnd;
    friend class BitwiseNot;
    friend class BitwiseOp;
    friend class BitwiseOr;
    friend class BitwiseXor;
    friend class BreakStatement;
    friend class Bundle;
    friend class CmpEq;
    friend class CmpGe;
    friend class CmpGt;
    friend class CmpLe;
    friend class CmpLt;
    friend class CmpNe;
    friend class CmpOp;
    friend class ContinueStatement;
    friend class Divide;
    friend class ErroneousExpression;
    friend class ErroneousProgram;
    friend class ErroneousStatement;
    friend class Expression;
    friend class ExpressionList;
    friend class FloatLiteral;
    friend class ForLoop;
    friend class ForeachLoop;
    friend class FunctionCall;
    friend class Identifier;
    friend class IfElse;
    friend class IfElseBranch;
    friend class Index;
    friend class IndexEntry;
    friend class IndexItem;
    friend class IndexList;
    friend class IndexRange;
    friend class Instruction;
    friend class IntDivide;
    friend class IntegerLiteral;
    friend class JsonLiteral;
    friend class LogicalAnd;
    friend class LogicalNot;
    friend class LogicalOp;
    friend class LogicalOr;
    friend class LogicalXor;
    friend class Mapping;
    friend class MatrixLiteral;
    friend class Modulo;
    friend class Multiply;
    friend class Negate;
    friend class Power;
    friend class Program;
    friend class RepeatUntilLoop;
    friend class Root;
    friend class ShiftLeft;
    friend class ShiftOp;
    friend class ShiftRightArith;
    friend class ShiftRightLogic;
    friend class Statement;
    friend class StatementList;
    friend class StringLiteral;
    friend class Structured;
    friend class Subcircuit;
    friend class Subtract;
    friend class TernaryCond;
    friend class UnaryOp;
    friend class Variables;
    friend class Version;
    friend class WhileLoop;

    /**
     * Internal visitor function for nodes of any type.
     */
    virtual void raw_visit_node(Node &node, void *retval) = 0;

    /**
     * Internal visitor function for `Add` nodes.
     */
    virtual void raw_visit_add(Add &node, void *retval) = 0;

    /**
     * Internal visitor function for `Annotated` nodes.
     */
    virtual void raw_visit_annotated(Annotated &node, void *retval) = 0;

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    virtual void raw_visit_annotation_data(AnnotationData &node, void *retval) = 0;

    /**
     * Internal visitor function for `ArithOp` nodes.
     */
    virtual void raw_visit_arith_op(ArithOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `Assignment` nodes.
     */
    virtual void raw_visit_assignment(Assignment &node, void *retval) = 0;

    /**
     * Internal visitor function for `BinaryOp` nodes.
     */
    virtual void raw_visit_binary_op(BinaryOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitwiseAnd` nodes.
     */
    virtual void raw_visit_bitwise_and(BitwiseAnd &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitwiseNot` nodes.
     */
    virtual void raw_visit_bitwise_not(BitwiseNot &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitwiseOp` nodes.
     */
    virtual void raw_visit_bitwise_op(BitwiseOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitwiseOr` nodes.
     */
    virtual void raw_visit_bitwise_or(BitwiseOr &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitwiseXor` nodes.
     */
    virtual void raw_visit_bitwise_xor(BitwiseXor &node, void *retval) = 0;

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    virtual void raw_visit_break_statement(BreakStatement &node, void *retval) = 0;

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    virtual void raw_visit_bundle(Bundle &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpEq` nodes.
     */
    virtual void raw_visit_cmp_eq(CmpEq &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpGe` nodes.
     */
    virtual void raw_visit_cmp_ge(CmpGe &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpGt` nodes.
     */
    virtual void raw_visit_cmp_gt(CmpGt &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpLe` nodes.
     */
    virtual void raw_visit_cmp_le(CmpLe &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpLt` nodes.
     */
    virtual void raw_visit_cmp_lt(CmpLt &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpNe` nodes.
     */
    virtual void raw_visit_cmp_ne(CmpNe &node, void *retval) = 0;

    /**
     * Internal visitor function for `CmpOp` nodes.
     */
    virtual void raw_visit_cmp_op(CmpOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    virtual void raw_visit_continue_statement(ContinueStatement &node, void *retval) = 0;

    /**
     * Internal visitor function for `Divide` nodes.
     */
    virtual void raw_visit_divide(Divide &node, void *retval) = 0;

    /**
     * Internal visitor function for `ErroneousExpression` nodes.
     */
    virtual void raw_visit_erroneous_expression(ErroneousExpression &node, void *retval) = 0;

    /**
     * Internal visitor function for `ErroneousProgram` nodes.
     */
    virtual void raw_visit_erroneous_program(ErroneousProgram &node, void *retval) = 0;

    /**
     * Internal visitor function for `ErroneousStatement` nodes.
     */
    virtual void raw_visit_erroneous_statement(ErroneousStatement &node, void *retval) = 0;

    /**
     * Internal visitor function for `Expression` nodes.
     */
    virtual void raw_visit_expression(Expression &node, void *retval) = 0;

    /**
     * Internal visitor function for `ExpressionList` nodes.
     */
    virtual void raw_visit_expression_list(ExpressionList &node, void *retval) = 0;

    /**
     * Internal visitor function for `FloatLiteral` nodes.
     */
    virtual void raw_visit_float_literal(FloatLiteral &node, void *retval) = 0;

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    virtual void raw_visit_for_loop(ForLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    virtual void raw_visit_foreach_loop(ForeachLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `FunctionCall` nodes.
     */
    virtual void raw_visit_function_call(FunctionCall &node, void *retval) = 0;

    /**
     * Internal visitor function for `Identifier` nodes.
     */
    virtual void raw_visit_identifier(Identifier &node, void *retval) = 0;

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    virtual void raw_visit_if_else(IfElse &node, void *retval) = 0;

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    virtual void raw_visit_if_else_branch(IfElseBranch &node, void *retval) = 0;

    /**
     * Internal visitor function for `Index` nodes.
     */
    virtual void raw_visit_index(Index &node, void *retval) = 0;

    /**
     * Internal visitor function for `IndexEntry` nodes.
     */
    virtual void raw_visit_index_entry(IndexEntry &node, void *retval) = 0;

    /**
     * Internal visitor function for `IndexItem` nodes.
     */
    virtual void raw_visit_index_item(IndexItem &node, void *retval) = 0;

    /**
     * Internal visitor function for `IndexList` nodes.
     */
    virtual void raw_visit_index_list(IndexList &node, void *retval) = 0;

    /**
     * Internal visitor function for `IndexRange` nodes.
     */
    virtual void raw_visit_index_range(IndexRange &node, void *retval) = 0;

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    virtual void raw_visit_instruction(Instruction &node, void *retval) = 0;

    /**
     * Internal visitor function for `IntDivide` nodes.
     */
    virtual void raw_visit_int_divide(IntDivide &node, void *retval) = 0;

    /**
     * Internal visitor function for `IntegerLiteral` nodes.
     */
    virtual void raw_visit_integer_literal(IntegerLiteral &node, void *retval) = 0;

    /**
     * Internal visitor function for `JsonLiteral` nodes.
     */
    virtual void raw_visit_json_literal(JsonLiteral &node, void *retval) = 0;

    /**
     * Internal visitor function for `LogicalAnd` nodes.
     */
    virtual void raw_visit_logical_and(LogicalAnd &node, void *retval) = 0;

    /**
     * Internal visitor function for `LogicalNot` nodes.
     */
    virtual void raw_visit_logical_not(LogicalNot &node, void *retval) = 0;

    /**
     * Internal visitor function for `LogicalOp` nodes.
     */
    virtual void raw_visit_logical_op(LogicalOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `LogicalOr` nodes.
     */
    virtual void raw_visit_logical_or(LogicalOr &node, void *retval) = 0;

    /**
     * Internal visitor function for `LogicalXor` nodes.
     */
    virtual void raw_visit_logical_xor(LogicalXor &node, void *retval) = 0;

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    virtual void raw_visit_mapping(Mapping &node, void *retval) = 0;

    /**
     * Internal visitor function for `MatrixLiteral` nodes.
     */
    virtual void raw_visit_matrix_literal(MatrixLiteral &node, void *retval) = 0;

    /**
     * Internal visitor function for `Modulo` nodes.
     */
    virtual void raw_visit_modulo(Modulo &node, void *retval) = 0;

    /**
     * Internal visitor function for `Multiply` nodes.
     */
    virtual void raw_visit_multiply(Multiply &node, void *retval) = 0;

    /**
     * Internal visitor function for `Negate` nodes.
     */
    virtual void raw_visit_negate(Negate &node, void *retval) = 0;

    /**
     * Internal visitor function for `Power` nodes.
     */
    virtual void raw_visit_power(Power &node, void *retval) = 0;

    /**
     * Internal visitor function for `Program` nodes.
     */
    virtual void raw_visit_program(Program &node, void *retval) = 0;

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    virtual void raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `Root` nodes.
     */
    virtual void raw_visit_root(Root &node, void *retval) = 0;

    /**
     * Internal visitor function for `ShiftLeft` nodes.
     */
    virtual void raw_visit_shift_left(ShiftLeft &node, void *retval) = 0;

    /**
     * Internal visitor function for `ShiftOp` nodes.
     */
    virtual void raw_visit_shift_op(ShiftOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `ShiftRightArith` nodes.
     */
    virtual void raw_visit_shift_right_arith(ShiftRightArith &node, void *retval) = 0;

    /**
     * Internal visitor function for `ShiftRightLogic` nodes.
     */
    virtual void raw_visit_shift_right_logic(ShiftRightLogic &node, void *retval) = 0;

    /**
     * Internal visitor function for `Statement` nodes.
     */
    virtual void raw_visit_statement(Statement &node, void *retval) = 0;

    /**
     * Internal visitor function for `StatementList` nodes.
     */
    virtual void raw_visit_statement_list(StatementList &node, void *retval) = 0;

    /**
     * Internal visitor function for `StringLiteral` nodes.
     */
    virtual void raw_visit_string_literal(StringLiteral &node, void *retval) = 0;

    /**
     * Internal visitor function for `Structured` nodes.
     */
    virtual void raw_visit_structured(Structured &node, void *retval) = 0;

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    virtual void raw_visit_subcircuit(Subcircuit &node, void *retval) = 0;

    /**
     * Internal visitor function for `Subtract` nodes.
     */
    virtual void raw_visit_subtract(Subtract &node, void *retval) = 0;

    /**
     * Internal visitor function for `TernaryCond` nodes.
     */
    virtual void raw_visit_ternary_cond(TernaryCond &node, void *retval) = 0;

    /**
     * Internal visitor function for `UnaryOp` nodes.
     */
    virtual void raw_visit_unary_op(UnaryOp &node, void *retval) = 0;

    /**
     * Internal visitor function for `Variables` nodes.
     */
    virtual void raw_visit_variables(Variables &node, void *retval) = 0;

    /**
     * Internal visitor function for `Version` nodes.
     */
    virtual void raw_visit_version(Version &node, void *retval) = 0;

    /**
     * Internal visitor function for `WhileLoop` nodes.
     */
    virtual void raw_visit_while_loop(WhileLoop &node, void *retval) = 0;

};

/**
 * Base class for the visitor pattern for the tree.
 * 
 * To operate on the tree, derive from this class, describe your operation by
 * overriding the appropriate visit functions. and then call
 * `node->visit(your_visitor)`. The default implementations for the
 * node-specific functions fall back to the more generic functions, eventually
 * leading to `visit_node()`, which must be implemented with the desired
 * behavior for unknown nodes.
 */
template <typename T>
class Visitor : public VisitorBase {
protected:

    /**
     * Internal visitor function for nodes of any type.
     */
    void raw_visit_node(Node &node, void *retval) override;

    /**
     * Internal visitor function for `Add` nodes.
     */
    void raw_visit_add(Add &node, void *retval) override;

    /**
     * Internal visitor function for `Annotated` nodes.
     */
    void raw_visit_annotated(Annotated &node, void *retval) override;

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    void raw_visit_annotation_data(AnnotationData &node, void *retval) override;

    /**
     * Internal visitor function for `ArithOp` nodes.
     */
    void raw_visit_arith_op(ArithOp &node, void *retval) override;

    /**
     * Internal visitor function for `Assignment` nodes.
     */
    void raw_visit_assignment(Assignment &node, void *retval) override;

    /**
     * Internal visitor function for `BinaryOp` nodes.
     */
    void raw_visit_binary_op(BinaryOp &node, void *retval) override;

    /**
     * Internal visitor function for `BitwiseAnd` nodes.
     */
    void raw_visit_bitwise_and(BitwiseAnd &node, void *retval) override;

    /**
     * Internal visitor function for `BitwiseNot` nodes.
     */
    void raw_visit_bitwise_not(BitwiseNot &node, void *retval) override;

    /**
     * Internal visitor function for `BitwiseOp` nodes.
     */
    void raw_visit_bitwise_op(BitwiseOp &node, void *retval) override;

    /**
     * Internal visitor function for `BitwiseOr` nodes.
     */
    void raw_visit_bitwise_or(BitwiseOr &node, void *retval) override;

    /**
     * Internal visitor function for `BitwiseXor` nodes.
     */
    void raw_visit_bitwise_xor(BitwiseXor &node, void *retval) override;

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    void raw_visit_break_statement(BreakStatement &node, void *retval) override;

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    void raw_visit_bundle(Bundle &node, void *retval) override;

    /**
     * Internal visitor function for `CmpEq` nodes.
     */
    void raw_visit_cmp_eq(CmpEq &node, void *retval) override;

    /**
     * Internal visitor function for `CmpGe` nodes.
     */
    void raw_visit_cmp_ge(CmpGe &node, void *retval) override;

    /**
     * Internal visitor function for `CmpGt` nodes.
     */
    void raw_visit_cmp_gt(CmpGt &node, void *retval) override;

    /**
     * Internal visitor function for `CmpLe` nodes.
     */
    void raw_visit_cmp_le(CmpLe &node, void *retval) override;

    /**
     * Internal visitor function for `CmpLt` nodes.
     */
    void raw_visit_cmp_lt(CmpLt &node, void *retval) override;

    /**
     * Internal visitor function for `CmpNe` nodes.
     */
    void raw_visit_cmp_ne(CmpNe &node, void *retval) override;

    /**
     * Internal visitor function for `CmpOp` nodes.
     */
    void raw_visit_cmp_op(CmpOp &node, void *retval) override;

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    void raw_visit_continue_statement(ContinueStatement &node, void *retval) override;

    /**
     * Internal visitor function for `Divide` nodes.
     */
    void raw_visit_divide(Divide &node, void *retval) override;

    /**
     * Internal visitor function for `ErroneousExpression` nodes.
     */
    void raw_visit_erroneous_expression(ErroneousExpression &node, void *retval) override;

    /**
     * Internal visitor function for `ErroneousProgram` nodes.
     */
    void raw_visit_erroneous_program(ErroneousProgram &node, void *retval) override;

    /**
     * Internal visitor function for `ErroneousStatement` nodes.
     */
    void raw_visit_erroneous_statement(ErroneousStatement &node, void *retval) override;

    /**
     * Internal visitor function for `Expression` nodes.
     */
    void raw_visit_expression(Expression &node, void *retval) override;

    /**
     * Internal visitor function for `ExpressionList` nodes.
     */
    void raw_visit_expression_list(ExpressionList &node, void *retval) override;

    /**
     * Internal visitor function for `FloatLiteral` nodes.
     */
    void raw_visit_float_literal(FloatLiteral &node, void *retval) override;

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    void raw_visit_for_loop(ForLoop &node, void *retval) override;

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    void raw_visit_foreach_loop(ForeachLoop &node, void *retval) override;

    /**
     * Internal visitor function for `FunctionCall` nodes.
     */
    void raw_visit_function_call(FunctionCall &node, void *retval) override;

    /**
     * Internal visitor function for `Identifier` nodes.
     */
    void raw_visit_identifier(Identifier &node, void *retval) override;

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    void raw_visit_if_else(IfElse &node, void *retval) override;

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    void raw_visit_if_else_branch(IfElseBranch &node, void *retval) override;

    /**
     * Internal visitor function for `Index` nodes.
     */
    void raw_visit_index(Index &node, void *retval) override;

    /**
     * Internal visitor function for `IndexEntry` nodes.
     */
    void raw_visit_index_entry(IndexEntry &node, void *retval) override;

    /**
     * Internal visitor function for `IndexItem` nodes.
     */
    void raw_visit_index_item(IndexItem &node, void *retval) override;

    /**
     * Internal visitor function for `IndexList` nodes.
     */
    void raw_visit_index_list(IndexList &node, void *retval) override;

    /**
     * Internal visitor function for `IndexRange` nodes.
     */
    void raw_visit_index_range(IndexRange &node, void *retval) override;

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    void raw_visit_instruction(Instruction &node, void *retval) override;

    /**
     * Internal visitor function for `IntDivide` nodes.
     */
    void raw_visit_int_divide(IntDivide &node, void *retval) override;

    /**
     * Internal visitor function for `IntegerLiteral` nodes.
     */
    void raw_visit_integer_literal(IntegerLiteral &node, void *retval) override;

    /**
     * Internal visitor function for `JsonLiteral` nodes.
     */
    void raw_visit_json_literal(JsonLiteral &node, void *retval) override;

    /**
     * Internal visitor function for `LogicalAnd` nodes.
     */
    void raw_visit_logical_and(LogicalAnd &node, void *retval) override;

    /**
     * Internal visitor function for `LogicalNot` nodes.
     */
    void raw_visit_logical_not(LogicalNot &node, void *retval) override;

    /**
     * Internal visitor function for `LogicalOp` nodes.
     */
    void raw_visit_logical_op(LogicalOp &node, void *retval) override;

    /**
     * Internal visitor function for `LogicalOr` nodes.
     */
    void raw_visit_logical_or(LogicalOr &node, void *retval) override;

    /**
     * Internal visitor function for `LogicalXor` nodes.
     */
    void raw_visit_logical_xor(LogicalXor &node, void *retval) override;

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    void raw_visit_mapping(Mapping &node, void *retval) override;

    /**
     * Internal visitor function for `MatrixLiteral` nodes.
     */
    void raw_visit_matrix_literal(MatrixLiteral &node, void *retval) override;

    /**
     * Internal visitor function for `Modulo` nodes.
     */
    void raw_visit_modulo(Modulo &node, void *retval) override;

    /**
     * Internal visitor function for `Multiply` nodes.
     */
    void raw_visit_multiply(Multiply &node, void *retval) override;

    /**
     * Internal visitor function for `Negate` nodes.
     */
    void raw_visit_negate(Negate &node, void *retval) override;

    /**
     * Internal visitor function for `Power` nodes.
     */
    void raw_visit_power(Power &node, void *retval) override;

    /**
     * Internal visitor function for `Program` nodes.
     */
    void raw_visit_program(Program &node, void *retval) override;

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    void raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) override;

    /**
     * Internal visitor function for `Root` nodes.
     */
    void raw_visit_root(Root &node, void *retval) override;

    /**
     * Internal visitor function for `ShiftLeft` nodes.
     */
    void raw_visit_shift_left(ShiftLeft &node, void *retval) override;

    /**
     * Internal visitor function for `ShiftOp` nodes.
     */
    void raw_visit_shift_op(ShiftOp &node, void *retval) override;

    /**
     * Internal visitor function for `ShiftRightArith` nodes.
     */
    void raw_visit_shift_right_arith(ShiftRightArith &node, void *retval) override;

    /**
     * Internal visitor function for `ShiftRightLogic` nodes.
     */
    void raw_visit_shift_right_logic(ShiftRightLogic &node, void *retval) override;

    /**
     * Internal visitor function for `Statement` nodes.
     */
    void raw_visit_statement(Statement &node, void *retval) override;

    /**
     * Internal visitor function for `StatementList` nodes.
     */
    void raw_visit_statement_list(StatementList &node, void *retval) override;

    /**
     * Internal visitor function for `StringLiteral` nodes.
     */
    void raw_visit_string_literal(StringLiteral &node, void *retval) override;

    /**
     * Internal visitor function for `Structured` nodes.
     */
    void raw_visit_structured(Structured &node, void *retval) override;

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    void raw_visit_subcircuit(Subcircuit &node, void *retval) override;

    /**
     * Internal visitor function for `Subtract` nodes.
     */
    void raw_visit_subtract(Subtract &node, void *retval) override;

    /**
     * Internal visitor function for `TernaryCond` nodes.
     */
    void raw_visit_ternary_cond(TernaryCond &node, void *retval) override;

    /**
     * Internal visitor function for `UnaryOp` nodes.
     */
    void raw_visit_unary_op(UnaryOp &node, void *retval) override;

    /**
     * Internal visitor function for `Variables` nodes.
     */
    void raw_visit_variables(Variables &node, void *retval) override;

    /**
     * Internal visitor function for `Version` nodes.
     */
    void raw_visit_version(Version &node, void *retval) override;

    /**
     * Internal visitor function for `WhileLoop` nodes.
     */
    void raw_visit_while_loop(WhileLoop &node, void *retval) override;

public:

    /**
     * Fallback function for nodes of any type.
     */
    virtual T visit_node(Node &node) = 0;

    /**
     * Visitor function for `Add` nodes.
     */
    virtual T visit_add(Add &node) {
        return visit_arith_op(node);
    }

    /**
     * Fallback function for `Annotated` nodes.
     */
    virtual T visit_annotated(Annotated &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `AnnotationData` nodes.
     */
    virtual T visit_annotation_data(AnnotationData &node) {
        return visit_node(node);
    }

    /**
     * Fallback function for `ArithOp` nodes.
     */
    virtual T visit_arith_op(ArithOp &node) {
        return visit_binary_op(node);
    }

    /**
     * Visitor function for `Assignment` nodes.
     */
    virtual T visit_assignment(Assignment &node) {
        return visit_node(node);
    }

    /**
     * Fallback function for `BinaryOp` nodes.
     */
    virtual T visit_binary_op(BinaryOp &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `BitwiseAnd` nodes.
     */
    virtual T visit_bitwise_and(BitwiseAnd &node) {
        return visit_bitwise_op(node);
    }

    /**
     * Visitor function for `BitwiseNot` nodes.
     */
    virtual T visit_bitwise_not(BitwiseNot &node) {
        return visit_unary_op(node);
    }

    /**
     * Fallback function for `BitwiseOp` nodes.
     */
    virtual T visit_bitwise_op(BitwiseOp &node) {
        return visit_binary_op(node);
    }

    /**
     * Visitor function for `BitwiseOr` nodes.
     */
    virtual T visit_bitwise_or(BitwiseOr &node) {
        return visit_bitwise_op(node);
    }

    /**
     * Visitor function for `BitwiseXor` nodes.
     */
    virtual T visit_bitwise_xor(BitwiseXor &node) {
        return visit_bitwise_op(node);
    }

    /**
     * Visitor function for `BreakStatement` nodes.
     */
    virtual T visit_break_statement(BreakStatement &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `Bundle` nodes.
     */
    virtual T visit_bundle(Bundle &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `CmpEq` nodes.
     */
    virtual T visit_cmp_eq(CmpEq &node) {
        return visit_cmp_op(node);
    }

    /**
     * Visitor function for `CmpGe` nodes.
     */
    virtual T visit_cmp_ge(CmpGe &node) {
        return visit_cmp_op(node);
    }

    /**
     * Visitor function for `CmpGt` nodes.
     */
    virtual T visit_cmp_gt(CmpGt &node) {
        return visit_cmp_op(node);
    }

    /**
     * Visitor function for `CmpLe` nodes.
     */
    virtual T visit_cmp_le(CmpLe &node) {
        return visit_cmp_op(node);
    }

    /**
     * Visitor function for `CmpLt` nodes.
     */
    virtual T visit_cmp_lt(CmpLt &node) {
        return visit_cmp_op(node);
    }

    /**
     * Visitor function for `CmpNe` nodes.
     */
    virtual T visit_cmp_ne(CmpNe &node) {
        return visit_cmp_op(node);
    }

    /**
     * Fallback function for `CmpOp` nodes.
     */
    virtual T visit_cmp_op(CmpOp &node) {
        return visit_binary_op(node);
    }

    /**
     * Visitor function for `ContinueStatement` nodes.
     */
    virtual T visit_continue_statement(ContinueStatement &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `Divide` nodes.
     */
    virtual T visit_divide(Divide &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `ErroneousExpression` nodes.
     */
    virtual T visit_erroneous_expression(ErroneousExpression &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `ErroneousProgram` nodes.
     */
    virtual T visit_erroneous_program(ErroneousProgram &node) {
        return visit_root(node);
    }

    /**
     * Visitor function for `ErroneousStatement` nodes.
     */
    virtual T visit_erroneous_statement(ErroneousStatement &node) {
        return visit_statement(node);
    }

    /**
     * Fallback function for `Expression` nodes.
     */
    virtual T visit_expression(Expression &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `ExpressionList` nodes.
     */
    virtual T visit_expression_list(ExpressionList &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `FloatLiteral` nodes.
     */
    virtual T visit_float_literal(FloatLiteral &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `ForLoop` nodes.
     */
    virtual T visit_for_loop(ForLoop &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `ForeachLoop` nodes.
     */
    virtual T visit_foreach_loop(ForeachLoop &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `FunctionCall` nodes.
     */
    virtual T visit_function_call(FunctionCall &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `Identifier` nodes.
     */
    virtual T visit_identifier(Identifier &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `IfElse` nodes.
     */
    virtual T visit_if_else(IfElse &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `IfElseBranch` nodes.
     */
    virtual T visit_if_else_branch(IfElseBranch &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `Index` nodes.
     */
    virtual T visit_index(Index &node) {
        return visit_expression(node);
    }

    /**
     * Fallback function for `IndexEntry` nodes.
     */
    virtual T visit_index_entry(IndexEntry &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `IndexItem` nodes.
     */
    virtual T visit_index_item(IndexItem &node) {
        return visit_index_entry(node);
    }

    /**
     * Visitor function for `IndexList` nodes.
     */
    virtual T visit_index_list(IndexList &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `IndexRange` nodes.
     */
    virtual T visit_index_range(IndexRange &node) {
        return visit_index_entry(node);
    }

    /**
     * Visitor function for `Instruction` nodes.
     */
    virtual T visit_instruction(Instruction &node) {
        return visit_annotated(node);
    }

    /**
     * Visitor function for `IntDivide` nodes.
     */
    virtual T visit_int_divide(IntDivide &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `IntegerLiteral` nodes.
     */
    virtual T visit_integer_literal(IntegerLiteral &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `JsonLiteral` nodes.
     */
    virtual T visit_json_literal(JsonLiteral &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `LogicalAnd` nodes.
     */
    virtual T visit_logical_and(LogicalAnd &node) {
        return visit_logical_op(node);
    }

    /**
     * Visitor function for `LogicalNot` nodes.
     */
    virtual T visit_logical_not(LogicalNot &node) {
        return visit_unary_op(node);
    }

    /**
     * Fallback function for `LogicalOp` nodes.
     */
    virtual T visit_logical_op(LogicalOp &node) {
        return visit_binary_op(node);
    }

    /**
     * Visitor function for `LogicalOr` nodes.
     */
    virtual T visit_logical_or(LogicalOr &node) {
        return visit_logical_op(node);
    }

    /**
     * Visitor function for `LogicalXor` nodes.
     */
    virtual T visit_logical_xor(LogicalXor &node) {
        return visit_logical_op(node);
    }

    /**
     * Visitor function for `Mapping` nodes.
     */
    virtual T visit_mapping(Mapping &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `MatrixLiteral` nodes.
     */
    virtual T visit_matrix_literal(MatrixLiteral &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `Modulo` nodes.
     */
    virtual T visit_modulo(Modulo &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `Multiply` nodes.
     */
    virtual T visit_multiply(Multiply &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `Negate` nodes.
     */
    virtual T visit_negate(Negate &node) {
        return visit_unary_op(node);
    }

    /**
     * Visitor function for `Power` nodes.
     */
    virtual T visit_power(Power &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `Program` nodes.
     */
    virtual T visit_program(Program &node) {
        return visit_root(node);
    }

    /**
     * Visitor function for `RepeatUntilLoop` nodes.
     */
    virtual T visit_repeat_until_loop(RepeatUntilLoop &node) {
        return visit_structured(node);
    }

    /**
     * Fallback function for `Root` nodes.
     */
    virtual T visit_root(Root &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `ShiftLeft` nodes.
     */
    virtual T visit_shift_left(ShiftLeft &node) {
        return visit_shift_op(node);
    }

    /**
     * Fallback function for `ShiftOp` nodes.
     */
    virtual T visit_shift_op(ShiftOp &node) {
        return visit_binary_op(node);
    }

    /**
     * Visitor function for `ShiftRightArith` nodes.
     */
    virtual T visit_shift_right_arith(ShiftRightArith &node) {
        return visit_shift_op(node);
    }

    /**
     * Visitor function for `ShiftRightLogic` nodes.
     */
    virtual T visit_shift_right_logic(ShiftRightLogic &node) {
        return visit_shift_op(node);
    }

    /**
     * Fallback function for `Statement` nodes.
     */
    virtual T visit_statement(Statement &node) {
        return visit_annotated(node);
    }

    /**
     * Visitor function for `StatementList` nodes.
     */
    virtual T visit_statement_list(StatementList &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `StringLiteral` nodes.
     */
    virtual T visit_string_literal(StringLiteral &node) {
        return visit_expression(node);
    }

    /**
     * Fallback function for `Structured` nodes.
     */
    virtual T visit_structured(Structured &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `Subcircuit` nodes.
     */
    virtual T visit_subcircuit(Subcircuit &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `Subtract` nodes.
     */
    virtual T visit_subtract(Subtract &node) {
        return visit_arith_op(node);
    }

    /**
     * Visitor function for `TernaryCond` nodes.
     */
    virtual T visit_ternary_cond(TernaryCond &node) {
        return visit_expression(node);
    }

    /**
     * Fallback function for `UnaryOp` nodes.
     */
    virtual T visit_unary_op(UnaryOp &node) {
        return visit_expression(node);
    }

    /**
     * Visitor function for `Variables` nodes.
     */
    virtual T visit_variables(Variables &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `Version` nodes.
     */
    virtual T visit_version(Version &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `WhileLoop` nodes.
     */
    virtual T visit_while_loop(WhileLoop &node) {
        return visit_structured(node);
    }

};

    /**
     * Internal visitor function for nodes of any type.
     */
    template <typename T>
    void Visitor<T>::raw_visit_node(Node &node, void *retval) {
        if (retval == nullptr) {
            this->visit_node(node);
        } else {
            *((T*)retval) = this->visit_node(node);
        };
    }

    /**
     * Internal visitor function for nodes of any type.
     */
    template <>
    void Visitor<void>::raw_visit_node(Node &node, void *retval);

    /**
     * Internal visitor function for `Add` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_add(Add &node, void *retval) {
        if (retval == nullptr) {
            this->visit_add(node);
        } else {
            *((T*)retval) = this->visit_add(node);
        };
    }

    /**
     * Internal visitor function for `Add` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_add(Add &node, void *retval);

    /**
     * Internal visitor function for `Annotated` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_annotated(Annotated &node, void *retval) {
        if (retval == nullptr) {
            this->visit_annotated(node);
        } else {
            *((T*)retval) = this->visit_annotated(node);
        };
    }

    /**
     * Internal visitor function for `Annotated` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_annotated(Annotated &node, void *retval);

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_annotation_data(AnnotationData &node, void *retval) {
        if (retval == nullptr) {
            this->visit_annotation_data(node);
        } else {
            *((T*)retval) = this->visit_annotation_data(node);
        };
    }

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_annotation_data(AnnotationData &node, void *retval);

    /**
     * Internal visitor function for `ArithOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_arith_op(ArithOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_arith_op(node);
        } else {
            *((T*)retval) = this->visit_arith_op(node);
        };
    }

    /**
     * Internal visitor function for `ArithOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_arith_op(ArithOp &node, void *retval);

    /**
     * Internal visitor function for `Assignment` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_assignment(Assignment &node, void *retval) {
        if (retval == nullptr) {
            this->visit_assignment(node);
        } else {
            *((T*)retval) = this->visit_assignment(node);
        };
    }

    /**
     * Internal visitor function for `Assignment` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_assignment(Assignment &node, void *retval);

    /**
     * Internal visitor function for `BinaryOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_binary_op(BinaryOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_binary_op(node);
        } else {
            *((T*)retval) = this->visit_binary_op(node);
        };
    }

    /**
     * Internal visitor function for `BinaryOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_binary_op(BinaryOp &node, void *retval);

    /**
     * Internal visitor function for `BitwiseAnd` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bitwise_and(BitwiseAnd &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bitwise_and(node);
        } else {
            *((T*)retval) = this->visit_bitwise_and(node);
        };
    }

    /**
     * Internal visitor function for `BitwiseAnd` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bitwise_and(BitwiseAnd &node, void *retval);

    /**
     * Internal visitor function for `BitwiseNot` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bitwise_not(BitwiseNot &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bitwise_not(node);
        } else {
            *((T*)retval) = this->visit_bitwise_not(node);
        };
    }

    /**
     * Internal visitor function for `BitwiseNot` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bitwise_not(BitwiseNot &node, void *retval);

    /**
     * Internal visitor function for `BitwiseOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bitwise_op(BitwiseOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bitwise_op(node);
        } else {
            *((T*)retval) = this->visit_bitwise_op(node);
        };
    }

    /**
     * Internal visitor function for `BitwiseOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bitwise_op(BitwiseOp &node, void *retval);

    /**
     * Internal visitor function for `BitwiseOr` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bitwise_or(BitwiseOr &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bitwise_or(node);
        } else {
            *((T*)retval) = this->visit_bitwise_or(node);
        };
    }

    /**
     * Internal visitor function for `BitwiseOr` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bitwise_or(BitwiseOr &node, void *retval);

    /**
     * Internal visitor function for `BitwiseXor` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bitwise_xor(BitwiseXor &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bitwise_xor(node);
        } else {
            *((T*)retval) = this->visit_bitwise_xor(node);
        };
    }

    /**
     * Internal visitor function for `BitwiseXor` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bitwise_xor(BitwiseXor &node, void *retval);

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_break_statement(BreakStatement &node, void *retval) {
        if (retval == nullptr) {
            this->visit_break_statement(node);
        } else {
            *((T*)retval) = this->visit_break_statement(node);
        };
    }

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_break_statement(BreakStatement &node, void *retval);

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bundle(Bundle &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bundle(node);
        } else {
            *((T*)retval) = this->visit_bundle(node);
        };
    }

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bundle(Bundle &node, void *retval);

    /**
     * Internal visitor function for `CmpEq` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_eq(CmpEq &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_eq(node);
        } else {
            *((T*)retval) = this->visit_cmp_eq(node);
        };
    }

    /**
     * Internal visitor function for `CmpEq` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_eq(CmpEq &node, void *retval);

    /**
     * Internal visitor function for `CmpGe` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_ge(CmpGe &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_ge(node);
        } else {
            *((T*)retval) = this->visit_cmp_ge(node);
        };
    }

    /**
     * Internal visitor function for `CmpGe` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_ge(CmpGe &node, void *retval);

    /**
     * Internal visitor function for `CmpGt` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_gt(CmpGt &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_gt(node);
        } else {
            *((T*)retval) = this->visit_cmp_gt(node);
        };
    }

    /**
     * Internal visitor function for `CmpGt` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_gt(CmpGt &node, void *retval);

    /**
     * Internal visitor function for `CmpLe` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_le(CmpLe &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_le(node);
        } else {
            *((T*)retval) = this->visit_cmp_le(node);
        };
    }

    /**
     * Internal visitor function for `CmpLe` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_le(CmpLe &node, void *retval);

    /**
     * Internal visitor function for `CmpLt` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_lt(CmpLt &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_lt(node);
        } else {
            *((T*)retval) = this->visit_cmp_lt(node);
        };
    }

    /**
     * Internal visitor function for `CmpLt` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_lt(CmpLt &node, void *retval);

    /**
     * Internal visitor function for `CmpNe` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_ne(CmpNe &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_ne(node);
        } else {
            *((T*)retval) = this->visit_cmp_ne(node);
        };
    }

    /**
     * Internal visitor function for `CmpNe` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_ne(CmpNe &node, void *retval);

    /**
     * Internal visitor function for `CmpOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_cmp_op(CmpOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_cmp_op(node);
        } else {
            *((T*)retval) = this->visit_cmp_op(node);
        };
    }

    /**
     * Internal visitor function for `CmpOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_cmp_op(CmpOp &node, void *retval);

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_continue_statement(ContinueStatement &node, void *retval) {
        if (retval == nullptr) {
            this->visit_continue_statement(node);
        } else {
            *((T*)retval) = this->visit_continue_statement(node);
        };
    }

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_continue_statement(ContinueStatement &node, void *retval);

    /**
     * Internal visitor function for `Divide` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_divide(Divide &node, void *retval) {
        if (retval == nullptr) {
            this->visit_divide(node);
        } else {
            *((T*)retval) = this->visit_divide(node);
        };
    }

    /**
     * Internal visitor function for `Divide` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_divide(Divide &node, void *retval);

    /**
     * Internal visitor function for `ErroneousExpression` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_erroneous_expression(ErroneousExpression &node, void *retval) {
        if (retval == nullptr) {
            this->visit_erroneous_expression(node);
        } else {
            *((T*)retval) = this->visit_erroneous_expression(node);
        };
    }

    /**
     * Internal visitor function for `ErroneousExpression` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_erroneous_expression(ErroneousExpression &node, void *retval);

    /**
     * Internal visitor function for `ErroneousProgram` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_erroneous_program(ErroneousProgram &node, void *retval) {
        if (retval == nullptr) {
            this->visit_erroneous_program(node);
        } else {
            *((T*)retval) = this->visit_erroneous_program(node);
        };
    }

    /**
     * Internal visitor function for `ErroneousProgram` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_erroneous_program(ErroneousProgram &node, void *retval);

    /**
     * Internal visitor function for `ErroneousStatement` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_erroneous_statement(ErroneousStatement &node, void *retval) {
        if (retval == nullptr) {
            this->visit_erroneous_statement(node);
        } else {
            *((T*)retval) = this->visit_erroneous_statement(node);
        };
    }

    /**
     * Internal visitor function for `ErroneousStatement` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_erroneous_statement(ErroneousStatement &node, void *retval);

    /**
     * Internal visitor function for `Expression` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_expression(Expression &node, void *retval) {
        if (retval == nullptr) {
            this->visit_expression(node);
        } else {
            *((T*)retval) = this->visit_expression(node);
        };
    }

    /**
     * Internal visitor function for `Expression` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_expression(Expression &node, void *retval);

    /**
     * Internal visitor function for `ExpressionList` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_expression_list(ExpressionList &node, void *retval) {
        if (retval == nullptr) {
            this->visit_expression_list(node);
        } else {
            *((T*)retval) = this->visit_expression_list(node);
        };
    }

    /**
     * Internal visitor function for `ExpressionList` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_expression_list(ExpressionList &node, void *retval);

    /**
     * Internal visitor function for `FloatLiteral` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_float_literal(FloatLiteral &node, void *retval) {
        if (retval == nullptr) {
            this->visit_float_literal(node);
        } else {
            *((T*)retval) = this->visit_float_literal(node);
        };
    }

    /**
     * Internal visitor function for `FloatLiteral` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_float_literal(FloatLiteral &node, void *retval);

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_for_loop(ForLoop &node, void *retval) {
        if (retval == nullptr) {
            this->visit_for_loop(node);
        } else {
            *((T*)retval) = this->visit_for_loop(node);
        };
    }

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_for_loop(ForLoop &node, void *retval);

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_foreach_loop(ForeachLoop &node, void *retval) {
        if (retval == nullptr) {
            this->visit_foreach_loop(node);
        } else {
            *((T*)retval) = this->visit_foreach_loop(node);
        };
    }

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_foreach_loop(ForeachLoop &node, void *retval);

    /**
     * Internal visitor function for `FunctionCall` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_function_call(FunctionCall &node, void *retval) {
        if (retval == nullptr) {
            this->visit_function_call(node);
        } else {
            *((T*)retval) = this->visit_function_call(node);
        };
    }

    /**
     * Internal visitor function for `FunctionCall` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_function_call(FunctionCall &node, void *retval);

    /**
     * Internal visitor function for `Identifier` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_identifier(Identifier &node, void *retval) {
        if (retval == nullptr) {
            this->visit_identifier(node);
        } else {
            *((T*)retval) = this->visit_identifier(node);
        };
    }

    /**
     * Internal visitor function for `Identifier` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_identifier(Identifier &node, void *retval);

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_if_else(IfElse &node, void *retval) {
        if (retval == nullptr) {
            this->visit_if_else(node);
        } else {
            *((T*)retval) = this->visit_if_else(node);
        };
    }

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_if_else(IfElse &node, void *retval);

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_if_else_branch(IfElseBranch &node, void *retval) {
        if (retval == nullptr) {
            this->visit_if_else_branch(node);
        } else {
            *((T*)retval) = this->visit_if_else_branch(node);
        };
    }

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_if_else_branch(IfElseBranch &node, void *retval);

    /**
     * Internal visitor function for `Index` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_index(Index &node, void *retval) {
        if (retval == nullptr) {
            this->visit_index(node);
        } else {
            *((T*)retval) = this->visit_index(node);
        };
    }

    /**
     * Internal visitor function for `Index` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_index(Index &node, void *retval);

    /**
     * Internal visitor function for `IndexEntry` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_index_entry(IndexEntry &node, void *retval) {
        if (retval == nullptr) {
            this->visit_index_entry(node);
        } else {
            *((T*)retval) = this->visit_index_entry(node);
        };
    }

    /**
     * Internal visitor function for `IndexEntry` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_index_entry(IndexEntry &node, void *retval);

    /**
     * Internal visitor function for `IndexItem` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_index_item(IndexItem &node, void *retval) {
        if (retval == nullptr) {
            this->visit_index_item(node);
        } else {
            *((T*)retval) = this->visit_index_item(node);
        };
    }

    /**
     * Internal visitor function for `IndexItem` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_index_item(IndexItem &node, void *retval);

    /**
     * Internal visitor function for `IndexList` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_index_list(IndexList &node, void *retval) {
        if (retval == nullptr) {
            this->visit_index_list(node);
        } else {
            *((T*)retval) = this->visit_index_list(node);
        };
    }

    /**
     * Internal visitor function for `IndexList` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_index_list(IndexList &node, void *retval);

    /**
     * Internal visitor function for `IndexRange` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_index_range(IndexRange &node, void *retval) {
        if (retval == nullptr) {
            this->visit_index_range(node);
        } else {
            *((T*)retval) = this->visit_index_range(node);
        };
    }

    /**
     * Internal visitor function for `IndexRange` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_index_range(IndexRange &node, void *retval);

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_instruction(Instruction &node, void *retval) {
        if (retval == nullptr) {
            this->visit_instruction(node);
        } else {
            *((T*)retval) = this->visit_instruction(node);
        };
    }

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_instruction(Instruction &node, void *retval);

    /**
     * Internal visitor function for `IntDivide` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_int_divide(IntDivide &node, void *retval) {
        if (retval == nullptr) {
            this->visit_int_divide(node);
        } else {
            *((T*)retval) = this->visit_int_divide(node);
        };
    }

    /**
     * Internal visitor function for `IntDivide` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_int_divide(IntDivide &node, void *retval);

    /**
     * Internal visitor function for `IntegerLiteral` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_integer_literal(IntegerLiteral &node, void *retval) {
        if (retval == nullptr) {
            this->visit_integer_literal(node);
        } else {
            *((T*)retval) = this->visit_integer_literal(node);
        };
    }

    /**
     * Internal visitor function for `IntegerLiteral` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_integer_literal(IntegerLiteral &node, void *retval);

    /**
     * Internal visitor function for `JsonLiteral` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_json_literal(JsonLiteral &node, void *retval) {
        if (retval == nullptr) {
            this->visit_json_literal(node);
        } else {
            *((T*)retval) = this->visit_json_literal(node);
        };
    }

    /**
     * Internal visitor function for `JsonLiteral` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_json_literal(JsonLiteral &node, void *retval);

    /**
     * Internal visitor function for `LogicalAnd` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_logical_and(LogicalAnd &node, void *retval) {
        if (retval == nullptr) {
            this->visit_logical_and(node);
        } else {
            *((T*)retval) = this->visit_logical_and(node);
        };
    }

    /**
     * Internal visitor function for `LogicalAnd` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_logical_and(LogicalAnd &node, void *retval);

    /**
     * Internal visitor function for `LogicalNot` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_logical_not(LogicalNot &node, void *retval) {
        if (retval == nullptr) {
            this->visit_logical_not(node);
        } else {
            *((T*)retval) = this->visit_logical_not(node);
        };
    }

    /**
     * Internal visitor function for `LogicalNot` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_logical_not(LogicalNot &node, void *retval);

    /**
     * Internal visitor function for `LogicalOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_logical_op(LogicalOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_logical_op(node);
        } else {
            *((T*)retval) = this->visit_logical_op(node);
        };
    }

    /**
     * Internal visitor function for `LogicalOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_logical_op(LogicalOp &node, void *retval);

    /**
     * Internal visitor function for `LogicalOr` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_logical_or(LogicalOr &node, void *retval) {
        if (retval == nullptr) {
            this->visit_logical_or(node);
        } else {
            *((T*)retval) = this->visit_logical_or(node);
        };
    }

    /**
     * Internal visitor function for `LogicalOr` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_logical_or(LogicalOr &node, void *retval);

    /**
     * Internal visitor function for `LogicalXor` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_logical_xor(LogicalXor &node, void *retval) {
        if (retval == nullptr) {
            this->visit_logical_xor(node);
        } else {
            *((T*)retval) = this->visit_logical_xor(node);
        };
    }

    /**
     * Internal visitor function for `LogicalXor` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_logical_xor(LogicalXor &node, void *retval);

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_mapping(Mapping &node, void *retval) {
        if (retval == nullptr) {
            this->visit_mapping(node);
        } else {
            *((T*)retval) = this->visit_mapping(node);
        };
    }

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_mapping(Mapping &node, void *retval);

    /**
     * Internal visitor function for `MatrixLiteral` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_matrix_literal(MatrixLiteral &node, void *retval) {
        if (retval == nullptr) {
            this->visit_matrix_literal(node);
        } else {
            *((T*)retval) = this->visit_matrix_literal(node);
        };
    }

    /**
     * Internal visitor function for `MatrixLiteral` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_matrix_literal(MatrixLiteral &node, void *retval);

    /**
     * Internal visitor function for `Modulo` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_modulo(Modulo &node, void *retval) {
        if (retval == nullptr) {
            this->visit_modulo(node);
        } else {
            *((T*)retval) = this->visit_modulo(node);
        };
    }

    /**
     * Internal visitor function for `Modulo` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_modulo(Modulo &node, void *retval);

    /**
     * Internal visitor function for `Multiply` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_multiply(Multiply &node, void *retval) {
        if (retval == nullptr) {
            this->visit_multiply(node);
        } else {
            *((T*)retval) = this->visit_multiply(node);
        };
    }

    /**
     * Internal visitor function for `Multiply` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_multiply(Multiply &node, void *retval);

    /**
     * Internal visitor function for `Negate` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_negate(Negate &node, void *retval) {
        if (retval == nullptr) {
            this->visit_negate(node);
        } else {
            *((T*)retval) = this->visit_negate(node);
        };
    }

    /**
     * Internal visitor function for `Negate` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_negate(Negate &node, void *retval);

    /**
     * Internal visitor function for `Power` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_power(Power &node, void *retval) {
        if (retval == nullptr) {
            this->visit_power(node);
        } else {
            *((T*)retval) = this->visit_power(node);
        };
    }

    /**
     * Internal visitor function for `Power` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_power(Power &node, void *retval);

    /**
     * Internal visitor function for `Program` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_program(Program &node, void *retval) {
        if (retval == nullptr) {
            this->visit_program(node);
        } else {
            *((T*)retval) = this->visit_program(node);
        };
    }

    /**
     * Internal visitor function for `Program` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_program(Program &node, void *retval);

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) {
        if (retval == nullptr) {
            this->visit_repeat_until_loop(node);
        } else {
            *((T*)retval) = this->visit_repeat_until_loop(node);
        };
    }

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval);

    /**
     * Internal visitor function for `Root` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_root(Root &node, void *retval) {
        if (retval == nullptr) {
            this->visit_root(node);
        } else {
            *((T*)retval) = this->visit_root(node);
        };
    }

    /**
     * Internal visitor function for `Root` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_root(Root &node, void *retval);

    /**
     * Internal visitor function for `ShiftLeft` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_shift_left(ShiftLeft &node, void *retval) {
        if (retval == nullptr) {
            this->visit_shift_left(node);
        } else {
            *((T*)retval) = this->visit_shift_left(node);
        };
    }

    /**
     * Internal visitor function for `ShiftLeft` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_shift_left(ShiftLeft &node, void *retval);

    /**
     * Internal visitor function for `ShiftOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_shift_op(ShiftOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_shift_op(node);
        } else {
            *((T*)retval) = this->visit_shift_op(node);
        };
    }

    /**
     * Internal visitor function for `ShiftOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_shift_op(ShiftOp &node, void *retval);

    /**
     * Internal visitor function for `ShiftRightArith` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_shift_right_arith(ShiftRightArith &node, void *retval) {
        if (retval == nullptr) {
            this->visit_shift_right_arith(node);
        } else {
            *((T*)retval) = this->visit_shift_right_arith(node);
        };
    }

    /**
     * Internal visitor function for `ShiftRightArith` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_shift_right_arith(ShiftRightArith &node, void *retval);

    /**
     * Internal visitor function for `ShiftRightLogic` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_shift_right_logic(ShiftRightLogic &node, void *retval) {
        if (retval == nullptr) {
            this->visit_shift_right_logic(node);
        } else {
            *((T*)retval) = this->visit_shift_right_logic(node);
        };
    }

    /**
     * Internal visitor function for `ShiftRightLogic` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_shift_right_logic(ShiftRightLogic &node, void *retval);

    /**
     * Internal visitor function for `Statement` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_statement(Statement &node, void *retval) {
        if (retval == nullptr) {
            this->visit_statement(node);
        } else {
            *((T*)retval) = this->visit_statement(node);
        };
    }

    /**
     * Internal visitor function for `Statement` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_statement(Statement &node, void *retval);

    /**
     * Internal visitor function for `StatementList` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_statement_list(StatementList &node, void *retval) {
        if (retval == nullptr) {
            this->visit_statement_list(node);
        } else {
            *((T*)retval) = this->visit_statement_list(node);
        };
    }

    /**
     * Internal visitor function for `StatementList` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_statement_list(StatementList &node, void *retval);

    /**
     * Internal visitor function for `StringLiteral` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_string_literal(StringLiteral &node, void *retval) {
        if (retval == nullptr) {
            this->visit_string_literal(node);
        } else {
            *((T*)retval) = this->visit_string_literal(node);
        };
    }

    /**
     * Internal visitor function for `StringLiteral` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_string_literal(StringLiteral &node, void *retval);

    /**
     * Internal visitor function for `Structured` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_structured(Structured &node, void *retval) {
        if (retval == nullptr) {
            this->visit_structured(node);
        } else {
            *((T*)retval) = this->visit_structured(node);
        };
    }

    /**
     * Internal visitor function for `Structured` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_structured(Structured &node, void *retval);

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_subcircuit(Subcircuit &node, void *retval) {
        if (retval == nullptr) {
            this->visit_subcircuit(node);
        } else {
            *((T*)retval) = this->visit_subcircuit(node);
        };
    }

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_subcircuit(Subcircuit &node, void *retval);

    /**
     * Internal visitor function for `Subtract` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_subtract(Subtract &node, void *retval) {
        if (retval == nullptr) {
            this->visit_subtract(node);
        } else {
            *((T*)retval) = this->visit_subtract(node);
        };
    }

    /**
     * Internal visitor function for `Subtract` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_subtract(Subtract &node, void *retval);

    /**
     * Internal visitor function for `TernaryCond` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_ternary_cond(TernaryCond &node, void *retval) {
        if (retval == nullptr) {
            this->visit_ternary_cond(node);
        } else {
            *((T*)retval) = this->visit_ternary_cond(node);
        };
    }

    /**
     * Internal visitor function for `TernaryCond` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_ternary_cond(TernaryCond &node, void *retval);

    /**
     * Internal visitor function for `UnaryOp` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_unary_op(UnaryOp &node, void *retval) {
        if (retval == nullptr) {
            this->visit_unary_op(node);
        } else {
            *((T*)retval) = this->visit_unary_op(node);
        };
    }

    /**
     * Internal visitor function for `UnaryOp` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_unary_op(UnaryOp &node, void *retval);

    /**
     * Internal visitor function for `Variables` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_variables(Variables &node, void *retval) {
        if (retval == nullptr) {
            this->visit_variables(node);
        } else {
            *((T*)retval) = this->visit_variables(node);
        };
    }

    /**
     * Internal visitor function for `Variables` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_variables(Variables &node, void *retval);

    /**
     * Internal visitor function for `Version` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_version(Version &node, void *retval) {
        if (retval == nullptr) {
            this->visit_version(node);
        } else {
            *((T*)retval) = this->visit_version(node);
        };
    }

    /**
     * Internal visitor function for `Version` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_version(Version &node, void *retval);

    /**
     * Internal visitor function for `WhileLoop` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_while_loop(WhileLoop &node, void *retval) {
        if (retval == nullptr) {
            this->visit_while_loop(node);
        } else {
            *((T*)retval) = this->visit_while_loop(node);
        };
    }

    /**
     * Internal visitor function for `WhileLoop` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_while_loop(WhileLoop &node, void *retval);

/**
 * Visitor base class defaulting to DFS pre-order traversal.
 * 
 * The visitor functions for nodes with subnode fields default to DFS traversal
 * in addition to falling back to more generic node types.Links and OptLinks are
 * *not* followed.
 */
class RecursiveVisitor : public Visitor<void> {
public:

    /**
     * Recursive traversal for `Add` nodes.
     */
    void visit_add(Add &node) override;

    /**
     * Recursive traversal for `Annotated` nodes.
     */
    void visit_annotated(Annotated &node) override;

    /**
     * Recursive traversal for `AnnotationData` nodes.
     */
    void visit_annotation_data(AnnotationData &node) override;

    /**
     * Recursive traversal for `ArithOp` nodes.
     */
    void visit_arith_op(ArithOp &node) override;

    /**
     * Recursive traversal for `Assignment` nodes.
     */
    void visit_assignment(Assignment &node) override;

    /**
     * Recursive traversal for `BinaryOp` nodes.
     */
    void visit_binary_op(BinaryOp &node) override;

    /**
     * Recursive traversal for `BitwiseAnd` nodes.
     */
    void visit_bitwise_and(BitwiseAnd &node) override;

    /**
     * Recursive traversal for `BitwiseNot` nodes.
     */
    void visit_bitwise_not(BitwiseNot &node) override;

    /**
     * Recursive traversal for `BitwiseOp` nodes.
     */
    void visit_bitwise_op(BitwiseOp &node) override;

    /**
     * Recursive traversal for `BitwiseOr` nodes.
     */
    void visit_bitwise_or(BitwiseOr &node) override;

    /**
     * Recursive traversal for `BitwiseXor` nodes.
     */
    void visit_bitwise_xor(BitwiseXor &node) override;

    /**
     * Recursive traversal for `BreakStatement` nodes.
     */
    void visit_break_statement(BreakStatement &node) override;

    /**
     * Recursive traversal for `Bundle` nodes.
     */
    void visit_bundle(Bundle &node) override;

    /**
     * Recursive traversal for `CmpEq` nodes.
     */
    void visit_cmp_eq(CmpEq &node) override;

    /**
     * Recursive traversal for `CmpGe` nodes.
     */
    void visit_cmp_ge(CmpGe &node) override;

    /**
     * Recursive traversal for `CmpGt` nodes.
     */
    void visit_cmp_gt(CmpGt &node) override;

    /**
     * Recursive traversal for `CmpLe` nodes.
     */
    void visit_cmp_le(CmpLe &node) override;

    /**
     * Recursive traversal for `CmpLt` nodes.
     */
    void visit_cmp_lt(CmpLt &node) override;

    /**
     * Recursive traversal for `CmpNe` nodes.
     */
    void visit_cmp_ne(CmpNe &node) override;

    /**
     * Recursive traversal for `CmpOp` nodes.
     */
    void visit_cmp_op(CmpOp &node) override;

    /**
     * Recursive traversal for `ContinueStatement` nodes.
     */
    void visit_continue_statement(ContinueStatement &node) override;

    /**
     * Recursive traversal for `Divide` nodes.
     */
    void visit_divide(Divide &node) override;

    /**
     * Recursive traversal for `ErroneousExpression` nodes.
     */
    void visit_erroneous_expression(ErroneousExpression &node) override;

    /**
     * Recursive traversal for `ErroneousProgram` nodes.
     */
    void visit_erroneous_program(ErroneousProgram &node) override;

    /**
     * Recursive traversal for `ErroneousStatement` nodes.
     */
    void visit_erroneous_statement(ErroneousStatement &node) override;

    /**
     * Recursive traversal for `Expression` nodes.
     */
    void visit_expression(Expression &node) override;

    /**
     * Recursive traversal for `ExpressionList` nodes.
     */
    void visit_expression_list(ExpressionList &node) override;

    /**
     * Recursive traversal for `FloatLiteral` nodes.
     */
    void visit_float_literal(FloatLiteral &node) override;

    /**
     * Recursive traversal for `ForLoop` nodes.
     */
    void visit_for_loop(ForLoop &node) override;

    /**
     * Recursive traversal for `ForeachLoop` nodes.
     */
    void visit_foreach_loop(ForeachLoop &node) override;

    /**
     * Recursive traversal for `FunctionCall` nodes.
     */
    void visit_function_call(FunctionCall &node) override;

    /**
     * Recursive traversal for `Identifier` nodes.
     */
    void visit_identifier(Identifier &node) override;

    /**
     * Recursive traversal for `IfElse` nodes.
     */
    void visit_if_else(IfElse &node) override;

    /**
     * Recursive traversal for `IfElseBranch` nodes.
     */
    void visit_if_else_branch(IfElseBranch &node) override;

    /**
     * Recursive traversal for `Index` nodes.
     */
    void visit_index(Index &node) override;

    /**
     * Recursive traversal for `IndexEntry` nodes.
     */
    void visit_index_entry(IndexEntry &node) override;

    /**
     * Recursive traversal for `IndexItem` nodes.
     */
    void visit_index_item(IndexItem &node) override;

    /**
     * Recursive traversal for `IndexList` nodes.
     */
    void visit_index_list(IndexList &node) override;

    /**
     * Recursive traversal for `IndexRange` nodes.
     */
    void visit_index_range(IndexRange &node) override;

    /**
     * Recursive traversal for `Instruction` nodes.
     */
    void visit_instruction(Instruction &node) override;

    /**
     * Recursive traversal for `IntDivide` nodes.
     */
    void visit_int_divide(IntDivide &node) override;

    /**
     * Recursive traversal for `IntegerLiteral` nodes.
     */
    void visit_integer_literal(IntegerLiteral &node) override;

    /**
     * Recursive traversal for `JsonLiteral` nodes.
     */
    void visit_json_literal(JsonLiteral &node) override;

    /**
     * Recursive traversal for `LogicalAnd` nodes.
     */
    void visit_logical_and(LogicalAnd &node) override;

    /**
     * Recursive traversal for `LogicalNot` nodes.
     */
    void visit_logical_not(LogicalNot &node) override;

    /**
     * Recursive traversal for `LogicalOp` nodes.
     */
    void visit_logical_op(LogicalOp &node) override;

    /**
     * Recursive traversal for `LogicalOr` nodes.
     */
    void visit_logical_or(LogicalOr &node) override;

    /**
     * Recursive traversal for `LogicalXor` nodes.
     */
    void visit_logical_xor(LogicalXor &node) override;

    /**
     * Recursive traversal for `Mapping` nodes.
     */
    void visit_mapping(Mapping &node) override;

    /**
     * Recursive traversal for `MatrixLiteral` nodes.
     */
    void visit_matrix_literal(MatrixLiteral &node) override;

    /**
     * Recursive traversal for `Modulo` nodes.
     */
    void visit_modulo(Modulo &node) override;

    /**
     * Recursive traversal for `Multiply` nodes.
     */
    void visit_multiply(Multiply &node) override;

    /**
     * Recursive traversal for `Negate` nodes.
     */
    void visit_negate(Negate &node) override;

    /**
     * Recursive traversal for `Power` nodes.
     */
    void visit_power(Power &node) override;

    /**
     * Recursive traversal for `Program` nodes.
     */
    void visit_program(Program &node) override;

    /**
     * Recursive traversal for `RepeatUntilLoop` nodes.
     */
    void visit_repeat_until_loop(RepeatUntilLoop &node) override;

    /**
     * Recursive traversal for `Root` nodes.
     */
    void visit_root(Root &node) override;

    /**
     * Recursive traversal for `ShiftLeft` nodes.
     */
    void visit_shift_left(ShiftLeft &node) override;

    /**
     * Recursive traversal for `ShiftOp` nodes.
     */
    void visit_shift_op(ShiftOp &node) override;

    /**
     * Recursive traversal for `ShiftRightArith` nodes.
     */
    void visit_shift_right_arith(ShiftRightArith &node) override;

    /**
     * Recursive traversal for `ShiftRightLogic` nodes.
     */
    void visit_shift_right_logic(ShiftRightLogic &node) override;

    /**
     * Recursive traversal for `Statement` nodes.
     */
    void visit_statement(Statement &node) override;

    /**
     * Recursive traversal for `StatementList` nodes.
     */
    void visit_statement_list(StatementList &node) override;

    /**
     * Recursive traversal for `StringLiteral` nodes.
     */
    void visit_string_literal(StringLiteral &node) override;

    /**
     * Recursive traversal for `Structured` nodes.
     */
    void visit_structured(Structured &node) override;

    /**
     * Recursive traversal for `Subcircuit` nodes.
     */
    void visit_subcircuit(Subcircuit &node) override;

    /**
     * Recursive traversal for `Subtract` nodes.
     */
    void visit_subtract(Subtract &node) override;

    /**
     * Recursive traversal for `TernaryCond` nodes.
     */
    void visit_ternary_cond(TernaryCond &node) override;

    /**
     * Recursive traversal for `UnaryOp` nodes.
     */
    void visit_unary_op(UnaryOp &node) override;

    /**
     * Recursive traversal for `Variables` nodes.
     */
    void visit_variables(Variables &node) override;

    /**
     * Recursive traversal for `Version` nodes.
     */
    void visit_version(Version &node) override;

    /**
     * Recursive traversal for `WhileLoop` nodes.
     */
    void visit_while_loop(WhileLoop &node) override;

};

/**
 * Visitor class that debug-dumps a tree to a stream
 */
class Dumper : public RecursiveVisitor {
protected:

    /**
     * Output stream to dump to.
     */
    std::ostream &out;

    /**
     * Current indentation level.
     */
    int indent = 0;

    /**
     * When non-null, the print node IDs from here instead of link contents.
     */
    ::tree::base::PointerMap *ids;
    /**
     * Whether we're printing the contents of a link.
     */
    bool in_link = false;

    /**
     * Writes the current indentation level's worth of spaces.
     */
    void write_indent();

public:

    /**
     * Construct a dumping visitor.
     */
    Dumper(std::ostream &out, int indent=0, ::tree::base::PointerMap *ids = nullptr) : out(out), indent(indent), ids(ids) {};

    /**
     * Dumps a `Node`.
     */
    void visit_node(Node &node) override;
    /**
     * Dumps a `Add` node.
     */
    void visit_add(Add &node) override;

    /**
     * Dumps a `Annotated` node.
     */
    void visit_annotated(Annotated &node) override;

    /**
     * Dumps a `AnnotationData` node.
     */
    void visit_annotation_data(AnnotationData &node) override;

    /**
     * Dumps a `ArithOp` node.
     */
    void visit_arith_op(ArithOp &node) override;

    /**
     * Dumps a `Assignment` node.
     */
    void visit_assignment(Assignment &node) override;

    /**
     * Dumps a `BinaryOp` node.
     */
    void visit_binary_op(BinaryOp &node) override;

    /**
     * Dumps a `BitwiseAnd` node.
     */
    void visit_bitwise_and(BitwiseAnd &node) override;

    /**
     * Dumps a `BitwiseNot` node.
     */
    void visit_bitwise_not(BitwiseNot &node) override;

    /**
     * Dumps a `BitwiseOp` node.
     */
    void visit_bitwise_op(BitwiseOp &node) override;

    /**
     * Dumps a `BitwiseOr` node.
     */
    void visit_bitwise_or(BitwiseOr &node) override;

    /**
     * Dumps a `BitwiseXor` node.
     */
    void visit_bitwise_xor(BitwiseXor &node) override;

    /**
     * Dumps a `BreakStatement` node.
     */
    void visit_break_statement(BreakStatement &node) override;

    /**
     * Dumps a `Bundle` node.
     */
    void visit_bundle(Bundle &node) override;

    /**
     * Dumps a `CmpEq` node.
     */
    void visit_cmp_eq(CmpEq &node) override;

    /**
     * Dumps a `CmpGe` node.
     */
    void visit_cmp_ge(CmpGe &node) override;

    /**
     * Dumps a `CmpGt` node.
     */
    void visit_cmp_gt(CmpGt &node) override;

    /**
     * Dumps a `CmpLe` node.
     */
    void visit_cmp_le(CmpLe &node) override;

    /**
     * Dumps a `CmpLt` node.
     */
    void visit_cmp_lt(CmpLt &node) override;

    /**
     * Dumps a `CmpNe` node.
     */
    void visit_cmp_ne(CmpNe &node) override;

    /**
     * Dumps a `CmpOp` node.
     */
    void visit_cmp_op(CmpOp &node) override;

    /**
     * Dumps a `ContinueStatement` node.
     */
    void visit_continue_statement(ContinueStatement &node) override;

    /**
     * Dumps a `Divide` node.
     */
    void visit_divide(Divide &node) override;

    /**
     * Dumps a `ErroneousExpression` node.
     */
    void visit_erroneous_expression(ErroneousExpression &node) override;

    /**
     * Dumps a `ErroneousProgram` node.
     */
    void visit_erroneous_program(ErroneousProgram &node) override;

    /**
     * Dumps a `ErroneousStatement` node.
     */
    void visit_erroneous_statement(ErroneousStatement &node) override;

    /**
     * Dumps a `Expression` node.
     */
    void visit_expression(Expression &node) override;

    /**
     * Dumps a `ExpressionList` node.
     */
    void visit_expression_list(ExpressionList &node) override;

    /**
     * Dumps a `FloatLiteral` node.
     */
    void visit_float_literal(FloatLiteral &node) override;

    /**
     * Dumps a `ForLoop` node.
     */
    void visit_for_loop(ForLoop &node) override;

    /**
     * Dumps a `ForeachLoop` node.
     */
    void visit_foreach_loop(ForeachLoop &node) override;

    /**
     * Dumps a `FunctionCall` node.
     */
    void visit_function_call(FunctionCall &node) override;

    /**
     * Dumps a `Identifier` node.
     */
    void visit_identifier(Identifier &node) override;

    /**
     * Dumps a `IfElse` node.
     */
    void visit_if_else(IfElse &node) override;

    /**
     * Dumps a `IfElseBranch` node.
     */
    void visit_if_else_branch(IfElseBranch &node) override;

    /**
     * Dumps a `Index` node.
     */
    void visit_index(Index &node) override;

    /**
     * Dumps a `IndexEntry` node.
     */
    void visit_index_entry(IndexEntry &node) override;

    /**
     * Dumps a `IndexItem` node.
     */
    void visit_index_item(IndexItem &node) override;

    /**
     * Dumps a `IndexList` node.
     */
    void visit_index_list(IndexList &node) override;

    /**
     * Dumps a `IndexRange` node.
     */
    void visit_index_range(IndexRange &node) override;

    /**
     * Dumps a `Instruction` node.
     */
    void visit_instruction(Instruction &node) override;

    /**
     * Dumps a `IntDivide` node.
     */
    void visit_int_divide(IntDivide &node) override;

    /**
     * Dumps a `IntegerLiteral` node.
     */
    void visit_integer_literal(IntegerLiteral &node) override;

    /**
     * Dumps a `JsonLiteral` node.
     */
    void visit_json_literal(JsonLiteral &node) override;

    /**
     * Dumps a `LogicalAnd` node.
     */
    void visit_logical_and(LogicalAnd &node) override;

    /**
     * Dumps a `LogicalNot` node.
     */
    void visit_logical_not(LogicalNot &node) override;

    /**
     * Dumps a `LogicalOp` node.
     */
    void visit_logical_op(LogicalOp &node) override;

    /**
     * Dumps a `LogicalOr` node.
     */
    void visit_logical_or(LogicalOr &node) override;

    /**
     * Dumps a `LogicalXor` node.
     */
    void visit_logical_xor(LogicalXor &node) override;

    /**
     * Dumps a `Mapping` node.
     */
    void visit_mapping(Mapping &node) override;

    /**
     * Dumps a `MatrixLiteral` node.
     */
    void visit_matrix_literal(MatrixLiteral &node) override;

    /**
     * Dumps a `Modulo` node.
     */
    void visit_modulo(Modulo &node) override;

    /**
     * Dumps a `Multiply` node.
     */
    void visit_multiply(Multiply &node) override;

    /**
     * Dumps a `Negate` node.
     */
    void visit_negate(Negate &node) override;

    /**
     * Dumps a `Power` node.
     */
    void visit_power(Power &node) override;

    /**
     * Dumps a `Program` node.
     */
    void visit_program(Program &node) override;

    /**
     * Dumps a `RepeatUntilLoop` node.
     */
    void visit_repeat_until_loop(RepeatUntilLoop &node) override;

    /**
     * Dumps a `Root` node.
     */
    void visit_root(Root &node) override;

    /**
     * Dumps a `ShiftLeft` node.
     */
    void visit_shift_left(ShiftLeft &node) override;

    /**
     * Dumps a `ShiftOp` node.
     */
    void visit_shift_op(ShiftOp &node) override;

    /**
     * Dumps a `ShiftRightArith` node.
     */
    void visit_shift_right_arith(ShiftRightArith &node) override;

    /**
     * Dumps a `ShiftRightLogic` node.
     */
    void visit_shift_right_logic(ShiftRightLogic &node) override;

    /**
     * Dumps a `Statement` node.
     */
    void visit_statement(Statement &node) override;

    /**
     * Dumps a `StatementList` node.
     */
    void visit_statement_list(StatementList &node) override;

    /**
     * Dumps a `StringLiteral` node.
     */
    void visit_string_literal(StringLiteral &node) override;

    /**
     * Dumps a `Structured` node.
     */
    void visit_structured(Structured &node) override;

    /**
     * Dumps a `Subcircuit` node.
     */
    void visit_subcircuit(Subcircuit &node) override;

    /**
     * Dumps a `Subtract` node.
     */
    void visit_subtract(Subtract &node) override;

    /**
     * Dumps a `TernaryCond` node.
     */
    void visit_ternary_cond(TernaryCond &node) override;

    /**
     * Dumps a `UnaryOp` node.
     */
    void visit_unary_op(UnaryOp &node) override;

    /**
     * Dumps a `Variables` node.
     */
    void visit_variables(Variables &node) override;

    /**
     * Dumps a `Version` node.
     */
    void visit_version(Version &node) override;

    /**
     * Dumps a `WhileLoop` node.
     */
    void visit_while_loop(WhileLoop &node) override;

};

/**
 * Visit this object.
 */
template <typename T>
T Node::visit(Visitor<T> &visitor) {
    T retval;
    this->visit_internal(visitor, &retval);
    return retval;
}

/**
 * Visit this object.
 */
template <>
void Node::visit(Visitor<void> &visitor);

/**
 * Stream << overload for tree nodes (writes debug dump).
 */
std::ostream &operator<<(std::ostream &os, const Node &object);

} // namespace ast
} // namespace v1
} // namespace cqasm

