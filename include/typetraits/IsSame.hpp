#pragma once

#include "Types.hpp"

namespace util
{
    template <typename A, typename B> struct is_same      { DEFINE_VALUE(false) };
    template <typename T>             struct is_same<T,T> { DEFINE_VALUE(true)  };
}