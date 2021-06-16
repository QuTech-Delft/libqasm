/**
 * \dir
 * Contains the non-generated public header files for libqasm's new API.
 *
 * \file
 * Main include file for libqasm; this is what you should be `#include`ing.
 */

#pragma once

#include "cqasm-v1.hpp"

/**
 * Toplevel namespace with entry points for the new API.
 */
namespace cqasm {

// This is the toplevel public header for the new API for v1.x. At the time it
// was written, there were no major version namespaces yet (added in
// preparation for v2.x, though unfortunately this hasn't come to fruition yet
// at the time of writing). Thus, to maintain API compatibility, we need to
// pull the v1 namespace into this one when this file is used.
using namespace v1;

} // namespace cqasm
