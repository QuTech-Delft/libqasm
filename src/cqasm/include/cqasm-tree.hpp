/** \file
 * Defines the base types used by \ref tree-gen to construct the AST and
 * semantic trees, among others.
 */

#pragma once

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>
#include "cqasm-annotatable.hpp"

// ssize_t is not defined on Windows, so we replace it with a long long.
#ifdef _WIN32
#define ssize_t long long
#endif

namespace cqasm {

/**
 * Namespace for the base types that \ref tree-gen relies on.
 */
namespace tree {

/**
 * Interface class for all tree nodes and the Maybe/One/Any/Many containers.
 */
class Completable {
public:
    virtual ~Completable() = default;

    /**
     * Returns whether this object is complete/fully defined.
     */
    virtual bool is_complete() const {
        return true;
    }

};

/**
 * Base class for all tree nodes.
 */
class Base : public annotatable::Annotatable, public Completable {
};

/**
 * Convenience class for a reference to an optional AST node.
 */
template <class T>
class Maybe : public Completable {
protected:

    /**
     * The contained value. Must be non-null for a completed AST.
     */
    std::shared_ptr<T> val;

public:

    /**
     * Constructor for an empty node.
     */
    Maybe() : val() {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    explicit Maybe(const std::shared_ptr<S> &value) : val(std::static_pointer_cast<T>(value)) {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    explicit Maybe(std::shared_ptr<S> &&value) : val(std::static_pointer_cast<T>(std::move(value))) {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    Maybe(const Maybe<S> &value) : val(std::static_pointer_cast<T>(value.val)) {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    Maybe(Maybe<S> &&value) : val(std::static_pointer_cast<T>(std::move(value.val))) {}

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    void set(const std::shared_ptr<S> &value) {
        val = std::static_pointer_cast<T>(value);
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    Maybe &operator=(const std::shared_ptr<S> &value) {
        set<S>(value);
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    void set(std::shared_ptr<S> &&value) {
        val = std::static_pointer_cast<T>(std::move(value));
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    Maybe &operator=(std::shared_ptr<S> &&value) {
        set<S>(std::move(value));
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    void set(const Maybe<S> &value) {
        val = std::static_pointer_cast<T>(value.get_ptr());
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    Maybe &operator=(const Maybe<S> &value) {
        set<S>(std::move(value));
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    void set(Maybe<S> &&value) {
        val = std::static_pointer_cast<T>(std::move(value.get_ptr()));
    }

    /**
     * Sets the value to a reference to the given object, or clears it if null.
     */
    template <class S>
    Maybe &operator=(Maybe<S> &&value) {
        set<S>(std::move(value));
    }

    /**
     * Sets the value to a NEW-ALLOCATED value pointed to AND TAKES OWNERSHIP.
     * In almost all cases, you should use set(make(...)) instead! This only
     * exists because the Yacc parser is one of the exceptions where you can't
     * help it, because the nodes have to be stored in a union while parsing,
     * and that can only be done with raw pointers.
     */
    template <class S>
    void set_raw(S *ob) {
        val = std::shared_ptr<T>(static_cast<T*>(ob));
    }

    /**
     * Removes the contained value.
     */
    void reset() {
        val.reset();
    }

    /**
     * Returns whether this Maybe is empty.
     */
    virtual bool empty() const {
        return val == nullptr;
    }

    /**
     * Returns whether this Maybe is empty.
     */
    size_t size() const {
        return val ? 1 : 0;
    }

    /**
     * Returns a mutable reference to the contained value. Raises an
     * `out_of_range` when the reference is empty.
     */
    T &deref() {
        if (!val) {
            throw std::out_of_range("dereferencing empty Maybe/One object");
        }
        return *val;
    }

    /**
     * Mutable dereference operator, shorthand for `deref()`.
     */
    T &operator*() {
        return deref();
    }

    /**
     * Mutable dereference operator, shorthand for `deref()`.
     */
    T *operator->() {
        return &deref();
    }

    /**
     * Returns a const reference to the contained value. Raises an
     * `out_of_range` when the reference is empty.
     */
    const T &deref() const {
        if (!val) {
            throw std::out_of_range("dereferencing empty Maybe/One object");
        }
        return *val;
    }

    /**
     * Constant dereference operator, shorthand for `deref()`.
     */
    const T &operator*() const {
        return deref();
    }

    /**
     * Constant dereference operator, shorthand for `deref()`.
     */
    const T *operator->() const {
        return &deref();
    }

    /**
     * Returns an immutable copy of the underlying shared_ptr.
     */
    const std::shared_ptr<T> &get_ptr() const {
        return val;
    }

    /**
     * Returns a mutable copy of the underlying shared_ptr.
     */
    std::shared_ptr<T> &get_ptr() {
        return val;
    }

    /**
     * Up- or downcasts this value. If the cast succeeds, the returned value
     * is nonempty and its shared_ptr points to the same data block as this
     * value does. If the cast fails, an empty Maybe is returned.
     */
    template <class S>
    Maybe<S> as() const {
        return Maybe<S>(std::dynamic_pointer_cast<S>(val));
    }

    /**
     * Equality operator.
     */
    bool operator==(const Maybe& rhs) const {
        if (val && rhs.get_ptr()) {
            return *val == *rhs;
        } else {
            return val == rhs.get_ptr();
        }
    }

    /**
     * Inequality operator.
     */
    inline bool operator!=(const Maybe& rhs) const {
        return !(*this == rhs);
    }

    /**
     * Returns whether this object is complete/fully defined.
     */
    bool is_complete() const override {
        return !val || val->is_complete();
    }

    /**
     * Visit this object.
     */
    template <class V>
    void visit(V &visitor) {
        if (val) {
            val->visit(visitor);
        }
    }

};

/**
 * Convenience class for a reference to exactly one other AST node.
 */
template <class T>
class One : public Maybe<T> {
public:

    /**
     * Constructor for an empty (invalid) node.
     */
    One() : Maybe<T>() {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    explicit One(const std::shared_ptr<S> &value) : Maybe<T>(value) {}

    /**
     * Constructor for an empty or filled node given an existing shared_ptr.
     */
    template <class S>
    explicit One(std::shared_ptr<S> &&value) : Maybe<T>(std::move(value)) {}

    /**
     * Constructor for an empty or filled node given an existing Maybe.
     */
    template <class S>
    One(const Maybe<S> &value) : Maybe<T>(value.get_ptr()) {}

    /**
     * Constructor for an empty or filled node given an existing Maybe.
     */
    template <class S>
    One(Maybe<S> &&value) : Maybe<T>(std::move(value.get_ptr())) {}

    /**
     * Returns whether this object is complete/fully defined.
     */
    bool is_complete() const override {
        return this->val && this->val->is_complete();
    }

};

/**
 * Constructs a One object, analogous to std::make_shared.
 */
template <class T, typename... Args>
One<T> make(Args... args) {
    return One<T>(std::make_shared<T>(args...));
}

/**
 * Convenience class for zero or more AST nodes.
 */
template <class T>
class Any : public Completable {
protected:

    /**
     * The contained vector.
     */
    std::vector<One<T>> vec;

public:

    /**
     * Adds the given value. No-op when the value is empty.
     */
    template <class S>
    void add(const Maybe<S> &ob, ssize_t pos=-1) {
        if (ob.empty()) {
            return;
        }
        if (pos < 0 || (size_t)pos >= size()) {
            this->vec.emplace_back(
                std::static_pointer_cast<T>(ob.get_ptr()));
        } else {
            this->vec.emplace(this->vec.cbegin() + pos,
                              std::static_pointer_cast<T>(
                                  ob.get_ptr()));
        }
    }

    /**
     * Adds the NEW-ALLOCATED value pointed to AND TAKES OWNERSHIP. In almost
     * all cases, you should use add(make(...), pos) instead! This only exists
     * because the Yacc parser is one of the exceptions where you can't help
     * it, because the nodes have to be stored in a union while parsing, and
     * that can only be done with raw pointers.
     */
    template <class S>
    void add_raw(S *ob, ssize_t pos=-1) {
        if (!ob) {
            throw std::runtime_error("add_raw called with nullptr!");
        }
        if (pos < 0 || (size_t)pos >= size()) {
            this->vec.emplace_back(std::shared_ptr<T>(static_cast<T*>(ob)));
        } else {
            this->vec.emplace(this->vec.cbegin() + pos, std::shared_ptr<T>(static_cast<T*>(ob)));
        }
    }

    /**
     * Extends this Any with another.
     */
    void extend(Any<T> &other) {
        this->vec.insert(this->vec.end(), other.vec.begin(), other.vec.end());
    }

    /**
     * Removes the object at the given index, or at the back if no index is
     * given.
     */
    void remove(ssize_t pos=-1) {
        if (size() == 0) {
            return;
        }
        if (pos < 0 || pos >= size()) {
            pos = size() - 1;
        }
        this->vec.erase(this->vec.cbegin() + pos);
    }

    /**
     * Removes the contained values.
     */
    void reset() {
        vec.clear();
    }

    /**
     * Returns whether this Any is empty.
     */
    virtual bool empty() const {
        return vec.empty();
    }

    /**
     * Returns the number of elements in this Any.
     */
    size_t size() const {
        return vec.size();
    }

    /**
     * Returns a mutable reference to the contained value at the given index.
     * Raises an `out_of_range` when the reference is empty.
     */
    One<T> &at(size_t index) {
        return vec.at(index);
    }

    /**
     * Shorthand for `at()`. Unlike std::vector's operator[], this also checks
     * bounds.
     */
    One<T> &operator[] (size_t index) {
        return at(index);
    }

    /**
     * Returns an immutable reference to the contained value at the given
     * index. Raises an `out_of_range` when the reference is empty.
     */
    const One<T> &at(size_t index) const {
        return vec.at(index);
    }

    /**
     * Shorthand for `at()`. Unlike std::vector's operator[], this also checks
     * bounds.
     */
    const One<T> &operator[] (size_t index) const {
        return at(index);
    }

    /**
     * Returns a copy of the reference to the last value in the list. If the
     * list is empty, an empty reference is returned.
     */
    Maybe<T> back() const {
        if (vec.empty()) {
            return Maybe<T>();
        } else {
            return vec.back();
        }
    }

    /**
     * `begin()` for for-each loops.
     */
    typename std::vector<One<T>>::iterator begin() {
        return vec.begin();
    }

    /**
     * `begin()` for for-each loops.
     */
    typename std::vector<One<T>>::const_iterator begin() const {
        return vec.begin();
    }

    /**
     * `end()` for for-each loops.
     */
    typename std::vector<One<T>>::iterator end() {
        return vec.end();
    }

    /**
     * `end()` for for-each loops.
     */
    typename std::vector<One<T>>::const_iterator end() const {
        return vec.end();
    }

    /**
     * Equality operator.
     */
    bool operator==(const Any& rhs) const {
        return vec == rhs.vec;
    }

    /**
     * Inequality operator.
     */
    inline bool operator!=(const Any& rhs) const {
        return !(*this == rhs);
    }

    /**
     * Returns whether this object is complete/fully defined.
     */
    bool is_complete() const override {
        for (auto &sptr : this->vec) {
            if (!sptr->is_complete()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Visit this object.
     */
    template <class V>
    void visit(V &visitor) {
        for (auto &sptr : this->vec) {
            if (!sptr.empty()) {
                sptr->visit(visitor);
            }
        }
    }

};

/**
 * Convenience class for one or more AST nodes.
 */
template <class T>
class Many : public Any<T> {
public:

    /**
     * Returns whether this object is complete/fully defined.
     */
    bool is_complete() const override {
        return !this->vec.empty() && Any<T>::is_complete();
    }

};

} // namespace tree
} // namespace cqasm
