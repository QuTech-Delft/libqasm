/** \file
 * Includes all .hpp.inc files relevant to build a macro-configurable tree
 * support library. This allows you to create an alternative tree library in a
 * different namespace with different structures backing the tree classes. For
 * example, you can override the exception type from the standard library with
 * one that includes a backtrace.
 *
 * To use this, you need the following things in your application:
 *
 *  - an include file that defines the configuration for the custom tree
 *    implementation (see tree-default-config.hpp.inc);
 *  - a header file for your tree that first includes your include file and
 *    then includes "tree-all.hpp.inc";
 *  - a source file for your tree that first includes your include file and
 *    then includes "tree-all.cpp.inc".
 */

// Set defaults for any unset configuration entries.
#include "tree-default-config.hpp.inc"

// Include headers.
#include "tree-compat.hpp.inc"
#include "tree-cbor.hpp.inc"
#include "tree-annotatable.hpp.inc"
#include "tree-base.hpp.inc"

// Undefine configuration.
#include "tree-undef.hpp.inc"
