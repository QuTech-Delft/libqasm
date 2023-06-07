/** \file
 * Header file for the semantic tree node classes.
 */

#pragma once

#include <iostream>
#include "cqasm-tree.hpp"
#include "v3/cqasm-primitives.hpp"
#include "v3/cqasm-values.hpp"
#include "v3/cqasm-error-model.hpp"
#include "v3/cqasm-instruction.hpp"

namespace cqasm {
namespace v3 {

/**
 * Namespace for the semantic tree node classes.
 */
/**
 * \dot
 * digraph example {
 *   node [shape=record, fontname=Helvetica, fontsize=10];
 *   Annotated [ label="Annotated" URL="\ref cqasm::v3::semantic::Annotated", style=dotted];
 *   AnnotationData [ label="AnnotationData" URL="\ref cqasm::v3::semantic::AnnotationData"];
 *   Block [ label="Block" URL="\ref cqasm::v3::semantic::Block"];
 *   BreakStatement [ label="BreakStatement" URL="\ref cqasm::v3::semantic::BreakStatement"];
 *   Bundle [ label="Bundle" URL="\ref cqasm::v3::semantic::Bundle"];
 *   BundleExt [ label="BundleExt" URL="\ref cqasm::v3::semantic::BundleExt"];
 *   ContinueStatement [ label="ContinueStatement" URL="\ref cqasm::v3::semantic::ContinueStatement"];
 *   ErrorModel [ label="ErrorModel" URL="\ref cqasm::v3::semantic::ErrorModel"];
 *   ForLoop [ label="ForLoop" URL="\ref cqasm::v3::semantic::ForLoop"];
 *   ForeachLoop [ label="ForeachLoop" URL="\ref cqasm::v3::semantic::ForeachLoop"];
 *   GotoInstruction [ label="GotoInstruction" URL="\ref cqasm::v3::semantic::GotoInstruction"];
 *   IfElse [ label="IfElse" URL="\ref cqasm::v3::semantic::IfElse"];
 *   IfElseBranch [ label="IfElseBranch" URL="\ref cqasm::v3::semantic::IfElseBranch"];
 *   Instruction [ label="Instruction" URL="\ref cqasm::v3::semantic::Instruction"];
 *   InstructionBase [ label="InstructionBase" URL="\ref cqasm::v3::semantic::InstructionBase", style=dotted];
 *   Mapping [ label="Mapping" URL="\ref cqasm::v3::semantic::Mapping"];
 *   Program [ label="Program" URL="\ref cqasm::v3::semantic::Program"];
 *   RepeatUntilLoop [ label="RepeatUntilLoop" URL="\ref cqasm::v3::semantic::RepeatUntilLoop"];
 *   SetInstruction [ label="SetInstruction" URL="\ref cqasm::v3::semantic::SetInstruction"];
 *   Statement [ label="Statement" URL="\ref cqasm::v3::semantic::Statement", style=dotted];
 *   Structured [ label="Structured" URL="\ref cqasm::v3::semantic::Structured", style=dotted];
 *   Subcircuit [ label="Subcircuit" URL="\ref cqasm::v3::semantic::Subcircuit"];
 *   Variable [ label="Variable" URL="\ref cqasm::v3::semantic::Variable"];
 *   Version [ label="Version" URL="\ref cqasm::v3::semantic::Version"];
 *   WhileLoop [ label="WhileLoop" URL="\ref cqasm::v3::semantic::WhileLoop"];
 *   Structured -> BreakStatement [ arrowhead=open, style=dotted ];
 *   Annotated -> Bundle [ arrowhead=open, style=dotted ];
 *   Statement -> BundleExt [ arrowhead=open, style=dotted ];
 *   Structured -> ContinueStatement [ arrowhead=open, style=dotted ];
 *   Annotated -> ErrorModel [ arrowhead=open, style=dotted ];
 *   Structured -> ForLoop [ arrowhead=open, style=dotted ];
 *   Structured -> ForeachLoop [ arrowhead=open, style=dotted ];
 *   InstructionBase -> GotoInstruction [ arrowhead=open, style=dotted ];
 *   Structured -> IfElse [ arrowhead=open, style=dotted ];
 *   InstructionBase -> Instruction [ arrowhead=open, style=dotted ];
 *   Annotated -> InstructionBase [ arrowhead=open, style=dotted ];
 *   Annotated -> Mapping [ arrowhead=open, style=dotted ];
 *   Structured -> RepeatUntilLoop [ arrowhead=open, style=dotted ];
 *   InstructionBase -> SetInstruction [ arrowhead=open, style=dotted ];
 *   Annotated -> Statement [ arrowhead=open, style=dotted ];
 *   Statement -> Structured [ arrowhead=open, style=dotted ];
 *   Annotated -> Subcircuit [ arrowhead=open, style=dotted ];
 *   Annotated -> Variable [ arrowhead=open, style=dotted ];
 *   Structured -> WhileLoop [ arrowhead=open, style=dotted ];
 *   Annotated -> AnnotationData [ label="annotations*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim0 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   AnnotationData -> prim0 [ label="interface", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim1 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   AnnotationData -> prim1 [ label="operation", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim2 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   AnnotationData -> prim2 [ label="operands*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   Block -> Statement [ label="statements*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   Bundle -> Instruction [ label="items+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   BundleExt -> InstructionBase [ label="items+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   prim3 [ label="error_model::ErrorModelRef" URL="\ref cqasm::v3::error_model::ErrorModelRef"];
 *   ErrorModel -> prim3 [ label="model", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim4 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   ErrorModel -> prim4 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim5 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   ErrorModel -> prim5 [ label="parameters*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   ForLoop -> SetInstruction [ label="initialize?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   prim6 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   ForLoop -> prim6 [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> SetInstruction [ label="update?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   ForLoop -> Block [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim7 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   ForeachLoop -> prim7 [ label="lhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim8 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   ForeachLoop -> prim8 [ label="frm", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim9 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   ForeachLoop -> prim9 [ label="to", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   ForeachLoop -> Block [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   GotoInstruction -> Subcircuit [ label="target@", arrowhead=normal, style=dashed, fontname=Helvetica, fontsize=10];
 *   IfElse -> IfElseBranch [ label="branches+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   IfElse -> Block [ label="otherwise?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   prim10 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   IfElseBranch -> prim10 [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   IfElseBranch -> Block [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim11 [ label="instruction::InstructionRef" URL="\ref cqasm::v3::instruction::InstructionRef"];
 *   Instruction -> prim11 [ label="instruction", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim12 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   Instruction -> prim12 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim13 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   Instruction -> prim13 [ label="operands*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim14 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   InstructionBase -> prim14 [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim15 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   Mapping -> prim15 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim16 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   Mapping -> prim16 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Program -> Version [ label="version", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim17 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   Program -> prim17 [ label="num_qubits", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Program -> ErrorModel [ label="error_model?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   Program -> Subcircuit [ label="subcircuits*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   Program -> Mapping [ label="mappings*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   Program -> Variable [ label="variables*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim18 [ label="primitives::Version" URL="\ref cqasm::v3::primitives::Version"];
 *   Program -> prim18 [ label="api_version", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   RepeatUntilLoop -> Block [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim19 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   RepeatUntilLoop -> prim19 [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim20 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   SetInstruction -> prim20 [ label="lhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim21 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   SetInstruction -> prim21 [ label="rhs", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim22 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   Subcircuit -> prim22 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim23 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   Subcircuit -> prim23 [ label="iterations", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   Subcircuit -> Bundle [ label="bundles*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   Subcircuit -> Block [ label="body?", arrowhead=open, style=solid, fontname=Helvetica, fontsize=10];
 *   prim24 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   Variable -> prim24 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim25 [ label="types::Node" URL="\ref cqasm::v3::types::Node"];
 *   Variable -> prim25 [ label="typ", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim26 [ label="primitives::Version" URL="\ref cqasm::v3::primitives::Version"];
 *   Version -> prim26 [ label="items", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim27 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   WhileLoop -> prim27 [ label="condition", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   WhileLoop -> Block [ label="body", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 * }
 * \enddot
 */
namespace semantic {

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
class Annotated;
class AnnotationData;
class Block;
class BreakStatement;
class Bundle;
class BundleExt;
class ContinueStatement;
class ErrorModel;
class ForLoop;
class ForeachLoop;
class GotoInstruction;
class IfElse;
class IfElseBranch;
class Instruction;
class InstructionBase;
class Mapping;
class Program;
class RepeatUntilLoop;
class SetInstruction;
class Statement;
class Structured;
class Subcircuit;
class Variable;
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
    AnnotationData,
    Block,
    BreakStatement,
    Bundle,
    BundleExt,
    ContinueStatement,
    ErrorModel,
    ForLoop,
    ForeachLoop,
    GotoInstruction,
    IfElse,
    IfElseBranch,
    Instruction,
    Mapping,
    Program,
    RepeatUntilLoop,
    SetInstruction,
    Subcircuit,
    Variable,
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
     * Interprets this node to a node of type Block. Returns null if it has the
     * wrong type.
     */
    virtual Block *as_block();

