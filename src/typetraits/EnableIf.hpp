// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
    template<bool, typename _Tp = void> struct enable_if          { };
    template<typename T>                struct enable_if<true, T> { DEFINE_TYPE(T) };
}
