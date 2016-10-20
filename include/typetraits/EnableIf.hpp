#pragma once

#include "Types.hpp"

namespace util
{
    template<bool, typename _Tp = void> struct enable_if          { };
    template<typename T>                struct enable_if<true, T> { DEFINE_TYPE(T) };
}