    /**
     * Interprets this node to a node of type Block. Returns null if it has the
     * wrong type.
     */
    virtual const Block *as_block() const;

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
     * Interprets this node to a node of type BundleExt. Returns null if it has
     * the wrong type.
     */
    virtual BundleExt *as_bundle_ext();

    /**
     * Interprets this node to a node of type BundleExt. Returns null if it has
     * the wrong type.
     */
    virtual const BundleExt *as_bundle_ext() const;

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
     * Interprets this node to a node of type ErrorModel. Returns null if it has
     * the wrong type.
     */
    virtual ErrorModel *as_error_model();

    /**
     * Interprets this node to a node of type ErrorModel. Returns null if it has
     * the wrong type.
     */
    virtual const ErrorModel *as_error_model() const;

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
     * Interprets this node to a node of type GotoInstruction. Returns null if
     * it has the wrong type.
     */
    virtual GotoInstruction *as_goto_instruction();

    /**
     * Interprets this node to a node of type GotoInstruction. Returns null if
     * it has the wrong type.
     */
    virtual const GotoInstruction *as_goto_instruction() const;

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
     * Interprets this node to a node of type InstructionBase. Returns null if
     * it has the wrong type.
     */
    virtual InstructionBase *as_instruction_base();

    /**
     * Interprets this node to a node of type InstructionBase. Returns null if
     * it has the wrong type.
     */
    virtual const InstructionBase *as_instruction_base() const;

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
     * Interprets this node to a node of type SetInstruction. Returns null if it
     * has the wrong type.
     */
    virtual SetInstruction *as_set_instruction();

