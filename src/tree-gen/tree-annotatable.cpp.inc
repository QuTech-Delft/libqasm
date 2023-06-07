#include <unordered_map>

TREE_NAMESPACE_BEGIN
namespace annotatable {

/**
 * Constructs an Anything object.
 */
Anything::Anything(void *data, std::function<void(void *data)> destructor, std::type_index type) :
    data(data),
    destructor(destructor),
    type(type)
{}

/**
 * Constructs an empty Anything object.
 */
Anything::Anything() :
    data(nullptr),
    destructor([](void*){}),
    type(std::type_index(typeid(nullptr)))
{}

/**
 * Destructor.
 */
Anything::~Anything() {
    if (data) {
        destructor(data);
    }
}

/**
 * Move constructor.
 */
Anything::Anything(Anything &&src) :
    data(src.data),
    destructor(std::move(src.destructor)),
    type(std::move(src.type))
{
    src.data = nullptr;
}

/**
 * Move assignment.
 */
Anything& Anything::operator=(Anything &&src) {
    if (data) {
        destructor(data);
    }
    data = src.data;
    destructor = std::move(src.destructor);
    type = std::move(src.type);
    src.data = nullptr;
    return *this;
}

/**
 * Returns the type index corresponding to the wrapped object.
 */
std::type_index Anything::get_type_index() const {
    return type;
};

/**
 * Serializes the given Anything object to a single value in the given
 * map, if and only if a serializer was previously registered for this type.
 * If no serializer is known, this is no-op.
 */
void SerDesRegistry::serialize(std::shared_ptr<Anything> obj, cbor::MapWriter &map) const {
    if (!obj) return;
    auto it = serializers.find(obj->get_type_index());
    if (it != serializers.end()) {
        it->second(obj, map);
    }
}

/**
 * Deserializes the given CBOR key/value pair to the corresponding Anything
 * object, if the type is known. If the type is not known, an empty/null
 * shared_ptr is returned.
 */
std::shared_ptr<Anything> SerDesRegistry::deserialize(const std::string &key, const cbor::Reader &value) const {
    auto it = deserializers.find(key);
    if (it != deserializers.end()) {
        return it->second(value.as_map());
    } else {
        return nullptr;
    }
}

/**
 * Global variable keeping track of all registered serialization and
 * deserialization functions for annotation objects.
 */
SerDesRegistry serdes_registry = SerDesRegistry();

/**
 * We're using inheritance, so we need a virtual destructor for proper
 * cleanup.
 */
Annotatable::~Annotatable() {
};

/**
 * Copies *all* the annotations from the source object to this object *by
 * reference*. Existing annotations in this object that also exist in src
 * are overwritten.
 */
void Annotatable::copy_annotations(const Annotatable &src) {
    for (const auto &src_it : src.annotations) {
        TREE_MAP_SET(annotations, src_it.first, src_it.second);
    }
}

/**
 * Serializes all the annotations that have a known serialization format
 * (previously registered through serdes_registry.add()) to the given map.
 * Each annotation results in a single map entry, with the C++ typename
 * wrapped in curly braces as key, and a type-dependent submap populated by
 * the registered serialization function as value. Annotations with no known
 * serialization format are silently ignored.
 */
void Annotatable::serialize_annotations(cbor::MapWriter &map) const {
    for (auto it : annotations) {
        serdes_registry.serialize(it.second, map);
    }
}

/**
 * Deserializes all annotations that have a known deserialization function
 * (previously registered through serdes_registry.add()) into the annotation
 * list. Annotations are expected to have a key formed by the C++ typename
 * wrapped in curly braces and a value of type map, of which the contents
 * are passed to the registered deserialization function. Previously added
 * annotations with conflicting types are silently overwritten. Any unknown
 * annotation types are silently ignored.
 */
void Annotatable::deserialize_annotations(const cbor::MapReader &map) {
    for (auto it : map) {
        // All annotation keys start with an { and close with a }. We
        // immediately ignore any other keys.
        if (!it.first.empty() && (it.first[0] == '{') && (it.first[it.first.size() - 1] == '}')) {
            std::shared_ptr<Anything> value{};
            value = serdes_registry.deserialize(it.first, it.second);
            if (value) {
                TREE_MAP_SET(annotations, value->get_type_index(), value);
            }
        }
    }
}

} // namespace annotatable
TREE_NAMESPACE_END
