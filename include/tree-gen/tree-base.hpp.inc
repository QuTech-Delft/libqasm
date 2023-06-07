/** \file
 * Generalized contents of tree-base.hpp.
 */

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <fstream>

TREE_NAMESPACE_BEGIN

/**
 * Namespace for the base types that \ref tree-gen relies on.
 */
namespace base {

// Forward declarations for classes.
template <class T>
class Maybe;
template <class T>
class One;
template <class T>
class Any;
template <class T>
class Many;
class LinkBase;
template <class T>
class OptLink;
template <class T>
class Link;

/**
 * Exception used for generic runtime errors.
 */
class RuntimeError : public TREE_RUNTIME_ERROR {
public:
    explicit RuntimeError(const std::string &msg) : TREE_RUNTIME_ERROR(msg) {}
};

/**
 * Exception used by PointerMap to indicate not-well-formedness.
 */
class NotWellFormed : public RuntimeError {
public:
    explicit NotWellFormed(const std::string &msg) : RuntimeError(msg) {}
};

/**
 * Exception used when an index is out of range or an empty reference is
 * dereferenced.
 */
class OutOfRange : public TREE_RANGE_ERROR {
public:
    explicit OutOfRange(const std::string &msg) : TREE_RANGE_ERROR(msg) {}
};

/**
 * Helper class used to assign unique, stable numbers the nodes in a tree for
 * serialization and well-formedness checks in terms of lack of duplicate nodes
 * and dead links.
 */
class PointerMap {
private:

    /**
     * Map of all raw pointers found so far with sequence numbers attached to
     * them.
     */
    TREE_MAP(const void*, size_t) map;

    /**
     * Internal implementation for add(), given only the raw pointer and the
     * name of its type for the error message.
     */
    size_t add_raw(const void *ptr, const char *name);

    /**
     * Internal implementation for get(), given only the raw pointer and the
     * name of its type for the error message.
     */
    size_t get_raw(const void *ptr, const char *name) const;

public:

    /**
     * The value returned for invalid links when exceptions are disabled.
     */
    static const size_t INVALID = (size_t)-1;

    /**
     * Whether to throw exceptions upon invalid access due to an ill-formed
     * tree. This is enabled for the well-formedness check, but disabled when
     * debug-dumping trees.
     */
    bool enable_exceptions = true;

    /**
     * Registers a node pointer and gives it a sequence number. If a duplicate
     * node is found and exceptions are enabled, this raises a NotWellFormed.
     * Otherwise, the previously added link number is returned.
     */
    template <class T>
    size_t add(const Maybe<T> &ob);

    /**
     * Registers a node pointer by means of a direct reference and gives it a
     * sequence number. If a duplicate node is found and exceptions are enabled,
     * this raises a NotWellFormed. Otherwise, the previously added link number is
     * returned.
     */
    template <class T>
    size_t add_ref(const T &ob);

    /**
     * Returns the sequence number of a previously added node. If the node was
     * not previously added and exceptions are enabled, this raises a
     * NotWellFormed. Otherwise, INVALID is returned.
     */
    template <class T>
    size_t get(const Maybe<T> &ob) const;

    /**
     * Returns the sequence number of a previously added node. If the node was
     * not previously added and exceptions are enabled, this raises a
     * NotWellFormed. Otherwise, INVALID is returned.
     */
    template <class T>
    size_t get(const OptLink<T> &ob) const;

    /**
     * Returns the sequence number of a previously added node by means of a
     * direct reference. If the node was not previously added and exceptions are
     * enabled, this raises a NotWellFormed. Otherwise, INVALID is returned.
     */
    template <class T>
    size_t get_ref(const T &ob) const;

};

/**
 * Helper class for mapping the identifiers stored with One/Maybe edges in a
 * serialized tree to the constructed shared_ptrs, such that (Opt)Link edges can
 * be restored once the tree is rebuilt.
 */
class IdentifierMap {
private:

    /**
     * Map from identifier to node.
     */
    TREE_MAP(size_t, std::shared_ptr<void>) nodes;

    /**
     * List of links registered for restoration.
     */
    using Link = std::pair<LinkBase&, size_t>;
    TREE_VECTOR(Link) links;

public:

