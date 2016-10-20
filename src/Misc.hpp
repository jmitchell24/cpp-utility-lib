// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "misc/Integer.hpp"
#include "misc/NoCopy.hpp"
#include "misc/Optional.hpp"
#include "misc/ParameterPack.hpp"
#include "misc/Random.hpp"
#include "misc/StaticBuffer.hpp"
#include "misc/SortedVector.hpp"
#include "misc/Timer.hpp"
#include "misc/TypeName.hpp"
#include "misc/UniqueID.hpp"
#include "misc/File.hpp"

#include "Types.hpp"

namespace util
{
/*
    template<typename Fn, int I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    ForTuple(Fn& fn, std::tuple<Tp...>& t)
    {}

    template<typename Fn, int I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    ForTuple(Fn& fn, std::tuple<Tp...>& t)
    {
        fn(std::get<I>(t),I);
        ForTuple<Fn,I + 1, Tp...>(fn,t);
    }

    template <typename T, size_t N> inline size_t getArraySize(const T(&)[N])
    {
        return N;
    }


*/
}
