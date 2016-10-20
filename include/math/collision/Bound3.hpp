// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "BoundN.hpp"
#include "../Vector3D.hpp"

namespace util
{
    template <typename N> class BoundND<N, 3>
    {
    public:
        typedef N            Num;
        typedef Vec3N<Num>   Vec;
        typedef BoundND<N,3> Bound;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Bound> BoundParam;

        size_t static constexpr Size = 3;

        union
        {
            struct { Num left, top, back; };
            Vec min;
        };

        union
        {
            struct { Num right, bottom, front; };
            Vec max;
        };

        inline constexpr BoundND()
            : min(), max()
        {}

        inline constexpr BoundND(NumParam left, NumParam right, NumParam top, NumParam bottom, NumParam back, NumParam front)
            : min(left,top,back), max(right,bottom,front)
        {}

        inline constexpr BoundND(Bound const& bound)
            : min(bound.min), max(bound.max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoundND<P, 3> const& bound)
            : min((Vec)bound.min), max((Vec)bound.max)
        {}

        template <typename P>
        inline explicit constexpr BoundND(BoxND<P, 3> const& box)
            : min((Vec)box.getMin()), max((Vec)box.getMax())
        {}

        template <typename P>
        inline constexpr operator BoxND<P, 3>() const
        { return BoxND<P,3>(min, max - min); }

        inline constexpr Vec getSize  () const { return max   - min;   }
        inline constexpr Num getWidth () const { return max.x - min.x; }
        inline constexpr Num getHeight() const { return max.y - min.y; }
        inline constexpr Num getDepth () const { return max.z - min.z; }

        inline constexpr bool contains(VecParam point) const
        {
            return util::contains(min.x,max.x,point.x) &&
                   util::contains(min.y,max.y,point.y) &&
                   util::contains(min.z,max.z,point.z);
        }

        inline constexpr bool contains(BoundParam bound) const
        {
            return util::contains(min.x,max.x,bound.min.x,bound.max.x) &&
                   util::contains(min.y,max.y,bound.min.y,bound.max.y) &&
                   util::contains(min.z,max.z,bound.min.z,bound.max.z);
        }

        inline constexpr bool intersects(VecParam point) const
        {
            return util::intersects(min.x,max.x,point.x) &&
                   util::intersects(min.y,max.y,point.y) &&
                   util::intersects(min.z,max.z,point.z);
        }

        inline constexpr bool intersects(BoundParam rect) const
        {
            return util::intersects(min.x,max.x,rect.min.x,rect.max.x) &&
                   util::intersects(min.y,max.y,rect.min.y,rect.max.y) &&
                   util::intersects(min.z,max.z,rect.min.z,rect.max.z);
        }

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

    template<typename N> using Bound3N = BoundND<N, 3>;

    typedef Bound3N<real_t  > Bound3;
    typedef Bound3N<float   > FBound3;
    typedef Bound3N<double  > DBound3;
    typedef Bound3N<int     > IBound3;
    typedef Bound3N<unsigned> UBound3;

    extern template struct BoundND<float   , 3>;
    extern template struct BoundND<double  , 3>;
    extern template struct BoundND<int     , 3>;
    extern template struct BoundND<unsigned, 3>;
}
