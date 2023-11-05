#pragma once

#include "cqasm-overload.hpp"
#include "cqasm-tree.hpp"

#include <exception>
#include <string>
#include <unordered_map>
#include <utility>  // pair
#include <vector>


/**
 * Overload templates are case-sensitive.
 * The code using them can be case-insensitive by converting to lowercase before adding and resolving an overload.
 */
namespace cqasm::overload {

struct NameResolutionFailure : public std::exception {};
struct OverloadResolutionFailure : public std::exception {};

/**
 * Represents a possible overload for the parameter types of a function, gate, or error model.
 * T is some tag type identifying the overload.
 */
template <class T, class TypeBase>
class Overload {
    using Type = tree::One<TypeBase>;
    using Types = tree::Any<TypeBase>;

    T tag_;
    Types param_types_;

public:
    Overload(const T &tag, const Types &param_types) : tag_{ tag }, param_types_{ param_types} {}
    Overload(const Overload &other) : tag_{ other.tag_ }, param_types_{ other.param_types_ } {}
    Overload(Overload &&other) noexcept : tag_{ std::move(other.tag_) }, param_types_{ other.param_types_ } {}
    Overload& operator=(const Overload &other) { tag_ = other.tag_; param_types_ = other.param_types_; return *this; }
    Overload& operator=(Overload &&other) noexcept { tag_ = std::move(other.tag_); param_types_ = other.param_types_; return *this; }

    /**
     * Returns the tag for this overload.
     */
    [[nodiscard]] const T &get_tag() const {
        return tag_;
    }

    /**
     * Returns the number of parameters for this overload.
     */
    [[nodiscard]] std::size_t num_params() const {
        return param_types_.size();
    }

    /**
     * Returns the parameter type object for the parameter at the specified index.
     */
    [[nodiscard]] const Type &param_type_at(std::size_t index) const {
        return param_types_.at(index);
    }
};

/**
 * Represents a set of possible overloads for the parameter types of a
 * function, gate, or error model. T is some tag type identifying the overload.
 * In case of a function, T would contain at least the return type, but maybe
 * also a lambda to represent the actual function. Note that ambiguous
 * overloads are silently resolved by using the last applicable overload, so
 * more specific overloads should always be added last.
 */
template <class T, class TypeBase, class Node>
class OverloadResolver {
    using Type = tree::One<TypeBase>;
    using Types = tree::Any<TypeBase>;
    using Value = tree::One<Node>;
    using Values = tree::Any<Node>;

    std::vector<Overload<T, TypeBase>> overloads;

public:
    /**
     * Adds a possible overload to the resolver. Note that ambiguous
     * overloads are silently resolved by using the last applicable overload,
     * so more specific overloads should always be added last.
     */
    void add_overload(const T &tag, const Types &param_types) {
        overloads.emplace_back(tag, param_types);
    }

    /**
     * Tries to resolve which overload belongs to the given argument list, if
     * any. Raises an OverloadResolutionFailure if no applicable overload
     * exists, otherwise the tag corresponding to the first proper overload and
     * the appropriately promoted vector of value pointers are returned.
     */
    [[nodiscard]] std::pair<T, Values> resolve(const Values &args) {
        for (auto overload = overloads.rbegin(); overload != overloads.rend(); ++overload) {
            if (overload->num_params() != args.size()) {
                continue;
            }
            Values promoted_args;
            bool ok = true;
            for (size_t i = 0; i < args.size(); i++) {
                auto promoted_arg = promote(args.at(i), overload->param_type_at(i));
                if (promoted_arg.empty()) {
                    ok = false;
                    break;
                }
                promoted_args.add(promoted_arg);
            }
            if (ok) {
                return std::pair<T, Values>(overload->get_tag(), promoted_args);
            }
        }
        throw OverloadResolutionFailure{};
    }
};

/**
 * Table of overloaded callables with case-sensitive identifier matching.
 * T is the tag type of the callable/overload pair.
 */
template <class T, class TypeBase, class Node>
class OverloadedNameResolver {
    using Type = tree::One<TypeBase>;
    using Types = tree::Any<TypeBase>;
    using Value = tree::One<Node>;
    using Values = tree::Any<Node>;

    std::unordered_map<std::string, OverloadResolver<T, TypeBase, Node>> table;

public:
    /**
     * Registers a callable.
     * Matching will be done case-sensitively.
     * The param_types variadic specifies the amount and types of the parameters that
     * (this particular overload of) the function expects.
     * The C++ implementation of the function can assume that
     * the value list it gets is of the right size and the values are of the right types.
     * Note that ambiguous overloads are silently resolved by using the last applicable overload,
     * so more specific overloads should always be added last.
     */
    virtual void add_overload(const std::string &name, const T &tag, const Types &param_types) {
        auto entry = table.find(name);
        if (entry == table.end()) {
            auto resolver = OverloadResolver<T, TypeBase, Node>();
            resolver.add_overload(tag, param_types);
            table.insert(std::pair<std::string, OverloadResolver<T, TypeBase, Node>>(name, std::move(resolver)));
        } else {
            entry->second.add_overload(tag, param_types);
        }
    }

    /**
     * Resolves the particular overload for the callable with the given case-sensitively matched name.
     * Raises NameResolutionFailure if no callable with the requested name is found,
     * raises an OverloadResolutionFailure if overload resolution fails, or otherwise
     * returns the tag of the first applicable callable/overload pair and
     * the appropriately promoted vector of value pointers.
     */
    [[nodiscard]] virtual std::pair<T, Values> resolve(const std::string &name, const Values &args) {
        auto entry = table.find(name);
        if (entry == table.end()) {
            throw NameResolutionFailure{};
        } else {
            return entry->second.resolve(args);
        }
    }
};

}  // cqasm::overload