    /**
     * Interprets this node to a node of type SetInstruction. Returns null if it
     * has the wrong type.
     */
    virtual const SetInstruction *as_set_instruction() const;

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
     * Interprets this node to a node of type Variable. Returns null if it has
     * the wrong type.
     */
    virtual Variable *as_variable();

    /**
     * Interprets this node to a node of type Variable. Returns null if it has
     * the wrong type.
     */
    virtual const Variable *as_variable() const;

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
    cqasm::v3::primitives::Str interface;

    /**
     * The operation within the interface that this annotation is intended for.
     * If a supports the corresponding interface but not the operation, it
     * should throw an error.
     */
    cqasm::v3::primitives::Str operation;

    /**
     * Any operands attached to the annotation.
     */
    Any<cqasm::v3::values::Node> operands;

    /**
     * Constructor.
     */
    AnnotationData(const cqasm::v3::primitives::Str &interface = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const cqasm::v3::primitives::Str &operation = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const Any<cqasm::v3::values::Node> &operands = cqasm::v3::primitives::initialize<Any<cqasm::v3::values::Node>>());

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
 * A block of statements. Only used for API level 1.2+.
 */
class Block : public Node {
public:

    /**
     * The statements contained by the block.
     */
    Any<Statement> statements;

    /**
     * Constructor.
     */
    Block(const Any<Statement> &statements = Any<Statement>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Block` is complete/fully defined.
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
     * Interprets this node to a node of type Block. Returns null if it has the
     * wrong type.
     */
    Block *as_block() override;

    /**
     * Interprets this node to a node of type Block. Returns null if it has the
     * wrong type.
     */
    const Block *as_block() const override;

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
    static std::shared_ptr<Block> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A statement. Unused before 1.2, as only bundles existed before then.
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
 * A bundle of instructions, to be executed in parallel. Only used for API level
 * 1.0 and 1.1; 1.2+ uses bundle_ext.
 */
class Bundle : public Annotated {
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
 * A bundle of instructions, to be executed in parallel. Only used for API level
 * 1.2+.
 */
class BundleExt : public Statement {
public:

    /**
     * The list of parallel instructions.
     */
    Many<InstructionBase> items;

    /**
     * Constructor.
     */
    BundleExt(const Many<InstructionBase> &items = Many<InstructionBase>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BundleExt` is complete/fully defined.
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
     * Interprets this node to a node of type BundleExt. Returns null if it has
     * the wrong type.
     */
    BundleExt *as_bundle_ext() override;

    /**
     * Interprets this node to a node of type BundleExt. Returns null if it has
     * the wrong type.
     */
    const BundleExt *as_bundle_ext() const override;

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
    static std::shared_ptr<BundleExt> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

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
 * Error model information.
 */
class ErrorModel : public Annotated {
public:

    /**
     * Error model type as registered through the API.
     */
    cqasm::v3::error_model::ErrorModelRef model;

    /**
     * Name as it appears in the cQASM file.
     */
    cqasm::v3::primitives::Str name;

    /**
     * Error model parameters.
     */
    Any<cqasm::v3::values::Node> parameters;

    /**
     * Constructor.
     */
    ErrorModel(const cqasm::v3::error_model::ErrorModelRef &model = cqasm::v3::primitives::initialize<cqasm::v3::error_model::ErrorModelRef>(), const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const Any<cqasm::v3::values::Node> &parameters = cqasm::v3::primitives::initialize<Any<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ErrorModel` is complete/fully defined.
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
     * Interprets this node to a node of type ErrorModel. Returns null if it has
     * the wrong type.
     */
    ErrorModel *as_error_model() override;

    /**
     * Interprets this node to a node of type ErrorModel. Returns null if it has
     * the wrong type.
     */
    const ErrorModel *as_error_model() const override;

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
    static std::shared_ptr<ErrorModel> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A C-style for loop.
 */
class ForLoop : public Structured {
public:

    /**
     * The optional initializing assignment, run before the loop starts.
     */
    Maybe<SetInstruction> initialize;

    /**
     * The condition for starting another iteration.
     */
    One<cqasm::v3::values::Node> condition;

    /**
     * The updating assignment, done at the end of the loop body and upon
     * continue.
     */
    Maybe<SetInstruction> update;

    /**
     * The loop body.
     */
    One<Block> body;

    /**
     * Constructor.
     */
    ForLoop(const Maybe<SetInstruction> &initialize = Maybe<SetInstruction>(), const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Maybe<SetInstruction> &update = Maybe<SetInstruction>(), const One<Block> &body = One<Block>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
    One<cqasm::v3::values::Node> lhs;

    /**
     * The first value.
     */
    cqasm::v3::primitives::Int frm;

    /**
     * The last value.
     */
    cqasm::v3::primitives::Int to;

    /**
     * The loop body.
     */
    One<Block> body;

    /**
     * Constructor.
     */
    ForeachLoop(const One<cqasm::v3::values::Node> &lhs = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const cqasm::v3::primitives::Int &frm = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const cqasm::v3::primitives::Int &to = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const One<Block> &body = One<Block>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
 * An instruction (a.k.a. gate).
 */
class InstructionBase : public Annotated {
public:

    /**
     * Condition (c- notation). When there is no condition, this is a constant
     * boolean set to true.
     */
    One<cqasm::v3::values::Node> condition;

    /**
     * Constructor.
     */
    InstructionBase(const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Interprets this node to a node of type InstructionBase. Returns null if
     * it has the wrong type.
     */
    InstructionBase *as_instruction_base() override;

    /**
     * Interprets this node to a node of type InstructionBase. Returns null if
     * it has the wrong type.
     */
    const InstructionBase *as_instruction_base() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<InstructionBase> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A version 1.2+ goto instruction.
 */
class GotoInstruction : public InstructionBase {
public:

    /**
     * Link to the target subcircuit, used as a label.
     */
    Link<Subcircuit> target;

    /**
     * Constructor.
     */
    GotoInstruction(const Link<Subcircuit> &target = Link<Subcircuit>(), const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `GotoInstruction` is complete/fully defined.
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
     * Interprets this node to a node of type GotoInstruction. Returns null if
     * it has the wrong type.
     */
    GotoInstruction *as_goto_instruction() override;

    /**
     * Interprets this node to a node of type GotoInstruction. Returns null if
     * it has the wrong type.
     */
    const GotoInstruction *as_goto_instruction() const override;

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
    static std::shared_ptr<GotoInstruction> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

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
    Maybe<Block> otherwise;

    /**
     * Constructor.
     */
    IfElse(const Many<IfElseBranch> &branches = Many<IfElseBranch>(), const Maybe<Block> &otherwise = Maybe<Block>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
    One<cqasm::v3::values::Node> condition;

    /**
     * The body.
     */
    One<Block> body;

    /**
     * Constructor.
     */
    IfElseBranch(const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const One<Block> &body = One<Block>());

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
 * A regular instruction. Until 1.2, this is the only type of instruction that
 * exists.
 */
class Instruction : public InstructionBase {
public:

    /**
     * Instruction type as registered through the API.
     */
    cqasm::v3::instruction::InstructionRef instruction;

    /**
     * Name as it appears in the cQASM file.
     */
    cqasm::v3::primitives::Str name;

    /**
     * Operands for the instruction.
     */
    Any<cqasm::v3::values::Node> operands;

    /**
     * Constructor.
     */
    Instruction(const cqasm::v3::instruction::InstructionRef &instruction = cqasm::v3::primitives::initialize<cqasm::v3::instruction::InstructionRef>(), const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<cqasm::v3::values::Node> &operands = cqasm::v3::primitives::initialize<Any<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
 * A mapping. That is, a user-defined identifier mapping to some value.
 */
class Mapping : public Annotated {
public:

    /**
     * The name of the mapping.
     */
    cqasm::v3::primitives::Str name;

    /**
     * The value it maps to.
     */
    One<cqasm::v3::values::Node> value;

    /**
     * Constructor.
     */
    Mapping(const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const One<cqasm::v3::values::Node> &value = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
 * A complete program.
 */
class Program : public Node {
public:

    /**
     * File version.
     */
    One<Version> version;

    /**
     * The required qubit register size.
     */
    cqasm::v3::primitives::Int num_qubits;

    /**
     * Error model information.
     */
    Maybe<ErrorModel> error_model;

    /**
     * The list of subcircuit.
     */
    Any<Subcircuit> subcircuits;

    /**
     * The list of all user-defined mappings after parsing.
     */
    Any<Mapping> mappings;

    /**
     * This list of all user-defined variables at any point in the code. Only
     * used for API level 1.1+.
     */
    Any<Variable> variables;

    /**
     * API version. This may be greater than or equal to the file version. This
     * controls which fields of the tree are used, where such usage depends on
     * the version.
     */
    cqasm::v3::primitives::Version api_version;

    /**
     * Constructor.
     */
    Program(const One<Version> &version = One<Version>(), const cqasm::v3::primitives::Int &num_qubits = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const Maybe<ErrorModel> &error_model = Maybe<ErrorModel>(), const Any<Subcircuit> &subcircuits = Any<Subcircuit>(), const Any<Mapping> &mappings = Any<Mapping>(), const Any<Variable> &variables = Any<Variable>(), const cqasm::v3::primitives::Version &api_version = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Version>());

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
    One<Block> body;

    /**
     * The condition for stopping iteration.
     */
    One<cqasm::v3::values::Node> condition;

    /**
     * Constructor.
     */
    RepeatUntilLoop(const One<Block> &body = One<Block>(), const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
 * A version 1.2+ assignment instruction.
 */
class SetInstruction : public InstructionBase {
public:

    /**
     * The assignment target.
     */
    One<cqasm::v3::values::Node> lhs;

    /**
     * The value to assign.
     */
    One<cqasm::v3::values::Node> rhs;

    /**
     * Constructor.
     */
    SetInstruction(const One<cqasm::v3::values::Node> &lhs = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const One<cqasm::v3::values::Node> &rhs = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `SetInstruction` is complete/fully defined.
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
     * Interprets this node to a node of type SetInstruction. Returns null if it
     * has the wrong type.
     */
    SetInstruction *as_set_instruction() override;

    /**
     * Interprets this node to a node of type SetInstruction. Returns null if it
     * has the wrong type.
     */
    const SetInstruction *as_set_instruction() const override;

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
    static std::shared_ptr<SetInstruction> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * A subcircuit. That is, a named collection of bundles, possibly repeated a
 * number of times.
 */
class Subcircuit : public Annotated {
public:

    /**
     * The name of the subcircuit. If the file doesn't start with a subcircuit
     * definition, this is empty for the first subcircuit.
     */
    cqasm::v3::primitives::Str name;

    /**
     * An optional integer expression representing the number of iterations for
     * this subcircuit. This is 1 when the iteration count is not specified.
     */
    cqasm::v3::primitives::Int iterations;

    /**
     * The instruction bundles contained by this subcircuit. Only used for API
     * level 1.0 and 1.1; use body for 1.2+.
     */
    Any<Bundle> bundles;

    /**
     * The statements contained by this subcircuit. Only used for API level
     * 1.2+; use bundles for 1.0 and 1.1.
     */
    Maybe<Block> body;

    /**
     * Constructor.
     */
    Subcircuit(const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const cqasm::v3::primitives::Int &iterations = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const Any<Bundle> &bundles = Any<Bundle>(), const Any<AnnotationData> &annotations = Any<AnnotationData>(), const Maybe<Block> &body = Maybe<Block>());

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
 * A variable.
 */
class Variable : public Annotated {
public:

    /**
     * The name of the variable.
     */
    cqasm::v3::primitives::Str name;

    /**
     * The type of the variable.
     */
    One<cqasm::v3::types::Node> typ;

    /**
     * Constructor.
     */
    Variable(const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const One<cqasm::v3::types::Node> &typ = cqasm::v3::primitives::initialize<One<cqasm::v3::types::Node>>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Variable` is complete/fully defined.
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
     * Interprets this node to a node of type Variable. Returns null if it has
     * the wrong type.
     */
    Variable *as_variable() override;

    /**
     * Interprets this node to a node of type Variable. Returns null if it has
     * the wrong type.
     */
    const Variable *as_variable() const override;

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
    static std::shared_ptr<Variable> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * The file version identifier.
 */
class Version : public Node {
public:

    /**
     * The list of version components, ordered major to minor.
     */
    cqasm::v3::primitives::Version items;

    /**
     * Constructor.
     */
    Version(const cqasm::v3::primitives::Version &items = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Version>());

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
    One<cqasm::v3::values::Node> condition;

    /**
     * The loop body.
     */
    One<Block> body;

    /**
     * Constructor.
     */
    WhileLoop(const One<cqasm::v3::values::Node> &condition = cqasm::v3::primitives::initialize<One<cqasm::v3::values::Node>>(), const One<Block> &body = One<Block>(), const Any<AnnotationData> &annotations = Any<AnnotationData>());

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
    friend class Annotated;
    friend class AnnotationData;
    friend class Block;
    friend class BreakStatement;
    friend class Bundle;
    friend class BundleExt;
    friend class ContinueStatement;
    friend class ErrorModel;
    friend class ForLoop;
    friend class ForeachLoop;
    friend class GotoInstruction;
    friend class IfElse;
    friend class IfElseBranch;
    friend class Instruction;
    friend class InstructionBase;
    friend class Mapping;
    friend class Program;
    friend class RepeatUntilLoop;
    friend class SetInstruction;
    friend class Statement;
    friend class Structured;
    friend class Subcircuit;
    friend class Variable;
    friend class Version;
    friend class WhileLoop;

    /**
     * Internal visitor function for nodes of any type.
     */
    virtual void raw_visit_node(Node &node, void *retval) = 0;

    /**
     * Internal visitor function for `Annotated` nodes.
     */
    virtual void raw_visit_annotated(Annotated &node, void *retval) = 0;

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    virtual void raw_visit_annotation_data(AnnotationData &node, void *retval) = 0;

    /**
     * Internal visitor function for `Block` nodes.
     */
    virtual void raw_visit_block(Block &node, void *retval) = 0;

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    virtual void raw_visit_break_statement(BreakStatement &node, void *retval) = 0;

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    virtual void raw_visit_bundle(Bundle &node, void *retval) = 0;

    /**
     * Internal visitor function for `BundleExt` nodes.
     */
    virtual void raw_visit_bundle_ext(BundleExt &node, void *retval) = 0;

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    virtual void raw_visit_continue_statement(ContinueStatement &node, void *retval) = 0;

    /**
     * Internal visitor function for `ErrorModel` nodes.
     */
    virtual void raw_visit_error_model(ErrorModel &node, void *retval) = 0;

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    virtual void raw_visit_for_loop(ForLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    virtual void raw_visit_foreach_loop(ForeachLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `GotoInstruction` nodes.
     */
    virtual void raw_visit_goto_instruction(GotoInstruction &node, void *retval) = 0;

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    virtual void raw_visit_if_else(IfElse &node, void *retval) = 0;

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    virtual void raw_visit_if_else_branch(IfElseBranch &node, void *retval) = 0;

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    virtual void raw_visit_instruction(Instruction &node, void *retval) = 0;

    /**
     * Internal visitor function for `InstructionBase` nodes.
     */
    virtual void raw_visit_instruction_base(InstructionBase &node, void *retval) = 0;

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    virtual void raw_visit_mapping(Mapping &node, void *retval) = 0;

    /**
     * Internal visitor function for `Program` nodes.
     */
    virtual void raw_visit_program(Program &node, void *retval) = 0;

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    virtual void raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) = 0;

    /**
     * Internal visitor function for `SetInstruction` nodes.
     */
    virtual void raw_visit_set_instruction(SetInstruction &node, void *retval) = 0;

    /**
     * Internal visitor function for `Statement` nodes.
     */
    virtual void raw_visit_statement(Statement &node, void *retval) = 0;

    /**
     * Internal visitor function for `Structured` nodes.
     */
    virtual void raw_visit_structured(Structured &node, void *retval) = 0;

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    virtual void raw_visit_subcircuit(Subcircuit &node, void *retval) = 0;

    /**
     * Internal visitor function for `Variable` nodes.
     */
    virtual void raw_visit_variable(Variable &node, void *retval) = 0;

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
     * Internal visitor function for `Annotated` nodes.
     */
    void raw_visit_annotated(Annotated &node, void *retval) override;

    /**
     * Internal visitor function for `AnnotationData` nodes.
     */
    void raw_visit_annotation_data(AnnotationData &node, void *retval) override;

    /**
     * Internal visitor function for `Block` nodes.
     */
    void raw_visit_block(Block &node, void *retval) override;

    /**
     * Internal visitor function for `BreakStatement` nodes.
     */
    void raw_visit_break_statement(BreakStatement &node, void *retval) override;

    /**
     * Internal visitor function for `Bundle` nodes.
     */
    void raw_visit_bundle(Bundle &node, void *retval) override;

    /**
     * Internal visitor function for `BundleExt` nodes.
     */
    void raw_visit_bundle_ext(BundleExt &node, void *retval) override;

    /**
     * Internal visitor function for `ContinueStatement` nodes.
     */
    void raw_visit_continue_statement(ContinueStatement &node, void *retval) override;

    /**
     * Internal visitor function for `ErrorModel` nodes.
     */
    void raw_visit_error_model(ErrorModel &node, void *retval) override;

    /**
     * Internal visitor function for `ForLoop` nodes.
     */
    void raw_visit_for_loop(ForLoop &node, void *retval) override;

    /**
     * Internal visitor function for `ForeachLoop` nodes.
     */
    void raw_visit_foreach_loop(ForeachLoop &node, void *retval) override;

    /**
     * Internal visitor function for `GotoInstruction` nodes.
     */
    void raw_visit_goto_instruction(GotoInstruction &node, void *retval) override;

    /**
     * Internal visitor function for `IfElse` nodes.
     */
    void raw_visit_if_else(IfElse &node, void *retval) override;

    /**
     * Internal visitor function for `IfElseBranch` nodes.
     */
    void raw_visit_if_else_branch(IfElseBranch &node, void *retval) override;

    /**
     * Internal visitor function for `Instruction` nodes.
     */
    void raw_visit_instruction(Instruction &node, void *retval) override;

    /**
     * Internal visitor function for `InstructionBase` nodes.
     */
    void raw_visit_instruction_base(InstructionBase &node, void *retval) override;

    /**
     * Internal visitor function for `Mapping` nodes.
     */
    void raw_visit_mapping(Mapping &node, void *retval) override;

    /**
     * Internal visitor function for `Program` nodes.
     */
    void raw_visit_program(Program &node, void *retval) override;

    /**
     * Internal visitor function for `RepeatUntilLoop` nodes.
     */
    void raw_visit_repeat_until_loop(RepeatUntilLoop &node, void *retval) override;

    /**
     * Internal visitor function for `SetInstruction` nodes.
     */
    void raw_visit_set_instruction(SetInstruction &node, void *retval) override;

    /**
     * Internal visitor function for `Statement` nodes.
     */
    void raw_visit_statement(Statement &node, void *retval) override;

    /**
     * Internal visitor function for `Structured` nodes.
     */
    void raw_visit_structured(Structured &node, void *retval) override;

    /**
     * Internal visitor function for `Subcircuit` nodes.
     */
    void raw_visit_subcircuit(Subcircuit &node, void *retval) override;

    /**
     * Internal visitor function for `Variable` nodes.
     */
    void raw_visit_variable(Variable &node, void *retval) override;

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
     * Visitor function for `Block` nodes.
     */
    virtual T visit_block(Block &node) {
        return visit_node(node);
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
        return visit_annotated(node);
    }

    /**
     * Visitor function for `BundleExt` nodes.
     */
    virtual T visit_bundle_ext(BundleExt &node) {
        return visit_statement(node);
    }

    /**
     * Visitor function for `ContinueStatement` nodes.
     */
    virtual T visit_continue_statement(ContinueStatement &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `ErrorModel` nodes.
     */
    virtual T visit_error_model(ErrorModel &node) {
        return visit_annotated(node);
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
     * Visitor function for `GotoInstruction` nodes.
     */
    virtual T visit_goto_instruction(GotoInstruction &node) {
        return visit_instruction_base(node);
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
     * Visitor function for `Instruction` nodes.
     */
    virtual T visit_instruction(Instruction &node) {
        return visit_instruction_base(node);
    }

    /**
     * Fallback function for `InstructionBase` nodes.
     */
    virtual T visit_instruction_base(InstructionBase &node) {
        return visit_annotated(node);
    }

    /**
     * Visitor function for `Mapping` nodes.
     */
    virtual T visit_mapping(Mapping &node) {
        return visit_annotated(node);
    }

    /**
     * Visitor function for `Program` nodes.
     */
    virtual T visit_program(Program &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `RepeatUntilLoop` nodes.
     */
    virtual T visit_repeat_until_loop(RepeatUntilLoop &node) {
        return visit_structured(node);
    }

    /**
     * Visitor function for `SetInstruction` nodes.
     */
    virtual T visit_set_instruction(SetInstruction &node) {
        return visit_instruction_base(node);
    }

    /**
     * Fallback function for `Statement` nodes.
     */
    virtual T visit_statement(Statement &node) {
        return visit_annotated(node);
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
        return visit_annotated(node);
    }

    /**
     * Visitor function for `Variable` nodes.
     */
    virtual T visit_variable(Variable &node) {
        return visit_annotated(node);
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
     * Internal visitor function for `Block` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_block(Block &node, void *retval) {
        if (retval == nullptr) {
            this->visit_block(node);
        } else {
            *((T*)retval) = this->visit_block(node);
        };
    }

    /**
     * Internal visitor function for `Block` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_block(Block &node, void *retval);

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
     * Internal visitor function for `BundleExt` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bundle_ext(BundleExt &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bundle_ext(node);
        } else {
            *((T*)retval) = this->visit_bundle_ext(node);
        };
    }

    /**
     * Internal visitor function for `BundleExt` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bundle_ext(BundleExt &node, void *retval);

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
     * Internal visitor function for `ErrorModel` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_error_model(ErrorModel &node, void *retval) {
        if (retval == nullptr) {
            this->visit_error_model(node);
        } else {
            *((T*)retval) = this->visit_error_model(node);
        };
    }

    /**
     * Internal visitor function for `ErrorModel` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_error_model(ErrorModel &node, void *retval);

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
     * Internal visitor function for `GotoInstruction` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_goto_instruction(GotoInstruction &node, void *retval) {
        if (retval == nullptr) {
            this->visit_goto_instruction(node);
        } else {
            *((T*)retval) = this->visit_goto_instruction(node);
        };
    }

    /**
     * Internal visitor function for `GotoInstruction` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_goto_instruction(GotoInstruction &node, void *retval);

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
     * Internal visitor function for `InstructionBase` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_instruction_base(InstructionBase &node, void *retval) {
        if (retval == nullptr) {
            this->visit_instruction_base(node);
        } else {
            *((T*)retval) = this->visit_instruction_base(node);
        };
    }

    /**
     * Internal visitor function for `InstructionBase` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_instruction_base(InstructionBase &node, void *retval);

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
     * Internal visitor function for `SetInstruction` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_set_instruction(SetInstruction &node, void *retval) {
        if (retval == nullptr) {
            this->visit_set_instruction(node);
        } else {
            *((T*)retval) = this->visit_set_instruction(node);
        };
    }

    /**
     * Internal visitor function for `SetInstruction` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_set_instruction(SetInstruction &node, void *retval);

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
     * Internal visitor function for `Variable` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_variable(Variable &node, void *retval) {
        if (retval == nullptr) {
            this->visit_variable(node);
        } else {
            *((T*)retval) = this->visit_variable(node);
        };
    }

    /**
     * Internal visitor function for `Variable` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_variable(Variable &node, void *retval);

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
     * Recursive traversal for `Annotated` nodes.
     */
    void visit_annotated(Annotated &node) override;

    /**
     * Recursive traversal for `AnnotationData` nodes.
     */
    void visit_annotation_data(AnnotationData &node) override;

    /**
     * Recursive traversal for `Block` nodes.
     */
    void visit_block(Block &node) override;

    /**
     * Recursive traversal for `BreakStatement` nodes.
     */
    void visit_break_statement(BreakStatement &node) override;

    /**
     * Recursive traversal for `Bundle` nodes.
     */
    void visit_bundle(Bundle &node) override;

    /**
     * Recursive traversal for `BundleExt` nodes.
     */
    void visit_bundle_ext(BundleExt &node) override;

    /**
     * Recursive traversal for `ContinueStatement` nodes.
     */
    void visit_continue_statement(ContinueStatement &node) override;

    /**
     * Recursive traversal for `ErrorModel` nodes.
     */
    void visit_error_model(ErrorModel &node) override;

    /**
     * Recursive traversal for `ForLoop` nodes.
     */
    void visit_for_loop(ForLoop &node) override;

    /**
     * Recursive traversal for `ForeachLoop` nodes.
     */
    void visit_foreach_loop(ForeachLoop &node) override;

    /**
     * Recursive traversal for `GotoInstruction` nodes.
     */
    void visit_goto_instruction(GotoInstruction &node) override;

    /**
     * Recursive traversal for `IfElse` nodes.
     */
    void visit_if_else(IfElse &node) override;

    /**
     * Recursive traversal for `IfElseBranch` nodes.
     */
    void visit_if_else_branch(IfElseBranch &node) override;

    /**
     * Recursive traversal for `Instruction` nodes.
     */
    void visit_instruction(Instruction &node) override;

    /**
     * Recursive traversal for `InstructionBase` nodes.
     */
    void visit_instruction_base(InstructionBase &node) override;

    /**
     * Recursive traversal for `Mapping` nodes.
     */
    void visit_mapping(Mapping &node) override;

    /**
     * Recursive traversal for `Program` nodes.
     */
    void visit_program(Program &node) override;

    /**
     * Recursive traversal for `RepeatUntilLoop` nodes.
     */
    void visit_repeat_until_loop(RepeatUntilLoop &node) override;

    /**
     * Recursive traversal for `SetInstruction` nodes.
     */
    void visit_set_instruction(SetInstruction &node) override;

    /**
     * Recursive traversal for `Statement` nodes.
     */
    void visit_statement(Statement &node) override;

    /**
     * Recursive traversal for `Structured` nodes.
     */
    void visit_structured(Structured &node) override;

    /**
     * Recursive traversal for `Subcircuit` nodes.
     */
    void visit_subcircuit(Subcircuit &node) override;

    /**
     * Recursive traversal for `Variable` nodes.
     */
    void visit_variable(Variable &node) override;

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
     * Dumps a `Annotated` node.
     */
    void visit_annotated(Annotated &node) override;

    /**
     * Dumps a `AnnotationData` node.
     */
    void visit_annotation_data(AnnotationData &node) override;

    /**
     * Dumps a `Block` node.
     */
    void visit_block(Block &node) override;

    /**
     * Dumps a `BreakStatement` node.
     */
    void visit_break_statement(BreakStatement &node) override;

    /**
     * Dumps a `Bundle` node.
     */
    void visit_bundle(Bundle &node) override;

    /**
     * Dumps a `BundleExt` node.
     */
    void visit_bundle_ext(BundleExt &node) override;

    /**
     * Dumps a `ContinueStatement` node.
     */
    void visit_continue_statement(ContinueStatement &node) override;

    /**
     * Dumps a `ErrorModel` node.
     */
    void visit_error_model(ErrorModel &node) override;

    /**
     * Dumps a `ForLoop` node.
     */
    void visit_for_loop(ForLoop &node) override;

    /**
     * Dumps a `ForeachLoop` node.
     */
    void visit_foreach_loop(ForeachLoop &node) override;

    /**
     * Dumps a `GotoInstruction` node.
     */
    void visit_goto_instruction(GotoInstruction &node) override;

    /**
     * Dumps a `IfElse` node.
     */
    void visit_if_else(IfElse &node) override;

    /**
     * Dumps a `IfElseBranch` node.
     */
    void visit_if_else_branch(IfElseBranch &node) override;

    /**
     * Dumps a `Instruction` node.
     */
    void visit_instruction(Instruction &node) override;

    /**
     * Dumps a `InstructionBase` node.
     */
    void visit_instruction_base(InstructionBase &node) override;

    /**
     * Dumps a `Mapping` node.
     */
    void visit_mapping(Mapping &node) override;

    /**
     * Dumps a `Program` node.
     */
    void visit_program(Program &node) override;

    /**
     * Dumps a `RepeatUntilLoop` node.
     */
    void visit_repeat_until_loop(RepeatUntilLoop &node) override;

    /**
     * Dumps a `SetInstruction` node.
     */
    void visit_set_instruction(SetInstruction &node) override;

    /**
     * Dumps a `Statement` node.
     */
    void visit_statement(Statement &node) override;

    /**
     * Dumps a `Structured` node.
     */
    void visit_structured(Structured &node) override;

    /**
     * Dumps a `Subcircuit` node.
     */
    void visit_subcircuit(Subcircuit &node) override;

    /**
     * Dumps a `Variable` node.
     */
    void visit_variable(Variable &node) override;

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

} // namespace semantic
} // namespace v3
} // namespace cqasm

