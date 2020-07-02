/** \dir
 * Contains the source files for the \ref tree-gen tool.
 *
 * \file
 * Header file for \ref tree-gen.
 *
 * \page tree-gen tree-gen
 * tree-gen is a code generator that outputs all the repetitive stuff needed
 * for defining a proper tree structure in C++.
 *
 * Trees are described by *.tree files. Such a file consists of a number of
 * directives at the top and one or more tree node descriptions. Each tree node
 * gets its own C++ class, with a bunch of methods defined on it for traversal,
 * the visitor pattern, safe typecasting, cloning, and so on. It would be
 * extremely tedious and error prone to define all this stuff manually, hence
 * the generator.
 *
 * It is important to realize and perhaps not immediately obvious that the
 * recursive structure in a *.tree file represents C++ inheritance rather than
 * the actual tree structure. You may for instance encounter something like
 * this:
 *
 * ```
 * expression {
 *     addition {
 *         ...
 *     }
 *     subtraction {
 *         ...
 *     }
 * }
 * ```
 *
 * That doesn't mean an expression consists of an addition and a subtraction,
 * but rather that an addition is a type of expression, and subtraction is
 * another.
 *
 * \section base Base classes and primitives
 *
 * Before we can properly define what a tree looks like, we need a few base
 * classes and primitives defined outside of the generated code, for the
 * generated code to use/depend on. The five required base classes are:
 *
 *  - `Base`: all generated node classes inherit from this class.
 *  - `Maybe<T>`: class used when zero or one references to a child node of
 *    type `T` are needed.
 *  - `One<T>`: class used when exactly one reference to a child node of type
 *    `T` is needed.
 *  - `Any<T>`: class used when zero or more references to child nodes of type
 *    `T` are needed.
 *  - `Many<T>`: class used when one or more references to child nodes of type
 *    `T` are needed.
 *
 * The namespace these classes are to be found in can be set using the
 * `tree_namespace` directive at the top of the tree file. The generator
 * expects `Maybe<T>` and `One<T>` to have an overload of dereferencing to
 * access the child node, whereas it expects `Any<T>` and `Many<T>` to behave
 * like a vector of `One<T>`. It also expects Maybe/One/Any/Many to have an
 * is_complete() method, returning whether all constraints are met (One has a
 * node, the array for a Many is not empty, all array entries of Any/Many are
 * filled), in order to be able to generate such a method for the nodes, to
 * check the constraints of the entire (sub)tree.
 *
 * The other set of types needed are primitive types. These are used to
 * represent data on the nodes. For instance, an integer literal node have some
 * primitive integer attached to it to represent the value. Primitive nodes
 * need to have a stream operator<< overload for the debug dumps. The generated
 * code also assumes that there is a function with prototype `T()` defined for
 * all primitive types T, that returns a sane default value to initialize the
 * members of nodes with when they are not specified in the constructor. This
 * prevents node members of an actual C++ primitive type from being
 * uninitialized by default. The namespace and name of the function is specified
 * with the `initialize_function` directive.
 *
 * \section nodes Defining nodes
 *
 * Nodes have the following form in the tree file:
 *
 * ```
 * # [documentation for node class]
 * [snake_case_node_name] {
 *
 *     # [documentation for primitive]
 *     [snake_case_member_name]: [C++ namespace path];
 *
 *     # [documentation for child node]
 *     [snake_case_member_name]: [Maybe|One|Any|Many]<[TitleCaseNodeName]>;
 *
 *     # [documentation for external child node]
 *     [snake_case_member_name]: external [Maybe|One|Any|Many]<[C++ namespace path]>;
 *
 *     [zero or more specializations of this node; recursive structure]
 *
 * }
 * ```
 *
 * Nodes can have zero or more primitives, zero or more child nodes, zero or
 * more *external* child nodes, and zero or more specializations.
 *
 * The difference between normal child nodes and external child nodes is that
 * normal child nodes must resolve to another node in the same tree file, while
 * an external node uses any C++ type name for the node class. That allows you
 * to refer to nodes defined in other tree files as well. Note however that the
 * generator expects external nodes to actually be node classes, such that it
 * can properly generate the debug dump indentation logic, among other things.
 *
 * Note that the name of a node *has* to be specified in `snake_case`, while
 * the generated class is named in `TitleCase`. This is just because it's easier
 * to convert snake case to title case than vice versa, and the generator uses
 * both forms (title case for the class names, snake case for methods and
 * members).
 *
 * Note also the comments explicitly added to the example. These comments are
 * actually used by the generator; they are copied into the source code using
 * javadoc-style comment blocks.
 *
 * The order in which nodes are defined doesn't matter. This allows you to make
 * recursive tree structures, without having to worry about forward
 * declarations. The generator handles those.
 *
 * \section directive Directives
 *
 * The following directives exist. They should be placed at the top of the tree
 * file, before the first node. Their order doesn't matter, unless otherwise
 * specified. Note that most directives are required.
 *
 *  - `source "<filename>"`: sets the filename for the generated C++ file. Any
 *    comment above the directive is copied into the file as file-level doxygen
 *    documentation.
 *
 *  - `header "<filename>"`: sets the filename for the generated header file.
 *    Any comment above the directive is copied into the file as file-level
 *    doxygen documentation.
 *
 *  - `tree_namespace <namespace::path>`: the namespace that the
 *    Base/Maybe/One/Any/Many classes live in. Comment is ignored.
 *
 *  - `initialize_function <namespace::path::initialize>`: the name (including
 *    namespace path leading up to it) of the `T()` function used for getting
 *    the default value of any of the used primitive classes. Comment is
 *    ignored.
 *
 *  - `location <namespace_path::SourceLocation>`: optionally, the name of the
 *    source location annotation class (including namespace path leading up to
 *    it). The debug dump will look for an annotation of this type on each node
 *    when doing a debug dump, and if it exists, uses it to add source
 *    information to the node, by streaming out a `#` followed by the stream
 *    overload for the class. The base class needs to be capable of annotations
 *    if you use this. Comment is ignored.
 *
 *  - `include "<path>"`: adds an `#include` statement to the top of the
 *    generated header file. Comment is ignored.
 *
 *  - `src_include "<path>"`: like `include`, but adds to the top of the
 *    generated C++ file only.
 *
 *  - `namespace <name>`: used to specify the namespace for the generated tree
 *    classes. As in C++, you need multiple of these to specify the full path.
 *    The comment for the *first* annotation of this type that has a comment
 *    in front is used to document the innermost namespace javadoc-style for
 *    Doxygen documentation.
 *
 * \section apis Generated APIs
 *
 * The following methods are generated for each node class:
 *
 *  - a constructor with all members of the node in its signature as optional
 *    values, defaulting to the values returned by the initializer function.
 *
 *  - `bool is_complete() const`: returns whether the node its called on and
 *    any subtree rooted in that node is fully defined (no empty One or Many,
 *    all array entries in Any/Many nonempty).
 *
 *  - `%NodeType type() const`: returns the type of this node, using the
 *    also-generated %NodeType enumeration.
 *
 *  - `One<Node> clone() const`: returns a shallow copy of this node.
 *
 *  - An equality and inequality operator. Note that this ignores equality of
 *    any annotations.
 *
 *  - `void visit(Visitor &visitor)`: implements the visitor pattern using the
 *    also-generated abstract Visitor/RecursiveVisitor classes. See below.
 *
 *  - `void dump(std::ostream &out=std::cout, int indent=0)`: does a debug
 *    dump of the node to the given stream with the given indentation level.
 *
 *  - `SomeNodeType *as_some_node_type()`: does the equivalent of a
 *    `dynamic_cast` to the given node type, returning `this` if the type is
 *    correct or `nullptr` if not.
 *
 * An implicit node class simply named `Node` is always generated, serving as
 * the base class for all other nodes. It is what derives from the `Base` class
 * defined along with `Maybe/One/Any/Many`.
 *
 * Tree traversal is complicated by the fact that you often don't know exactly
 * which type a class is. For example, an expression node may actually be a
 * subtraction or addition. There are a few different patterns supported by
 * tree-gen:
 *
 *  - *Visitor pattern.* You define a class inheriting from the generated
 *    `Visitor` or `RecursiveVisitor` classes. These abstract classes provide
 *    or require you to implement functions for each node type. The appropriate
 *    one then gets called when you pass an instance of your visitor class to
 *    the `visit()` method on a node. You must always override `visit_node()`,
 *    usually to throw a suitable exception in case an unexpected node is
 *    encountered. The default implementation for all the other node types
 *    differs between the two classes. For `Visitor`, the default implementation
 *    falls back to the next more generically typed function
 *    (`visit_subtraction()` falls back to `visit_expression()`,
 *    `visit_expression()` falls back to `visit_node()`). For
 *    `RecursiveVisitor`, the default implementation for non-leaf node types
 *    recursively calls `visit()` for all child nodes, thus recursively
 *    traversing the tree. For leaf nodes, both visitor classes have the same
 *    behavior.
 *
 *  - *Using the `as_*()` methods.* Given for instance an expression node, you
 *    might do
 *    ```
 *    if (auto addition = expression.as_addition()) {
 *      ...
 *    } else if (auto subtraction = expression.as_subtraction()) {
 *      ...
 *    } else {
 *      ...
 *    }
 *    ```
 *    Because a `nullptr` evaluates to false in C++, the blocks will only be
 *    executed if the cast succeeds. Be careful copypasting though; you can
 *    accidentally use the `addition` variable in the `subtraction` block if
 *    you'd want, but it's obviously null in that case. C++ scoping is weird.
 *
 *  - *Using a switch statement.* You might do
 *    ```
 *    switch (expression.type()) {
 *      case NodeType::Addition:
 *        ...
 *      case NodeType::Subtraction:
 *        ...
 *    }
 *    ```
 *    This doesn't handle the cast for you, but in cases where you only need to
 *    switch based on the type and don't need access to members of the nodes
 *    this is more descriptive than the if/else form.
 *
 * Just choose the method that makes the most sense within context.
*/

