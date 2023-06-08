/** \file
 * Includes all .hpp.inc files relevant to build a macro-configurable tree
 * support library. See tree-all.hpp.inc for details.
 */

// Set defaults for any unset configuration entries.
#include "tree-default-config.hpp.inc"

// Include headers.
#include "tree-compat.hpp.inc"
#include "tree-cbor.hpp.inc"
#include "tree-annotatable.hpp.inc"
#include "tree-base.hpp.inc"

// Include sources.
#include "tree-cbor.cpp.inc"
#include "tree-annotatable.cpp.inc"
#include "tree-base.cpp.inc"

// Undefine configuration.
#include "tree-undef.hpp.inc"
