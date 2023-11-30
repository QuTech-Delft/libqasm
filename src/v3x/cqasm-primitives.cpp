/** \file
 * Implementation for \ref include/v3x/cqasm-primitives.hpp "v3x/cqasm-primitives.hpp".
 */

#include "v3x/cqasm-primitives.hpp"

#include <algorithm>  // transform


namespace cqasm::v3x::primitives {

/**
 * String
 */
template <>
Str initialize<Str>() { return ""; }

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
Axis initialize<Axis>() { return Axis::X; }

template <>
void serialize(const Axis &obj, ::tree::cbor::MapWriter &map) {
    switch (obj) {
        case Axis::X: map.append_int("x", 0); break;
        case Axis::Y: map.append_int("x", 1); break;
        case Axis::Z: map.append_int("x", 2); break;
    }
}

template <>
Axis deserialize(const ::tree::cbor::MapReader &map) {
    switch (map.at("x").as_int()) {
        case 0: return Axis::X;
        case 1: return Axis::Y;
        case 2: return Axis::Z;
    }
    throw std::runtime_error("invalid value for axis enum during deserialization");
}

/**
 * Bool
 */
template <>
Bool initialize<Bool>() { return false; }

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
Int initialize<Int>() { return 0; }

template <>
void serialize(const Int &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("x", obj);
}

template <>
Int deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_int();
}

/**
 * Real
 */
template <>
Real initialize<Real>() { return 0.0; }

template <>
void serialize(const Real &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("x", obj);
}

template <>
Real deserialize(const ::tree::cbor::MapReader &map) {
    return map.at("x").as_float();
}

template <>
void serialize(const Complex &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("r", obj.real());
    map.append_float("i", obj.imag());
}

/**
 * Complex
 */
template <>
Complex deserialize(const ::tree::cbor::MapReader &map) {
    return {map.at("r").as_float(), map.at("i").as_float()};
}

template <>
void serialize(const Version &obj, ::tree::cbor::MapWriter &map) {
    auto aw = map.append_array("x");
    for (auto x : obj) {
        aw.append_int(x);
    }
    aw.close();
}

/**
 * Version
 */
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
    switch (axis) {
        case Axis::X: os << "X"; break;
        case Axis::Y: os << "Y"; break;
        case Axis::Z: os << "Z"; break;
        default: break;
    }
    return os;
}

} // namespace cqasm::v3x::primitives
