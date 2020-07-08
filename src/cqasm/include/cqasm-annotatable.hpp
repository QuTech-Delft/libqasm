/** \file
 * This file contains the magic needed to allow users to add their own data to
 * libqasm's classes without needing to change libqasm.
 *
 * Data can be added by way of annotations (not to be confused with the JSON
 * annotations available in cQASM itself). Annotations can be any kind of
 * object; in fact they are identified by their type, so each annotatable
 * object can have zero or one instance of every C++ type associated with it.
 * The goal is to allow users to use the AST, semantic tree, and a few other
 * things in their own code as they operate on the tree.
 *
 * Note that libqasm itself uses this mechanism to record line number
 * information. Any kind of metadata goes.
 */

#pragma once

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>

namespace cqasm {

/**
 * Namespace for the \ref cqasm::annotatable::Annotatable "Annotatable" base
 * class and support classes.
 */
namespace annotatable {

/**
 * Utility class for carrying any kind of value. Basically, `std::any` within
 * C++11.
 */
class Anything {
private:

    /**
     * Pointer to the contained data, or nullptr if no data is contained.
     */
    void *data;

    /**
     * Function used to free the contained data.
     */
    std::function<void(void *data)> destructor;

    /**
     * Type information.
     */
    std::type_index type;

    /**
     * Constructs an Anything object.
     */
    Anything(void *data, std::function<void(void *data)> destructor, std::type_index type) :
        data(data),
        destructor(destructor),
        type(type)
    {}

public:

    /**
     * Constructs an empty Anything object.
     */
    Anything() :
        data(nullptr),
        destructor([](void*){}),
        type(std::type_index(typeid(nullptr)))
    {}

    /**
     * Constructs an Anything object by copying a value into it.
     */
    template <typename T>
    static Anything make(const T &ob) {
        return Anything(
            new T(ob),
            [](void *data) {
                delete static_cast<T*>(data);
            },
            std::type_index(typeid(T))
        );
    }

    /**
     * Constructs an Anything object by moving a value into it.
     */
    template <typename T>
    static Anything make(T &&ob) {
        return Anything(
            new T(std::move(ob)),
            [](void *data) {
                delete static_cast<T*>(data);
            },
            std::type_index(typeid(T))
        );
    }

    /**
     * Destructor.
     */
    ~Anything() {
        if (data) {
            destructor(data);
        }
    }

    // Anything objects are not copyable, because type information is lost
    // after the initial construction.
    Anything(const Anything&) = delete;
    Anything& operator=(const Anything&) = delete;

    /**
     * Move constructor.
     */
    Anything(Anything &&src) :
        data(src.data),
        destructor(std::move(src.destructor)),
        type(std::move(src.type))
    {
        src.data = nullptr;
    }

    /**
     * Move assignment.
     */
    Anything& operator=(Anything &&src) {
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
     * Returns a mutable pointer to the contents.
     *
     * @throws std::bad_cast when the type is incorrect.
     */
    template <typename T>
    T *get_mut() {
        if (std::type_index(typeid(T)) != type) {
            throw std::bad_cast();
        }
        return static_cast<T*>(data);
    }

    /**
     * Returns a const pointer to the contents.
     *
     * @throws std::bad_cast when the type is incorrect.
     */
    template <typename T>
    const T *get_const() const {
        if (std::type_index(typeid(T)) != type) {
            throw std::bad_cast();
        }
        return static_cast<T*>(data);
    }

};

/**
 * Base class for anything that can have user-specified annotations.
 */
class Annotatable {
private:

    /**
     * The annotations stored with this node.
     */
    std::unordered_map<std::type_index, std::shared_ptr<Anything>> annotations;

public:

    /**
     * We're using inheritance, so we need a virtual destructor for proper
     * cleanup.
     */
    virtual ~Annotatable() {
    };

    /**
     * Adds an annotation object to this node.
     *
     * Annotations are keyed by their type. That is, a node can contain zero or
     * one annotation for every C++ type, meaning you can attach any data you
     * want to a node by defining your own struct or class.
     *
     * The annotations object is copied into the node. If you don't want to
     * make a copy, you can store a (smart) pointer to the object instead, in
     * which case the copied object is the pointer.
     */
    template <typename T>
    void set_annotation(const T &ob) {
        annotations[std::type_index(typeid(T))] = std::make_shared<Anything>(Anything::make<T>(ob));
    }

    /**
     * Adds an annotation object to this node.
     *
     * Annotations are keyed by their type. That is, a node can contain zero or
     * one annotation for every C++ type, meaning you can attach any data you
     * want to a node by defining your own struct or class.
     *
     * The annotations object is moved into the node.
     */
    template <typename T>
    void set_annotation(T &&ob) {
        annotations[std::type_index(typeid(T))] = std::make_shared<Anything>(Anything::make<T>(std::move(ob)));
    }

    /**
     * Returns whether this object holds an annotation object of the given
     * type.
     */
    template <typename T>
    bool has_annotation() const {
        return annotations.count(std::type_index(typeid(T)));
    }

    /**
     * Returns a mutable pointer to the annotation object of the given type
     * held by this object, or `nullptr` if there is no such annotation.
     */
    template <typename T>
    T *get_annotation_ptr() {
        try {
            return annotations.at(std::type_index(typeid(T)))->get_mut<T>();
        } catch (const std::out_of_range&) {
            return nullptr;
        }
    }

    /**
     * Returns an immutable pointer to the annotation object of the given type
     * held by this object, or `nullptr` if there is no such annotation.
     */
    template <typename T>
    const T *get_annotation_ptr() const {
        try {
            return annotations.at(std::type_index(typeid(T)))->get_const<T>();
        } catch (const std::out_of_range&) {
            return nullptr;
        }
    }

    /**
     * Returns a mutable pointer to the annotation object of the given type
     * held by this object, or `nullptr` if there is no such annotation.
     */
    template <typename T>
    T &get_annotation() {
        if (auto annotation = get_annotation_ptr<T>()) {
            return *annotation;
        } else {
            throw std::runtime_error("object does not have an annotation of this type");
        }
    }

    /**
     * Returns an immutable pointer to the annotation object of the given type
     * held by this object, or `nullptr` if there is no such annotation.
     */
    template <typename T>
    const T &get_annotation() const {
        if (auto annotation = get_annotation_ptr<T>()) {
            return *annotation;
        } else {
            throw std::runtime_error("object does not have an annotation of this type");
        }
    }

    /**
     * Removes the annotation object of the given type, if any.
     */
    template <typename T>
    void erase_annotation() {
        annotations.erase(std::type_index(typeid(T)));
    }

    /**
     * Copies the annotation of type T from the source object to this object.
     * If the source object doesn't have an annotation of type T, any
     * such annotation on this object is removed.
     */
    template <typename T>
    void copy_annotation(const Annotatable &src) {
        if (auto annotation = src.get_annotation_ptr<T>()) {
            set_annotation(*annotation);
        } else {
            erase_annotation<T>();
        }
    }

};

} // namespace annotatable
} // namespace cqasm
