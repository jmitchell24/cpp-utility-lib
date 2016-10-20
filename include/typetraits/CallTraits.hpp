// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <cstddef>

#include "IsArithmetic.hpp"
#include "IsPointer.hpp"

namespace util
{
namespace detail
{
    template <typename T, bool small_> struct ct_imp2
    { typedef T const& param_type; };

    template <typename T> struct ct_imp2<T, true>
    { typedef T const param_type; };

    template <typename T, bool isp, bool b1> struct ct_imp
    { typedef T const& param_type; };

    template <typename T, bool isp>
    struct ct_imp<T, isp, true>
    { typedef typename ct_imp2<T, sizeof(T) <= sizeof(void*)>::param_type param_type; };

    template <typename T, bool b1>
    struct ct_imp<T, true, b1>
    { typedef T const param_type; };
}
    template <typename T>
    struct call_traits
    {
       typedef T value_type;
       typedef T& reference;
       typedef T const& const_reference;
       typedef typename util::detail::ct_imp<T, is_pointer<T>::value,is_arithmetic<T>::value>::param_type param_type;
    };

    template <typename T>
    struct call_traits<T&>
    {
       typedef T& value_type;
       typedef T& reference;
       typedef T const& const_reference;
       typedef T& param_type;  // hh removed const
    };

    template <typename T> using best_param = typename call_traits<T>::param_type;
}
