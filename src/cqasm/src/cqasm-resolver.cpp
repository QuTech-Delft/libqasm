#include <unordered_set>
#include "cqasm-resolver.hpp"
#include "cqasm-error.hpp"
#include "cqasm-utils.hpp"

namespace cqasm {
namespace resolver {

using Type = types::Type;
using Types = types::Types;
using Value = values::Value;
using Values = values::Values;


/**
 * Adds a mapping.
 */
void MappingTable::add(
    const std::string &name,
    const values::Value &value,
    const tree::Maybe<ast::Mapping> &node
) {
    table.insert(
        std::make_pair(utils::lowercase(name),
        std::pair<const values::Value, tree::Maybe<ast::Mapping>>(value, node))
    );
}

/**
 * Resolves a mapping. Throws NameResolutionFailure if no mapping by the
 * given name exists.
 */
Value MappingTable::resolve(const std::string &name) const {
    auto entry = table.find(utils::lowercase(name));
    if (entry == table.end()) {
        throw NameResolutionFailure("failed to resolve " + name);
    } else {
        return Value(entry->second.first->clone());
    }
}

/**
 * Grants read access to the underlying map.
 */
const std::unordered_map<std::string, std::pair<const values::Value, tree::Maybe<ast::Mapping>>> &MappingTable::get_table() const {
    return table;
}

/**
 * Represents a possible overload for the parameter types of a function, gate,
 * or error model. T is some tag type identifying the overload.
 */
template <class T>
class Overload {
private:
    T tag;
    Types param_types;
public:
    /**
     * Construct a possible overload.
     */
    template <class... Ts>
    Overload(const T &tag, const Types &param_types)
        : tag(tag), param_types(param_types)
    {}

    /**
     * Returns the tag for this overload.
     */
    const T &get_tag() const {
        return tag;
    }

    /**
     * Returns the number of parameters for this overload.
     */
    size_t num_params() const {
        return param_types.size();
    }

    /**
     * Returns the parameter type object for the parameter at the specified
     * index.
     */
    const Type &param_type_at(size_t index) const {
        return param_types.at(index);
    }
};

/**
 * Represents a set of possible overloads for the parameter types of a
 * function, gate, or error model. T is some tag type identifying the overload.
 * In case of a function, T would contain at least the return type, but maybe
 * also a lambda to represent the actual function. Note that ambiguous
 * overloads are silently resolved by using the first applicable overload, so
 * more specific overloads should always be added first.
 */
template <class T>
class OverloadResolver {
private:
    std::vector<Overload<T>> overloads;

public:

    /**
     * Adds a possible overload to the resolver. Note that ambiguous
     * overloads are silently resolved by using the first applicable overload,
     * so more specific overloads should always be added first.
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
    std::pair<T, Values> resolve(const Values &args) {
        for (const auto &overload : overloads) {
            if (overload.num_params() != args.size()) {
                continue;
            }
            Values promoted_args;
            bool ok = true;
            for (size_t i = 0; i < args.size(); i++) {
                auto promoted_arg = promote(args.at(i), overload.param_type_at(i));
                if (promoted_arg.empty()) {
                    ok = false;
                    break;
                }
                promoted_args.add(promoted_arg);
            }
            if (ok) {
                return std::pair<T, Values>(overload.get_tag(), promoted_args);
            }
        }
        throw OverloadResolutionFailure("failed to resolve overload");
    }

};

/**
 * Table of overloaded callables with case-insensitive identifier matching. T
 * is the tag type of the callable/overload pair.
 */
template <class T>
class OverloadedNameResolver {
private:
    std::unordered_map<std::string, OverloadResolver<T>> table;
public:
    /**
     * Registers a callable. The name should be lowercase; matching will be done
     * case-insensitively. The param_types variadic specifies the amount and
     * types of the parameters that (this particular overload of) the function
     * expects. The C++ implementation of the function can assume that the
     * value list it gets is of the right size and the values are of the right
     * types. Note that ambiguous overloads are silently resolved by using the
     * first applicable overload, so more specific overloads should always be
     * added first.
     */
    void add_overload(const std::string &name, const T &tag, const Types &param_types) {
        std::string name_lower = utils::lowercase(name);
        auto entry = table.find(name_lower);
        if (entry == table.end()) {
            auto resolver = OverloadResolver<T>();
            resolver.add_overload(tag, param_types);
            table.insert(std::pair<std::string, OverloadResolver<T>>(name_lower, std::move(resolver)));
        } else {
            entry->second.add_overload(tag, param_types);
        }
    }

