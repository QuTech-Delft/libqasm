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
     * The include statements to stick at the top of the header file.
     */
    std::vector<std::string> includes;

    /**
     * The include statements to stick at the top of the source file.
     */
    std::vector<std::string> src_includes;

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
    void set_source(const std::string &fname) {
        if (!source_filename.empty()) {
            throw std::runtime_error("duplicate source filename");
        }
        source_filename = fname;
    }

    /**
     * Sets the header filename.
     */
    void set_header(const std::string &fname) {
        if (!header_filename.empty()) {
            throw std::runtime_error("duplicate header filename");
        }
        header_filename = fname;
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
    void add_namespace(const std::string &name_space) {
        namespaces.push_back(name_space);
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
