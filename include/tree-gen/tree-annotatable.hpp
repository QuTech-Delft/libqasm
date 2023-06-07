/** \file
 * This file contains the magic needed to allow users to add their own data to
 * tree nodes without needing to change the tree structure.
 *
 * Data can be added by way of annotations. Annotations can be any kind of
 * object; in fact they are identified by their type, so each annotatable
 * object can have zero or one instance of every C++ type associated with it.
 * The goal is to allow users to use the AST, semantic tree, and a few other
 * things in their own code as they operate on the tree.
 */

#pragma once

#include "tree-cbor.hpp"

#include "tree-default-config.hpp.inc"
#include "tree-annotatable.hpp.inc"
#include "tree-undef.hpp.inc"
