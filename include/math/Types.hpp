// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <cstddef>
#include <cstdint>

namespace util
{
    typedef float  real_t;
    typedef real_t radians_t;
    typedef real_t degrees_t;

    template <typename T, size_t S>
    using static_initializer_list = std::array<T,S>&&;

    template <typename N, size_t D> struct VecND;
    template <typename N, size_t R, size_t C> struct MatNRC;

    template <typename N, size_t D, typename It>
    inline VecND<N,D> make_vector(It first, It last);

    template <typename N, size_t R, size_t C, typename It>
    inline MatNRC<N,R,C> make_matrix(It first, It last);
}