    /**
     * Registers a constructed node.
     */
    void register_node(size_t identifier, const std::shared_ptr<void> &ptr);

    /**
     * Registers a constructed link.
     */
    void register_link(LinkBase &link, size_t identifier);

    /**
     * Restores all the links after the tree finishes constructing.
     */
    void restore_links() const;

};

/**
 * Interface class for all tree nodes and the edge containers.
 */
class Completable {
public:
    virtual ~Completable() = default;

    /**
     * Traverses the tree to register all reachable Maybe/One nodes with the
     * given map. This also checks whether all One/Maybe nodes only appear once
     * in the tree (except through links). If there are duplicates, a
     * NotWellFormed exception is thrown.
     */
    virtual void find_reachable(PointerMap &map) const;

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    virtual void check_complete(const PointerMap &map) const;

    /**
     * Checks whether the tree starting at this node is well-formed. That is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node that's reachable from
     *    this node;
     *  - the nodes referred to be One/Maybe only appear once in the tree
     *    (except through links).
     * If it isn't well-formed, a NotWellFormed exception is thrown.
     */
    virtual void check_well_formed() const final;

    /**
     * Returns whether the tree starting at this node is well-formed. That is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node that's reachable from
     *    this node;
     *  - the nodes referred to be One/Maybe only appear once in the tree
     *    (except through links).
     */
    virtual bool is_well_formed() const final;

};

/**
 * Base class for all tree nodes.
 */
class Base : public annotatable::Annotatable, public Completable {
};

/**
 * Convenience class for a reference to an optional tree node.
 */
template <class T>
class Maybe : public Completable {
protected:

    /**
     * The contained value.
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
     * Constructor for an empty or filled node given an existing Maybe. Only
     * the reference is copied; use clone() if you want an actual copy.
     */
    template <class S>
    Maybe(const Maybe<S> &value) : val(std::static_pointer_cast<T>(value.get_ptr())) {}

    /**
     * Constructor for an empty or filled node given an existing Maybe. Only
     * the reference is copied; use clone() if you want an actual copy.
     */
    template <class S>
    Maybe(Maybe<S> &&value) : val(std::static_pointer_cast<T>(std::move(value.get_ptr()))) {}

    /**
     * Constructs a new node in-place.
     */
    template<typename S = T, class... Args>
    void emplace(Args&&... args) {
        val = std::static_pointer_cast<T>(std::make_shared<S>(std::forward<Args>(args)...));
    }

    /**
     * Constructs a new node.
     */
    template<typename S = T, class... Args>
    static One<T> make(Args&&... args);

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
        return *this;
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
        return *this;
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
        return *this;
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
        return *this;
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
     * `out_of_range` when the reference is empty. Note that this is const
     * because the pointer does not change.
     */
    T &deref() const {
        if (!val) {
            throw OutOfRange(
                std::string("dereferencing empty Maybe/One object of type ") +
                typeid(T).name()
            );
        }
        return *val;
    }

    /**
     * Mutable dereference operator, shorthand for `deref()`. Note that this is
     * const because the pointer does not change.
     */
    T &operator*() const {
        return deref();
    }

    /**
     * Mutable dereference operator, shorthand for `deref()`. Note that this is
     * const because the pointer does not change.
     */
    T *operator->() const {
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
     * Makes the contained value const.
     */
    Maybe<const T> as_const() const {
        return Maybe<const T>(std::const_pointer_cast<const T>(val));
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

    /**
     * Value-based equality operator.
     */
    bool equals(const Maybe &rhs) const {
        if (val && rhs.get_ptr()) {
            if (val == rhs.val) {
                return true;
            } else {
                return val->equals(*rhs);
            }
        } else {
            return val == rhs.get_ptr();
        }
    }

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Maybe &rhs) const {
        return val == rhs.get_ptr();
    }

    /**
     * Pointer-based inequality operator.
     */
    bool operator!=(const Maybe &rhs) const {
        return val != rhs.get_ptr();
    }

    /**
     * Pointer-based greater-than.
     */
    bool operator>(const Maybe &rhs) const {
        return val > rhs.val;
    }

    /**
     * Pointer-based greater-equal.
     */
    bool operator>=(const Maybe &rhs) const {
        return val >= rhs.val;
    }

    /**
     * Pointer-based less-than.
     */
    bool operator<(const Maybe &rhs) const {
        return val < rhs.val;
    }

    /**
     * Pointer-based less-equal.
     */
    bool operator<=(const Maybe &rhs) const {
        return val <= rhs.val;
    }

    /**
     * Traverses the tree to register all reachable Maybe/One nodes with the
     * given map. This also checks whether all One/Maybe nodes only appear once
     * in the tree (except through links). If there are duplicates, a
     * NotWellFormed exception is thrown.
     */
    void find_reachable(PointerMap &map) const override {
        if (val) {
            map.add(*this);
            val->find_reachable(map);
        }
    }

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        if (val) {
            val->check_complete(map);
        }
    }

