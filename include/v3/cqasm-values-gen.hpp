/** \file
 * Header file for the various classes representing constants, references, and
 * dynamic expressions in cQASM.
 */

#pragma once

#include <iostream>
#include "cqasm-tree.hpp"
#include "v3/cqasm-primitives.hpp"
#include "v3/cqasm-types.hpp"

namespace cqasm {
namespace v3 {

/**
 * Namespace for the various classes representing constants, references, and
 * dynamic expressions in cQASM.
 */
/**
 * \dot
 * digraph example {
 *   node [shape=record, fontname=Helvetica, fontsize=10];
 *   BitRefs [ label="BitRefs" URL="\ref cqasm::v3::values::BitRefs"];
 *   ConstAxis [ label="ConstAxis" URL="\ref cqasm::v3::values::ConstAxis"];
 *   ConstBool [ label="ConstBool" URL="\ref cqasm::v3::values::ConstBool"];
 *   ConstComplex [ label="ConstComplex" URL="\ref cqasm::v3::values::ConstComplex"];
 *   ConstComplexMatrix [ label="ConstComplexMatrix" URL="\ref cqasm::v3::values::ConstComplexMatrix"];
 *   ConstInt [ label="ConstInt" URL="\ref cqasm::v3::values::ConstInt"];
 *   ConstJson [ label="ConstJson" URL="\ref cqasm::v3::values::ConstJson"];
 *   ConstReal [ label="ConstReal" URL="\ref cqasm::v3::values::ConstReal"];
 *   ConstRealMatrix [ label="ConstRealMatrix" URL="\ref cqasm::v3::values::ConstRealMatrix"];
 *   ConstString [ label="ConstString" URL="\ref cqasm::v3::values::ConstString"];
 *   Constant [ label="Constant" URL="\ref cqasm::v3::values::Constant", style=dotted];
 *   Function [ label="Function" URL="\ref cqasm::v3::values::Function"];
 *   QubitRefs [ label="QubitRefs" URL="\ref cqasm::v3::values::QubitRefs"];
 *   Reference [ label="Reference" URL="\ref cqasm::v3::values::Reference", style=dotted];
 *   VariableRef [ label="VariableRef" URL="\ref cqasm::v3::values::VariableRef"];
 *   Reference -> BitRefs [ arrowhead=open, style=dotted ];
 *   Constant -> ConstAxis [ arrowhead=open, style=dotted ];
 *   Constant -> ConstBool [ arrowhead=open, style=dotted ];
 *   Constant -> ConstComplex [ arrowhead=open, style=dotted ];
 *   Constant -> ConstComplexMatrix [ arrowhead=open, style=dotted ];
 *   Constant -> ConstInt [ arrowhead=open, style=dotted ];
 *   Constant -> ConstJson [ arrowhead=open, style=dotted ];
 *   Constant -> ConstReal [ arrowhead=open, style=dotted ];
 *   Constant -> ConstRealMatrix [ arrowhead=open, style=dotted ];
 *   Constant -> ConstString [ arrowhead=open, style=dotted ];
 *   Reference -> QubitRefs [ arrowhead=open, style=dotted ];
 *   Reference -> VariableRef [ arrowhead=open, style=dotted ];
 *   BitRefs -> ConstInt [ label="index+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   prim0 [ label="primitives::Axis" URL="\ref cqasm::v3::primitives::Axis"];
 *   ConstAxis -> prim0 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim1 [ label="primitives::Bool" URL="\ref cqasm::v3::primitives::Bool"];
 *   ConstBool -> prim1 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim2 [ label="primitives::Complex" URL="\ref cqasm::v3::primitives::Complex"];
 *   ConstComplex -> prim2 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim3 [ label="primitives::CMatrix" URL="\ref cqasm::v3::primitives::CMatrix"];
 *   ConstComplexMatrix -> prim3 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim4 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   ConstInt -> prim4 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim5 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   ConstJson -> prim5 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim6 [ label="primitives::Real" URL="\ref cqasm::v3::primitives::Real"];
 *   ConstReal -> prim6 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim7 [ label="primitives::RMatrix" URL="\ref cqasm::v3::primitives::RMatrix"];
 *   ConstRealMatrix -> prim7 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim8 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   ConstString -> prim8 [ label="value", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim9 [ label="primitives::Str" URL="\ref cqasm::v3::primitives::Str"];
 *   Function -> prim9 [ label="name", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim10 [ label="values::Node" URL="\ref cqasm::v3::values::Node"];
 *   Function -> prim10 [ label="operands*", arrowhead=open, style=bold, fontname=Helvetica, fontsize=10];
 *   prim11 [ label="types::Node" URL="\ref cqasm::v3::types::Node"];
 *   Function -> prim11 [ label="return_type", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   QubitRefs -> ConstInt [ label="index+", arrowhead=normal, style=bold, fontname=Helvetica, fontsize=10];
 *   prim12 [ label="semantic::Variable" URL="\ref cqasm::v3::semantic::Variable"];
 *   VariableRef -> prim12 [ label="variable@", arrowhead=normal, style=dashed, fontname=Helvetica, fontsize=10];
 * }
 * \enddot
 */
namespace values {

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
class BitRefs;
class ConstAxis;
class ConstBool;
class ConstComplex;
class ConstComplexMatrix;
class ConstInt;
class ConstJson;
class ConstReal;
class ConstRealMatrix;
class ConstString;
class Constant;
class Function;
class QubitRefs;
class Reference;
class VariableRef;
class VisitorBase;
template <typename T = void>
class Visitor;
class RecursiveVisitor;
class Dumper;

/**
 * Enumeration of all node types.
 */
enum class NodeType {
    BitRefs,
    ConstAxis,
    ConstBool,
    ConstComplex,
    ConstComplexMatrix,
    ConstInt,
    ConstJson,
    ConstReal,
    ConstRealMatrix,
    ConstString,
    Function,
    QubitRefs,
    VariableRef
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
     * Interprets this node to a node of type BitRefs. Returns null if it has
     * the wrong type.
     */
    virtual BitRefs *as_bit_refs();

