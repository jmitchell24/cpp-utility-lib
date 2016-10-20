// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "BoundN.hpp"
#include "../Vector2D.hpp"

namespace util
{
    template <typename N> class BoundND<N, 2>
    {
    public:
        typedef N            Num;
        typedef Vec2N<Num>   Vec;
        typedef BoundND<N,2> Bound;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Bound> BoundParam;

        size_t static constexpr Size = 2;

        union
        {
            struct { Num left, top; };
            Vec min;
        };

        union
        {
            struct { Num right, bottom; };
            Vec max;
        };

        inline constexpr BoundND()
            : min(), max()
        {}

        inline constexpr BoundND(NumParam left, NumParam right, NumParam top, NumParam bottom)
            : min(left,top), max(right, bottom)
        {}

        inline constexpr BoundND(Bound const& bound)
            : min(bound.min), max(bound.max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoundND<P, 2> const& bound)
            : min((Vec)bound.min), max((Vec)bound.max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoxND<P, 2> const& box)
            : min((Vec)box.getMin()), max((Vec)box.getMax())
        {}

        template <typename P>
        inline constexpr operator BoxND<P, 2>() const
        { return BoxND<P,2>(min, max - min); }

        inline constexpr Vec getSize  () const { return max - min; }
        inline constexpr Num getWidth () const { return max.x - min.x; }
        inline constexpr Num getHeight() const { return max.y - min.y; }

        inline Bound& operator= (Bound const& bound)
        {
            min = bound.min;
            max = bound.max;
            return *this;
        }

        inline constexpr bool contains(VecParam point) const
        {
            return util::contains(min.x,max.x,point.x) &&
                   util::contains(min.y,max.y,point.y);
        }

        inline constexpr bool contains(BoundParam bound) const
        {
            return util::contains(min.x,max.x, bound.min.x,bound.max.x) &&
                   util::contains(min.y,max.y, bound.min.y,bound.max.y);
        }

        inline constexpr bool intersects(VecParam point) const
        {
            return util::intersects(min.x,max.x,point.x) &&
                   util::intersects(min.y,max.y,point.y);
        }

        inline constexpr bool intersects(BoundParam rect) const
        {
            return util::intersects(min.x,max.x, rect.min.x,rect.max.x) &&
                   util::intersects(min.y,max.y, rect.min.y,rect.max.y);
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

    template<typename N> using Bound2N = BoundND<N, 2>;

    typedef Bound2N<real_t  > Bound2;
    typedef Bound2N<float   > FBound2;
    typedef Bound2N<double  > DBound2;
    typedef Bound2N<int     > IBound2;
    typedef Bound2N<unsigned> UBound2;

    extern template struct BoundND<float   , 2>;
    extern template struct BoundND<double  , 2>;
    extern template struct BoundND<int     , 2>;
    extern template struct BoundND<unsigned, 2>;
}
