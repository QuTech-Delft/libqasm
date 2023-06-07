/** \file
 * Implementation for the semantic tree node classes.
 */

#include "cqasm-annotations.hpp"
#include "v3/cqasm-semantic-gen.hpp"

namespace cqasm {
namespace v3 {
namespace semantic {

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
 * Interprets this node to a node of type Block. Returns null if it has the
 * wrong type.
 */
Block *Node::as_block() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Block. Returns null if it has the
 * wrong type.
 */
const Block *Node::as_block() const {
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
 * Interprets this node to a node of type BundleExt. Returns null if it has the
 * wrong type.
 */
BundleExt *Node::as_bundle_ext() {
    return nullptr;
}

/**
 * Interprets this node to a node of type BundleExt. Returns null if it has the
 * wrong type.
 */
const BundleExt *Node::as_bundle_ext() const {
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
 * Interprets this node to a node of type ErrorModel. Returns null if it has the
 * wrong type.
 */
ErrorModel *Node::as_error_model() {
    return nullptr;
}

/**
 * Interprets this node to a node of type ErrorModel. Returns null if it has the
 * wrong type.
 */
const ErrorModel *Node::as_error_model() const {
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
 * Interprets this node to a node of type GotoInstruction. Returns null if it
 * has the wrong type.
 */
GotoInstruction *Node::as_goto_instruction() {
    return nullptr;
}

/**
 * Interprets this node to a node of type GotoInstruction. Returns null if it
 * has the wrong type.
 */
const GotoInstruction *Node::as_goto_instruction() const {
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
 * Interprets this node to a node of type InstructionBase. Returns null if it
 * has the wrong type.
 */
InstructionBase *Node::as_instruction_base() {
    return nullptr;
}

/**
 * Interprets this node to a node of type InstructionBase. Returns null if it
 * has the wrong type.
 */
const InstructionBase *Node::as_instruction_base() const {
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
 * Interprets this node to a node of type SetInstruction. Returns null if it has
 * the wrong type.
 */
SetInstruction *Node::as_set_instruction() {
    return nullptr;
}

/**
 * Interprets this node to a node of type SetInstruction. Returns null if it has
 * the wrong type.
 */
const SetInstruction *Node::as_set_instruction() const {
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
 * Interprets this node to a node of type Variable. Returns null if it has the
 * wrong type.
 */
Variable *Node::as_variable() {
    return nullptr;
}

/**
 * Interprets this node to a node of type Variable. Returns null if it has the
 * wrong type.
 */
const Variable *Node::as_variable() const {
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
    if (type == "AnnotationData") return AnnotationData::deserialize(map, ids);
    if (type == "Block") return Block::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "Bundle") return Bundle::deserialize(map, ids);
    if (type == "BundleExt") return BundleExt::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    if (type == "ErrorModel") return ErrorModel::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "GotoInstruction") return GotoInstruction::deserialize(map, ids);
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "IfElseBranch") return IfElseBranch::deserialize(map, ids);
    if (type == "Instruction") return Instruction::deserialize(map, ids);
    if (type == "Mapping") return Mapping::deserialize(map, ids);
    if (type == "Program") return Program::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "SetInstruction") return SetInstruction::deserialize(map, ids);
    if (type == "Subcircuit") return Subcircuit::deserialize(map, ids);
    if (type == "Variable") return Variable::deserialize(map, ids);
    if (type == "Version") return Version::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
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
    if (type == "ErrorModel") return ErrorModel::deserialize(map, ids);
    if (type == "Instruction") return Instruction::deserialize(map, ids);
    if (type == "SetInstruction") return SetInstruction::deserialize(map, ids);
    if (type == "GotoInstruction") return GotoInstruction::deserialize(map, ids);
    if (type == "Bundle") return Bundle::deserialize(map, ids);
    if (type == "BundleExt") return BundleExt::deserialize(map, ids);
    if (type == "IfElse") return IfElse::deserialize(map, ids);
    if (type == "ForLoop") return ForLoop::deserialize(map, ids);
    if (type == "ForeachLoop") return ForeachLoop::deserialize(map, ids);
    if (type == "WhileLoop") return WhileLoop::deserialize(map, ids);
    if (type == "RepeatUntilLoop") return RepeatUntilLoop::deserialize(map, ids);
    if (type == "BreakStatement") return BreakStatement::deserialize(map, ids);
    if (type == "ContinueStatement") return ContinueStatement::deserialize(map, ids);
    if (type == "Subcircuit") return Subcircuit::deserialize(map, ids);
    if (type == "Mapping") return Mapping::deserialize(map, ids);
    if (type == "Variable") return Variable::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
AnnotationData::AnnotationData(const cqasm::v3::primitives::Str &interface, const cqasm::v3::primitives::Str &operation, const Any<cqasm::v3::values::Node> &operands)
    : interface(interface), operation(operation), operands(operands)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void AnnotationData::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    operands.find_reachable(map);
}

/**
 * Returns whether this `AnnotationData` is complete/fully defined.
 */
void AnnotationData::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
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
    node->operands = this->operands.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool AnnotationData::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::AnnotationData) return false;
    auto rhsc = dynamic_cast<const AnnotationData&>(rhs);
    if (this->interface != rhsc.interface) return false;
    if (this->operation != rhsc.operation) return false;
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
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(interface, submap);
    submap.close();
    submap = map.append_map("operation");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(operation, submap);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("interface").as_map()),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("operation").as_map()),
        Any<cqasm::v3::values::Node>(map.at("operands").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Block::Block(const Any<Statement> &statements)
    : statements(statements)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Block::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    statements.find_reachable(map);
}

/**
 * Returns whether this `Block` is complete/fully defined.
 */
void Block::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    statements.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Block::type() const {
    return NodeType::Block;
}

/**
 * Helper method for visiting nodes.
 */
void Block::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_block(*this, retval);
}

/**
 * Interprets this node to a node of type Block. Returns null if it has the
 * wrong type.
 */
Block *Block::as_block() {
    return dynamic_cast<Block*>(this);
}

/**
 * Interprets this node to a node of type Block. Returns null if it has the
 * wrong type.
 */
const Block *Block::as_block() const {
    return dynamic_cast<const Block*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Block::copy() const {
    return cqasm::tree::make<Block>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Block::clone() const {
    auto node = cqasm::tree::make<Block>(*this);
    node->statements = this->statements.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Block::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Block) return false;
    auto rhsc = dynamic_cast<const Block&>(rhs);
    if (!this->statements.equals(rhsc.statements)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Block::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Block) return false;
    auto rhsc = dynamic_cast<const Block&>(rhs);
    if (this->statements != rhsc.statements) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Block::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Block");
    auto submap = map.append_map("statements");
    statements.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<Block> Block::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Block") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Block>(
        Any<Statement>(map.at("statements").as_map(), ids)
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
    if (type == "BundleExt") return BundleExt::deserialize(map, ids);
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
    : Annotated(annotations), items(items)
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
BundleExt::BundleExt(const Many<InstructionBase> &items, const Any<AnnotationData> &annotations)
    : Statement(annotations), items(items)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void BundleExt::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    items.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `BundleExt` is complete/fully defined.
 */
void BundleExt::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    items.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType BundleExt::type() const {
    return NodeType::BundleExt;
}

/**
 * Helper method for visiting nodes.
 */
void BundleExt::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_bundle_ext(*this, retval);
}

/**
 * Interprets this node to a node of type BundleExt. Returns null if it has the
 * wrong type.
 */
BundleExt *BundleExt::as_bundle_ext() {
    return dynamic_cast<BundleExt*>(this);
}

/**
 * Interprets this node to a node of type BundleExt. Returns null if it has the
 * wrong type.
 */
const BundleExt *BundleExt::as_bundle_ext() const {
    return dynamic_cast<const BundleExt*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> BundleExt::copy() const {
    return cqasm::tree::make<BundleExt>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> BundleExt::clone() const {
    auto node = cqasm::tree::make<BundleExt>(*this);
    node->items = this->items.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool BundleExt::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::BundleExt) return false;
    auto rhsc = dynamic_cast<const BundleExt&>(rhs);
    if (!this->items.equals(rhsc.items)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool BundleExt::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::BundleExt) return false;
    auto rhsc = dynamic_cast<const BundleExt&>(rhs);
    if (this->items != rhsc.items) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void BundleExt::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "BundleExt");
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
std::shared_ptr<BundleExt> BundleExt::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "BundleExt") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<BundleExt>(
        Many<InstructionBase>(map.at("items").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
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
ErrorModel::ErrorModel(const cqasm::v3::error_model::ErrorModelRef &model, const cqasm::v3::primitives::Str &name, const Any<cqasm::v3::values::Node> &parameters, const Any<AnnotationData> &annotations)
    : Annotated(annotations), model(model), name(name), parameters(parameters)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ErrorModel::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    parameters.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ErrorModel` is complete/fully defined.
 */
void ErrorModel::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    parameters.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType ErrorModel::type() const {
    return NodeType::ErrorModel;
}

/**
 * Helper method for visiting nodes.
 */
void ErrorModel::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_error_model(*this, retval);
}

/**
 * Interprets this node to a node of type ErrorModel. Returns null if it has the
 * wrong type.
 */
ErrorModel *ErrorModel::as_error_model() {
    return dynamic_cast<ErrorModel*>(this);
}

/**
 * Interprets this node to a node of type ErrorModel. Returns null if it has the
 * wrong type.
 */
const ErrorModel *ErrorModel::as_error_model() const {
    return dynamic_cast<const ErrorModel*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> ErrorModel::copy() const {
    return cqasm::tree::make<ErrorModel>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> ErrorModel::clone() const {
    auto node = cqasm::tree::make<ErrorModel>(*this);
    node->parameters = this->parameters.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool ErrorModel::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::ErrorModel) return false;
    auto rhsc = dynamic_cast<const ErrorModel&>(rhs);
    if (this->model != rhsc.model) return false;
    if (this->name != rhsc.name) return false;
    if (!this->parameters.equals(rhsc.parameters)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool ErrorModel::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::ErrorModel) return false;
    auto rhsc = dynamic_cast<const ErrorModel&>(rhs);
    if (this->model != rhsc.model) return false;
    if (this->name != rhsc.name) return false;
    if (this->parameters != rhsc.parameters) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void ErrorModel::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "ErrorModel");
    auto submap = map.append_map("model");
    cqasm::v3::primitives::serialize<cqasm::v3::error_model::ErrorModelRef>(model, submap);
    submap.close();
    submap = map.append_map("name");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
    submap.close();
    submap = map.append_map("parameters");
    parameters.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    serialize_annotations(map);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<ErrorModel> ErrorModel::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "ErrorModel") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<ErrorModel>(
        cqasm::v3::primitives::deserialize<cqasm::v3::error_model::ErrorModelRef>(map.at("model").as_map()),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        Any<cqasm::v3::values::Node>(map.at("parameters").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ForLoop::ForLoop(const Maybe<SetInstruction> &initialize, const One<cqasm::v3::values::Node> &condition, const Maybe<SetInstruction> &update, const One<Block> &body, const Any<AnnotationData> &annotations)
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
        Maybe<SetInstruction>(map.at("initialize").as_map(), ids),
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        Maybe<SetInstruction>(map.at("update").as_map(), ids),
        One<Block>(map.at("body").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
ForeachLoop::ForeachLoop(const One<cqasm::v3::values::Node> &lhs, const cqasm::v3::primitives::Int &frm, const cqasm::v3::primitives::Int &to, const One<Block> &body, const Any<AnnotationData> &annotations)
    : Structured(annotations), lhs(lhs), frm(frm), to(to), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void ForeachLoop::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    body.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `ForeachLoop` is complete/fully defined.
 */
void ForeachLoop::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
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
    if (this->frm != rhsc.frm) return false;
    if (this->to != rhsc.to) return false;
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
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Int>(frm, submap);
    submap.close();
    submap = map.append_map("to");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Int>(to, submap);
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
        One<cqasm::v3::values::Node>(map.at("lhs").as_map(), ids),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Int>(map.at("frm").as_map()),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Int>(map.at("to").as_map()),
        One<Block>(map.at("body").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
InstructionBase::InstructionBase(const One<cqasm::v3::values::Node> &condition, const Any<AnnotationData> &annotations)
    : Annotated(annotations), condition(condition)
{}

/**
 * Interprets this node to a node of type InstructionBase. Returns null if it
 * has the wrong type.
 */
InstructionBase *InstructionBase::as_instruction_base() {
    return dynamic_cast<InstructionBase*>(this);
}

/**
 * Interprets this node to a node of type InstructionBase. Returns null if it
 * has the wrong type.
 */
const InstructionBase *InstructionBase::as_instruction_base() const {
    return dynamic_cast<const InstructionBase*>(this);
}

/**
 * Writes a debug dump of this node to the given stream.
 */
std::shared_ptr<InstructionBase> InstructionBase::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    auto type = map.at("@t").as_string();
    if (type == "Instruction") return Instruction::deserialize(map, ids);
    if (type == "SetInstruction") return SetInstruction::deserialize(map, ids);
    if (type == "GotoInstruction") return GotoInstruction::deserialize(map, ids);
    throw std::runtime_error("Schema validation failed: unexpected node type " + type);
}

/**
 * Constructor.
 */
GotoInstruction::GotoInstruction(const Link<Subcircuit> &target, const One<cqasm::v3::values::Node> &condition, const Any<AnnotationData> &annotations)
    : InstructionBase(condition, annotations), target(target)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void GotoInstruction::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    target.find_reachable(map);
    condition.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `GotoInstruction` is complete/fully defined.
 */
void GotoInstruction::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    target.check_complete(map);
    condition.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType GotoInstruction::type() const {
    return NodeType::GotoInstruction;
}

/**
 * Helper method for visiting nodes.
 */
void GotoInstruction::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_goto_instruction(*this, retval);
}

/**
 * Interprets this node to a node of type GotoInstruction. Returns null if it
 * has the wrong type.
 */
GotoInstruction *GotoInstruction::as_goto_instruction() {
    return dynamic_cast<GotoInstruction*>(this);
}

/**
 * Interprets this node to a node of type GotoInstruction. Returns null if it
 * has the wrong type.
 */
const GotoInstruction *GotoInstruction::as_goto_instruction() const {
    return dynamic_cast<const GotoInstruction*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> GotoInstruction::copy() const {
    return cqasm::tree::make<GotoInstruction>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> GotoInstruction::clone() const {
    auto node = cqasm::tree::make<GotoInstruction>(*this);
    node->condition = this->condition.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool GotoInstruction::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::GotoInstruction) return false;
    auto rhsc = dynamic_cast<const GotoInstruction&>(rhs);
    if (!this->target.equals(rhsc.target)) return false;
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool GotoInstruction::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::GotoInstruction) return false;
    auto rhsc = dynamic_cast<const GotoInstruction&>(rhs);
    if (this->target != rhsc.target) return false;
    if (this->condition != rhsc.condition) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void GotoInstruction::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "GotoInstruction");
    auto submap = map.append_map("target");
    target.serialize(submap, ids);
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
std::shared_ptr<GotoInstruction> GotoInstruction::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "GotoInstruction") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<GotoInstruction>(
        Link<Subcircuit>(map.at("target").as_map(), ids),
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    auto link = map.at("target").as_map().at("@l");
    if (!link.is_null()) {
        ids.register_link(node->target, link.as_int());
    }
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IfElse::IfElse(const Many<IfElseBranch> &branches, const Maybe<Block> &otherwise, const Any<AnnotationData> &annotations)
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
        Maybe<Block>(map.at("otherwise").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
IfElseBranch::IfElseBranch(const One<cqasm::v3::values::Node> &condition, const One<Block> &body)
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
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        One<Block>(map.at("body").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Instruction::Instruction(const cqasm::v3::instruction::InstructionRef &instruction, const cqasm::v3::primitives::Str &name, const One<cqasm::v3::values::Node> &condition, const Any<cqasm::v3::values::Node> &operands, const Any<AnnotationData> &annotations)
    : InstructionBase(condition, annotations), instruction(instruction), name(name), operands(operands)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Instruction::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    condition.find_reachable(map);
    operands.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Instruction` is complete/fully defined.
 */
void Instruction::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
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
    if (this->instruction != rhsc.instruction) return false;
    if (this->name != rhsc.name) return false;
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
    if (this->instruction != rhsc.instruction) return false;
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
    auto submap = map.append_map("instruction");
    cqasm::v3::primitives::serialize<cqasm::v3::instruction::InstructionRef>(instruction, submap);
    submap.close();
    submap = map.append_map("name");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::instruction::InstructionRef>(map.at("instruction").as_map()),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        Any<cqasm::v3::values::Node>(map.at("operands").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Mapping::Mapping(const cqasm::v3::primitives::Str &name, const One<cqasm::v3::values::Node> &value, const Any<AnnotationData> &annotations)
    : Annotated(annotations), name(name), value(value)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Mapping::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    value.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Mapping` is complete/fully defined.
 */
void Mapping::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    value.check_complete(map);
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
    node->value = this->value.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Mapping::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Mapping) return false;
    auto rhsc = dynamic_cast<const Mapping&>(rhs);
    if (this->name != rhsc.name) return false;
    if (!this->value.equals(rhsc.value)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Mapping::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Mapping) return false;
    auto rhsc = dynamic_cast<const Mapping&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->value != rhsc.value) return false;
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
    auto submap = map.append_map("name");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
    submap.close();
    submap = map.append_map("value");
    value.serialize(submap, ids);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        One<cqasm::v3::values::Node>(map.at("value").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Program::Program(const One<Version> &version, const cqasm::v3::primitives::Int &num_qubits, const Maybe<ErrorModel> &error_model, const Any<Subcircuit> &subcircuits, const Any<Mapping> &mappings, const Any<Variable> &variables, const cqasm::v3::primitives::Version &api_version)
    : version(version), num_qubits(num_qubits), error_model(error_model), subcircuits(subcircuits), mappings(mappings), variables(variables), api_version(api_version)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Program::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    version.find_reachable(map);
    error_model.find_reachable(map);
    subcircuits.find_reachable(map);
    mappings.find_reachable(map);
    variables.find_reachable(map);
}

/**
 * Returns whether this `Program` is complete/fully defined.
 */
void Program::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    version.check_complete(map);
    error_model.check_complete(map);
    subcircuits.check_complete(map);
    mappings.check_complete(map);
    variables.check_complete(map);
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
    node->error_model = this->error_model.clone();
    node->subcircuits = this->subcircuits.clone();
    node->mappings = this->mappings.clone();
    node->variables = this->variables.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Program::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Program) return false;
    auto rhsc = dynamic_cast<const Program&>(rhs);
    if (!this->version.equals(rhsc.version)) return false;
    if (this->num_qubits != rhsc.num_qubits) return false;
    if (!this->error_model.equals(rhsc.error_model)) return false;
    if (!this->subcircuits.equals(rhsc.subcircuits)) return false;
    if (!this->mappings.equals(rhsc.mappings)) return false;
    if (!this->variables.equals(rhsc.variables)) return false;
    if (this->api_version != rhsc.api_version) return false;
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
    if (this->error_model != rhsc.error_model) return false;
    if (this->subcircuits != rhsc.subcircuits) return false;
    if (this->mappings != rhsc.mappings) return false;
    if (this->variables != rhsc.variables) return false;
    if (this->api_version != rhsc.api_version) return false;
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
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Int>(num_qubits, submap);
    submap.close();
    submap = map.append_map("error_model");
    error_model.serialize(submap, ids);
    submap.close();
    submap = map.append_map("subcircuits");
    subcircuits.serialize(submap, ids);
    submap.close();
    submap = map.append_map("mappings");
    mappings.serialize(submap, ids);
    submap.close();
    submap = map.append_map("variables");
    variables.serialize(submap, ids);
    submap.close();
    submap = map.append_map("api_version");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Version>(api_version, submap);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Int>(map.at("num_qubits").as_map()),
        Maybe<ErrorModel>(map.at("error_model").as_map(), ids),
        Any<Subcircuit>(map.at("subcircuits").as_map(), ids),
        Any<Mapping>(map.at("mappings").as_map(), ids),
        Any<Variable>(map.at("variables").as_map(), ids),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Version>(map.at("api_version").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
RepeatUntilLoop::RepeatUntilLoop(const One<Block> &body, const One<cqasm::v3::values::Node> &condition, const Any<AnnotationData> &annotations)
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
        One<Block>(map.at("body").as_map(), ids),
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
SetInstruction::SetInstruction(const One<cqasm::v3::values::Node> &lhs, const One<cqasm::v3::values::Node> &rhs, const One<cqasm::v3::values::Node> &condition, const Any<AnnotationData> &annotations)
    : InstructionBase(condition, annotations), lhs(lhs), rhs(rhs)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void SetInstruction::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    lhs.find_reachable(map);
    rhs.find_reachable(map);
    condition.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `SetInstruction` is complete/fully defined.
 */
void SetInstruction::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    lhs.check_complete(map);
    rhs.check_complete(map);
    condition.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType SetInstruction::type() const {
    return NodeType::SetInstruction;
}

/**
 * Helper method for visiting nodes.
 */
void SetInstruction::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_set_instruction(*this, retval);
}

/**
 * Interprets this node to a node of type SetInstruction. Returns null if it has
 * the wrong type.
 */
SetInstruction *SetInstruction::as_set_instruction() {
    return dynamic_cast<SetInstruction*>(this);
}

/**
 * Interprets this node to a node of type SetInstruction. Returns null if it has
 * the wrong type.
 */
const SetInstruction *SetInstruction::as_set_instruction() const {
    return dynamic_cast<const SetInstruction*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> SetInstruction::copy() const {
    return cqasm::tree::make<SetInstruction>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> SetInstruction::clone() const {
    auto node = cqasm::tree::make<SetInstruction>(*this);
    node->lhs = this->lhs.clone();
    node->rhs = this->rhs.clone();
    node->condition = this->condition.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool SetInstruction::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::SetInstruction) return false;
    auto rhsc = dynamic_cast<const SetInstruction&>(rhs);
    if (!this->lhs.equals(rhsc.lhs)) return false;
    if (!this->rhs.equals(rhsc.rhs)) return false;
    if (!this->condition.equals(rhsc.condition)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool SetInstruction::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::SetInstruction) return false;
    auto rhsc = dynamic_cast<const SetInstruction&>(rhs);
    if (this->lhs != rhsc.lhs) return false;
    if (this->rhs != rhsc.rhs) return false;
    if (this->condition != rhsc.condition) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void SetInstruction::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "SetInstruction");
    auto submap = map.append_map("lhs");
    lhs.serialize(submap, ids);
    submap.close();
    submap = map.append_map("rhs");
    rhs.serialize(submap, ids);
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
std::shared_ptr<SetInstruction> SetInstruction::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "SetInstruction") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<SetInstruction>(
        One<cqasm::v3::values::Node>(map.at("lhs").as_map(), ids),
        One<cqasm::v3::values::Node>(map.at("rhs").as_map(), ids),
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Subcircuit::Subcircuit(const cqasm::v3::primitives::Str &name, const cqasm::v3::primitives::Int &iterations, const Any<Bundle> &bundles, const Any<AnnotationData> &annotations, const Maybe<Block> &body)
    : Annotated(annotations), name(name), iterations(iterations), bundles(bundles), body(body)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Subcircuit::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    bundles.find_reachable(map);
    annotations.find_reachable(map);
    body.find_reachable(map);
}

/**
 * Returns whether this `Subcircuit` is complete/fully defined.
 */
void Subcircuit::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    bundles.check_complete(map);
    annotations.check_complete(map);
    body.check_complete(map);
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
    node->bundles = this->bundles.clone();
    node->annotations = this->annotations.clone();
    node->body = this->body.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Subcircuit::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Subcircuit) return false;
    auto rhsc = dynamic_cast<const Subcircuit&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->iterations != rhsc.iterations) return false;
    if (!this->bundles.equals(rhsc.bundles)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    if (!this->body.equals(rhsc.body)) return false;
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
    if (this->bundles != rhsc.bundles) return false;
    if (this->annotations != rhsc.annotations) return false;
    if (this->body != rhsc.body) return false;
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
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
    submap.close();
    submap = map.append_map("iterations");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Int>(iterations, submap);
    submap.close();
    submap = map.append_map("bundles");
    bundles.serialize(submap, ids);
    submap.close();
    submap = map.append_map("annotations");
    annotations.serialize(submap, ids);
    submap.close();
    submap = map.append_map("body");
    body.serialize(submap, ids);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Int>(map.at("iterations").as_map()),
        Any<Bundle>(map.at("bundles").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids),
        Maybe<Block>(map.at("body").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Variable::Variable(const cqasm::v3::primitives::Str &name, const One<cqasm::v3::types::Node> &typ, const Any<AnnotationData> &annotations)
    : Annotated(annotations), name(name), typ(typ)
{}

/**
 * Registers all reachable nodes with the given PointerMap.
 */
void Variable::find_reachable(::tree::base::PointerMap &map) const {
    (void)map;
    typ.find_reachable(map);
    annotations.find_reachable(map);
}

/**
 * Returns whether this `Variable` is complete/fully defined.
 */
void Variable::check_complete(const ::tree::base::PointerMap &map) const {
    (void)map;
    typ.check_complete(map);
    annotations.check_complete(map);
}

/**
 * Returns the `NodeType` of this node.
 */
NodeType Variable::type() const {
    return NodeType::Variable;
}

/**
 * Helper method for visiting nodes.
 */
void Variable::visit_internal(VisitorBase &visitor, void *retval) {
    visitor.raw_visit_variable(*this, retval);
}

/**
 * Interprets this node to a node of type Variable. Returns null if it has the
 * wrong type.
 */
Variable *Variable::as_variable() {
    return dynamic_cast<Variable*>(this);
}

/**
 * Interprets this node to a node of type Variable. Returns null if it has the
 * wrong type.
 */
const Variable *Variable::as_variable() const {
    return dynamic_cast<const Variable*>(this);
}

/**
 * Returns a shallow copy of this node.
 */
One<Node> Variable::copy() const {
    return cqasm::tree::make<Variable>(*this);
}

/**
 * Returns a deep copy of this node.
 */
One<Node> Variable::clone() const {
    auto node = cqasm::tree::make<Variable>(*this);
    node->typ = this->typ.clone();
    node->annotations = this->annotations.clone();
    return node;
}

/**
 * Value-based equality operator. Ignores annotations!
 */
bool Variable::equals(const Node &rhs) const {
    if (rhs.type() != NodeType::Variable) return false;
    auto rhsc = dynamic_cast<const Variable&>(rhs);
    if (this->name != rhsc.name) return false;
    if (!this->typ.equals(rhsc.typ)) return false;
    if (!this->annotations.equals(rhsc.annotations)) return false;
    return true;
}

/**
 * Pointer-based equality operator.
 */
bool Variable::operator==(const Node &rhs) const {
    if (rhs.type() != NodeType::Variable) return false;
    auto rhsc = dynamic_cast<const Variable&>(rhs);
    if (this->name != rhsc.name) return false;
    if (this->typ != rhsc.typ) return false;
    if (this->annotations != rhsc.annotations) return false;
    return true;
}

/**
 * Serializes this node to the given map.
 */
void Variable::serialize(
    ::tree::cbor::MapWriter &map,
    const ::tree::base::PointerMap &ids
) const {
    (void)ids;
    map.append_string("@t", "Variable");
    auto submap = map.append_map("name");
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Str>(name, submap);
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
std::shared_ptr<Variable> Variable::deserialize(const ::tree::cbor::MapReader &map, ::tree::base::IdentifierMap &ids) {
    (void)ids;
    auto type = map.at("@t").as_string();
    if (type != "Variable") {
        throw std::runtime_error("Schema validation failed: unexpected node type " + type);
    }
    auto node = std::make_shared<Variable>(
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Str>(map.at("name").as_map()),
        One<cqasm::v3::types::Node>(map.at("typ").as_map(), ids),
        Any<AnnotationData>(map.at("annotations").as_map(), ids)
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
Version::Version(const cqasm::v3::primitives::Version &items)
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
    cqasm::v3::primitives::serialize<cqasm::v3::primitives::Version>(items, submap);
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
        cqasm::v3::primitives::deserialize<cqasm::v3::primitives::Version>(map.at("items").as_map())
    );
    node->deserialize_annotations(map);
    return node;
}

/**
 * Constructor.
 */
WhileLoop::WhileLoop(const One<cqasm::v3::values::Node> &condition, const One<Block> &body, const Any<AnnotationData> &annotations)
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
        One<cqasm::v3::values::Node>(map.at("condition").as_map(), ids),
        One<Block>(map.at("body").as_map(), ids),
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
 * Internal visitor function for `Block` nodes.
 */
template <>
void Visitor<void>::raw_visit_block(Block &node, void *retval) {
    (void)retval;
    this->visit_block(node);
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
 * Internal visitor function for `BundleExt` nodes.
 */
template <>
void Visitor<void>::raw_visit_bundle_ext(BundleExt &node, void *retval) {
    (void)retval;
    this->visit_bundle_ext(node);
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
 * Internal visitor function for `ErrorModel` nodes.
 */
template <>
void Visitor<void>::raw_visit_error_model(ErrorModel &node, void *retval) {
    (void)retval;
    this->visit_error_model(node);
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
 * Internal visitor function for `GotoInstruction` nodes.
 */
template <>
void Visitor<void>::raw_visit_goto_instruction(GotoInstruction &node, void *retval) {
    (void)retval;
    this->visit_goto_instruction(node);
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
 * Internal visitor function for `Instruction` nodes.
 */
template <>
void Visitor<void>::raw_visit_instruction(Instruction &node, void *retval) {
    (void)retval;
    this->visit_instruction(node);
}

/**
 * Internal visitor function for `InstructionBase` nodes.
 */
template <>
void Visitor<void>::raw_visit_instruction_base(InstructionBase &node, void *retval) {
    (void)retval;
    this->visit_instruction_base(node);
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
 * Internal visitor function for `SetInstruction` nodes.
 */
template <>
void Visitor<void>::raw_visit_set_instruction(SetInstruction &node, void *retval) {
    (void)retval;
    this->visit_set_instruction(node);
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
 * Internal visitor function for `Variable` nodes.
 */
template <>
void Visitor<void>::raw_visit_variable(Variable &node, void *retval) {
    (void)retval;
    this->visit_variable(node);
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
}

/**
 * Recursive traversal for `Block` nodes.
 */
void RecursiveVisitor::visit_block(Block &node) {
    visit_node(node);
    node.statements.visit(*this);
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
    visit_annotated(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `BundleExt` nodes.
 */
void RecursiveVisitor::visit_bundle_ext(BundleExt &node) {
    visit_statement(node);
    node.items.visit(*this);
}

/**
 * Recursive traversal for `ContinueStatement` nodes.
 */
void RecursiveVisitor::visit_continue_statement(ContinueStatement &node) {
    visit_structured(node);
}

/**
 * Recursive traversal for `ErrorModel` nodes.
 */
void RecursiveVisitor::visit_error_model(ErrorModel &node) {
    visit_annotated(node);
}

/**
 * Recursive traversal for `ForLoop` nodes.
 */
void RecursiveVisitor::visit_for_loop(ForLoop &node) {
    visit_structured(node);
    node.initialize.visit(*this);
    node.update.visit(*this);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `ForeachLoop` nodes.
 */
void RecursiveVisitor::visit_foreach_loop(ForeachLoop &node) {
    visit_structured(node);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `GotoInstruction` nodes.
 */
void RecursiveVisitor::visit_goto_instruction(GotoInstruction &node) {
    visit_instruction_base(node);
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
    node.body.visit(*this);
}

/**
 * Recursive traversal for `Instruction` nodes.
 */
void RecursiveVisitor::visit_instruction(Instruction &node) {
    visit_instruction_base(node);
}

/**
 * Recursive traversal for `InstructionBase` nodes.
 */
void RecursiveVisitor::visit_instruction_base(InstructionBase &node) {
    visit_annotated(node);
}

/**
 * Recursive traversal for `Mapping` nodes.
 */
void RecursiveVisitor::visit_mapping(Mapping &node) {
    visit_annotated(node);
}

/**
 * Recursive traversal for `Program` nodes.
 */
void RecursiveVisitor::visit_program(Program &node) {
    visit_node(node);
    node.version.visit(*this);
    node.error_model.visit(*this);
    node.subcircuits.visit(*this);
    node.mappings.visit(*this);
    node.variables.visit(*this);
}

/**
 * Recursive traversal for `RepeatUntilLoop` nodes.
 */
void RecursiveVisitor::visit_repeat_until_loop(RepeatUntilLoop &node) {
    visit_structured(node);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `SetInstruction` nodes.
 */
void RecursiveVisitor::visit_set_instruction(SetInstruction &node) {
    visit_instruction_base(node);
}

/**
 * Recursive traversal for `Statement` nodes.
 */
void RecursiveVisitor::visit_statement(Statement &node) {
    visit_annotated(node);
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
    visit_annotated(node);
    node.bundles.visit(*this);
    node.body.visit(*this);
}

/**
 * Recursive traversal for `Variable` nodes.
 */
void RecursiveVisitor::visit_variable(Variable &node) {
    visit_annotated(node);
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
    std::stringstream ss;
    size_t pos;
    ss << node.interface;
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
    out << "operation: ";
    ss.str("");
    ss.clear();
    ss << node.operation;
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
    indent--;
    write_indent();
    out << ")" << std::endl;
}

/**
 * Dumps a `Block` node.
 */
void Dumper::visit_block(Block &node) {
    write_indent();
    out << "Block";
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
    out << "statements: ";
    if (node.statements.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.statements) {
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
 * Dumps a `BundleExt` node.
 */
void Dumper::visit_bundle_ext(BundleExt &node) {
    write_indent();
    out << "BundleExt";
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
 * Dumps a `ErrorModel` node.
 */
void Dumper::visit_error_model(ErrorModel &node) {
    write_indent();
    out << "ErrorModel";
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
    out << "model: ";
    std::stringstream ss;
    size_t pos;
    ss << node.model;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v3::error_model::ErrorModelRef<<" << std::endl;
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
    out << "name: ";
    ss.str("");
    ss.clear();
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
    out << "parameters: ";
    if (node.parameters.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.parameters) {
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
        if (!node.lhs.empty()) {
            node.lhs->dump(out, indent);
        }
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "frm: ";
    std::stringstream ss;
    size_t pos;
    ss << node.frm;
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
    write_indent();
    out << "to: ";
    ss.str("");
    ss.clear();
    ss << node.to;
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
 * Dumps a `GotoInstruction` node.
 */
void Dumper::visit_goto_instruction(GotoInstruction &node) {
    write_indent();
    out << "GotoInstruction";
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
    out << "target --> ";
    if (node.target.empty()) {
        out << "!MISSING" << std::endl;
    } else if (ids != nullptr && ids->get(node.target) != (size_t)-1) {
        out << "Subcircuit@" << ids->get(node.target) << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!in_link) {
            in_link = true;
            node.target.visit(*this);
            in_link = false;
        } else {
            write_indent();
            out << "..." << std::endl;
        }
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
    out << "instruction: ";
    std::stringstream ss;
    size_t pos;
    ss << node.instruction;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v3::instruction::InstructionRef<<" << std::endl;
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
    out << "name: ";
    ss.str("");
    ss.clear();
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
    out << "condition: ";
    if (node.condition.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
        indent--;
        write_indent();
        out << ">" << std::endl;
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
 * Dumps a `InstructionBase` node.
 */
void Dumper::visit_instruction_base(InstructionBase &node) {
    write_indent();
    out << "InstructionBase";
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
    out << "value: ";
    if (node.value.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!node.value.empty()) {
            node.value->dump(out, indent);
        }
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
    std::stringstream ss;
    size_t pos;
    ss << node.num_qubits;
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
    write_indent();
    out << "error_model: ";
    if (node.error_model.empty()) {
        out << "-" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        node.error_model.visit(*this);
        indent--;
        write_indent();
        out << ">" << std::endl;
    }
    write_indent();
    out << "subcircuits: ";
    if (node.subcircuits.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.subcircuits) {
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
    out << "mappings: ";
    if (node.mappings.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.mappings) {
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
    out << "variables: ";
    if (node.variables.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.variables) {
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
    out << "api_version: ";
    ss.str("");
    ss.clear();
    ss << node.api_version;
    pos = ss.str().find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        ss.str(ss.str().erase(pos+1));
    }
    if (ss.str().find('\n') == std::string::npos) {
        out << ss.str() << std::endl;
    } else {
        out << "cqasm::v3::primitives::Version<<" << std::endl;
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
 * Dumps a `SetInstruction` node.
 */
void Dumper::visit_set_instruction(SetInstruction &node) {
    write_indent();
    out << "SetInstruction";
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
        if (!node.lhs.empty()) {
            node.lhs->dump(out, indent);
        }
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
        if (!node.rhs.empty()) {
            node.rhs->dump(out, indent);
        }
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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
    out << "iterations: ";
    ss.str("");
    ss.clear();
    ss << node.iterations;
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
    write_indent();
    out << "bundles: ";
    if (node.bundles.empty()) {
        out << "[]" << std::endl;
    } else {
        out << "[" << std::endl;
        indent++;
        for (auto &sptr : node.bundles) {
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
    write_indent();
    out << "body: ";
    if (node.body.empty()) {
        out << "-" << std::endl;
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
 * Dumps a `Variable` node.
 */
void Dumper::visit_variable(Variable &node) {
    write_indent();
    out << "Variable";
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
    out << "typ: ";
    if (node.typ.empty()) {
        out << "!MISSING" << std::endl;
    } else {
        out << "<" << std::endl;
        indent++;
        if (!node.typ.empty()) {
            node.typ->dump(out, indent);
        }
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
        out << "cqasm::v3::primitives::Version<<" << std::endl;
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
        if (!node.condition.empty()) {
            node.condition->dump(out, indent);
        }
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

} // namespace semantic
} // namespace v3
} // namespace cqasm

