/** \file
 * Undefines all macros defined by a tree configuration file. This is done at
 * the end of every tree support library header to prevent inclusion order from
 * accidentally affecting things.
 */

#undef TREE_NAMESPACE_BEGIN
#undef TREE_NAMESPACE_END
#undef TREE_VECTOR
#undef TREE_MAP
#undef TREE_MAP_SET
#undef TREE_RUNTIME_ERROR
#undef TREE_RANGE_ERROR
