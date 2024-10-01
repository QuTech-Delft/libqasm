/** \file
 * Implementation for \ref include/libqasm/v3x/primitives.hpp "v3x/primitives.hpp".
 */

#include "libqasm/v3x/primitives.hpp"

#include <algorithm>  // transform
#include <fmt/format.h>

namespace cqasm::v3x::primitives {

/**
 * String
 */
template <>
Str initialize<Str>() {
    return "";
}

template <>
void serialize(const Str &obj, ::tree::cbor::MapWriter &map) {
    map.append_binary("x", obj);
}

template <>
Str deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_binary();
}

/**
 * Axis
 */
template <>
Axis initialize<Axis>() {
    return Axis{ 1.0, 0.0, 0.0 };
}

template <>
void serialize(const Axis &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("x", obj.x);
    map.append_float("y", obj.y);
    map.append_float("z", obj.z);
}

template <>
Axis deserialize(const ::tree::cbor::MapReader &map) {
    return { map.at("x").as_float(), map.at("y").as_float(), map.at("z").as_float() };
}

/**
 * Bool
 */
template <>
Bool initialize<Bool>() {
    return false;
}

template <>
void serialize(const Bool &obj, ::tree::cbor::MapWriter &map) {
    map.append_bool("x", obj);
}

template <>
Bool deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_bool();
}

/**
 * Int
 */
template <>
Int initialize<Int>() {
    return 0;
}

template <>
void serialize(const Int &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("x", obj);
}

template <>
Int deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_int();
}

/**
 * Float
 */
template <>
Float initialize<Float>() {
    return 0.0;
}

template <>
void serialize(const Float &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("x", obj);
}

template <>
Float deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_float();
}

/**
 * Complex
 */
template <>
void serialize(const Complex &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("r", obj.real());
    map.append_float("i", obj.imag());
}

template <>
Complex deserialize(const ::tree::cbor::MapReader &map) {
    return { map.at("r").as_float(), map.at("i").as_float() };
}

/**
 * Version
 */
template <>
void serialize(const Version &obj, ::tree::cbor::MapWriter &map) {
    auto aw = map.append_array("x");
    for (auto x : obj) {
        aw.append_int(x);
    }
    aw.close();
}

template <>
Version deserialize(const ::tree::cbor::MapReader &map) {
    auto ar = map.at("x").as_array();
    auto v = Version("");
    v.resize(ar.size());
    std::transform(ar.begin(), ar.end(), v.begin(), [](const auto &e) { return e.as_int(); });
    return v;
}

/**
 * Stream << overload for axis nodes.
 */
std::ostream &operator<<(std::ostream &os, const Axis &axis) {
    return os << fmt::format("[{}, {}, {}]", axis.x, axis.y, axis.z);
}

}  // namespace cqasm::v3x::primitives
