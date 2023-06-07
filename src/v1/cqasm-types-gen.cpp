/** \file
 * Implementation for the various classes representing the types of values
 * available in cQASM.
 */

#include "v1/cqasm-semantic.hpp"
#include "v1/cqasm-types-gen.hpp"

namespace cqasm {
namespace v1 {
namespace types {

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
 * Interprets this node to a node of type Axis. Returns null if it has the wrong
 * type.
 */
Axis *Node::as_axis() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Axis. Returns null if it has the wrong
 * type.
 */
const Axis *Node::as_axis() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Bool. Returns null if it has the wrong
 * type.
 */
Bool *Node::as_bool() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Bool. Returns null if it has the wrong
 * type.
 */
const Bool *Node::as_bool() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Complex. Returns null if it has the
 * wrong type.
 */
Complex *Node::as_complex() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Complex. Returns null if it has the
 * wrong type.
 */
const Complex *Node::as_complex() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ComplexMatrix. Returns null if it has
 * the wrong type.
 */
ComplexMatrix *Node::as_complex_matrix() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ComplexMatrix. Returns null if it has
 * the wrong type.
 */
const ComplexMatrix *Node::as_complex_matrix() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Int. Returns null if it has the wrong
 * type.
 */
Int *Node::as_int() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Int. Returns null if it has the wrong
 * type.
 */
const Int *Node::as_int() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Json. Returns null if it has the wrong
 * type.
 */
Json *Node::as_json() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Json. Returns null if it has the wrong
 * type.
 */
const Json *Node::as_json() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Qubit. Returns null if it has the
 * wrong type.
 */
Qubit *Node::as_qubit() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Qubit. Returns null if it has the
 * wrong type.
 */
const Qubit *Node::as_qubit() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Real. Returns null if it has the wrong
 * type.
 */
Real *Node::as_real() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Real. Returns null if it has the wrong
 * type.
 */
const Real *Node::as_real() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type RealMatrix. Returns null if it has the
 * wrong type.
 */
RealMatrix *Node::as_real_matrix() {
    return nullptr;
}

/**
 * Interprets this node to a node of type RealMatrix. Returns null if it has the
 * wrong type.
 */
const RealMatrix *Node::as_real_matrix() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type String. Returns null if it has the
 * wrong type.
 */
String *Node::as_string() {
    return nullptr;
}

/**
 * Interprets this node to a node of type String. Returns null if it has the
 * wrong type.
 */
const String *Node::as_string() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type TypeBase. Returns null if it has the
 * wrong type.
 */
TypeBase *Node::as_type_base() {
    return nullptr;
}

/**
 * Interprets this node to a node of type TypeBase. Returns null if it has the
 * wrong type.
 */
const TypeBase *Node::as_type_base() const {
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
    if (type == "Axis") return Axis::deserialize(map, ids);
    if (type == "Bool") return Bool::deserialize(map, ids);
    if (type == "Complex") return Complex::deserialize(map, ids);
    if (type == "ComplexMatrix") return ComplexMatrix::deserialize(map, ids);
    if (type == "Int") return Int::deserialize(map, ids);
    if (type == "Json") return Json::deserialize(map, ids);
    if (type == "Qubit") return Qubit::deserialize(map, ids);
    if (type == "Real") return Real::deserialize(map, ids);
    if (type == "RealMatrix") return RealMatrix::deserialize(map, ids);
    if (type == "String") return String::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
TypeBase::TypeBase(const cqasm::v1::primitives::Bool &assignable)
    : assignable(assignable)
{}

/**
 * Interprets this node to a node of type TypeBase. Returns null if it has the
 * wrong type.
 */
TypeBase *TypeBase::as_type_base() {
    return dynamic_cast<TypeBase*>(this);
}

/**
 * Interprets this node to a node of type TypeBase. Returns null if it has the
 * wrong type.
 */
const TypeBase *TypeBase::as_type_base() const {
    return dynamic_cast<const TypeBase*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<TypeBase> TypeBase::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Qubit") return Qubit::deserialize(map, ids);
    if (type == "Bool") return Bool::deserialize(map, ids);
    if (type == "Axis") return Axis::deserialize(map, ids);
    if (type == "Int") return Int::deserialize(map, ids);
    if (type == "Real") return Real::deserialize(map, ids);
    if (type == "Complex") return Complex::deserialize(map, ids);
    if (type == "RealMatrix") return RealMatrix::deserialize(map, ids);
    if (type == "ComplexMatrix") return ComplexMatrix::deserialize(map, ids);
    if (type == "String") return String::deserialize(map, ids);
    if (type == "Json") return Json::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
Axis::Axis(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Axis::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Axis` is complete/fully defined.
 */
void Axis::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Axis::type() const {
    return NodeType::Axis;
}

/**
 * Helper method for visiting nodes.
 */
void Axis::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_axis(*this, retval);
}

/**
 * Interprets this node to a node of type Axis. Returns null if it has the wrong
 * type.
 */
Axis *Axis::as_axis() {
    return dynamic_cast<Axis*>(this);
}

/**
 * Interprets this node to a node of type Axis. Returns null if it has the wrong
 * type.
 */
const Axis *Axis::as_axis() const {
    return dynamic_cast<const Axis*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Axis::copy() const {
    return cqasm::tree::make<Axis>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Axis::clone() const {
    auto node = cqasm::tree::make<Axis>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Axis::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Axis) return false;
    auto rhsc = dynamic_cast<const Axis&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Axis::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Axis) return false;
    auto rhsc = dynamic_cast<const Axis&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Axis::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Axis");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Axis> Axis::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Axis") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Axis>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Bool::Bool(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Bool::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Bool` is complete/fully defined.
 */
void Bool::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Bool::type() const {
    return NodeType::Bool;
}

/**
 * Helper method for visiting nodes.
 */
void Bool::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bool(*this, retval);
}

/**
 * Interprets this node to a node of type Bool. Returns null if it has the wrong
 * type.
 */
Bool *Bool::as_bool() {
    return dynamic_cast<Bool*>(this);
}

/**
 * Interprets this node to a node of type Bool. Returns null if it has the wrong
 * type.
 */
const Bool *Bool::as_bool() const {
    return dynamic_cast<const Bool*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Bool::copy() const {
    return cqasm::tree::make<Bool>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Bool::clone() const {
    auto node = cqasm::tree::make<Bool>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Bool::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Bool) return false;
    auto rhsc = dynamic_cast<const Bool&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Bool::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Bool) return false;
    auto rhsc = dynamic_cast<const Bool&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Bool::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Bool");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Bool> Bool::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Bool") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Bool>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Complex::Complex(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Complex::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Complex` is complete/fully defined.
 */
void Complex::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Complex::type() const {
    return NodeType::Complex;
}

/**
 * Helper method for visiting nodes.
 */
void Complex::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_complex(*this, retval);
}

/**
 * Interprets this node to a node of type Complex. Returns null if it has the
 * wrong type.
 */
Complex *Complex::as_complex() {
    return dynamic_cast<Complex*>(this);
}

/**
 * Interprets this node to a node of type Complex. Returns null if it has the
 * wrong type.
 */
const Complex *Complex::as_complex() const {
    return dynamic_cast<const Complex*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Complex::copy() const {
    return cqasm::tree::make<Complex>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Complex::clone() const {
    auto node = cqasm::tree::make<Complex>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Complex::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Complex) return false;
    auto rhsc = dynamic_cast<const Complex&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Complex::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Complex) return false;
    auto rhsc = dynamic_cast<const Complex&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Complex::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Complex");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Complex> Complex::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Complex") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Complex>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ComplexMatrix::ComplexMatrix(const cqasm::v1::primitives::Int &num_rows, const cqasm::v1::primitives::Int &num_cols, const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable), num_rows(num_rows), num_cols(num_cols)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ComplexMatrix::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ComplexMatrix` is complete/fully defined.
 */
void ComplexMatrix::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ComplexMatrix::type() const {
    return NodeType::ComplexMatrix;
}

/**
 * Helper method for visiting nodes.
 */
void ComplexMatrix::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_complex_matrix(*this, retval);
}

/**
 * Interprets this node to a node of type ComplexMatrix. Returns null if it has
 * the wrong type.
 */
ComplexMatrix *ComplexMatrix::as_complex_matrix() {
    return dynamic_cast<ComplexMatrix*>(this);
}

/**
 * Interprets this node to a node of type ComplexMatrix. Returns null if it has
 * the wrong type.
 */
const ComplexMatrix *ComplexMatrix::as_complex_matrix() const {
    return dynamic_cast<const ComplexMatrix*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ComplexMatrix::copy() const {
    return cqasm::tree::make<ComplexMatrix>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ComplexMatrix::clone() const {
    auto node = cqasm::tree::make<ComplexMatrix>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ComplexMatrix::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ComplexMatrix) return false;
    auto rhsc = dynamic_cast<const ComplexMatrix&>(rhs);
    if (this->num_rows != rhsc.num_rows) return false;
    if (this->num_cols != rhsc.num_cols) return false;
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ComplexMatrix::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ComplexMatrix) return false;
    auto rhsc = dynamic_cast<const ComplexMatrix&>(rhs);
    if (this->num_rows != rhsc.num_rows) return false;
    if (this->num_cols != rhsc.num_cols) return false;
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ComplexMatrix::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ComplexMatrix");
    auto submap = map.append_map("num_rows");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Int>(num_rows, submap);
    submap.close();
    submap = map.append_map("num_cols");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Int>(num_cols, submap);
    submap.close();
    submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ComplexMatrix> ComplexMatrix::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ComplexMatrix") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ComplexMatrix>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Int>(map.at("num_rows").as_map()),
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Int>(map.at("num_cols").as_map()),
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Int::Int(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Int::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Int` is complete/fully defined.
 */
void Int::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Int::type() const {
    return NodeType::Int;
}

/**
 * Helper method for visiting nodes.
 */
void Int::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_int(*this, retval);
}

/**
 * Interprets this node to a node of type Int. Returns null if it has the wrong
 * type.
 */
Int *Int::as_int() {
    return dynamic_cast<Int*>(this);
}

/**
 * Interprets this node to a node of type Int. Returns null if it has the wrong
 * type.
 */
const Int *Int::as_int() const {
    return dynamic_cast<const Int*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Int::copy() const {
    return cqasm::tree::make<Int>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Int::clone() const {
    auto node = cqasm::tree::make<Int>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Int::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Int) return false;
    auto rhsc = dynamic_cast<const Int&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Int::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Int) return false;
    auto rhsc = dynamic_cast<const Int&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Int::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Int");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Int> Int::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Int") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Int>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Json::Json(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Json::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Json` is complete/fully defined.
 */
void Json::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Json::type() const {
    return NodeType::Json;
}

/**
 * Helper method for visiting nodes.
 */
void Json::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_json(*this, retval);
}

/**
 * Interprets this node to a node of type Json. Returns null if it has the wrong
 * type.
 */
Json *Json::as_json() {
    return dynamic_cast<Json*>(this);
}

/**
 * Interprets this node to a node of type Json. Returns null if it has the wrong
 * type.
 */
const Json *Json::as_json() const {
    return dynamic_cast<const Json*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Json::copy() const {
    return cqasm::tree::make<Json>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Json::clone() const {
    auto node = cqasm::tree::make<Json>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Json::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Json) return false;
    auto rhsc = dynamic_cast<const Json&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Json::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Json) return false;
    auto rhsc = dynamic_cast<const Json&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Json::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Json");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Json> Json::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Json") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Json>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Qubit::Qubit(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Qubit::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Qubit` is complete/fully defined.
 */
void Qubit::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Qubit::type() const {
    return NodeType::Qubit;
}

/**
 * Helper method for visiting nodes.
 */
void Qubit::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_qubit(*this, retval);
}

/**
 * Interprets this node to a node of type Qubit. Returns null if it has the
 * wrong type.
 */
Qubit *Qubit::as_qubit() {
    return dynamic_cast<Qubit*>(this);
}

/**
 * Interprets this node to a node of type Qubit. Returns null if it has the
 * wrong type.
 */
const Qubit *Qubit::as_qubit() const {
    return dynamic_cast<const Qubit*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Qubit::copy() const {
    return cqasm::tree::make<Qubit>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Qubit::clone() const {
    auto node = cqasm::tree::make<Qubit>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Qubit::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Qubit) return false;
    auto rhsc = dynamic_cast<const Qubit&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Qubit::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Qubit) return false;
    auto rhsc = dynamic_cast<const Qubit&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Qubit::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Qubit");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Qubit> Qubit::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Qubit") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Qubit>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Real::Real(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Real::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `Real` is complete/fully defined.
 */
void Real::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Real::type() const {
    return NodeType::Real;
}

/**
 * Helper method for visiting nodes.
 */
void Real::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_real(*this, retval);
}

/**
 * Interprets this node to a node of type Real. Returns null if it has the wrong
 * type.
 */
Real *Real::as_real() {
    return dynamic_cast<Real*>(this);
}

/**
 * Interprets this node to a node of type Real. Returns null if it has the wrong
 * type.
 */
const Real *Real::as_real() const {
    return dynamic_cast<const Real*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Real::copy() const {
    return cqasm::tree::make<Real>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Real::clone() const {
    auto node = cqasm::tree::make<Real>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Real::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Real) return false;
    auto rhsc = dynamic_cast<const Real&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Real::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Real) return false;
    auto rhsc = dynamic_cast<const Real&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Real::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Real");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Real> Real::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Real") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Real>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
RealMatrix::RealMatrix(const cqasm::v1::primitives::Int &num_rows, const cqasm::v1::primitives::Int &num_cols, const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable), num_rows(num_rows), num_cols(num_cols)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void RealMatrix::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `RealMatrix` is complete/fully defined.
 */
void RealMatrix::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType RealMatrix::type() const {
    return NodeType::RealMatrix;
}

/**
 * Helper method for visiting nodes.
 */
void RealMatrix::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_real_matrix(*this, retval);
}

/**
 * Interprets this node to a node of type RealMatrix. Returns null if it has the
 * wrong type.
 */
RealMatrix *RealMatrix::as_real_matrix() {
    return dynamic_cast<RealMatrix*>(this);
}

/**
 * Interprets this node to a node of type RealMatrix. Returns null if it has the
 * wrong type.
 */
const RealMatrix *RealMatrix::as_real_matrix() const {
    return dynamic_cast<const RealMatrix*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> RealMatrix::copy() const {
    return cqasm::tree::make<RealMatrix>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> RealMatrix::clone() const {
    auto node = cqasm::tree::make<RealMatrix>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool RealMatrix::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::RealMatrix) return false;
    auto rhsc = dynamic_cast<const RealMatrix&>(rhs);
    if (this->num_rows != rhsc.num_rows) return false;
    if (this->num_cols != rhsc.num_cols) return false;
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool RealMatrix::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::RealMatrix) return false;
    auto rhsc = dynamic_cast<const RealMatrix&>(rhs);
    if (this->num_rows != rhsc.num_rows) return false;
    if (this->num_cols != rhsc.num_cols) return false;
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void RealMatrix::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "RealMatrix");
    auto submap = map.append_map("num_rows");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Int>(num_rows, submap);
    submap.close();
    submap = map.append_map("num_cols");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Int>(num_cols, submap);
    submap.close();
    submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<RealMatrix> RealMatrix::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "RealMatrix") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<RealMatrix>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Int>(map.at("num_rows").as_map()),
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Int>(map.at("num_cols").as_map()),
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
String::String(const cqasm::v1::primitives::Bool &assignable)
    : TypeBase(assignable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void String::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `String` is complete/fully defined.
 */
void String::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType String::type() const {
    return NodeType::String;
}

/**
 * Helper method for visiting nodes.
 */
void String::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_string(*this, retval);
}

/**
 * Interprets this node to a node of type String. Returns null if it has the
 * wrong type.
 */
String *String::as_string() {
    return dynamic_cast<String*>(this);
}

/**
 * Interprets this node to a node of type String. Returns null if it has the
 * wrong type.
 */
const String *String::as_string() const {
    return dynamic_cast<const String*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> String::copy() const {
    return cqasm::tree::make<String>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> String::clone() const {
    auto node = cqasm::tree::make<String>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool String::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::String) return false;
    auto rhsc = dynamic_cast<const String&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool String::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::String) return false;
    auto rhsc = dynamic_cast<const String&>(rhs);
    if (this->assignable != rhsc.assignable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void String::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "String");
    auto submap = map.append_map("assignable");
    cqasm::v1::primitives::serialize<cqasm::v1::primitives::Bool>(assignable, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<String> String::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "String") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<String>(
        cqasm::v1::primitives::deserialize<cqasm::v1::primitives::Bool>(map.at("assignable").as_map())
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
 * Internal visitor function for `Axis` nodes.
 */
template <>
void Visitor<void>::raw_visit_axis(Axis &node, void *retval) {
    (void)retval;
    this->visit_axis(node);
}

/**
 * Internal visitor function for `Bool` nodes.
 */
template <>
void Visitor<void>::raw_visit_bool(Bool &node, void *retval) {
    (void)retval;
    this->visit_bool(node);
}

/**
 * Internal visitor function for `Complex` nodes.
 */
template <>
void Visitor<void>::raw_visit_complex(Complex &node, void *retval) {
    (void)retval;
    this->visit_complex(node);
}

/**
 * Internal visitor function for `ComplexMatrix` nodes.
 */
template <>
void Visitor<void>::raw_visit_complex_matrix(ComplexMatrix &node, void *retval) {
    (void)retval;
    this->visit_complex_matrix(node);
}

/**
 * Internal visitor function for `Int` nodes.
 */
template <>
void Visitor<void>::raw_visit_int(Int &node, void *retval) {
    (void)retval;
    this->visit_int(node);
}

/**
 * Internal visitor function for `Json` nodes.
 */
template <>
void Visitor<void>::raw_visit_json(Json &node, void *retval) {
    (void)retval;
    this->visit_json(node);
}

/**
 * Internal visitor function for `Qubit` nodes.
 */
template <>
void Visitor<void>::raw_visit_qubit(Qubit &node, void *retval) {
    (void)retval;
    this->visit_qubit(node);
}

/**
 * Internal visitor function for `Real` nodes.
 */
template <>
void Visitor<void>::raw_visit_real(Real &node, void *retval) {
    (void)retval;
    this->visit_real(node);
}

/**
 * Internal visitor function for `RealMatrix` nodes.
 */
template <>
void Visitor<void>::raw_visit_real_matrix(RealMatrix &node, void *retval) {
    (void)retval;
    this->visit_real_matrix(node);
}

/**
 * Internal visitor function for `String` nodes.
 */
template <>
void Visitor<void>::raw_visit_string(String &node, void *retval) {
    (void)retval;
    this->visit_string(node);
}

/**
 * Internal visitor function for `TypeBase` nodes.
 */
template <>
void Visitor<void>::raw_visit_type_base(TypeBase &node, void *retval) {
    (void)retval;
    this->visit_type_base(node);
}

/**
 * Recursive traversal for `Axis` nodes.
 */
void RecursiveVisitor::visit_axis(Axis &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Bool` nodes.
 */
void RecursiveVisitor::visit_bool(Bool &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Complex` nodes.
 */
void RecursiveVisitor::visit_complex(Complex &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `ComplexMatrix` nodes.
 */
void RecursiveVisitor::visit_complex_matrix(ComplexMatrix &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Int` nodes.
 */
void RecursiveVisitor::visit_int(Int &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Json` nodes.
 */
void RecursiveVisitor::visit_json(Json &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Qubit` nodes.
 */
void RecursiveVisitor::visit_qubit(Qubit &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `Real` nodes.
 */
void RecursiveVisitor::visit_real(Real &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `RealMatrix` nodes.
 */
void RecursiveVisitor::visit_real_matrix(RealMatrix &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `String` nodes.
 */
void RecursiveVisitor::visit_string(String &node) {
    visit_type_base(node);
}

/**
 * Recursive traversal for `TypeBase` nodes.
 */
void RecursiveVisitor::visit_type_base(TypeBase &node) {
    visit_node(node);
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
 * Dumps a `Axis` node.
 */
void Dumper::visit_axis(Axis &node) {
    write_indent();
    out << "Axis";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Bool` node.
 */
void Dumper::visit_bool(Bool &node) {
    write_indent();
    out << "Bool";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Complex` node.
 */
void Dumper::visit_complex(Complex &node) {
    write_indent();
    out << "Complex";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `ComplexMatrix` node.
 */
void Dumper::visit_complex_matrix(ComplexMatrix &node) {
    write_indent();
    out << "ComplexMatrix";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "num_rows: ";
    std::stringstream ss;
    size_t pos;
    ss << node.num_rows;
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
    write_indent();
    out << "num_cols: ";
    ss.str("");
    ss.clear();
    ss << node.num_cols;
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
    write_indent();
    out << "assignable: ";
    ss.str("");
    ss.clear();
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Int` node.
 */
void Dumper::visit_int(Int &node) {
    write_indent();
    out << "Int";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Json` node.
 */
void Dumper::visit_json(Json &node) {
    write_indent();
    out << "Json";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Qubit` node.
 */
void Dumper::visit_qubit(Qubit &node) {
    write_indent();
    out << "Qubit";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `Real` node.
 */
void Dumper::visit_real(Real &node) {
    write_indent();
    out << "Real";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `RealMatrix` node.
 */
void Dumper::visit_real_matrix(RealMatrix &node) {
    write_indent();
    out << "RealMatrix";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "num_rows: ";
    std::stringstream ss;
    size_t pos;
    ss << node.num_rows;
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
    write_indent();
    out << "num_cols: ";
    ss.str("");
    ss.clear();
    ss << node.num_cols;
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
    write_indent();
    out << "assignable: ";
    ss.str("");
    ss.clear();
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `String` node.
 */
void Dumper::visit_string(String &node) {
    write_indent();
    out << "String";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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
 * Dumps a `TypeBase` node.
 */
void Dumper::visit_type_base(TypeBase &node) {
    write_indent();
    out << "TypeBase";
    if (ids != nullptr) {
        out << "@" << ids->get_ref(node);
    }
    out << "(";
    out << std::endl;
    indent++;
    write_indent();
    out << "assignable: ";
    std::stringstream ss;
    size_t pos;
    ss << node.assignable;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v1::primitives::Bool<<" << std::endl;
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

} // namespace types
} // namespace v1
} // namespace cqasm

