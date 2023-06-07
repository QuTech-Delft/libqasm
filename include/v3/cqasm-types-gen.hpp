/** \file
 * Header file for the various classes representing the types of values
 * available in cQASM.
 */

#pragma once

#include <iostream>
#include "cqasm-tree.hpp"
#include "v3/cqasm-primitives.hpp"
#include "v3/cqasm-semantic-helper.hpp"

namespace cqasm {
namespace v3 {

/**
 * Namespace for the various classes representing the types of values available
 * in cQASM.
 */
/**
 * \dot
 * digraph example {
 *   node [shape=record, fontname=Helvetica, fontsize=10];
 *   Axis [ label="Axis" URL="\ref cqasm::v3::types::Axis"];
 *   Bool [ label="Bool" URL="\ref cqasm::v3::types::Bool"];
 *   Complex [ label="Complex" URL="\ref cqasm::v3::types::Complex"];
 *   ComplexMatrix [ label="ComplexMatrix" URL="\ref cqasm::v3::types::ComplexMatrix"];
 *   Int [ label="Int" URL="\ref cqasm::v3::types::Int"];
 *   Json [ label="Json" URL="\ref cqasm::v3::types::Json"];
 *   Qubit [ label="Qubit" URL="\ref cqasm::v3::types::Qubit"];
 *   Real [ label="Real" URL="\ref cqasm::v3::types::Real"];
 *   RealMatrix [ label="RealMatrix" URL="\ref cqasm::v3::types::RealMatrix"];
 *   String [ label="String" URL="\ref cqasm::v3::types::String"];
 *   TypeBase [ label="TypeBase" URL="\ref cqasm::v3::types::TypeBase", style=dotted];
 *   TypeBase -> Axis [ arrowhead=open, style=dotted ];
 *   TypeBase -> Bool [ arrowhead=open, style=dotted ];
 *   TypeBase -> Complex [ arrowhead=open, style=dotted ];
 *   TypeBase -> ComplexMatrix [ arrowhead=open, style=dotted ];
 *   TypeBase -> Int [ arrowhead=open, style=dotted ];
 *   TypeBase -> Json [ arrowhead=open, style=dotted ];
 *   TypeBase -> Qubit [ arrowhead=open, style=dotted ];
 *   TypeBase -> Real [ arrowhead=open, style=dotted ];
 *   TypeBase -> RealMatrix [ arrowhead=open, style=dotted ];
 *   TypeBase -> String [ arrowhead=open, style=dotted ];
 *   prim0 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   ComplexMatrix -> prim0 [ label="num_rows", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim1 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   ComplexMatrix -> prim1 [ label="num_cols", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim2 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   RealMatrix -> prim2 [ label="num_rows", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim3 [ label="primitives::Int" URL="\ref cqasm::v3::primitives::Int"];
 *   RealMatrix -> prim3 [ label="num_cols", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 *   prim4 [ label="primitives::Bool" URL="\ref cqasm::v3::primitives::Bool"];
 *   TypeBase -> prim4 [ label="assignable", arrowhead=normal, style=solid, fontname=Helvetica, fontsize=10];
 * }
 * \enddot
 */
namespace types {

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
class Axis;
class Bool;
class Complex;
class ComplexMatrix;
class Int;
class Json;
class Qubit;
class Real;
class RealMatrix;
class String;
class TypeBase;
class VisitorBase;
template <typename T = void>
class Visitor;
class RecursiveVisitor;
class Dumper;

/**
 * Enumeration of all node types.
 */
enum class NodeType {
    Axis,
    Bool,
    Complex,
    ComplexMatrix,
    Int,
    Json,
    Qubit,
    Real,
    RealMatrix,
    String
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
     * Interprets this node to a node of type Axis. Returns null if it has the
     * wrong type.
     */
    virtual Axis *as_axis();

    /**
     * Interprets this node to a node of type Axis. Returns null if it has the
     * wrong type.
     */
    virtual const Axis *as_axis() const;

    /**
     * Interprets this node to a node of type Bool. Returns null if it has the
     * wrong type.
     */
    virtual Bool *as_bool();