    /**
     * Makes a shallow copy of this subtree.
     */
    One<typename std::remove_const<T>::type> copy() const;

    /**
     * Makes a deep copy of this subtree. Note that links are not modified; if
     * you want to completely clone a full tree that contains links you'll have
     * to use serdes or relink all links yourself.
     */
    One<typename std::remove_const<T>::type> clone() const;

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    virtual std::string serdes_edge_type() const {
        return "?";
    }

    /**
     * Deserializes the subtree corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap. Any existing tree contained
     * by the Maybe is overridden.
     */
    void deserialize(const cbor::MapReader &map, IdentifierMap &ids) {
        // Note: this is in a function rather than in the constructor, because
        // serdes_edge_type() would map to the base class if we just chain
        // constructors, and we don't want to repeat this whole mess for One.
        if (map.at("@T").as_string() != serdes_edge_type()) {
            throw RuntimeError("Schema validation failed: unexpected edge type");
        }
        auto type = map.at("@t");
        if (type.is_null()) {
            val.reset();
        } else {
            val = T::deserialize(map, ids);
            ids.register_node(map.at("@i").as_int(), std::static_pointer_cast<void>(val));
        }
    }

public:

    /**
     * Serializes the subtree that this edge points to. Note that this is only
     * available when the contained tree is generated with serialization
     * support.
     */
    void serialize(cbor::MapWriter &map, const PointerMap &ids) const {
        map.append_string("@T", serdes_edge_type());
        if (val) {
            map.append_int("@i", ids.get(*this));
            val->serialize(map, ids);
        } else {
            map.append_null("@t");
        }
    }

    /**
     * Deserializes the subtree corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap.
     */
    Maybe(const cbor::MapReader &map, IdentifierMap &ids) : val() {
        deserialize(map, ids);
    }

};

/**
 * Convenience class for a reference to exactly one other tree node.
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
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        if (!this->val) {
            std::ostringstream ss{};
            ss << "'One' edge of type " << typeid(T).name() << " is empty";
            throw NotWellFormed(ss.str());
        }
        this->val->check_complete(map);
    }

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    std::string serdes_edge_type() const override {
        return "1";
    }

public:

    /**
     * Deserializes the subtree corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap.
     */
    One(const cbor::MapReader &map, IdentifierMap &ids) : Maybe<T>() {
        this->deserialize(map, ids);
    }

};

/**
 * Makes a shallow copy of this value.
 */
template <class T>
One<typename std::remove_const<T>::type> Maybe<T>::copy() const {
    if (val) {
        return val->copy();
    } else {
        return Maybe<T>();
    }
}

/**
 * Makes a deep copy of this value.
 */
template <class T>
One<typename std::remove_const<T>::type> Maybe<T>::clone() const {
    if (val) {
        return val->clone();
    } else {
        return Maybe<T>();
    }
}

/**
 * Constructs a new node.
 */
template<typename T>
template<typename S, class... Args>
One<T> Maybe<T>::make(Args&&... args) {
    return One<T>(std::static_pointer_cast<T>(std::make_shared<S>(std::forward<Args>(args)...)));
}

/**
 * Constructs a One object, analogous to std::make_shared.
 */
template <class T, typename... Args>
One<T> make(Args... args) {
    return One<T>(std::make_shared<T>(args...));
}