#ifndef _TREE_GEN_HPP_INCLUDED_
#define _TREE_GEN_HPP_INCLUDED_

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <memory>
#include <cctype>

/**
 * Repetition modes for AST nodes.
 */
enum ChildType {

    /**
     * Zero or one nodes.
     */
    Maybe,

    /**
     * Exactly one nodes.
     */
    One,

    /**
     * Zero or more nodes.
     */
    Any,

    /**
     * One or more nodes.
     */
    Many,

    /**
     * Primitive type.
     */
    Prim,

};

struct NodeType;

/**
 * Represents a child node.
 */
struct ChildNode {

    /**
     * The type of child node.
     */
    ChildType type;

    /**
     * The child node type, if any (depends on type).
     */
    std::shared_ptr<NodeType> node_type;

    /**
     * The primitive type name, if any (depends on type). If type is not
     * `Prim`, this is used for storing the node name before name resolution.
     */
    std::string prim_type;

    /**
     * Class member name.
     */
    std::string name;

    /**
     * Class member documentation.
     */
    std::string doc;

    /**
     * External node type. This is valid when `type` is Prim. If this is Prim
     * as well, then it's actually a primitive, otherwise it's a node from
     * another tree. This is kind of a kludge for having the dumper class
     * properly handle indentation here; that's the only place it's used.
     */
    ChildType ext_type;
};