    /**
     * Interprets this node to a node of type Bool. Returns null if it has the
     * wrong type.
     */
    virtual const Bool *as_bool() const;

    /**
     * Interprets this node to a node of type Complex. Returns null if it has
     * the wrong type.
     */
    virtual Complex *as_complex();

    /**
     * Interprets this node to a node of type Complex. Returns null if it has
     * the wrong type.
     */
    virtual const Complex *as_complex() const;

    /**
     * Interprets this node to a node of type ComplexMatrix. Returns null if it
     * has the wrong type.
     */
    virtual ComplexMatrix *as_complex_matrix();

    /**
     * Interprets this node to a node of type ComplexMatrix. Returns null if it
     * has the wrong type.
     */
    virtual const ComplexMatrix *as_complex_matrix() const;

    /**
     * Interprets this node to a node of type Int. Returns null if it has the
     * wrong type.
     */
    virtual Int *as_int();

    /**
     * Interprets this node to a node of type Int. Returns null if it has the
     * wrong type.
     */
    virtual const Int *as_int() const;

    /**
     * Interprets this node to a node of type Json. Returns null if it has the
     * wrong type.
     */
    virtual Json *as_json();

    /**
     * Interprets this node to a node of type Json. Returns null if it has the
     * wrong type.
     */
    virtual const Json *as_json() const;

    /**
     * Interprets this node to a node of type Qubit. Returns null if it has the
     * wrong type.
     */
    virtual Qubit *as_qubit();

    /**
     * Interprets this node to a node of type Qubit. Returns null if it has the
     * wrong type.
     */
    virtual const Qubit *as_qubit() const;

    /**
     * Interprets this node to a node of type Real. Returns null if it has the
     * wrong type.
     */
    virtual Real *as_real();

    /**
     * Interprets this node to a node of type Real. Returns null if it has the
     * wrong type.
     */
    virtual const Real *as_real() const;

    /**
     * Interprets this node to a node of type RealMatrix. Returns null if it has
     * the wrong type.
     */
    virtual RealMatrix *as_real_matrix();

    /**
     * Interprets this node to a node of type RealMatrix. Returns null if it has
     * the wrong type.
     */
    virtual const RealMatrix *as_real_matrix() const;

    /**
     * Interprets this node to a node of type String. Returns null if it has the
     * wrong type.
     */
    virtual String *as_string();

    /**
     * Interprets this node to a node of type String. Returns null if it has the
     * wrong type.
     */
    virtual const String *as_string() const;

    /**
     * Interprets this node to a node of type TypeBase. Returns null if it has
     * the wrong type.
     */
    virtual TypeBase *as_type_base();

    /**
     * Interprets this node to a node of type TypeBase. Returns null if it has
     * the wrong type.
     */
    virtual const TypeBase *as_type_base() const;

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
 * Base for all types, with a member indicating whether values of this type are
 * assignable (references) or not (constants, expression results).
 */
class TypeBase : public Node {
public:

    /**
     * Whether a value of this type can be assigned (true), or can only be read
     * (false). For constraints, whether the value must be assignable (true), or
     * only has to be read. In C++ terminology, true = lvalue, false = rvalue
     * (more or less).
     */
    cqasm::v3::primitives::Bool assignable;