    /**
     * Interprets this node to a node of type BitRefs. Returns null if it has
     * the wrong type.
     */
    virtual const BitRefs *as_bit_refs() const;

    /**
     * Interprets this node to a node of type ConstAxis. Returns null if it has
     * the wrong type.
     */
    virtual ConstAxis *as_const_axis();

    /**
     * Interprets this node to a node of type ConstAxis. Returns null if it has
     * the wrong type.
     */
    virtual const ConstAxis *as_const_axis() const;

    /**
     * Interprets this node to a node of type ConstBool. Returns null if it has
     * the wrong type.
     */
    virtual ConstBool *as_const_bool();

    /**
     * Interprets this node to a node of type ConstBool. Returns null if it has
     * the wrong type.
     */
    virtual const ConstBool *as_const_bool() const;

    /**
     * Interprets this node to a node of type ConstComplex. Returns null if it
     * has the wrong type.
     */
    virtual ConstComplex *as_const_complex();

    /**
     * Interprets this node to a node of type ConstComplex. Returns null if it
     * has the wrong type.
     */
    virtual const ConstComplex *as_const_complex() const;

    /**
     * Interprets this node to a node of type ConstComplexMatrix. Returns null
     * if it has the wrong type.
     */
    virtual ConstComplexMatrix *as_const_complex_matrix();

    /**
     * Interprets this node to a node of type ConstComplexMatrix. Returns null
     * if it has the wrong type.
     */
    virtual const ConstComplexMatrix *as_const_complex_matrix() const;

    /**
     * Interprets this node to a node of type ConstInt. Returns null if it has
     * the wrong type.
     */
    virtual ConstInt *as_const_int();

    /**
     * Interprets this node to a node of type ConstInt. Returns null if it has
     * the wrong type.
     */
    virtual const ConstInt *as_const_int() const;

    /**
     * Interprets this node to a node of type ConstJson. Returns null if it has
     * the wrong type.
     */
    virtual ConstJson *as_const_json();

    /**
     * Interprets this node to a node of type ConstJson. Returns null if it has
     * the wrong type.
     */
    virtual const ConstJson *as_const_json() const;

    /**
     * Interprets this node to a node of type ConstReal. Returns null if it has
     * the wrong type.
     */
    virtual ConstReal *as_const_real();

    /**
     * Interprets this node to a node of type ConstReal. Returns null if it has
     * the wrong type.
     */
    virtual const ConstReal *as_const_real() const;

    /**
     * Interprets this node to a node of type ConstRealMatrix. Returns null if
     * it has the wrong type.
     */
    virtual ConstRealMatrix *as_const_real_matrix();

    /**
     * Interprets this node to a node of type ConstRealMatrix. Returns null if
     * it has the wrong type.
     */
    virtual const ConstRealMatrix *as_const_real_matrix() const;