/**
 * Convenience class for zero or more tree nodes.
 */
template <class T>
class Any : public Completable {
protected:

    /**
     * The contained vector.
     */
    TREE_VECTOR(One<T>) vec;

public:

    using iterator = typename TREE_VECTOR(One<T>)::iterator;
    using Iterator = iterator;
    using const_iterator = typename TREE_VECTOR(One<T>)::const_iterator;
    using ConstIterator = const_iterator;
    using reverse_iterator = typename TREE_VECTOR(One<T>)::reverse_iterator;
    using ReverseIterator = reverse_iterator;
    using const_reverse_iterator = typename TREE_VECTOR(One<T>)::const_reverse_iterator;
    using ConstReverseIterator = const_reverse_iterator;

    /**
     * Constructs an empty Any.
     */
    Any() = default;

    /**
     * Constructs an initialized Any.
     */
    Any(std::initializer_list<One<T>> inits) : vec(inits) {
    }

    /**
     * Adds the given value. No-op when the value is empty.
     */
    template <class S>
    void add(const Maybe<S> &ob, signed_size_t pos=-1) {
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
     * Less versatile alternative for adding nodes with less verbosity.
     */
    template <class S = T, typename... Args>
    Any &emplace(Args... args) {
        this->vec.emplace_back(std::static_pointer_cast<T>(std::make_shared<S>(std::forward<Args>(args)...)));
        return *this;
    }

    /**
     * Adds the NEW-ALLOCATED value pointed to AND TAKES OWNERSHIP. In almost
     * all cases, you should use add(make(...), pos) instead! This only exists
     * because the Yacc parser is one of the exceptions where you can't help
     * it, because the nodes have to be stored in a union while parsing, and
     * that can only be done with raw pointers.
     */
    template <class S>
    void add_raw(S *ob, signed_size_t pos=-1) {
        if (!ob) {
            throw RuntimeError("add_raw called with nullptr!");
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
    void extend(const Any<T> &other) {
        this->vec.insert(this->vec.end(), other.vec.begin(), other.vec.end());
    }

    /**
     * Removes the object at the given index, or at the back if no index is
     * given.
     */
    void remove(signed_size_t pos=-1) {
        if (size() == 0) {
            return;
        }
        if (pos < 0 || (size_t)pos >= size()) {
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
    const One<T> &at(size_t index) const {
        return vec.at(index);
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
    const One<T> &operator[] (size_t index) const {
        return at(index);
    }

    /**
     * Shorthand for `at()`. Unlike std::vector's operator[], this also checks
     * bounds.
     */
    One<T> &operator[] (size_t index) {
        return at(index);
    }

    /**
     * Returns a copy of the reference to the first value in the list. If the
     * list is empty, an empty reference is returned.
     */
    Maybe<T> front() const {
        if (vec.empty()) {
            return Maybe<T>();
        } else {
            return vec.front();
        }
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
    Iterator begin() {
        return vec.begin();
    }

    /**
     * `begin()` for for-each loops.
     */
    ConstIterator begin() const {
        return vec.begin();
    }

    /**
     * `end()` for for-each loops.
     */
    Iterator end() {
        return vec.end();
    }

    /**
     * `end()` for for-each loops.
     */
    ConstIterator end() const {
        return vec.end();
    }

    /**
     * `begin()` for for-each loops.
     */
    ReverseIterator rbegin() {
        return vec.rbegin();
    }

    /**
     * `begin()` for for-each loops.
     */
    ConstReverseIterator rbegin() const {
        return vec.rbegin();
    }

    /**
     * `end()` for for-each loops.
     */
    ReverseIterator rend() {
        return vec.rend();
    }

    /**
     * `end()` for for-each loops.
     */
    ConstReverseIterator rend() const {
        return vec.rend();
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

    /**
     * Value-based equality operator.
     */
    bool equals(const Any &rhs) const {
        if (vec.size() != rhs.vec.size()) {
            return false;
        }
        for (size_t i = 0; i < vec.size(); i++) {
            if (!vec[i].equals(rhs.vec[i])) {
                return false;
            }
        }
        return true;
    }

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const Any &rhs) const {
        return vec == rhs.vec;
    }

    /**
     * Pointer-based inequality operator.
     */
    inline bool operator!=(const Any &rhs) const {
        return !(*this == rhs);
    }

    /**
     * Returns an immutable reference to the underlying vector.
     */
    const TREE_VECTOR(One<T>) &get_vec() const {
        return vec;
    }

    /**
     * Returns a mutable reference to the underlying vector.
     */
    TREE_VECTOR(One<T>) &get_vec() {
        return vec;
    }

    /**
     * Traverses the tree to register all reachable Maybe/One nodes with the
     * given map. This also checks whether all One/Maybe nodes only appear once
     * in the tree (except through links). If there are duplicates, a
     * NotWellFormed exception is thrown.
     */
    void find_reachable(PointerMap &map) const override {
        for (auto &sptr : this->vec) {
            sptr.find_reachable(map);
        }
    }

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        for (auto &sptr : this->vec) {
            sptr.check_complete(map);
        }
    }

    /**
     * Makes a shallow copy of these values.
     */
    Many<typename std::remove_const<T>::type> copy() const;

    /**
     * Makes a deep copy of these values.
     */
    Many<typename std::remove_const<T>::type> clone() const;

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    virtual std::string serdes_edge_type() const {
        return "*";
    }

    /**
     * Deserializes the subtrees corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap. The subtrees are appended
     * to the back of the Any.
     */
    void deserialize(const cbor::MapReader &map, IdentifierMap &ids) {
        // Note: this is in a function rather than in the constructor, because
        // serdes_edge_type() would map to the base class if we just chain
        // constructors, and we don't want to repeat this whole mess for Many.
        if (map.at("@T").as_string() != serdes_edge_type()) {
            throw RuntimeError("Schema validation failed: unexpected edge type");
        }
        for (const auto &it : map.at("@d").as_array()) {
            vec.emplace_back(it.as_map(), ids);
        }
    }

public:

    /**
     * Serializes the subtrees that this edge points to. Note that this is only
     * available when the contained tree is generated with serialization
     * support.
     */
    void serialize(cbor::MapWriter &map, const PointerMap &ids) const {
        map.append_string("@T", serdes_edge_type());
        auto ar = map.append_array("@d");
        for (auto &sptr : this->vec) {
            auto submap = ar.append_map();
            sptr.serialize(submap, ids);
        }
    }

    /**
     * Deserializes the subtree corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap.
     */
    Any(const cbor::MapReader &map, IdentifierMap &ids) : vec() {
        deserialize(map, ids);
    }

};

/**
 * Convenience class for one or more tree nodes.
 */
template <class T>
class Many : public Any<T> {
public:

    /**
     * Constructs an empty Many.
     */
    Many() = default;

    /**
     * Constructs an initialized Any.
     */
    Many(std::initializer_list<One<T>> inits) : Any<T>(inits) {
    }

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        if (this->empty()) {
            std::ostringstream ss{};
            ss << "'Many' edge of type " << typeid(T).name() << " is empty";
            throw NotWellFormed(ss.str());
        }
        Any<T>::check_complete(map);
    }

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    std::string serdes_edge_type() const override {
        return "+";
    }

public:

    /**
     * Deserializes the subtrees corresponding to the given map, and registers
     * the nodes encountered with the IdentifierMap.
     */
    Many(const cbor::MapReader &map, IdentifierMap &ids) : Any<T>() {
        this->deserialize(map, ids);
    }

};

/**
 * Makes a shallow copy of these values.
 */
template <class T>
Many<typename std::remove_const<T>::type> Any<T>::copy() const {
    Many<typename std::remove_const<T>::type> c{};
    for (auto &sptr : this->vec) {
        c.add(sptr.copy());
    }
    return c;
}

/**
 * Makes a deep copy of these values.
 */
template <class T>
Many<typename std::remove_const<T>::type> Any<T>::clone() const {
    Many<typename std::remove_const<T>::type> c{};
    for (auto &sptr : this->vec) {
        c.add(sptr.clone());
    }
    return c;
}

/**
 * Helper interface class for restoring links after deserialization.
 */
class LinkBase : public Completable {
protected:
    friend class IdentifierMap;

    /**
     * Restores a link after deserialization.
     */
    virtual void set_void_ptr(const std::shared_ptr<void> &ptr) = 0;

};

/**
 * Convenience class for a reference to an optional tree node.
 */
template <class T>
class OptLink : public LinkBase {
protected:

    /**
     * The linked value.
     */
    std::weak_ptr<T> val;

public:

    /**
     * Constructor for an empty link.
     */
    OptLink() : val() {}

    /**
     * Constructor for an empty or filled node given the node to link to.
     */
    template <class S>
    OptLink(const Maybe<S> &value) : val(std::static_pointer_cast<T>(value.get_ptr())) {}

    /**
     * Constructor for an empty or filled node given the node to link to.
     */
    template <class S>
    OptLink(Maybe<S> &&value) : val(std::static_pointer_cast<T>(std::move(value.get_ptr()))) {}

    /**
     * Constructor for an empty or filled node given an existing link.
     */
    template <class S>
    OptLink(const OptLink<S> &value) : val(std::static_pointer_cast<T>(value.get_ptr())) {}

    /**
     * Constructor for an empty or filled node given an existing link.
     */
    template <class S>
    OptLink(OptLink<S> &&value) : val(std::static_pointer_cast<T>(std::move(value.get_ptr()))) {}

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
    OptLink &operator=(const Maybe<S> &value) {
        set<S>(value);
        return *this;
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
    OptLink &operator=(Maybe<S> &&value) {
        set<S>(std::move(value));
        return *this;
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
        return val.expired();
    }

    /**
     * Returns whether this Maybe is empty.
     */
    size_t size() const {
        return val.expired() ? 0 : 1;
    }

    /**
     * Returns a mutable reference to the contained value. Raises an
     * `out_of_range` when the reference is empty.
     */
    T &deref() {
        if (val.expired()) {
            throw OutOfRange(
                std::string("dereferencing empty or expired (Opt)Link object of type ") +
                typeid(T).name()
            );
        }
        return *(val.lock());
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
     * Returns a reference to the contained value. Raises an `out_of_range`
     * when the reference is empty.
     */
    T &deref() const {
        if (val.expired()) {
            throw OutOfRange("dereferencing empty or expired (Opt)Link object");
        }
        return *(val.lock());
    }

    /**
     * Dereference operator, shorthand for `deref()`.
     */
    T &operator*() const {
        return deref();
    }

    /**
     * Dereference operator, shorthand for `deref()`.
     */
    T *operator->() const {
        return &deref();
    }

    /**
     * Returns a copy of the underlying shared_ptr.
     */
    std::shared_ptr<T> get_ptr() const {
        return val.lock();
    }

    /**
     * Up- or downcasts this value. If the cast succeeds, the returned value
     * is nonempty and its shared_ptr points to the same data block as this
     * value does. If the cast fails, an empty Maybe is returned.
     */
    template <class S>
    Maybe<S> as() const {
        return Maybe<S>(std::dynamic_pointer_cast<S>(val.lock()));
    }

    /**
     * Converts the link to a mutable reference to the Maybe node it links to.
     */
    Maybe<T> as_mut() const {
        return Maybe<T>(val.lock());
    }

    /**
     * Converts the link to a const reference to the Maybe node it links to.
     */
    Maybe<const T> as_const() const {
        return Maybe<const T>(std::const_pointer_cast<const T>(val.lock()));
    }

    /**
     * Visit this object.
     */
    template <class V>
    void visit(V &visitor) {
        if (!val.expired()) {
            val.lock()->visit(visitor);
        }
    }

    /**
     * Value-based equality operator. Note that this is identical to
     * pointer-based equality for links.
     */
    bool equals(const OptLink &rhs) const {
        return get_ptr() == rhs.get_ptr();
    }

    /**
     * Pointer-based equality operator.
     */
    bool operator==(const OptLink &rhs) const {
        return get_ptr() == rhs.get_ptr();
    }

    /**
     * Pointer-based inequality operator.
     */
    inline bool operator!=(const OptLink &rhs) const {
        return !(*this == rhs);
    }

    /**
     * Pointer-based greater-than.
     */
    bool operator>(const OptLink &rhs) const {
        return get_ptr() > rhs.get_ptr();
    }

    /**
     * Pointer-based greater-equal.
     */
    bool operator>=(const OptLink &rhs) const {
        return get_ptr() >= rhs.get_ptr();
    }

    /**
     * Pointer-based less-than.
     */
    bool operator<(const OptLink &rhs) const {
        return get_ptr() < rhs.get_ptr();
    }

    /**
     * Pointer-based less-equal.
     */
    bool operator<=(const OptLink &rhs) const {
        return get_ptr() <= rhs.get_ptr();
    }

    /**
     * Returns whether this link links to the given node.
     */
    template <class S>
    bool links_to(const Maybe<S> target) {
        return get_ptr() == std::dynamic_pointer_cast<T>(target.get_ptr());
    }

    /**
     * Traverses the tree to register all reachable Maybe/One nodes with the
     * given map. This also checks whether all One/Maybe nodes only appear once
     * in the tree (except through links). If there are duplicates, a
     * NotWellFormed exception is thrown.
     */
    void find_reachable(PointerMap &map) const override {
        (void)map;
    }

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        if (!this->empty()) {
            map.get(*this);
        }
    }

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    virtual std::string serdes_edge_type() const {
        return "@";
    }

    /**
     * Restores a link after deserialization.
     */
    void set_void_ptr(const std::shared_ptr<void> &ptr) override {
        val = std::static_pointer_cast<T>(ptr);
    }

    /**
     * Constructs a link, checking whether the edge type of the serialized link
     * is correct. The link is NOT registered with the IdentifierMap, because
     * the this pointer is still on the stack at this point and not in the
     * actual tree yet.
     */
    void deserialize(const cbor::MapReader &map, IdentifierMap &ids) {
        (void)ids;
        // Note: this is in a function rather than in the constructor, because
        // serdes_edge_type() would map to the base class if we just chain
        // constructors, and we don't want to repeat this whole mess for Many.
        if (map.at("@T").as_string() != serdes_edge_type()) {
            throw RuntimeError("Schema validation failed: unexpected edge type");
        }
        val.reset();
    }

public:

    /**
     * Serializes this link.
     */
    void serialize(cbor::MapWriter &map, const PointerMap &ids) const {
        map.append_string("@T", serdes_edge_type());
        map.append_int("@l", ids.get(*this));
    }

    /**
     * Constructs a link and registers it with the IdentifierMap to be linked up
     * once the rest of the tree finishes constructing.
     */
    OptLink(const cbor::MapReader &map, IdentifierMap &ids) : val() {
        deserialize(map, ids);
    }

};

/**
 * Convenience class for a reference to exactly one other tree node.
 */
template <class T>
class Link : public OptLink<T> {
public:

    /**
     * Constructor for an empty (invalid) node.
     */
    Link() : OptLink<T>() {}

    /**
     * Constructor for an empty or filled node given the node to link to.
     */
    template <class S>
    Link(const Maybe<S> &value) : OptLink<T>(value) {}

    /**
     * Constructor for an empty or filled node given the node to link to.
     */
    template <class S>
    Link(Maybe<S> &&value) : OptLink<T>(std::move(value)) {}

    /**
     * Constructor for an empty or filled node given an existing link.
     */
    template <class S>
    Link(const OptLink<S> &value) : OptLink<T>(value) {}

    /**
     * Constructor for an empty or filled node given an existing link.
     */
    template <class S>
    Link(OptLink<S> &&value) : OptLink<T>(std::move(value)) {}

    /**
     * Checks completeness of this node given a map of raw, internal Node
     * pointers to sequence numbers for all nodes reachable from the root. That
     * is:
     *  - all One, Link, and Many edges have (at least) one entry;
     *  - all the One entries internally stored by Any/Many have an entry;
     *  - all Link and filled OptLink nodes link to a node previously registered
     *    with the PointerMap.
     * If not complete, a NotWellFormed exception is thrown.
     */
    void check_complete(const PointerMap &map) const override {
        if (this->empty()) {
            std::ostringstream ss{};
            ss << "'Link' edge of type " << typeid(T).name() << " is empty";
            throw NotWellFormed(ss.str());
        }
        map.get(*this);
    }

protected:

    /**
     * Returns the value for the `@T` tag.
     */
    std::string serdes_edge_type() const override {
        return "$";
    }

public:

    /**
     * Constructs a link and registers it with the IdentifierMap to be linked up
     * once the rest of the tree finishes constructing.
     */
    Link(const cbor::MapReader &map, IdentifierMap &ids) : OptLink<T>() {
        this->deserialize(map, ids);
    }

};

/**
 * Registers a node pointer and gives it a sequence number. If a duplicate
 * node is found and exceptions are enabled, this raises a NotWellFormed.
 * Otherwise, the previously added link number is returned.
 */
template <class T>
size_t PointerMap::add(const Maybe<T> &ob) {
    return add_raw(reinterpret_cast<const void*>(ob.get_ptr().get()), typeid(T).name());
}

/**
 * Registers a node pointer by means of a direct reference and gives it a
 * sequence number. If a duplicate node is found and exceptions are enabled,
 * this raises a NotWellFormed. Otherwise, the previously added link number is
 * returned.
 */
template <class T>
size_t PointerMap::add_ref(const T &ob) {
    return add_raw(reinterpret_cast<const void*>(&ob), typeid(T).name());
}

/**
 * Returns the sequence number of a previously added node. If the node was
 * not previously added and exceptions are enabled, this raises a
 * NotWellFormed. Otherwise, INVALID is returned.
 */
template <class T>
size_t PointerMap::get(const Maybe<T> &ob) const {
    return get_raw(reinterpret_cast<const void*>(ob.get_ptr().get()), typeid(T).name());
}

/**
 * Returns the sequence number of a previously added node. If the node was
 * not previously added and exceptions are enabled, this raises a
 * NotWellFormed. Otherwise, INVALID is returned.
 */
template <class T>
size_t PointerMap::get(const OptLink<T> &ob) const {
    return get_raw(reinterpret_cast<const void*>(ob.get_ptr().get()), typeid(T).name());
}

/**
 * Returns the sequence number of a previously added node by means of a
 * direct reference. If the node was not previously added and exceptions are
 * enabled, this raises a NotWellFormed. Otherwise, INVALID is returned.
 */
template <class T>
size_t PointerMap::get_ref(const T &ob) const {
    return get_raw(reinterpret_cast<const void*>(&ob), typeid(T).name());
}

/**
 * Entry point for tree serialization to a stream.
 */
template <class T>
void serialize(const Maybe<T> tree, std::ostream &stream) {
    tree::cbor::Writer writer{stream};
    PointerMap ids{};
    tree.find_reachable(ids);
    tree.check_complete(ids);
    auto map = writer.start();
    tree.serialize(map, ids);
    map.close();
}

/**
 * Entry point for tree serialization to a string.
 */
template <class T>
std::string serialize(const Maybe<T> tree) {
    std::ostringstream stream{};
    serialize<T>(tree, stream);
    return stream.str();
}

/**
 * Entry point for tree serialization to a file.
 */
template <class T>
void serialize_file(const Maybe<T> tree, const std::string &filename) {
    serialize<T>(tree, std::ofstream(filename));
}

/**
 * Entry point for tree deserialization from a string.
 */
template <class T>
Maybe<T> deserialize(const std::string &cbor) {
    cbor::Reader reader{cbor};
    IdentifierMap ids{};
    Maybe<T> tree{reader.as_map(), ids};
    ids.restore_links();
    tree.check_well_formed();
    return tree;
}

/**
 * Entry point for tree deserialization from a stream.
 */
template <class T>
Maybe<T> deserialize(std::istream &stream) {
    std::ostringstream ss;
    ss << stream.rdbuf();
    return deserialize<T>(ss.str());
}

/**
 * Entry point for tree deserialization from a file.
 */
template <class T>
Maybe<T> deserialize_file(const std::string &&filename) {
    return deserialize<T>(std::ifstream(filename));
}

} // namespace base
TREE_NAMESPACE_END
