/** \file
 * Generalized contents of tree-compat.hpp.
 */

#ifndef _WIN32
#include <sys/types.h>
#endif

TREE_NAMESPACE_BEGIN

/**
 * ssize_t is not defined on Windows, so we replace it with a long long.
 */
#ifdef _WIN32
typedef long long signed_size_t;
#else
typedef ssize_t signed_size_t;
#endif

TREE_NAMESPACE_END