    /**
     * Constructor.
     */
    TypeBase(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Interprets this node to a node of type TypeBase. Returns null if it has
     * the wrong type.
     */
    TypeBase *as_type_base() override;

    /**
     * Interprets this node to a node of type TypeBase. Returns null if it has
     * the wrong type.
     */
    const TypeBase *as_type_base() const override;

    /**
     * Deserializes the given node.
     */
    static std::shared_ptr<TypeBase> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of an axis (x, y, or z).
 */
class Axis : public TypeBase {
public:

    /**
     * Constructor.
     */
    Axis(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Axis` is complete/fully defined.
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
     * Interprets this node to a node of type Axis. Returns null if it has the
     * wrong type.
     */
    Axis *as_axis() override;

    /**
     * Interprets this node to a node of type Axis. Returns null if it has the
     * wrong type.
     */
    const Axis *as_axis() const override;

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
    static std::shared_ptr<Axis> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a boolean/bit.
 */
class Bool : public TypeBase {
public:

    /**
     * Constructor.
     */
    Bool(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Bool` is complete/fully defined.
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
     * Interprets this node to a node of type Bool. Returns null if it has the
     * wrong type.
     */
    Bool *as_bool() override;

    /**
     * Interprets this node to a node of type Bool. Returns null if it has the
     * wrong type.
     */
    const Bool *as_bool() const override;

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
    static std::shared_ptr<Bool> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a complex number (2x IEEE double).
 */
class Complex : public TypeBase {
public:

    /**
     * Constructor.
     */
    Complex(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Complex` is complete/fully defined.
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
     * Interprets this node to a node of type Complex. Returns null if it has
     * the wrong type.
     */
    Complex *as_complex() override;

    /**
     * Interprets this node to a node of type Complex. Returns null if it has
     * the wrong type.
     */
    const Complex *as_complex() const override;

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
    static std::shared_ptr<Complex> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a complex matrix. Also used for one-dimensional arrays and vectors.
 */
class ComplexMatrix : public TypeBase {
public:

    /**
     * Number of rows. Negative indicates that the value is unconstrained. Must
     * be one or more otherwise.
     */
    cqasm::v3::primitives::Int num_rows;

    /**
     * Number of columns. Negative indicates that the value is unconstrained.
     * Can be zero, unlike num_rows.
     */
    cqasm::v3::primitives::Int num_cols;

    /**
     * Constructor.
     */
    ComplexMatrix(const cqasm::v3::primitives::Int &num_rows = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const cqasm::v3::primitives::Int &num_cols = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `ComplexMatrix` is complete/fully defined.
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
     * Interprets this node to a node of type ComplexMatrix. Returns null if it
     * has the wrong type.
     */
    ComplexMatrix *as_complex_matrix() override;

    /**
     * Interprets this node to a node of type ComplexMatrix. Returns null if it
     * has the wrong type.
     */
    const ComplexMatrix *as_complex_matrix() const override;

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
    static std::shared_ptr<ComplexMatrix> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of an integer (signed 64-bit).
 */
class Int : public TypeBase {
public:

    /**
     * Constructor.
     */
    Int(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Int` is complete/fully defined.
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
     * Interprets this node to a node of type Int. Returns null if it has the
     * wrong type.
     */
    Int *as_int() override;

    /**
     * Interprets this node to a node of type Int. Returns null if it has the
     * wrong type.
     */
    const Int *as_int() const override;

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
    static std::shared_ptr<Int> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a JSON string.
 */
class Json : public TypeBase {
public:

    /**
     * Constructor.
     */
    Json(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Json` is complete/fully defined.
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
     * Interprets this node to a node of type Json. Returns null if it has the
     * wrong type.
     */
    Json *as_json() override;

    /**
     * Interprets this node to a node of type Json. Returns null if it has the
     * wrong type.
     */
    const Json *as_json() const override;

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
    static std::shared_ptr<Json> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of one or more qubit references.
 */
class Qubit : public TypeBase {
public:

    /**
     * Constructor.
     */
    Qubit(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Qubit` is complete/fully defined.
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
     * Interprets this node to a node of type Qubit. Returns null if it has the
     * wrong type.
     */
    Qubit *as_qubit() override;

    /**
     * Interprets this node to a node of type Qubit. Returns null if it has the
     * wrong type.
     */
    const Qubit *as_qubit() const override;

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
    static std::shared_ptr<Qubit> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a real number (IEEE double).
 */
class Real : public TypeBase {
public:

    /**
     * Constructor.
     */
    Real(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `Real` is complete/fully defined.
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
     * Interprets this node to a node of type Real. Returns null if it has the
     * wrong type.
     */
    Real *as_real() override;

    /**
     * Interprets this node to a node of type Real. Returns null if it has the
     * wrong type.
     */
    const Real *as_real() const override;

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
    static std::shared_ptr<Real> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of a real matrix. Also used for one-dimensional arrays and vectors.
 */
class RealMatrix : public TypeBase {
public:

    /**
     * Number of rows. Negative indicates that the value is unconstrained. Must
     * be one or more otherwise.
     */
    cqasm::v3::primitives::Int num_rows;

    /**
     * Number of columns. Negative indicates that the value is unconstrained.
     * Can be zero, unlike num_rows.
     */
    cqasm::v3::primitives::Int num_cols;

    /**
     * Constructor.
     */
    RealMatrix(const cqasm::v3::primitives::Int &num_rows = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const cqasm::v3::primitives::Int &num_cols = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Int>(), const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `RealMatrix` is complete/fully defined.
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
     * Interprets this node to a node of type RealMatrix. Returns null if it has
     * the wrong type.
     */
    RealMatrix *as_real_matrix() override;

    /**
     * Interprets this node to a node of type RealMatrix. Returns null if it has
     * the wrong type.
     */
    const RealMatrix *as_real_matrix() const override;

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
    static std::shared_ptr<RealMatrix> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

};

/**
 * Type of an arbitrary string.
 */
class String : public TypeBase {
public:

    /**
     * Constructor.
     */
    String(const cqasm::v3::primitives::Bool &assignable = cqasm::v3::primitives::initialize<cqasm::v3::primitives::Bool>());

    /**
     * Registers all reachable nodes with the given PointerMap.
     */
    void find_reachable(::tree::base::PointerMap &map) const override;

    /**
     * Returns whether this `String` is complete/fully defined.
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
     * Interprets this node to a node of type String. Returns null if it has the
     * wrong type.
     */
    String *as_string() override;

    /**
     * Interprets this node to a node of type String. Returns null if it has the
     * wrong type.
     */
    const String *as_string() const override;

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
    static std::shared_ptr<String> deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids);

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
    friend class Axis;
    friend class Bool;
    friend class Complex;
    friend class ComplexMatrix;
    friend class Int;
    friend class Json;
    friend class Qubit;
    friend class Real;
    friend class RealMatrix;
    friend class String;
    friend class TypeBase;

    /**
     * Internal visitor function for nodes of any type.
     */
    virtual void raw_visit_node(Node &node, void *retval) = 0;

    /**
     * Internal visitor function for `Axis` nodes.
     */
    virtual void raw_visit_axis(Axis &node, void *retval) = 0;

    /**
     * Internal visitor function for `Bool` nodes.
     */
    virtual void raw_visit_bool(Bool &node, void *retval) = 0;

    /**
     * Internal visitor function for `Complex` nodes.
     */
    virtual void raw_visit_complex(Complex &node, void *retval) = 0;

    /**
     * Internal visitor function for `ComplexMatrix` nodes.
     */
    virtual void raw_visit_complex_matrix(ComplexMatrix &node, void *retval) = 0;

    /**
     * Internal visitor function for `Int` nodes.
     */
    virtual void raw_visit_int(Int &node, void *retval) = 0;

    /**
     * Internal visitor function for `Json` nodes.
     */
    virtual void raw_visit_json(Json &node, void *retval) = 0;

    /**
     * Internal visitor function for `Qubit` nodes.
     */
    virtual void raw_visit_qubit(Qubit &node, void *retval) = 0;

    /**
     * Internal visitor function for `Real` nodes.
     */
    virtual void raw_visit_real(Real &node, void *retval) = 0;

    /**
     * Internal visitor function for `RealMatrix` nodes.
     */
    virtual void raw_visit_real_matrix(RealMatrix &node, void *retval) = 0;

    /**
     * Internal visitor function for `String` nodes.
     */
    virtual void raw_visit_string(String &node, void *retval) = 0;

    /**
     * Internal visitor function for `TypeBase` nodes.
     */
    virtual void raw_visit_type_base(TypeBase &node, void *retval) = 0;

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
     * Internal visitor function for `Axis` nodes.
     */
    void raw_visit_axis(Axis &node, void *retval) override;

    /**
     * Internal visitor function for `Bool` nodes.
     */
    void raw_visit_bool(Bool &node, void *retval) override;

    /**
     * Internal visitor function for `Complex` nodes.
     */
    void raw_visit_complex(Complex &node, void *retval) override;

    /**
     * Internal visitor function for `ComplexMatrix` nodes.
     */
    void raw_visit_complex_matrix(ComplexMatrix &node, void *retval) override;

    /**
     * Internal visitor function for `Int` nodes.
     */
    void raw_visit_int(Int &node, void *retval) override;

    /**
     * Internal visitor function for `Json` nodes.
     */
    void raw_visit_json(Json &node, void *retval) override;

    /**
     * Internal visitor function for `Qubit` nodes.
     */
    void raw_visit_qubit(Qubit &node, void *retval) override;

    /**
     * Internal visitor function for `Real` nodes.
     */
    void raw_visit_real(Real &node, void *retval) override;

    /**
     * Internal visitor function for `RealMatrix` nodes.
     */
    void raw_visit_real_matrix(RealMatrix &node, void *retval) override;

    /**
     * Internal visitor function for `String` nodes.
     */
    void raw_visit_string(String &node, void *retval) override;

    /**
     * Internal visitor function for `TypeBase` nodes.
     */
    void raw_visit_type_base(TypeBase &node, void *retval) override;

public:

    /**
     * Fallback function for nodes of any type.
     */
    virtual T visit_node(Node &node) = 0;

    /**
     * Visitor function for `Axis` nodes.
     */
    virtual T visit_axis(Axis &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Bool` nodes.
     */
    virtual T visit_bool(Bool &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Complex` nodes.
     */
    virtual T visit_complex(Complex &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `ComplexMatrix` nodes.
     */
    virtual T visit_complex_matrix(ComplexMatrix &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Int` nodes.
     */
    virtual T visit_int(Int &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Json` nodes.
     */
    virtual T visit_json(Json &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Qubit` nodes.
     */
    virtual T visit_qubit(Qubit &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `Real` nodes.
     */
    virtual T visit_real(Real &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `RealMatrix` nodes.
     */
    virtual T visit_real_matrix(RealMatrix &node) {
        return visit_type_base(node);
    }

    /**
     * Visitor function for `String` nodes.
     */
    virtual T visit_string(String &node) {
        return visit_type_base(node);
    }

    /**
     * Fallback function for `TypeBase` nodes.
     */
    virtual T visit_type_base(TypeBase &node) {
        return visit_node(node);
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
     * Internal visitor function for `Axis` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_axis(Axis &node, void *retval) {
        if (retval == nullptr) {
            this->visit_axis(node);
        } else {
            *((T*)retval) = this->visit_axis(node);
        };
    }

    /**
     * Internal visitor function for `Axis` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_axis(Axis &node, void *retval);

    /**
     * Internal visitor function for `Bool` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_bool(Bool &node, void *retval) {
        if (retval == nullptr) {
            this->visit_bool(node);
        } else {
            *((T*)retval) = this->visit_bool(node);
        };
    }

    /**
     * Internal visitor function for `Bool` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_bool(Bool &node, void *retval);

    /**
     * Internal visitor function for `Complex` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_complex(Complex &node, void *retval) {
        if (retval == nullptr) {
            this->visit_complex(node);
        } else {
            *((T*)retval) = this->visit_complex(node);
        };
    }

    /**
     * Internal visitor function for `Complex` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_complex(Complex &node, void *retval);

    /**
     * Internal visitor function for `ComplexMatrix` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_complex_matrix(ComplexMatrix &node, void *retval) {
        if (retval == nullptr) {
            this->visit_complex_matrix(node);
        } else {
            *((T*)retval) = this->visit_complex_matrix(node);
        };
    }

    /**
     * Internal visitor function for `ComplexMatrix` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_complex_matrix(ComplexMatrix &node, void *retval);

    /**
     * Internal visitor function for `Int` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_int(Int &node, void *retval) {
        if (retval == nullptr) {
            this->visit_int(node);
        } else {
            *((T*)retval) = this->visit_int(node);
        };
    }

    /**
     * Internal visitor function for `Int` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_int(Int &node, void *retval);

    /**
     * Internal visitor function for `Json` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_json(Json &node, void *retval) {
        if (retval == nullptr) {
            this->visit_json(node);
        } else {
            *((T*)retval) = this->visit_json(node);
        };
    }

    /**
     * Internal visitor function for `Json` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_json(Json &node, void *retval);

    /**
     * Internal visitor function for `Qubit` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_qubit(Qubit &node, void *retval) {
        if (retval == nullptr) {
            this->visit_qubit(node);
        } else {
            *((T*)retval) = this->visit_qubit(node);
        };
    }

    /**
     * Internal visitor function for `Qubit` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_qubit(Qubit &node, void *retval);

    /**
     * Internal visitor function for `Real` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_real(Real &node, void *retval) {
        if (retval == nullptr) {
            this->visit_real(node);
        } else {
            *((T*)retval) = this->visit_real(node);
        };
    }

    /**
     * Internal visitor function for `Real` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_real(Real &node, void *retval);

    /**
     * Internal visitor function for `RealMatrix` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_real_matrix(RealMatrix &node, void *retval) {
        if (retval == nullptr) {
            this->visit_real_matrix(node);
        } else {
            *((T*)retval) = this->visit_real_matrix(node);
        };
    }

    /**
     * Internal visitor function for `RealMatrix` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_real_matrix(RealMatrix &node, void *retval);

    /**
     * Internal visitor function for `String` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_string(String &node, void *retval) {
        if (retval == nullptr) {
            this->visit_string(node);
        } else {
            *((T*)retval) = this->visit_string(node);
        };
    }

    /**
     * Internal visitor function for `String` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_string(String &node, void *retval);

    /**
     * Internal visitor function for `TypeBase` nodes.
     */
    template <typename T>
    void Visitor<T>::raw_visit_type_base(TypeBase &node, void *retval) {
        if (retval == nullptr) {
            this->visit_type_base(node);
        } else {
            *((T*)retval) = this->visit_type_base(node);
        };
    }

    /**
     * Internal visitor function for `TypeBase` nodes.
     */
    template <>
    void Visitor<void>::raw_visit_type_base(TypeBase &node, void *retval);

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
     * Recursive traversal for `Axis` nodes.
     */
    void visit_axis(Axis &node) override;

    /**
     * Recursive traversal for `Bool` nodes.
     */
    void visit_bool(Bool &node) override;

    /**
     * Recursive traversal for `Complex` nodes.
     */
    void visit_complex(Complex &node) override;

    /**
     * Recursive traversal for `ComplexMatrix` nodes.
     */
    void visit_complex_matrix(ComplexMatrix &node) override;

    /**
     * Recursive traversal for `Int` nodes.
     */
    void visit_int(Int &node) override;

    /**
     * Recursive traversal for `Json` nodes.
     */
    void visit_json(Json &node) override;

    /**
     * Recursive traversal for `Qubit` nodes.
     */
    void visit_qubit(Qubit &node) override;

    /**
     * Recursive traversal for `Real` nodes.
     */
    void visit_real(Real &node) override;

    /**
     * Recursive traversal for `RealMatrix` nodes.
     */
    void visit_real_matrix(RealMatrix &node) override;

    /**
     * Recursive traversal for `String` nodes.
     */
    void visit_string(String &node) override;

    /**
     * Recursive traversal for `TypeBase` nodes.
     */
    void visit_type_base(TypeBase &node) override;

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
     * Dumps a `Axis` node.
     */
    void visit_axis(Axis &node) override;

    /**
     * Dumps a `Bool` node.
     */
    void visit_bool(Bool &node) override;

    /**
     * Dumps a `Complex` node.
     */
    void visit_complex(Complex &node) override;

    /**
     * Dumps a `ComplexMatrix` node.
     */
    void visit_complex_matrix(ComplexMatrix &node) override;

    /**
     * Dumps a `Int` node.
     */
    void visit_int(Int &node) override;

    /**
     * Dumps a `Json` node.
     */
    void visit_json(Json &node) override;

    /**
     * Dumps a `Qubit` node.
     */
    void visit_qubit(Qubit &node) override;

    /**
     * Dumps a `Real` node.
     */
    void visit_real(Real &node) override;

    /**
     * Dumps a `RealMatrix` node.
     */
    void visit_real_matrix(RealMatrix &node) override;

    /**
     * Dumps a `String` node.
     */
    void visit_string(String &node) override;

    /**
     * Dumps a `TypeBase` node.
     */
    void visit_type_base(TypeBase &node) override;

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

} // namespace types
} // namespace v3
} // namespace cqasm