    /**
     * Interprets this node to a node of type ConstString. Returns null if it
     * has the wrong type.
     */
    virtual ConstString *as_const_string();

    /**
     * Interprets this node to a node of type ConstString. Returns null if it
     * has the wrong type.
     */
    virtual const ConstString *as_const_string() const;

    /**
     * Interprets this node to a node of type Constant. Returns null if it has
     * the wrong type.
     */
    virtual Constant *as_constant();

    /**
     * Interprets this node to a node of type Constant. Returns null if it has
     * the wrong type.
     */
    virtual const Constant *as_constant() const;

    /**
     * Interprets this node to a node of type Function. Returns null if it has
     * the wrong type.
     */
    virtual Function *as_function();

    /**
     * Interprets this node to a node of type Function. Returns null if it has
     * the wrong type.
     */
    virtual const Function *as_function() const;

    /**
     * Interprets this node to a node of type QubitRefs. Returns null if it has
     * the wrong type.
     */
    virtual QubitRefs *as_qubit_refs();

    /**
     * Interprets this node to a node of type QubitRefs. Returns null if it has
     * the wrong type.
     */
    virtual const QubitRefs *as_qubit_refs() const;

    /**
     * Interprets this node to a node of type Reference. Returns null if it has
     * the wrong type.
     */
    virtual Reference *as_reference();

    /**
     * Interprets this node to a node of type Reference. Returns null if it has
     * the wrong type.
     */
    virtual const Reference *as_reference() const;

    /**
     * Interprets this node to a node of type VariableRef. Returns null if it
     * has the wrong type.
     */
    virtual VariableRef *as_variable_ref();

    /**
     * Interprets this node to a node of type VariableRef. Returns null if it
     * has the wrong type.
     */
    virtual const VariableRef *as_variable_ref() const;

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
 * Represents a reference to some storage location.
 */
class Reference : public Node {
public:

    /**
     * Interprets this node to a node of type Reference. Returns null if it has
     * the wrong type.
     */
    Reference *as_reference() override;

    /**
     * Interprets this node to a node of type Reference. Returns null if it has
     * the wrong type.
     */
    const Reference *as_reference() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Reference> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a measurement bit, or a number of measurement bits for conditional
 * gates with multiple conditions. When there are multiple, the conditions are
 * all AND'ed together.
 */
class BitRefs : public Reference {
public:

    /**
     * The qubit index that this is a measurement bit for, starting at 0.
     */
    Many<ConstInt> index;