/**
 * Represents a type of AST node.
 */
struct NodeType {

    /**
     * Name in snake_case.
     */
    std::string snake_case_name;

    /**
     * Name in TitleCase.
     */
    std::string title_case_name;

    /**
     * Class documentation.
     */
    std::string doc;

    /**
     * The node type this is derived from, if any.
     */
    std::shared_ptr<NodeType> parent;

    /**
     * Node types derived from this one.
     */
    std::vector<std::weak_ptr<NodeType>> derived;

    /**
     * Child nodes.
     */
    std::vector<ChildNode> children;

    /**
     * Whether this node represents a recovered parse error.
     */
    bool is_error_marker;

    /**
     * Gathers all child nodes, including those in parent classes.
     */
    std::vector<ChildNode> all_children() {
        std::vector<ChildNode> children = this->children;
        if (parent) {
            auto from_parent = parent->all_children();
            children.insert(children.end(), from_parent.begin(), from_parent.end());
        }
        return children;
    }

};

/**
 * List of nodes.
 */
using Nodes = std::vector<std::shared_ptr<NodeType>>;

/**
 * Convenience class for constructing a node.
 */
class NodeBuilder {
public:

    /**
     * The node being constructed.
     */
    std::shared_ptr<NodeType> node;

    /**
     * Construct a node with the given snake_case name and class documentation.
     */
    NodeBuilder(const std::string &name, const std::string &doc="") {
        node = std::make_shared<NodeType>();
        node->snake_case_name = name;
        node->doc = doc;
        node->is_error_marker = false;

        // Generate title case name.
        auto snake_ss = std::stringstream(name);
        auto title_ss = std::ostringstream();
        std::string token;
        while (std::getline(snake_ss, token, '_')) {
            title_ss << (char)std::toupper(token[0]) << token.substr(1);
        }
        node->title_case_name = title_ss.str();
    }

    /**
     * Marks this node as deriving from the given node type.
     */
    NodeBuilder *derive_from(std::shared_ptr<NodeType> parent) {
        node->parent = parent;
        parent->derived.push_back(node);
        return this;
    }

    /**
     * Adds a child node. `type` should be `One`, `Maybe`, `Any`, or `Many`.
     */
    NodeBuilder *with_child(
        ChildType type,
        const std::string &node_name,
        const std::string &name,
        const std::string &doc = ""
    ) {
        auto child = ChildNode();
        child.type = type;
        child.prim_type = node_name;
        child.name = name;
        child.doc = doc;
        child.ext_type = type;
        node->children.push_back(std::move(child));
        return this;
    }

