/** \file
 * Implementation for \ref include/cqasm-v1-primitives.hpp "cqasm-v1-primitives.hpp".
 */

#include "cqasm-v1-primitives.hpp"

namespace cqasm {
namespace v1 {
namespace primitives {

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

template <>
Complex deserialize(const ::tree::cbor::MapReader &map) {
    return {map.at("r").as_float(), map.at("i").as_float()};
}

template <>
void serialize(const RMatrix &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("c", obj.size_cols());
    auto aw = map.append_array("d");
    for (const auto &value : obj.get_data()) {
        aw.append_float(value);
    }
    aw.close();
}

template <>
RMatrix deserialize(const ::tree::cbor::MapReader &map) {
    size_t num_cols = map.at("c").as_int();
    auto ar = map.at("d").as_array();
    std::vector<Real> data;
    data.reserve(ar.size());
    for (size_t i = 0; i < ar.size(); i++) {
        data[i] = ar.at(i).as_float();
    }
    return {data, num_cols};
}

template <>
void serialize(const CMatrix &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("c", obj.size_cols());
    auto aw = map.append_array("d");
    for (const auto &value : obj.get_data()) {
        aw.append_float(value.real());
        aw.append_float(value.imag());
    }
    aw.close();
}

template <>
CMatrix deserialize(const ::tree::cbor::MapReader &map) {
    size_t num_cols = map.at("c").as_int();
    auto ar = map.at("d").as_array();
    std::vector<Complex> data;
    data.reserve(ar.size() / 2);
    for (size_t i = 0; i < ar.size() / 2; i++) {
        data[i] = {ar.at(i*2).as_float(), ar.at(i*2+1).as_float()};
    }
    return {data, num_cols};
}

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
    v.reserve(ar.size());
    for (size_t i = 0; i < ar.size(); i++) {
        v.push_back(ar.at(i).as_int());
    }
    return v;
}

/**
 * Stream << overload for axis nodes.
 */
std::ostream &operator<<(std::ostream &os, const Axis &axis) {
    switch (axis) {
        case Axis::X:
            os << "X";
            break;
        case Axis::Y:
            os << "Y";
            break;
        case Axis::Z:
            os << "Z";
            break;
    }
    return os;
}

} // namespace primitives
} // namespace v1
} // namespace cqasm