    /**
     * Constructor.
     */
    BitRefs(const Many<ConstInt> &index = Many<ConstInt>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `BitRefs` is complete/fully defined.
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
     * Interprets this node to a node of type BitRefs. Returns null if it has
     * the wrong type.
     */
    BitRefs *as_bit_refs() override;

    /**
     * Interprets this node to a node of type BitRefs. Returns null if it has
     * the wrong type.
     */
    const BitRefs *as_bit_refs() const override;

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
    static std::shared_ptr<BitRefs> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a constant value.
 */
class Constant : public Node {
public:

    /**
     * Interprets this node to a node of type Constant. Returns null if it has
     * the wrong type.
     */
    Constant *as_constant() override;

    /**
     * Interprets this node to a node of type Constant. Returns null if it has
     * the wrong type.
     */
    const Constant *as_constant() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<Constant> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents an axis value (x, y, or z).
 */
class ConstAxis : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Axis value;

    /**
     * Constructor.
     */
    ConstAxis(const cqasm::v3::primitives::Axis &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Axis>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstAxis` is complete/fully defined.
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
     * Interprets this node to a node of type ConstAxis. Returns null if it has
     * the wrong type.
     */
    ConstAxis *as_const_axis() override;

    /**
     * Interprets this node to a node of type ConstAxis. Returns null if it has
     * the wrong type.
     */
    const ConstAxis *as_const_axis() const override;

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
    static std::shared_ptr<ConstAxis> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type bool.
 */
class ConstBool : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Bool value;

    /**
     * Constructor.
     */
    ConstBool(const cqasm::v3::primitives::Bool &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstBool` is complete/fully defined.
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
     * Interprets this node to a node of type ConstBool. Returns null if it has
     * the wrong type.
     */
    ConstBool *as_const_bool() override;

    /**
     * Interprets this node to a node of type ConstBool. Returns null if it has
     * the wrong type.
     */
    const ConstBool *as_const_bool() const override;

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
    static std::shared_ptr<ConstBool> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type complex.
 */
class ConstComplex : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Complex value;

    /**
     * Constructor.
     */
    ConstComplex(const cqasm::v3::primitives::Complex &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Complex>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstComplex` is complete/fully defined.
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
     * Interprets this node to a node of type ConstComplex. Returns null if it
     * has the wrong type.
     */
    ConstComplex *as_const_complex() override;

    /**
     * Interprets this node to a node of type ConstComplex. Returns null if it
     * has the wrong type.
     */
    const ConstComplex *as_const_complex() const override;

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
    static std::shared_ptr<ConstComplex> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type complex_matrix.
 */
class ConstComplexMatrix : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::CMatrix value;

    /**
     * Constructor.
     */
    ConstComplexMatrix(const cqasm::v3::primitives::CMatrix &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::CMatrix>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstComplexMatrix` is complete/fully defined.
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
     * Interprets this node to a node of type ConstComplexMatrix. Returns null
     * if it has the wrong type.
     */
    ConstComplexMatrix *as_const_complex_matrix() override;

    /**
     * Interprets this node to a node of type ConstComplexMatrix. Returns null
     * if it has the wrong type.
     */
    const ConstComplexMatrix *as_const_complex_matrix() const override;

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
    static std::shared_ptr<ConstComplexMatrix> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type int.
 */
class ConstInt : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Int value;

    /**
     * Constructor.
     */
    ConstInt(const cqasm::v3::primitives::Int &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstInt` is complete/fully defined.
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
     * Interprets this node to a node of type ConstInt. Returns null if it has
     * the wrong type.
     */
    ConstInt *as_const_int() override;

    /**
     * Interprets this node to a node of type ConstInt. Returns null if it has
     * the wrong type.
     */
    const ConstInt *as_const_int() const override;

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
    static std::shared_ptr<ConstInt> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type json.
 */
class ConstJson : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Str value;

    /**
     * Constructor.
     */
    ConstJson(const cqasm::v3::primitives::Str &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstJson` is complete/fully defined.
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
     * Interprets this node to a node of type ConstJson. Returns null if it has
     * the wrong type.
     */
    ConstJson *as_const_json() override;

    /**
     * Interprets this node to a node of type ConstJson. Returns null if it has
     * the wrong type.
     */
    const ConstJson *as_const_json() const override;

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
    static std::shared_ptr<ConstJson> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type real.
 */
class ConstReal : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Real value;

    /**
     * Constructor.
     */
    ConstReal(const cqasm::v3::primitives::Real &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Real>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstReal` is complete/fully defined.
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
     * Interprets this node to a node of type ConstReal. Returns null if it has
     * the wrong type.
     */
    ConstReal *as_const_real() override;

    /**
     * Interprets this node to a node of type ConstReal. Returns null if it has
     * the wrong type.
     */
    const ConstReal *as_const_real() const override;

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
    static std::shared_ptr<ConstReal> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type real_matrix.
 */
class ConstRealMatrix : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::RMatrix value;

    /**
     * Constructor.
     */
    ConstRealMatrix(const cqasm::v3::primitives::RMatrix &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::RMatrix>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstRealMatrix` is complete/fully defined.
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
     * Interprets this node to a node of type ConstRealMatrix. Returns null if
     * it has the wrong type.
     */
    ConstRealMatrix *as_const_real_matrix() override;

    /**
     * Interprets this node to a node of type ConstRealMatrix. Returns null if
     * it has the wrong type.
     */
    const ConstRealMatrix *as_const_real_matrix() const override;

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
    static std::shared_ptr<ConstRealMatrix> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a value of type string.
 */
class ConstString : public Constant {
public:

    /**
     * The contained value.
     */
    cqasm::v3::primitives::Str value;

    /**
     * Constructor.
     */
    ConstString(const cqasm::v3::primitives::Str &value = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ConstString` is complete/fully defined.
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
     * Interprets this node to a node of type ConstString. Returns null if it
     * has the wrong type.
     */
    ConstString *as_const_string() override;

    /**
     * Interprets this node to a node of type ConstString. Returns null if it
     * has the wrong type.
     */
    const ConstString *as_const_string() const override;

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
    static std::shared_ptr<ConstString> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * This can be returned by user-defined functions as a placeholder value for
 * something that needs to be evaluated at runtime rather than during constant
 * propagation. Annotations should be used to attach semantic information.
 */
class Function : public Node {
public:

    /**
     * Name of the function as it appears or should appear in the cQASM file.
     */
    cqasm::v3::primitives::Str name;

    /**
     * Operands for the function.
     */
    Any<cqasm::v3::values::Node> operands;

    /**
     * Operands for the function.
     */
    One<cqasm::v3::types::Node> return_type;

    /**
     * Constructor.
     */
    Function(const cqasm::v3::primitives::Str &name = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Str>(), const Any<cqasm::v3::values::Node> &operands = cqasm::v3::primitives::initialize<Any<cqasm::v3::values::Node>>(), const One<cqasm::v3::types::Node> &return_type = cqasm::v3::primitives::initialize<One<cqasm::v3::types::Node>>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Function` is complete/fully defined.
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
     * Interprets this node to a node of type Function. Returns null if it has
     * the wrong type.
     */
    Function *as_function() override;

    /**
     * Interprets this node to a node of type Function. Returns null if it has
     * the wrong type.
     */
    const Function *as_function() const override;

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
    static std::shared_ptr<Function> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a qubit, or a set of qubits for single-gate-multiple-qubit
 * notation. The indices must not repeat.
 */
class QubitRefs : public Reference {
public:

    /**
     * Set of qubit indices referred to, starting at 0.
     */
    Many<ConstInt> index;

    /**
     * Constructor.
     */
    QubitRefs(const Many<ConstInt> &index = Many<ConstInt>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `QubitRefs` is complete/fully defined.
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
     * Interprets this node to a node of type QubitRefs. Returns null if it has
     * the wrong type.
     */
    QubitRefs *as_qubit_refs() override;

    /**
     * Interprets this node to a node of type QubitRefs. Returns null if it has
     * the wrong type.
     */
    const QubitRefs *as_qubit_refs() const override;

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
    static std::shared_ptr<QubitRefs> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Represents a variable reference.
 */
class VariableRef : public Reference {
public:

    /**
     * The referenced variable.
     */
    Link<cqasm::v3::semantic::Variable> variable;

    /**
     * Constructor.
     */
    VariableRef(const Link<cqasm::v3::semantic::Variable> &variable = cqasm::v3::primitives::initialize<Link<cqasm::v3::semantic::Variable>>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `VariableRef` is complete/fully defined.
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
     * Interprets this node to a node of type VariableRef. Returns null if it
     * has the wrong type.
     */
    VariableRef *as_variable_ref() override;

    /**
     * Interprets this node to a node of type VariableRef. Returns null if it
     * has the wrong type.
     */
    const VariableRef *as_variable_ref() const override;

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
    static std::shared_ptr<VariableRef> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

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
    friend class BitRefs;
    friend class ConstAxis;
    friend class ConstBool;
    friend class ConstComplex;
    friend class ConstComplexMatrix;
    friend class ConstInt;
    friend class ConstJson;
    friend class ConstReal;
    friend class ConstRealMatrix;
    friend class ConstString;
    friend class Constant;
    friend class Function;
    friend class QubitRefs;
    friend class Reference;
    friend class VariableRef;

    /**
     * Internal visitor function for nodes of any type.
     */
    virtual void raw_visit_node(Node &node, void *retval) = 0;

    /**
     * Internal visitor function for `BitRefs` nodes.
     */
    virtual void raw_visit_bit_refs(BitRefs &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstAxis` nodes.
     */
    virtual void raw_visit_const_axis(ConstAxis &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstBool` nodes.
     */
    virtual void raw_visit_const_bool(ConstBool &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstComplex` nodes.
     */
    virtual void raw_visit_const_complex(ConstComplex &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstComplexMatrix` nodes.
     */
    virtual void raw_visit_const_complex_matrix(ConstComplexMatrix &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstInt` nodes.
     */
    virtual void raw_visit_const_int(ConstInt &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstJson` nodes.
     */
    virtual void raw_visit_const_json(ConstJson &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstReal` nodes.
     */
    virtual void raw_visit_const_real(ConstReal &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstRealMatrix` nodes.
     */
    virtual void raw_visit_const_real_matrix(ConstRealMatrix &node, void *retval) = 0;

    /**
     * Internal visitor function for `ConstString` nodes.
     */
    virtual void raw_visit_const_string(ConstString &node, void *retval) = 0;

    /**
     * Internal visitor function for `Constant` nodes.
     */
    virtual void raw_visit_constant(Constant &node, void *retval) = 0;

    /**
     * Internal visitor function for `Function` nodes.
     */
    virtual void raw_visit_function(Function &node, void *retval) = 0;

    /**
     * Internal visitor function for `QubitRefs` nodes.
     */
    virtual void raw_visit_qubit_refs(QubitRefs &node, void *retval) = 0;

    /**
     * Internal visitor function for `Reference` nodes.
     */
    virtual void raw_visit_reference(Reference &node, void *retval) = 0;

    /**
     * Internal visitor function for `VariableRef` nodes.
     */
    virtual void raw_visit_variable_ref(VariableRef &node, void *retval) = 0;

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
     * Internal visitor function for `BitRefs` nodes.
     */
    void raw_visit_bit_refs(BitRefs &node, void *retval) override;

    /**
     * Internal visitor function for `ConstAxis` nodes.
     */
    void raw_visit_const_axis(ConstAxis &node, void *retval) override;

    /**
     * Internal visitor function for `ConstBool` nodes.
     */
    void raw_visit_const_bool(ConstBool &node, void *retval) override;

    /**
     * Internal visitor function for `ConstComplex` nodes.
     */
    void raw_visit_const_complex(ConstComplex &node, void *retval) override;

    /**
     * Internal visitor function for `ConstComplexMatrix` nodes.
     */
    void raw_visit_const_complex_matrix(ConstComplexMatrix &node, void *retval) override;

    /**
     * Internal visitor function for `ConstInt` nodes.
     */
    void raw_visit_const_int(ConstInt &node, void *retval) override;

    /**
     * Internal visitor function for `ConstJson` nodes.
     */
    void raw_visit_const_json(ConstJson &node, void *retval) override;

    /**
     * Internal visitor function for `ConstReal` nodes.
     */
    void raw_visit_const_real(ConstReal &node, void *retval) override;

    /**
     * Internal visitor function for `ConstRealMatrix` nodes.
     */
    void raw_visit_const_real_matrix(ConstRealMatrix &node, void *retval) override;

    /**
     * Internal visitor function for `ConstString` nodes.
     */
    void raw_visit_const_string(ConstString &node, void *retval) override;

    /**
     * Internal visitor function for `Constant` nodes.
     */
    void raw_visit_constant(Constant &node, void *retval) override;

    /**
     * Internal visitor function for `Function` nodes.
     */
    void raw_visit_function(Function &node, void *retval) override;

    /**
     * Internal visitor function for `QubitRefs` nodes.
     */
    void raw_visit_qubit_refs(QubitRefs &node, void *retval) override;

    /**
     * Internal visitor function for `Reference` nodes.
     */
    void raw_visit_reference(Reference &node, void *retval) override;

    /**
     * Internal visitor function for `VariableRef` nodes.
     */
    void raw_visit_variable_ref(VariableRef &node, void *retval) override;

public:

    /**
     * Fallback function for nodes of any type.
     */
    virtual T visit_node(Node &node) = 0;

    /**
     * Visitor function for `BitRefs` nodes.
     */
    virtual T visit_bit_refs(BitRefs &node) {
        return visit_reference(node);
    }

    /**
     * Visitor function for `ConstAxis` nodes.
     */
    virtual T visit_const_axis(ConstAxis &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstBool` nodes.
     */
    virtual T visit_const_bool(ConstBool &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstComplex` nodes.
     */
    virtual T visit_const_complex(ConstComplex &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstComplexMatrix` nodes.
     */
    virtual T visit_const_complex_matrix(ConstComplexMatrix &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstInt` nodes.
     */
    virtual T visit_const_int(ConstInt &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstJson` nodes.
     */
    virtual T visit_const_json(ConstJson &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstReal` nodes.
     */
    virtual T visit_const_real(ConstReal &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstRealMatrix` nodes.
     */
    virtual T visit_const_real_matrix(ConstRealMatrix &node) {
        return visit_constant(node);
    }

    /**
     * Visitor function for `ConstString` nodes.
     */
    virtual T visit_const_string(ConstString &node) {
        return visit_constant(node);
    }

    /**
     * Fallback function for `Constant` nodes.
     */
    virtual T visit_constant(Constant &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `Function` nodes.
     */
    virtual T visit_function(Function &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `QubitRefs` nodes.
     */
    virtual T visit_qubit_refs(QubitRefs &node) {
        return visit_reference(node);
    }

    /**
     * Fallback function for `Reference` nodes.
     */
    virtual T visit_reference(Reference &node) {
        return visit_node(node);
    }

    /**
     * Visitor function for `VariableRef` nodes.
     */
    virtual T visit_variable_ref(VariableRef &node) {
        return visit_reference(node);
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
     * Internal visitor function for `BitRefs` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bit_refs(BitRefs &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bit_refs(node);
        } else {
            *((T*)retval) = this->visit_bit_refs(node);
        };
    }

    /**
     * Internal visitor function for `BitRefs` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bit_refs(BitRefs &node, void *retval);

    /**
     * Internal visitor function for `ConstAxis` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_axis(ConstAxis &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_axis(node);
        } else {
            *((T*)retval) = this->visit_const_axis(node);
        };
    }

    /**
     * Internal visitor function for `ConstAxis` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_axis(ConstAxis &node, void *retval);

    /**
     * Internal visitor function for `ConstBool` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_bool(ConstBool &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_bool(node);
        } else {
            *((T*)retval) = this->visit_const_bool(node);
        };
    }

    /**
     * Internal visitor function for `ConstBool` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_bool(ConstBool &node, void *retval);

    /**
     * Internal visitor function for `ConstComplex` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_complex(ConstComplex &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_complex(node);
        } else {
            *((T*)retval) = this->visit_const_complex(node);
        };
    }

    /**
     * Internal visitor function for `ConstComplex` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_complex(ConstComplex &node, void *retval);

    /**
     * Internal visitor function for `ConstComplexMatrix` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_complex_matrix(ConstComplexMatrix &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_complex_matrix(node);
        } else {
            *((T*)retval) = this->visit_const_complex_matrix(node);
        };
    }

    /**
     * Internal visitor function for `ConstComplexMatrix` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_complex_matrix(ConstComplexMatrix &node, void *retval);

    /**
     * Internal visitor function for `ConstInt` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_int(ConstInt &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_int(node);
        } else {
            *((T*)retval) = this->visit_const_int(node);
        };
    }

    /**
     * Internal visitor function for `ConstInt` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_int(ConstInt &node, void *retval);

    /**
     * Internal visitor function for `ConstJson` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_json(ConstJson &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_json(node);
        } else {
            *((T*)retval) = this->visit_const_json(node);
        };
    }

    /**
     * Internal visitor function for `ConstJson` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_json(ConstJson &node, void *retval);

    /**
     * Internal visitor function for `ConstReal` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_real(ConstReal &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_real(node);
        } else {
            *((T*)retval) = this->visit_const_real(node);
        };
    }

    /**
     * Internal visitor function for `ConstReal` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_real(ConstReal &node, void *retval);

    /**
     * Internal visitor function for `ConstRealMatrix` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_real_matrix(ConstRealMatrix &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_real_matrix(node);
        } else {
            *((T*)retval) = this->visit_const_real_matrix(node);
        };
    }

    /**
     * Internal visitor function for `ConstRealMatrix` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_real_matrix(ConstRealMatrix &node, void *retval);

    /**
     * Internal visitor function for `ConstString` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_const_string(ConstString &node, void *retval) {
        if (retval == nullptr) {
            this->visit_const_string(node);
        } else {
            *((T*)retval) = this->visit_const_string(node);
        };
    }

    /**
     * Internal visitor function for `ConstString` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_const_string(ConstString &node, void *retval);

    /**
     * Internal visitor function for `Constant` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_constant(Constant &node, void *retval) {
        if (retval == nullptr) {
            this->visit_constant(node);
        } else {
            *((T*)retval) = this->visit_constant(node);
        };
    }

    /**
     * Internal visitor function for `Constant` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_constant(Constant &node, void *retval);

    /**
     * Internal visitor function for `Function` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_function(Function &node, void *retval) {
        if (retval == nullptr) {
            this->visit_function(node);
        } else {
            *((T*)retval) = this->visit_function(node);
        };
    }

    /**
     * Internal visitor function for `Function` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_function(Function &node, void *retval);

    /**
     * Internal visitor function for `QubitRefs` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_qubit_refs(QubitRefs &node, void *retval) {
        if (retval == nullptr) {
            this->visit_qubit_refs(node);
        } else {
            *((T*)retval) = this->visit_qubit_refs(node);
        };
    }

    /**
     * Internal visitor function for `QubitRefs` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_qubit_refs(QubitRefs &node, void *retval);

    /**
     * Internal visitor function for `Reference` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_reference(Reference &node, void *retval) {
        if (retval == nullptr) {
            this->visit_reference(node);
        } else {
            *((T*)retval) = this->visit_reference(node);
        };
    }

    /**
     * Internal visitor function for `Reference` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_reference(Reference &node, void *retval);

    /**
     * Internal visitor function for `VariableRef` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_variable_ref(VariableRef &node, void *retval) {
        if (retval == nullptr) {
            this->visit_variable_ref(node);
        } else {
            *((T*)retval) = this->visit_variable_ref(node);
        };
    }

    /**
     * Internal visitor function for `VariableRef` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_variable_ref(VariableRef &node, void *retval);

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
     * Recursive traversal for `BitRefs` nodes.
     */
    void visit_bit_refs(BitRefs &node) override;

    /**
     * Recursive traversal for `ConstAxis` nodes.
     */
    void visit_const_axis(ConstAxis &node) override;

    /**
     * Recursive traversal for `ConstBool` nodes.
     */
    void visit_const_bool(ConstBool &node) override;

    /**
     * Recursive traversal for `ConstComplex` nodes.
     */
    void visit_const_complex(ConstComplex &node) override;

    /**
     * Recursive traversal for `ConstComplexMatrix` nodes.
     */
    void visit_const_complex_matrix(ConstComplexMatrix &node) override;

    /**
     * Recursive traversal for `ConstInt` nodes.
     */
    void visit_const_int(ConstInt &node) override;

    /**
     * Recursive traversal for `ConstJson` nodes.
     */
    void visit_const_json(ConstJson &node) override;

    /**
     * Recursive traversal for `ConstReal` nodes.
     */
    void visit_const_real(ConstReal &node) override;

    /**
     * Recursive traversal for `ConstRealMatrix` nodes.
     */
    void visit_const_real_matrix(ConstRealMatrix &node) override;

    /**
     * Recursive traversal for `ConstString` nodes.
     */
    void visit_const_string(ConstString &node) override;

    /**
     * Recursive traversal for `Constant` nodes.
     */
    void visit_constant(Constant &node) override;

    /**
     * Recursive traversal for `Function` nodes.
     */
    void visit_function(Function &node) override;

    /**
     * Recursive traversal for `QubitRefs` nodes.
     */
    void visit_qubit_refs(QubitRefs &node) override;

    /**
     * Recursive traversal for `Reference` nodes.
     */
    void visit_reference(Reference &node) override;

    /**
     * Recursive traversal for `VariableRef` nodes.
     */
    void visit_variable_ref(VariableRef &node) override;

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
     * Dumps a `BitRefs` node.
     */
    void visit_bit_refs(BitRefs &node) override;

    /**
     * Dumps a `ConstAxis` node.
     */
    void visit_const_axis(ConstAxis &node) override;

    /**
     * Dumps a `ConstBool` node.
     */
    void visit_const_bool(ConstBool &node) override;

    /**
     * Dumps a `ConstComplex` node.
     */
    void visit_const_complex(ConstComplex &node) override;

    /**
     * Dumps a `ConstComplexMatrix` node.
     */
    void visit_const_complex_matrix(ConstComplexMatrix &node) override;

    /**
     * Dumps a `ConstInt` node.
     */
    void visit_const_int(ConstInt &node) override;

    /**
     * Dumps a `ConstJson` node.
     */
    void visit_const_json(ConstJson &node) override;

    /**
     * Dumps a `ConstReal` node.
     */
    void visit_const_real(ConstReal &node) override;

    /**
     * Dumps a `ConstRealMatrix` node.
     */
    void visit_const_real_matrix(ConstRealMatrix &node) override;

    /**
     * Dumps a `ConstString` node.
     */
    void visit_const_string(ConstString &node) override;

    /**
     * Dumps a `Constant` node.
     */
    void visit_constant(Constant &node) override;

    /**
     * Dumps a `Function` node.
     */
    void visit_function(Function &node) override;

    /**
     * Dumps a `QubitRefs` node.
     */
    void visit_qubit_refs(QubitRefs &node) override;

    /**
     * Dumps a `Reference` node.
     */
    void visit_reference(Reference &node) override;

    /**
     * Dumps a `VariableRef` node.
     */
    void visit_variable_ref(VariableRef &node) override;

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

} // namespace values
} // namespace v3
} // namespace cqasm