    /**
     * Adds a child primitive.
     */
    NodeBuilder *with_prim(
        const std::string &prim,
        const std::string &name,
        const std::string &doc = "",
        ChildType type = Prim
    ) {
        auto child = ChildNode();
        child.type = Prim;
        switch (type) {
            case Maybe: child.prim_type = "Maybe<" + prim + ">"; break;
            case One:   child.prim_type = "One<" + prim + ">"; break;
            case Any:   child.prim_type = "Any<" + prim + ">"; break;
            case Many:  child.prim_type = "Many<" + prim + ">"; break;
            default:    child.prim_type = prim; break;
        }
        child.name = name;
        child.doc = doc;
        child.ext_type = type;
        node->children.push_back(std::move(child));
        return this;
    }

    /**
     * Indicate that this node marks a recovered parse error.
     */
    NodeBuilder *mark_error() {
        node->is_error_marker = true;
        return this;
    }

};

/**
 * Struct containing everything needed for a complete specification.
 */
class Specification {
private:

    /**
     * The node builders.
     */
    std::map<std::string, std::shared_ptr<NodeBuilder>> builders;

public:

    /**
     * The file to output the cpp source to.
     */
    std::string source_filename;

    /**
     * The file to output the header to.
     */
    std::string header_filename;

    /**
     * Source file documentation.
     */
    std::string source_doc;

    /**
     * Header file documentation.
     */
    std::string header_doc;

    /**
     * The include statements to stick at the top of the header file.
     */
    std::vector<std::string> includes;

    /**
     * The include statements to stick at the top of the source file.
     */
    std::vector<std::string> src_includes;

    /**
     * Namespace documentation.
     */
    std::string namespace_doc;

    /**
     * The C++ namespaces to use.
     */
    std::vector<std::string> namespaces;

    /**
     * The namespace to take the tree base types from.
     */
    std::string tree_namespace;

    /**
     * The initialization function to use for default values of members.
     */
    std::string initialize_function;

    /**
     * Annotation object used for source location info, or empty if source
     * locations are not used or are not to be generated (applies to the dumper
     * only).
     */
    std::string source_location;

    /**
     * All the nodes.
     */
    Nodes nodes;

    /**
     * Sets the source filename.
     */
    void set_source(const std::string &fname, const std::string &doc) {
        if (!source_filename.empty()) {
            throw std::runtime_error("duplicate source filename");
        }
        source_filename = fname;
        source_doc = doc;
    }

    /**
     * Sets the header filename.
     */
    void set_header(const std::string &fname, const std::string &doc) {
        if (!header_filename.empty()) {
            throw std::runtime_error("duplicate header filename");
        }
        header_filename = fname;
        header_doc = doc;
    }

    /**
     * Sets the tree namespace.
     */
    void set_tree_namespace(const std::string &name_space) {
        if (!tree_namespace.empty()) {
            throw std::runtime_error("duplicate tree namespace declaration");
        }
        tree_namespace = name_space;
    }

    /**
     * Sets the initialization function.
     */
    void set_initialize_function(const std::string &init_fn) {
        if (!initialize_function.empty()) {
            throw std::runtime_error("duplicate initialization function declaration");
        }
        initialize_function = init_fn;
    }

    /**
     * Sets the source location object.
     */
    void set_source_location(const std::string &ident) {
        if (!source_location.empty()) {
            throw std::runtime_error("duplicate source location object declaration");
        }
        source_location = ident;
    }

    /**
     * Adds an include statement to the header file.
     */
    void add_include(const std::string &include) {
        includes.push_back(include);
    }

    /**
     * Adds an include statement to the source file.
     */
    void add_src_include(const std::string &include) {
        src_includes.push_back(include);
    }

    /**
     * Adds a namespace level.
     */
    void add_namespace(const std::string &name_space, const std::string &doc = "") {
        namespaces.push_back(name_space);
        if (!doc.empty()) {
            namespace_doc = doc;
        }
    }

    /**
     * Adds the given node.
     */
    void add_node(std::shared_ptr<NodeBuilder> &node_builder) {
        auto name = node_builder->node->snake_case_name;
        if (builders.count(name)) {
            throw std::runtime_error("duplicate node name " + name);
        }
        builders.insert(std::make_pair(name, node_builder));
    }

    /**
     * Checks for errors, resolves node names, and builds the nodes vector.
     */
    void build() {
        if (source_filename.empty()) {
            throw std::runtime_error("missing source filename");
        }
        if (header_filename.empty()) {
            throw std::runtime_error("missing header filename");
        }
        if (initialize_function.empty()) {
            throw std::runtime_error("initialization function not specified");
        }
        for (auto &it : builders) {
            for (auto &child : it.second->node->children) {
                if (child.type != Prim) {
                    auto name = child.prim_type;
                    child.prim_type = "";
                    auto nb_it = builders.find(name);
                    if (nb_it == builders.end()) {
                        throw std::runtime_error("use of undefined node " + name);
                    }
                    child.node_type = nb_it->second->node;
                }
            }
            nodes.push_back(it.second->node);
        }
    }

};

#endif
