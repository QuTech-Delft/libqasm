TREE_NAMESPACE_BEGIN
namespace base {

/**
 * Internal implementation for add(), given only the raw pointer and the
 * name of its type for the error message.
 */
size_t PointerMap::add_raw(const void *ptr, const char *name) {
    auto it = map.find(ptr);
    if (it != map.end()) {
        if (enable_exceptions) {
            std::ostringstream ss{};
            ss << "Duplicate node of type " << name;
            ss << "at address " << std::hex << ptr << " found in tree";
            throw NotWellFormed(ss.str());
        } else {
            return it->second;
        }
    }
    size_t sequence = map.size();
    map.emplace(ptr, sequence);
    return sequence;
}

/**
 * Internal implementation for get(), given only the raw pointer and the
 * name of its type for the error message.
 */
size_t PointerMap::get_raw(const void *ptr, const char *name) const {
    auto it = map.find(ptr);
    if (it == map.end()) {
        if (enable_exceptions) {
            std::ostringstream ss{};
            ss << "Link to node of type " << name;
            ss << " at address " << std::hex << ptr << " not found in tree";
            throw NotWellFormed(ss.str());
        } else {
            return (size_t)-1;
        }
    }
    return it->second;
}

/**
 * Registers a constructed node.
 */
void IdentifierMap::register_node(size_t identifier, const std::shared_ptr<void> &ptr) {
    nodes.emplace(identifier, ptr);
}

/**
 * Registers a constructed link.
 */
void IdentifierMap::register_link(LinkBase &link, size_t identifier) {
    links.emplace_back(link, identifier);
}

/**
 * Restores all the links after the tree finishes constructing.
 */
void IdentifierMap::restore_links() const {
    for (auto &it : links) {
        it.first.set_void_ptr(nodes.at(it.second));
    }
}

/**
 * Traverses the tree to register all reachable Maybe/One nodes with the
 * given map. This also checks whether all One/Maybe nodes only appear once
 * in the tree (except through links). If there are duplicates, a
 * NotWellFormed exception is thrown.
 */
void Completable::find_reachable(PointerMap &map) const {
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
void Completable::check_complete(const PointerMap &map) const {
    (void)map;
}

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
void Completable::check_well_formed() const {
    PointerMap map{};
    find_reachable(map);
    check_complete(map);
}

/**
 * Returns whether the tree starting at this node is well-formed. That is:
 *  - all One, Link, and Many edges have (at least) one entry;
 *  - all the One entries internally stored by Any/Many have an entry;
 *  - all Link and filled OptLink nodes link to a node that's reachable from
 *    this node;
 *  - the nodes referred to be One/Maybe only appear once in the tree
 *    (except through links).
 */
bool Completable::is_well_formed() const {
    try {
        check_well_formed();
        return true;
    } catch (NotWellFormed &) {
        return false;
    }
}

} // namespace base
TREE_NAMESPACE_END
