/** \file
 * Implementation for \ref include/cqasm-primitives.hpp "cqasm-primitives.hpp".
 */

#include "cqasm-primitives.hpp"

namespace cqasm {
namespace primitives {

template <>
Str initialize<Str>() {
    return "";
}

template <>
void serialize<Str>(const Str &obj, ::tree::cbor::MapWriter &map) {
    map.append_string("val", obj);
}

template <>
Str deserialize<Str>(const ::tree::cbor::MapReader &map) {
    return map.at("val").as_string();
}

template <>
Bool initialize<Bool>() {
    return false;
}

template <>
void serialize<Bool>(const Bool &obj, ::tree::cbor::MapWriter &map) {
    map.append_bool("val", obj);
}

template <>
Bool deserialize<Bool>(const ::tree::cbor::MapReader &map) {
    return map.at("val").as_bool();
}

template <>
Axis initialize<Axis>() {
    return Axis::X;
}

template <>
void serialize<Axis>(const Axis &obj, ::tree::cbor::MapWriter &map) {
    switch (obj) {
        case Axis::X:
            map.append_string("val", "X");
            break;
        case Axis::Y:
            map.append_string("val", "Y");
            break;
        case Axis::Z:
            map.append_string("val", "Z");
            break;
    }
}

template <>
Axis deserialize<Axis>(const ::tree::cbor::MapReader &map) {
    auto val = map.at("val").as_string();
    if (val == "X") {
        return Axis::X;
    } else if (val == "Y") {
        return Axis::Y;
    } else if (val == "Z") {
        return Axis::Z;
    } else {
        throw std::invalid_argument("unknown axis value in serialization");
    }
}

template <>
Int initialize<Int>() {
    return 0;
}

template <>
void serialize<Int>(const Int &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("val", obj);
}

template <>
Int deserialize<Int>(const ::tree::cbor::MapReader &map) {
    return map.at("val").as_int();
}

template <>
Real initialize<Real>() {
    return 0.0;
}

template <>
void serialize<Real>(const Real &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("val", obj);
}

template <>
Real deserialize<Real>(const ::tree::cbor::MapReader &map) {
    return map.at("val").as_float();
}

template <>
void serialize<Complex>(const Complex &obj, ::tree::cbor::MapWriter &map) {
    map.append_float("re", obj.real());
    map.append_float("im", obj.imag());
}

template <>
Complex deserialize<Complex>(const ::tree::cbor::MapReader &map) {
    return Complex{
        map.at("re").as_float(),
        map.at("im").as_float()
    };
}

template <>
void serialize<RMatrix>(const RMatrix &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("nc", obj.size_cols());
    map.append_int("nr", obj.size_rows());
    auto data = map.append_array("d");
    for (size_t row = 1; row <= obj.size_rows(); row++) {
        for (size_t col = 1; col <= obj.size_cols(); col++) {
            data.append_float(obj.at(row, col));
        }
    }
    data.close();
}

template <>
RMatrix deserialize<RMatrix>(const ::tree::cbor::MapReader &map) {
    auto nr = map.at("nr").as_int();
    auto nc = map.at("nc").as_int();
    if (nr < 1 || nc < 0) {
        throw std::invalid_argument("invalid matrix size in serialization");
    }
    auto obj = RMatrix(nr, nc);
    auto data = map.at("d").as_array();
    size_t i = 0;
    for (size_t row = 1; row <= obj.size_rows(); row++) {
        for (size_t col = 1; col <= obj.size_cols(); col++) {
            obj.at(row, col) = data.at(i++).as_float();
        }
    }
    if (i != data.size()) {
        throw std::invalid_argument("unexpected number of data elements in serialization");
    }
    return obj;
}

template <>
void serialize<CMatrix>(const CMatrix &obj, ::tree::cbor::MapWriter &map) {
    map.append_int("nc", obj.size_cols());
    map.append_int("nr", obj.size_rows());
    auto data = map.append_array("d");
    for (size_t row = 1; row <= obj.size_rows(); row++) {
        for (size_t col = 1; col <= obj.size_cols(); col++) {
            auto val = obj.at(row, col);
            data.append_float(val.real());
            data.append_float(val.imag());
        }
    }
    data.close();
}

template <>
CMatrix deserialize<CMatrix>(const ::tree::cbor::MapReader &map) {
    auto nr = map.at("nr").as_int();
    auto nc = map.at("nc").as_int();
    if (nr < 1 || nc < 0) {
        throw std::invalid_argument("invalid matrix size in serialization");
    }
    auto obj = CMatrix(nr, nc);
    auto data = map.at("d").as_array();
    size_t i = 0;
    for (size_t row = 1; row <= obj.size_rows(); row++) {
        for (size_t col = 1; col <= obj.size_cols(); col++) {
            auto re = data.at(i++).as_float();
            auto im = data.at(i++).as_float();
            obj.at(row, col) = Complex(re, im);
        }
    }
    if (i != data.size()) {
        throw std::invalid_argument("unexpected number of data elements in serialization");
    }
    return obj;
}

/**
 * Constructs a version object from a string, defaulting to 1.0.
 */
Version::Version(const std::string &version) {
    if (version.empty()) {
        return;
    }
    size_t next, last = 0;
    while ((next = version.find('.', last)) != std::string::npos) {
        push_back(std::stoi(version.substr(last, next - last)));
        last = next + 1;
    }
    push_back(std::stoi(version.substr(last)));
    for (auto component : *this) {
        if (component < 0) {
            throw std::invalid_argument("version component below zero");
        }
    }
}

/**
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
 */
int Version::compare(const Version &other) const {
    for (size_t i = 0; i < this->size() || i < other.size(); i++) {
        auto lhs = i < this->size() ? (*this)[i] : 0;
        auto rhs = i < other.size() ? other[i] : 0;
        if (lhs > rhs) return 1;
        if (lhs < rhs) return -1;
    }
    return 0;
}

/**
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
 */
int Version::compare(const std::string &other) const {
    return compare(Version(other));
}

template <>
void serialize<Version>(const Version &obj, ::tree::cbor::MapWriter &map) {
    auto data = map.append_array("d");
    for (auto el : obj) {
        data.append_int(el);
    }
    data.close();
}

template <>
Version deserialize<Version>(const ::tree::cbor::MapReader &map) {
    auto obj = Version("");
    auto data = map.at("d").as_array();
    for (const auto &el : data) {
        auto val = el.as_int();
        if (val < 0) {
            throw std::invalid_argument("negative version component in serialization");
        }
        obj.push_back(val);
    }
    return obj;
}

} // namespace primitives
} // namespace cqasm

/**
 * Stream << overload for axis nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Axis& axis) {
    switch (axis) {
        case ::cqasm::primitives::Axis::X:
            os << "X";
            break;
        case ::cqasm::primitives::Axis::Y:
            os << "Y";
            break;
        case ::cqasm::primitives::Axis::Z:
            os << "Z";
            break;
    }
    return os;
}

/**
 * Stream << overload for version nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Version& object) {
    bool first = true;
    for (auto item : object) {
        if (first) {
            first = false;
        } else {
            os << ".";
        }
        os << item;
    }
    return os;
}
