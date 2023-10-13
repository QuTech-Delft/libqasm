/** \file
 * Implementation for \ref include/v3x/cqasm-primitives.hpp "v3x/cqasm-primitives.hpp".
 */

#include "v3x/cqasm-primitives.hpp"


namespace cqasm::v3x::primitives {

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

} // namespace cqasm::v3x::primitives
