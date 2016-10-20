// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Types.hpp"

#include <array>

namespace util
{
    struct SetTuple
    {
        template<typename T, typename... Params> static void set(ostream_t& os, T&& t, Params&&... p)
        { os << t << UTIL_DEFAULT_DELIM; set(os, p...); }
        template <typename T> static void set(ostream_t& os, T&& t)
        { os << t; }
    };

    struct SetT
    {
        template<typename T, typename... Params> static void set(ostream_t& os, T&& t, Params&&... p)
        { os << t << ' '; set(os, p...); }
        template <typename T> static void set(ostream_t& os, T&& t)
        { os << t; }
    };

    template<typename... Params> static void buildTuple(ostream_t& os, Params... params)
    {
        os << UTIL_DEFAULT_NEST;
        SetTuple::set(os, std::forward<Params>(params)...);
        os << UTIL_DEFAULT_UNNEST;
    }

    template <typename... Params> static void buildT(ostream_t& os, Params... params)
    { SetT::set(os, std::forward<Params>(params)...); }

    template <typename It> static void buildList(ostream_t& os, It begin, It end)
    {
        os << UTIL_DEFAULT_NEST;
        if (begin != end--)
            for (; begin != end; ++begin)
                os << *begin << UTIL_DEFAULT_DELIM;
        os << *begin << UTIL_DEFAULT_UNNEST;
    }

    template <typename T, size_t N> static void buildList(ostream_t& os, T const(&array)[N])
    {
        os << UTIL_DEFAULT_NEST;
        for (unsigned i = 0; i < N-1; ++i)
            os << array[i] << UTIL_DEFAULT_DELIM;
        os << array[N-1] << UTIL_DEFAULT_UNNEST;
    }

    template <typename T, size_t N> static void buildList(ostream_t& os, std::array<T,N> const& array)
    {
        os << UTIL_DEFAULT_NEST;
        for (unsigned i = 0; i < N-1; ++i)
            os << array[i] << UTIL_DEFAULT_DELIM;
        os << array[N-1] << UTIL_DEFAULT_UNNEST;
    }
}