    /**
     * Resolves the particular overload for the callable with the given
     * case-insensitively matched name. Raises NameResolutionFailure if no
     * callable with the requested name is found, raises an
     * OverloadResolutionFailure if overload resolution fails, or otherwise
     * returns the tag of the first applicable callable/overload pair and the
     * appropriately promoted vector of value pointers.
     */
    std::pair<T, Values> resolve(const std::string &name, const Values &args) {
        std::string name_lower = utils::lowercase(name);
        auto entry = table.find(name_lower);
        if (entry == table.end()) {
            throw NameResolutionFailure("failed to resolve " + name);
        } else {
            try {
                return entry->second.resolve(args);
            } catch (OverloadResolutionFailure &e) {
                e.message = std::ostringstream();
                e.message << "failed to resolve overload for " << name;
                e.message << " with argument pack " << values::types_of(args);
                throw;
            }
        }
    }

};

// The following things *are all default*. Unfortunately, the compiler
// can't infer them because OverloadedNameResolver is incomplete.
FunctionTable::FunctionTable() : resolver(new OverloadedNameResolver<FunctionImpl>()) {}
FunctionTable::~FunctionTable() {}
FunctionTable::FunctionTable(const FunctionTable& t) : resolver(new OverloadedNameResolver<FunctionImpl>(*t.resolver)) {}
FunctionTable::FunctionTable(FunctionTable&& t) : resolver(std::move(t.resolver)) {}
FunctionTable& FunctionTable::operator=(const FunctionTable& t) {
    resolver = std::unique_ptr<OverloadedNameResolver<FunctionImpl>>(new OverloadedNameResolver<FunctionImpl>(*t.resolver));
    return *this;
}
FunctionTable& FunctionTable::operator=(FunctionTable&& t) {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers a function. The name should be lowercase; matching will be done
 * case-insensitively. The param_types variadic specifies the amount and
 * types of the parameters that (this particular overload of) the function
 * expects. The C++ implementation of the function can assume that the
 * value list it gets is of the right size and the values are of the right
 * types.
 */
void FunctionTable::add(const std::string &name, const Types &param_types, const FunctionImpl &impl) {
    resolver->add_overload(name, impl, param_types);
}

/**
 * Calls a function. Throws NameResolutionFailure if no function by the
 * given name exists, OverloadResolutionFailure if no overload of the
 * function exists for the given arguments, or otherwise returns the value
 * returned by the function.
 */
Value FunctionTable::call(const std::string &name, const Values &args) const {

    // Resolve the function and typecheck/promote the argument list.
    auto resolution = resolver->resolve(name, args);

    // Call the function with the typechecked/promoted argument list, and
    // return its result.
    return resolution.first(resolution.second);

}

// The following things *are all default*. Unfortunately, the compiler
// can't infer them because OverloadedNameResolver is incomplete.
ErrorModelTable::ErrorModelTable() : resolver(new OverloadedNameResolver<error_model::ErrorModel>()) {}
ErrorModelTable::~ErrorModelTable() {}
ErrorModelTable::ErrorModelTable(const ErrorModelTable& t) : resolver(new OverloadedNameResolver<error_model::ErrorModel>(*t.resolver)) {}
ErrorModelTable::ErrorModelTable(ErrorModelTable&& t) : resolver(std::move(t.resolver)) {}
ErrorModelTable& ErrorModelTable::operator=(const ErrorModelTable& t) {
    resolver = std::unique_ptr<OverloadedNameResolver<error_model::ErrorModel>>(new OverloadedNameResolver<error_model::ErrorModel>(*t.resolver));
    return *this;
}
ErrorModelTable& ErrorModelTable::operator=(ErrorModelTable&& t) {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers an error model.
 */
void ErrorModelTable::add(const error_model::ErrorModel &type) {
    resolver->add_overload(type.name, type, type.param_types);
}

/**
 * Resolves an error model. Throws NameResolutionFailure if no error model
 * by the given name exists, OverloadResolutionFailure if no overload
 * exists for the given arguments, or otherwise returns the resolved error
 * model node. Annotation data and line number information still needs to be
 * set by the caller.
 */
tree::One<semantic::ErrorModel> ErrorModelTable::resolve(const std::string &name, const Values &args) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::ErrorModel>(
        tree::make<error_model::ErrorModel>(resolved.first),
        name,
        resolved.second,
        tree::Any<semantic::AnnotationData>());
}

// The following things *are all default*. Unfortunately, the compiler
// can't infer them because OverloadedNameResolver is incomplete.
InstructionTable::InstructionTable() : resolver(new OverloadedNameResolver<instruction::Instruction>()) {}
InstructionTable::~InstructionTable() {}
InstructionTable::InstructionTable(const InstructionTable& t) : resolver(new OverloadedNameResolver<instruction::Instruction>(*t.resolver)) {}
InstructionTable::InstructionTable(InstructionTable&& t) : resolver(std::move(t.resolver)) {}
InstructionTable& InstructionTable::operator=(const InstructionTable& t) {
    resolver = std::unique_ptr<OverloadedNameResolver<instruction::Instruction>>(new OverloadedNameResolver<instruction::Instruction>(*t.resolver));
    return *this;
}
InstructionTable& InstructionTable::operator=(InstructionTable&& t) {
    resolver = std::move(t.resolver);
    return *this;
}

/**
 * Registers an instruction type.
 */
void InstructionTable::add(const instruction::Instruction &type) {
    resolver->add_overload(type.name, type, type.param_types);
}

/**
 * Resolves an instruction. Throws NameResolutionFailure if no instruction
 * by the given name exists, OverloadResolutionFailure if no overload
 * exists for the given arguments, or otherwise returns the resolved
 * instruction node. Annotation data, line number information, and the
 * condition still need to be set by the caller.
 */
tree::One<semantic::Instruction> InstructionTable::resolve(
    const std::string &name,
    const Values &args
) const {
    auto resolved = resolver->resolve(name, args);
    return tree::make<semantic::Instruction>(
        tree::make<instruction::Instruction>(resolved.first),
        name, values::Value(), resolved.second,
        tree::Any<semantic::AnnotationData>());
}

} // namespace resolver
} // namespace cqasm
