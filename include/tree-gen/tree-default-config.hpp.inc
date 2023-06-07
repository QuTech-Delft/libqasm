/** \file
 * Sets the default configuration for macro-generated tree support files. Does
 * not override previously-defined macros.
 */

#ifndef TREE_NAMESPACE_BEGIN
/// Opens the namespace(s) that the tree support structures are defined in.
#define TREE_NAMESPACE_BEGIN        namespace tree {
#endif

#ifndef TREE_NAMESPACE_END
/// Closes the namespace(s) that the tree support structures are defined in.
#define TREE_NAMESPACE_END          }
#endif

#ifndef TREE_VECTOR
/// Map used to store things like annotations in.
#define TREE_VECTOR(V)              std::vector<V>
#endif

#ifndef TREE_MAP
/// Map used to store things like annotations in.
#define TREE_MAP(K, V)              std::map<K, V>
#endif

#ifndef TREE_MAP_SET
/// Map operation for setting a value that may or may not exist yet.
#define TREE_MAP_SET(m, k, v)       (m)[k] = (v)
#endif

#ifndef TREE_RUNTIME_ERROR
/// The type used for generic exceptions.
#define TREE_RUNTIME_ERROR          std::runtime_error
#endif

#ifndef TREE_RANGE_ERROR
/// The type used for range errors and failed dereferences.
#define TREE_RANGE_ERROR            std::out_of_range
#endif
