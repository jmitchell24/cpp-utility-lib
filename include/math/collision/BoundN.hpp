// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "BoxN.hpp"

#include "../Types.hpp"
#include "../../typetraits/CallTraits.hpp"
#include "../../String.hpp"

namespace util
{
    template<typename N, size_t D> struct BoundND
    {
    public:
        typedef N              Num;
        typedef VecND<Num,D>   Vec;
        typedef BoundND<Num,D> Bound;

        typedef best_param<Num>   NumParam;
        typedef best_param<Vec>   VecParam;
        typedef best_param<Bound> BoundParam;

        size_t static constexpr Size = D;

        Vec min, max;

        inline constexpr BoundND()
            : min(), max()
        {}

        inline constexpr BoundND(VecParam min, VecParam max)
            : min(min), max(max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoundND<P, D> const& bound)
            : min((Vec)bound.min), max((Vec)bound.max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoxND<P, D> const& box)
            : min((Vec)box.getMin()), max((Vec)box.getMax())
        {}

        template <typename P>
        inline constexpr operator BoxND<P, D>() const
        { return BoxND<P,D>(min, max - min); }

        inline constexpr Vec getSize() const { return max - min; }

        inline Bound& operator= (Bound const& bound)
        {
            min = bound.min;
            max = bound.max;
            return *this;
        }

        inline string_t str() const
        {
            return sprintTuple(min, max);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseTuple(s, min, max);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseTuple(is, min, max);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Bound)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Bound)
    };
}
