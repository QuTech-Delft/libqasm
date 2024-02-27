/** \file
 * Defines primitive types for use in trees generated by \ref tree-gen.
 */

#pragma once

#include "cqasm-version.hpp"
#include "tree-cbor.hpp"

#include <array>
#include <cstdint>
#include <complex>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <string>
#include <vector>


namespace cqasm::v3x::primitives {

/**
 * Generates a default value for the given primitive type.
 * This is specialized for the primitives mapping to builtin types
 * (int, bool, etc, for which the "constructor" doesn't initialize the value at all)
 * such that they actually initialize with a sane default.
 * Used in the default constructors of the generated tree nodes to ensure that there's no garbage in the nodes.
 */
template <class T>
T initialize() { return T(); }

/**
 * Serializes the given primitive object to CBOR.
 */
template <typename T>
void serialize(const T &obj, ::tree::cbor::MapWriter &map);

/**
 * Deserializes the given primitive object from CBOR.
 */
template <typename T>
T deserialize(const ::tree::cbor::MapReader &map);

/**
 * String primitive used within the AST and semantic trees.
 */
using Str = std::string;
template <>
Str initialize<Str>();
template <>
void serialize(const Str &obj, ::tree::cbor::MapWriter &map);
template <>
Str deserialize(const ::tree::cbor::MapReader &map);

/**
 * Axis primitive used within the semantic trees.
 */
struct Axis {
    double x = 1.0;
    double y = 0.0;
    double z = 0.0;

    [[nodiscard]] bool operator==(const Axis &rhs) const = default;
};
template <>
Axis initialize<Axis>();
template <>
void serialize(const Axis &obj, ::tree::cbor::MapWriter &map);
template <>
Axis deserialize(const ::tree::cbor::MapReader &map);

/**
 * Boolean primitive used within the semantic trees. Defaults to false.
 */
using Bool = bool;
template <>
Bool initialize<Bool>();
template <>
void serialize(const Bool &obj, ::tree::cbor::MapWriter &map);
template <>
Bool deserialize(const ::tree::cbor::MapReader &map);

/**
 * Integer primitive used within the AST and semantic trees.
 */
using Int = std::int64_t;
template <>
Int initialize<Int>();
template <>
void serialize(const Int &obj, ::tree::cbor::MapWriter &map);
template <>
Int deserialize(const ::tree::cbor::MapReader &map);

/**
 * Float number primitive used within the AST and semantic trees.
 */
using Float = double;
template <>
Float initialize<Float>();
template <>
void serialize(const Float &obj, ::tree::cbor::MapWriter &map);
template <>
Float deserialize(const ::tree::cbor::MapReader &map);

/**
 * Complex number primitive used within the semantic trees.
 */
using Complex = std::complex<double>;
template <>
void serialize(const Complex &obj, ::tree::cbor::MapWriter &map);
template <>
Complex deserialize(const ::tree::cbor::MapReader &map);

/**
 * Version number primitive used within the AST and semantic trees.
 */
using Version = version::Version;
template <>
void serialize(const Version &obj, ::tree::cbor::MapWriter &map);
template <>
Version deserialize(const ::tree::cbor::MapReader &map);

/**
 * Stream << overload for axis nodes.
 */
std::ostream &operator<<(std::ostream &os, const Axis &axis);

} // namespace cqasm::v3x::primitives


template <> struct fmt::formatter<cqasm::v3x::primitives::Axis> : fmt::ostream_formatter {};
