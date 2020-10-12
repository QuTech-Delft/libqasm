/** \file
 * Wrapper for pulling parts of tree-gen's support library into libqasm.
 */

#pragma once

#include "tree-annotatable.hpp"
#include "tree-base.hpp"

namespace cqasm {

/**
 * Namespace for wrapping tree-gen's support library.
 */
namespace tree {

using signed_size_t = ::tree::signed_size_t;

using Annotatable = ::tree::annotatable::Annotatable;
using Base = ::tree::base::Base;

template <class T>
using Maybe = ::tree::base::Maybe<T>;

template <class T>
using One = ::tree::base::One<T>;

template <class T>
using Any = ::tree::base::Any<T>;

template <class T>
using Many = ::tree::base::Many<T>;

template <class T>
using Link = ::tree::base::Link<T>;

template <class T>
using OptLink = ::tree::base::OptLink<T>;

/**
 * Constructs a One object, analogous to std::make_shared.
 */
template <class T, typename... Args>
One<T> make(Args... args) {
    return One<T>(std::make_shared<T>(args...));
}

} // namespace tree
} // namespace cqasm
