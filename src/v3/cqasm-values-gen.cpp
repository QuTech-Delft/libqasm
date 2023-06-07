/** \file
 * Implementation for the various classes representing constants, references,
 * and dynamic expressions in cQASM.
 */

#include "v3/cqasm-semantic.hpp"
#include "cqasm-annotations.hpp"
#include "v3/cqasm-values-gen.hpp"

namespace cqasm {
namespace v3 {
namespace values {

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
 * Interprets this node to a node of type BitRefs. Returns null if it has the
 * wrong type.
 */
BitRefs *Node::as_bit_refs() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BitRefs. Returns null if it has the
 * wrong type.
 */
const BitRefs *Node::as_bit_refs() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstAxis. Returns null if it has the
 * wrong type.
 */
ConstAxis *Node::as_const_axis() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstAxis. Returns null if it has the
 * wrong type.
 */
const ConstAxis *Node::as_const_axis() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstBool. Returns null if it has the
 * wrong type.
 */
ConstBool *Node::as_const_bool() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstBool. Returns null if it has the
 * wrong type.
 */
const ConstBool *Node::as_const_bool() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstComplex. Returns null if it has
 * the wrong type.
 */
ConstComplex *Node::as_const_complex() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstComplex. Returns null if it has
 * the wrong type.
 */
const ConstComplex *Node::as_const_complex() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstComplexMatrix. Returns null if it
 * has the wrong type.
 */
ConstComplexMatrix *Node::as_const_complex_matrix() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstComplexMatrix. Returns null if it
 * has the wrong type.
 */
const ConstComplexMatrix *Node::as_const_complex_matrix() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstInt. Returns null if it has the
 * wrong type.
 */
ConstInt *Node::as_const_int() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstInt. Returns null if it has the
 * wrong type.
 */
const ConstInt *Node::as_const_int() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstJson. Returns null if it has the
 * wrong type.
 */
ConstJson *Node::as_const_json() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstJson. Returns null if it has the
 * wrong type.
 */
const ConstJson *Node::as_const_json() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstReal. Returns null if it has the
 * wrong type.
 */
ConstReal *Node::as_const_real() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstReal. Returns null if it has the
 * wrong type.
 */
const ConstReal *Node::as_const_real() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstRealMatrix. Returns null if it
 * has the wrong type.
 */
ConstRealMatrix *Node::as_const_real_matrix() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstRealMatrix. Returns null if it
 * has the wrong type.
 */
const ConstRealMatrix *Node::as_const_real_matrix() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstString. Returns null if it has
 * the wrong type.
 */
ConstString *Node::as_const_string() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ConstString. Returns null if it has
 * the wrong type.
 */
const ConstString *Node::as_const_string() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Constant. Returns null if it has the
 * wrong type.
 */
Constant *Node::as_constant() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Constant. Returns null if it has the
 * wrong type.
 */
const Constant *Node::as_constant() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Function. Returns null if it has the
 * wrong type.
 */
Function *Node::as_function() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Function. Returns null if it has the
 * wrong type.
 */
const Function *Node::as_function() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type QubitRefs. Returns null if it has the
 * wrong type.
 */
QubitRefs *Node::as_qubit_refs() {
    return nullptr;
}

/**
 * Interprets this node to a node of type QubitRefs. Returns null if it has the
 * wrong type.
 */
const QubitRefs *Node::as_qubit_refs() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type Reference. Returns null if it has the
 * wrong type.
 */
Reference *Node::as_reference() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Reference. Returns null if it has the
 * wrong type.
 */
const Reference *Node::as_reference() const {
    return nullptr;
}

/**
 * Interprets this node to a node of type VariableRef. Returns null if it has
 * the wrong type.
 */
VariableRef *Node::as_variable_ref() {
    return nullptr;
}

/**
 * Interprets this node to a node of type VariableRef. Returns null if it has
 * the wrong type.
 */
const VariableRef *Node::as_variable_ref() const {
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
    if (type == "BitRefs") return BitRefs::deserialize(map, ids);
    if (type == "ConstAxis") return ConstAxis::deserialize(map, ids);
    if (type == "ConstBool") return ConstBool::deserialize(map, ids);
    if (type == "ConstComplex") return ConstComplex::deserialize(map, ids);
    if (type == "ConstComplexMatrix") return ConstComplexMatrix::deserialize(map, ids);
    if (type == "ConstInt") return ConstInt::deserialize(map, ids);
    if (type == "ConstJson") return ConstJson::deserialize(map, ids);
    if (type == "ConstReal") return ConstReal::deserialize(map, ids);
    if (type == "ConstRealMatrix") return ConstRealMatrix::deserialize(map, ids);
    if (type == "ConstString") return ConstString::deserialize(map, ids);
    if (type == "Function") return Function::deserialize(map, ids);
    if (type == "QubitRefs") return QubitRefs::deserialize(map, ids);
    if (type == "VariableRef") return VariableRef::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Interprets this node to a node of type Reference. Returns null if it has the
 * wrong type.
 */
Reference *Reference::as_reference() {
    return dynamic_cast<Reference*>(this);
}

/**
 * Interprets this node to a node of type Reference. Returns null if it has the
 * wrong type.
 */
const Reference *Reference::as_reference() const {
    return dynamic_cast<const Reference*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Reference> Reference::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "QubitRefs") return QubitRefs::deserialize(map, ids);
    if (type == "BitRefs") return BitRefs::deserialize(map, ids);
    if (type == "VariableRef") return VariableRef::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
BitRefs::BitRefs(const Many<ConstInt> &index)
    : Reference(), index(index)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BitRefs::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    index.find_reachable(map);
}

/**
 * Returns whether this `BitRefs` is complete/fully defined.
 */
void BitRefs::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    index.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BitRefs::type() const {
    return NodeType::BitRefs;
}

/**
 * Helper method for visiting nodes.
 */
void BitRefs::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bit_refs(*this, retval);
}

/**
 * Interprets this node to a node of type BitRefs. Returns null if it has the
 * wrong type.
 */
BitRefs *BitRefs::as_bit_refs() {
    return dynamic_cast<BitRefs*>(this);
}

/**
 * Interprets this node to a node of type BitRefs. Returns null if it has the
 * wrong type.
 */
const BitRefs *BitRefs::as_bit_refs() const {
    return dynamic_cast<const BitRefs*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BitRefs::copy() const {
    return cqasm::tree::make<BitRefs>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BitRefs::clone() const {
    auto node = cqasm::tree::make<BitRefs>(*this);
    node->index = this->index.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BitRefs::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BitRefs) return false;
    auto rhsc = dynamic_cast<const BitRefs&>(rhs);
    if (!this->index.equals(rhsc.index)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BitRefs::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BitRefs) return false;
    auto rhsc = dynamic_cast<const BitRefs&>(rhs);
    if (this->index != rhsc.index) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BitRefs::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BitRefs");
    auto submap = map.append_map("index");
    index.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<BitRefs> BitRefs::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BitRefs") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BitRefs>(
        Many<ConstInt>(map.at("index").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Interprets this node to a node of type Constant. Returns null if it has the
 * wrong type.
 */
Constant *Constant::as_constant() {
    return dynamic_cast<Constant*>(this);
}

/**
 * Interprets this node to a node of type Constant. Returns null if it has the
 * wrong type.
 */
const Constant *Constant::as_constant() const {
    return dynamic_cast<const Constant*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Constant> Constant::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "ConstBool") return ConstBool::deserialize(map, ids);
    if (type == "ConstAxis") return ConstAxis::deserialize(map, ids);
    if (type == "ConstInt") return ConstInt::deserialize(map, ids);
    if (type == "ConstReal") return ConstReal::deserialize(map, ids);
    if (type == "ConstComplex") return ConstComplex::deserialize(map, ids);
    if (type == "ConstRealMatrix") return ConstRealMatrix::deserialize(map, ids);
    if (type == "ConstComplexMatrix") return ConstComplexMatrix::deserialize(map, ids);
    if (type == "ConstString") return ConstString::deserialize(map, ids);
    if (type == "ConstJson") return ConstJson::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
ConstAxis::ConstAxis(const cqasm::v3::primitives::Axis &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstAxis::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstAxis` is complete/fully defined.
 */
void ConstAxis::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstAxis::type() const {
    return NodeType::ConstAxis;
}

/**
 * Helper method for visiting nodes.
 */
void ConstAxis::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_axis(*this, retval);
}

/**
 * Interprets this node to a node of type ConstAxis. Returns null if it has the
 * wrong type.
 */
ConstAxis *ConstAxis::as_const_axis() {
    return dynamic_cast<ConstAxis*>(this);
}

/**
 * Interprets this node to a node of type ConstAxis. Returns null if it has the
 * wrong type.
 */
const ConstAxis *ConstAxis::as_const_axis() const {
    return dynamic_cast<const ConstAxis*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstAxis::copy() const {
    return cqasm::tree::make<ConstAxis>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstAxis::clone() const {
    auto node = cqasm::tree::make<ConstAxis>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstAxis::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstAxis) return false;
    auto rhsc = dynamic_cast<const ConstAxis&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstAxis::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstAxis) return false;
    auto rhsc = dynamic_cast<const ConstAxis&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstAxis::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstAxis");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Axis>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstAxis> ConstAxis::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstAxis") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstAxis>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Axis>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstBool::ConstBool(const cqasm::v3::primitives::Bool &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstBool::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstBool` is complete/fully defined.
 */
void ConstBool::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstBool::type() const {
    return NodeType::ConstBool;
}

/**
 * Helper method for visiting nodes.
 */
void ConstBool::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_bool(*this, retval);
}

/**
 * Interprets this node to a node of type ConstBool. Returns null if it has the
 * wrong type.
 */
ConstBool *ConstBool::as_const_bool() {
    return dynamic_cast<ConstBool*>(this);
}

/**
 * Interprets this node to a node of type ConstBool. Returns null if it has the
 * wrong type.
 */
const ConstBool *ConstBool::as_const_bool() const {
    return dynamic_cast<const ConstBool*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstBool::copy() const {
    return cqasm::tree::make<ConstBool>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstBool::clone() const {
    auto node = cqasm::tree::make<ConstBool>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstBool::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstBool) return false;
    auto rhsc = dynamic_cast<const ConstBool&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstBool::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstBool) return false;
    auto rhsc = dynamic_cast<const ConstBool&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstBool::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstBool");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Bool>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstBool> ConstBool::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstBool") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstBool>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Bool>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstComplex::ConstComplex(const cqasm::v3::primitives::Complex &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstComplex::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstComplex` is complete/fully defined.
 */
void ConstComplex::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstComplex::type() const {
    return NodeType::ConstComplex;
}

/**
 * Helper method for visiting nodes.
 */
void ConstComplex::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_complex(*this, retval);
}

/**
 * Interprets this node to a node of type ConstComplex. Returns null if it has
 * the wrong type.
 */
ConstComplex *ConstComplex::as_const_complex() {
    return dynamic_cast<ConstComplex*>(this);
}

/**
 * Interprets this node to a node of type ConstComplex. Returns null if it has
 * the wrong type.
 */
const ConstComplex *ConstComplex::as_const_complex() const {
    return dynamic_cast<const ConstComplex*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstComplex::copy() const {
    return cqasm::tree::make<ConstComplex>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstComplex::clone() const {
    auto node = cqasm::tree::make<ConstComplex>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstComplex::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstComplex) return false;
    auto rhsc = dynamic_cast<const ConstComplex&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstComplex::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstComplex) return false;
    auto rhsc = dynamic_cast<const ConstComplex&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstComplex::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstComplex");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Complex>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstComplex> ConstComplex::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstComplex") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstComplex>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Complex>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstComplexMatrix::ConstComplexMatrix(const cqasm::v3::primitives::CMatrix &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstComplexMatrix::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstComplexMatrix` is complete/fully defined.
 */
void ConstComplexMatrix::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstComplexMatrix::type() const {
    return NodeType::ConstComplexMatrix;
}

/**
 * Helper method for visiting nodes.
 */
void ConstComplexMatrix::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_complex_matrix(*this, retval);
}

/**
 * Interprets this node to a node of type ConstComplexMatrix. Returns null if it
 * has the wrong type.
 */
ConstComplexMatrix *ConstComplexMatrix::as_const_complex_matrix() {
    return dynamic_cast<ConstComplexMatrix*>(this);
}

/**
 * Interprets this node to a node of type ConstComplexMatrix. Returns null if it
 * has the wrong type.
 */
const ConstComplexMatrix *ConstComplexMatrix::as_const_complex_matrix() const {
    return dynamic_cast<const ConstComplexMatrix*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstComplexMatrix::copy() const {
    return cqasm::tree::make<ConstComplexMatrix>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstComplexMatrix::clone() const {
    auto node = cqasm::tree::make<ConstComplexMatrix>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstComplexMatrix::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstComplexMatrix) return false;
    auto rhsc = dynamic_cast<const ConstComplexMatrix&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstComplexMatrix::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstComplexMatrix) return false;
    auto rhsc = dynamic_cast<const ConstComplexMatrix&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstComplexMatrix::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstComplexMatrix");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::CMatrix>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstComplexMatrix> ConstComplexMatrix::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstComplexMatrix") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstComplexMatrix>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::CMatrix>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstInt::ConstInt(const cqasm::v3::primitives::Int &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstInt::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstInt` is complete/fully defined.
 */
void ConstInt::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstInt::type() const {
    return NodeType::ConstInt;
}

/**
 * Helper method for visiting nodes.
 */
void ConstInt::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_int(*this, retval);
}

/**
 * Interprets this node to a node of type ConstInt. Returns null if it has the
 * wrong type.
 */
ConstInt *ConstInt::as_const_int() {
    return dynamic_cast<ConstInt*>(this);
}

/**
 * Interprets this node to a node of type ConstInt. Returns null if it has the
 * wrong type.
 */
const ConstInt *ConstInt::as_const_int() const {
    return dynamic_cast<const ConstInt*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstInt::copy() const {
    return cqasm::tree::make<ConstInt>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstInt::clone() const {
    auto node = cqasm::tree::make<ConstInt>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstInt::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstInt) return false;
    auto rhsc = dynamic_cast<const ConstInt&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstInt::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstInt) return false;
    auto rhsc = dynamic_cast<const ConstInt&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstInt::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstInt");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Int>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstInt> ConstInt::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstInt") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstInt>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Int>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstJson::ConstJson(const cqasm::v3::primitives::Str &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstJson::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstJson` is complete/fully defined.
 */
void ConstJson::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstJson::type() const {
    return NodeType::ConstJson;
}

/**
 * Helper method for visiting nodes.
 */
void ConstJson::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_json(*this, retval);
}

/**
 * Interprets this node to a node of type ConstJson. Returns null if it has the
 * wrong type.
 */
ConstJson *ConstJson::as_const_json() {
    return dynamic_cast<ConstJson*>(this);
}

/**
 * Interprets this node to a node of type ConstJson. Returns null if it has the
 * wrong type.
 */
const ConstJson *ConstJson::as_const_json() const {
    return dynamic_cast<const ConstJson*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstJson::copy() const {
    return cqasm::tree::make<ConstJson>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstJson::clone() const {
    auto node = cqasm::tree::make<ConstJson>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstJson::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstJson) return false;
    auto rhsc = dynamic_cast<const ConstJson&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstJson::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstJson) return false;
    auto rhsc = dynamic_cast<const ConstJson&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstJson::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstJson");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstJson> ConstJson::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstJson") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstJson>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstReal::ConstReal(const cqasm::v3::primitives::Real &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstReal::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstReal` is complete/fully defined.
 */
void ConstReal::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstReal::type() const {
    return NodeType::ConstReal;
}

/**
 * Helper method for visiting nodes.
 */
void ConstReal::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_real(*this, retval);
}

/**
 * Interprets this node to a node of type ConstReal. Returns null if it has the
 * wrong type.
 */
ConstReal *ConstReal::as_const_real() {
    return dynamic_cast<ConstReal*>(this);
}

/**
 * Interprets this node to a node of type ConstReal. Returns null if it has the
 * wrong type.
 */
const ConstReal *ConstReal::as_const_real() const {
    return dynamic_cast<const ConstReal*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstReal::copy() const {
    return cqasm::tree::make<ConstReal>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstReal::clone() const {
    auto node = cqasm::tree::make<ConstReal>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstReal::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstReal) return false;
    auto rhsc = dynamic_cast<const ConstReal&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstReal::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstReal) return false;
    auto rhsc = dynamic_cast<const ConstReal&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstReal::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstReal");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Real>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstReal> ConstReal::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstReal") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstReal>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Real>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstRealMatrix::ConstRealMatrix(const cqasm::v3::primitives::RMatrix &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstRealMatrix::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstRealMatrix` is complete/fully defined.
 */
void ConstRealMatrix::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstRealMatrix::type() const {
    return NodeType::ConstRealMatrix;
}

/**
 * Helper method for visiting nodes.
 */
void ConstRealMatrix::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_real_matrix(*this, retval);
}

/**
 * Interprets this node to a node of type ConstRealMatrix. Returns null if it
 * has the wrong type.
 */
ConstRealMatrix *ConstRealMatrix::as_const_real_matrix() {
    return dynamic_cast<ConstRealMatrix*>(this);
}

/**
 * Interprets this node to a node of type ConstRealMatrix. Returns null if it
 * has the wrong type.
 */
const ConstRealMatrix *ConstRealMatrix::as_const_real_matrix() const {
    return dynamic_cast<const ConstRealMatrix*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstRealMatrix::copy() const {
    return cqasm::tree::make<ConstRealMatrix>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstRealMatrix::clone() const {
    auto node = cqasm::tree::make<ConstRealMatrix>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstRealMatrix::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstRealMatrix) return false;
    auto rhsc = dynamic_cast<const ConstRealMatrix&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstRealMatrix::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstRealMatrix) return false;
    auto rhsc = dynamic_cast<const ConstRealMatrix&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstRealMatrix::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstRealMatrix");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::RMatrix>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstRealMatrix> ConstRealMatrix::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstRealMatrix") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstRealMatrix>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::RMatrix>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ConstString::ConstString(const cqasm::v3::primitives::Str &value)
    : Constant(), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ConstString::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns whether this `ConstString` is complete/fully defined.
 */
void ConstString::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ConstString::type() const {
    return NodeType::ConstString;
}

/**
 * Helper method for visiting nodes.
 */
void ConstString::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_const_string(*this, retval);
}

/**
 * Interprets this node to a node of type ConstString. Returns null if it has
 * the wrong type.
 */
ConstString *ConstString::as_const_string() {
    return dynamic_cast<ConstString*>(this);
}

/**
 * Interprets this node to a node of type ConstString. Returns null if it has
 * the wrong type.
 */
const ConstString *ConstString::as_const_string() const {
    return dynamic_cast<const ConstString*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ConstString::copy() const {
    return cqasm::tree::make<ConstString>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ConstString::clone() const {
    auto node = cqasm::tree::make<ConstString>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ConstString::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstString) return false;
    auto rhsc = dynamic_cast<const ConstString&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ConstString::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ConstString) return false;
    auto rhsc = dynamic_cast<const ConstString&>(rhs);
    if (this->value != rhsc.value) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ConstString::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ConstString");
    auto submap = map.append_map("value");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(value, submap);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ConstString> ConstString::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ConstString") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ConstString>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("value").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Function::Function(const cqasm::v3::primitives::Str &name, const Any<cqasm::v3::values::Node> &operands, const One<cqasm::v3::types::Node> &return_type)
    : name(name), operands(operands), return_type(return_type)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Function::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    operands.find_reachable(map);
    return_type.find_reachable(map);
}

/**
 * Returns whether this `Function` is complete/fully defined.
 */
void Function::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    operands.check_complete(map);
    return_type.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Function::type() const {
    return NodeType::Function;
}

/**
 * Helper method for visiting nodes.
 */
void Function::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_function(*this, retval);
}

/**
 * Interprets this node to a node of type Function. Returns null if it has the
 * wrong type.
 */
Function *Function::as_function() {
    return dynamic_cast<Function*>(this);
}

/**
 * Interprets this node to a node of type Function. Returns null if it has the
 * wrong type.
 */
const Function *Function::as_function() const {
    return dynamic_cast<const Function*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Function::copy() const {
    return cqasm::tree::make<Function>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Function::clone() const {
    auto node = cqasm::tree::make<Function>(*this);
    node->operands = this->operands.clone();
    node->return_type = this->return_type.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Function::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Function) return false;
    auto rhsc = dynamic_cast<const Function&>(rhs);
    if (this->name != rhsc.name) return false;
    if (!this->operands.equals(rhsc.operands)) return false;
    if (!this->return_type.equals(rhsc.return_type)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Function::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Function) return false;
    auto rhsc = dynamic_cast<const Function&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->operands != rhsc.operands) return false;
    if (this->return_type != rhsc.return_type) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Function::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Function");
    auto submap = map.append_map("name");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
    submap.close();
    submap = map.append_map("operands");
    operands.serialize(submap, ids);
    submap.close();
    submap = map.append_map("return_type");
    return_type.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Function> Function::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Function") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Function>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        Any<cqasm::v3::values::Node>(map.at("operands").as_map(), ids),
        One<cqasm::v3::types::Node>(map.at("return_type").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
QubitRefs::QubitRefs(const Many<ConstInt> &index)
    : Reference(), index(index)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void QubitRefs::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    index.find_reachable(map);
}

/**
 * Returns whether this `QubitRefs` is complete/fully defined.
 */
void QubitRefs::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    index.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType QubitRefs::type() const {
    return NodeType::QubitRefs;
}

/**
 * Helper method for visiting nodes.
 */
void QubitRefs::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_qubit_refs(*this, retval);
}

/**
 * Interprets this node to a node of type QubitRefs. Returns null if it has the
 * wrong type.
 */
QubitRefs *QubitRefs::as_qubit_refs() {
    return dynamic_cast<QubitRefs*>(this);
}

/**
 * Interprets this node to a node of type QubitRefs. Returns null if it has the
 * wrong type.
 */
const QubitRefs *QubitRefs::as_qubit_refs() const {
    return dynamic_cast<const QubitRefs*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> QubitRefs::copy() const {
    return cqasm::tree::make<QubitRefs>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> QubitRefs::clone() const {
    auto node = cqasm::tree::make<QubitRefs>(*this);
    node->index = this->index.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool QubitRefs::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::QubitRefs) return false;
    auto rhsc = dynamic_cast<const QubitRefs&>(rhs);
    if (!this->index.equals(rhsc.index)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool QubitRefs::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::QubitRefs) return false;
    auto rhsc = dynamic_cast<const QubitRefs&>(rhs);
    if (this->index != rhsc.index) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void QubitRefs::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "QubitRefs");
    auto submap = map.append_map("index");
    index.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<QubitRefs> QubitRefs::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "QubitRefs") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<QubitRefs>(
        Many<ConstInt>(map.at("index").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
VariableRef::VariableRef(const Link<cqasm::v3::semantic::Variable> &variable)
    : Reference(), variable(variable)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void VariableRef::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    variable.find_reachable(map);
}

/**
 * Returns whether this `VariableRef` is complete/fully defined.
 */
void VariableRef::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    variable.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType VariableRef::type() const {
    return NodeType::VariableRef;
}

/**
 * Helper method for visiting nodes.
 */
void VariableRef::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_variable_ref(*this, retval);
}

/**
 * Interprets this node to a node of type VariableRef. Returns null if it has
 * the wrong type.
 */
VariableRef *VariableRef::as_variable_ref() {
    return dynamic_cast<VariableRef*>(this);
}

/**
 * Interprets this node to a node of type VariableRef. Returns null if it has
 * the wrong type.
 */
const VariableRef *VariableRef::as_variable_ref() const {
    return dynamic_cast<const VariableRef*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> VariableRef::copy() const {
    return cqasm::tree::make<VariableRef>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> VariableRef::clone() const {
    auto node = cqasm::tree::make<VariableRef>(*this);
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool VariableRef::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::VariableRef) return false;
    auto rhsc = dynamic_cast<const VariableRef&>(rhs);
    if (!this->variable.equals(rhsc.variable)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool VariableRef::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::VariableRef) return false;
    auto rhsc = dynamic_cast<const VariableRef&>(rhs);
    if (this->variable != rhsc.variable) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void VariableRef::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "VariableRef");
    auto submap = map.append_map("variable");
    variable.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<VariableRef> VariableRef::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "VariableRef") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<VariableRef>(
        Link<cqasm::v3::semantic::Variable>(map.at("variable").as_map(), ids)
    );
    auto link = map.at("variable").as_map().at("@l");
    if (!link.is_null()) {
        ids.register_link(node->variable, link.as_int());
    }
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
 * Internal visitor function for `BitRefs` nodes.
 */
template <>
void Visitor<void>::raw_visit_bit_refs(BitRefs &node, void *retval) {
    (void)retval;
    this->visit_bit_refs(node);
}

/**
 * Internal visitor function for `ConstAxis` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_axis(ConstAxis &node, void *retval) {
    (void)retval;
    this->visit_const_axis(node);
}

/**
 * Internal visitor function for `ConstBool` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_bool(ConstBool &node, void *retval) {
    (void)retval;
    this->visit_const_bool(node);
}

/**
 * Internal visitor function for `ConstComplex` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_complex(ConstComplex &node, void *retval) {
    (void)retval;
    this->visit_const_complex(node);
}

/**
 * Internal visitor function for `ConstComplexMatrix` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_complex_matrix(ConstComplexMatrix &node, void *retval) {
    (void)retval;
    this->visit_const_complex_matrix(node);
}

/**
 * Internal visitor function for `ConstInt` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_int(ConstInt &node, void *retval) {
    (void)retval;
    this->visit_const_int(node);
}

/**
 * Internal visitor function for `ConstJson` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_json(ConstJson &node, void *retval) {
    (void)retval;
    this->visit_const_json(node);
}

/**
 * Internal visitor function for `ConstReal` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_real(ConstReal &node, void *retval) {
    (void)retval;
    this->visit_const_real(node);
}

/**
 * Internal visitor function for `ConstRealMatrix` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_real_matrix(ConstRealMatrix &node, void *retval) {
    (void)retval;
    this->visit_const_real_matrix(node);
}

/**
 * Internal visitor function for `ConstString` nodes.
 */
template <>
void Visitor<void>::raw_visit_const_string(ConstString &node, void *retval) {
    (void)retval;
    this->visit_const_string(node);
}

/**
 * Internal visitor function for `Constant` nodes.
 */
template <>
void Visitor<void>::raw_visit_constant(Constant &node, void *retval) {
    (void)retval;
    this->visit_constant(node);
}

/**
 * Internal visitor function for `Function` nodes.
 */
template <>
void Visitor<void>::raw_visit_function(Function &node, void *retval) {
    (void)retval;
    this->visit_function(node);
}

/**
 * Internal visitor function for `QubitRefs` nodes.
 */
template <>
void Visitor<void>::raw_visit_qubit_refs(QubitRefs &node, void *retval) {
    (void)retval;
    this->visit_qubit_refs(node);
}

/**
 * Internal visitor function for `Reference` nodes.
 */
template <>
void Visitor<void>::raw_visit_reference(Reference &node, void *retval) {
    (void)retval;
    this->visit_reference(node);
}

/**
 * Internal visitor function for `VariableRef` nodes.
 */
template <>
void Visitor<void>::raw_visit_variable_ref(VariableRef &node, void *retval) {
    (void)retval;
    this->visit_variable_ref(node);
}

/**
 * Recursive traversal for `BitRefs` nodes.
 */
void RecursiveVisitor::visit_bit_refs(BitRefs &node) {
    visit_reference(node);
    node.index.visit(*this);
}

/**
 * Recursive traversal for `ConstAxis` nodes.
 */
void RecursiveVisitor::visit_const_axis(ConstAxis &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstBool` nodes.
 */
void RecursiveVisitor::visit_const_bool(ConstBool &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstComplex` nodes.
 */
void RecursiveVisitor::visit_const_complex(ConstComplex &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstComplexMatrix` nodes.
 */
void RecursiveVisitor::visit_const_complex_matrix(ConstComplexMatrix &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstInt` nodes.
 */
void RecursiveVisitor::visit_const_int(ConstInt &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstJson` nodes.
 */
void RecursiveVisitor::visit_const_json(ConstJson &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstReal` nodes.
 */
void RecursiveVisitor::visit_const_real(ConstReal &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstRealMatrix` nodes.
 */
void RecursiveVisitor::visit_const_real_matrix(ConstRealMatrix &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `ConstString` nodes.
 */
void RecursiveVisitor::visit_const_string(ConstString &node) {
    visit_constant(node);
}

/**
 * Recursive traversal for `Constant` nodes.
 */
void RecursiveVisitor::visit_constant(Constant &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `Function` nodes.
 */
void RecursiveVisitor::visit_function(Function &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `QubitRefs` nodes.
 */
void RecursiveVisitor::visit_qubit_refs(QubitRefs &node) {
    visit_reference(node);
    node.index.visit(*this);
}

/**
 * Recursive traversal for `Reference` nodes.
 */
void RecursiveVisitor::visit_reference(Reference &node) {
    visit_node(node);
}

/**
 * Recursive traversal for `VariableRef` nodes.
 */
void RecursiveVisitor::visit_variable_ref(VariableRef &node) {
    visit_reference(node);
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
 * Dumps a `BitRefs` node.
 */
void Dumper::visit_bit_refs(BitRefs &node) {
    write_indent();
    out << "BitRefs";
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
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.index) {
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
 * Dumps a `ConstAxis` node.
 */
void Dumper::visit_const_axis(ConstAxis &node) {
    write_indent();
    out << "ConstAxis";
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
        out << "cqasm::v3::primitives::Axis<<" << std::endl;
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
 * Dumps a `ConstBool` node.
 */
void Dumper::visit_const_bool(ConstBool &node) {
    write_indent();
    out << "ConstBool";
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
        out << "cqasm::v3::primitives::Bool<<" << std::endl;
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
 * Dumps a `ConstComplex` node.
 */
void Dumper::visit_const_complex(ConstComplex &node) {
    write_indent();
    out << "ConstComplex";
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
        out << "cqasm::v3::primitives::Complex<<" << std::endl;
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
 * Dumps a `ConstComplexMatrix` node.
 */
void Dumper::visit_const_complex_matrix(ConstComplexMatrix &node) {
    write_indent();
    out << "ConstComplexMatrix";
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
        out << "cqasm::v3::primitives::CMatrix<<" << std::endl;
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
 * Dumps a `ConstInt` node.
 */
void Dumper::visit_const_int(ConstInt &node) {
    write_indent();
    out << "ConstInt";
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
        out << "cqasm::v3::primitives::Int<<" << std::endl;
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
 * Dumps a `ConstJson` node.
 */
void Dumper::visit_const_json(ConstJson &node) {
    write_indent();
    out << "ConstJson";
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
        out << "cqasm::v3::primitives::Str<<" << std::endl;
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
 * Dumps a `ConstReal` node.
 */
void Dumper::visit_const_real(ConstReal &node) {
    write_indent();
    out << "ConstReal";
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
        out << "cqasm::v3::primitives::Real<<" << std::endl;
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
 * Dumps a `ConstRealMatrix` node.
 */
void Dumper::visit_const_real_matrix(ConstRealMatrix &node) {
    write_indent();
    out << "ConstRealMatrix";
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
        out << "cqasm::v3::primitives::RMatrix<<" << std::endl;
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
 * Dumps a `ConstString` node.
 */
void Dumper::visit_const_string(ConstString &node) {
    write_indent();
    out << "ConstString";
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
        out << "cqasm::v3::primitives::Str<<" << std::endl;
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
 * Dumps a `Constant` node.
 */
void Dumper::visit_constant(Constant &node) {
    write_indent();
    out << "Constant";
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
 * Dumps a `Function` node.
 */
void Dumper::visit_function(Function &node) {
    write_indent();
    out << "Function";
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
        out << "cqasm::v3::primitives::Str<<" << std::endl;
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
    out << "operands: ";
    if (node.operands.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.operands) {
            if (!sptr.empty()) {
                sptr->dump(out, indent);
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
    out << "return_type: ";
    if (node.return_type.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!node.return_type.empty()) {
            node.return_type->dump(out, indent);
        }
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `QubitRefs` node.
 */
void Dumper::visit_qubit_refs(QubitRefs &node) {
    write_indent();
    out << "QubitRefs";
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
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.index) {
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
 * Dumps a `Reference` node.
 */
void Dumper::visit_reference(Reference &node) {
    write_indent();
    out << "Reference";
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
 * Dumps a `VariableRef` node.
 */
void Dumper::visit_variable_ref(VariableRef &node) {
    write_indent();
    out << "VariableRef";
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
    out << "variable --> ";
    if (node.variable.empty()) {
        out << "!MISSING" << std::endl;
    } else if (ids != nullptr && ids->get(node.variable) != (size_t)-1) {
        out << "Link<cqasm::v3::semantic::Variable>@" << ids->get(node.variable) << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!in_link) {
            in_link = true;
            if (!node.variable.empty()) {
                node.variable->dump(out, indent);
            }
            in_link = false;
        } else {
            write_indent();
            out << "..." << std::endl;
        }
        indent--;
        write_indent();
        out << ">" << std::endl;
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

} // namespace values
} // namespace v3
} // namespace cqasm

