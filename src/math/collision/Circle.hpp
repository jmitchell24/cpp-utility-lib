// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Vector2D.hpp"
#include "../../String.hpp"

namespace util
{
    template <typename Num> struct CircleN
    {
    public:
        typedef Vec2N<Num>   Vec;
        typedef CircleN<Num> Circle;

        typedef best_param<Num>    NumParam;
        typedef best_param<Vec>    VecParam;
        typedef best_param<Circle> CircleParam;

        Vec position;
        Num   radius;

        inline constexpr CircleN()
            : position(), radius((Num)1)
        {}

        inline constexpr CircleN(VecParam position, NumParam radius)
            : position(position), radius(radius)
        {}

        inline constexpr CircleN(Circle const& circle)
            : position(circle.position), radius(circle.radius)
        {}

        template <typename P>
        inline explicit constexpr CircleN(CircleN<P> const& circle)
            : position((Vec)circle.position), radius((Num)circle.radius)
        {}

        inline constexpr bool intersects(VecParam point) const
        {
            return position.distance(point) <= radius;
        }

        inline constexpr bool intersects(CircleParam circle) const
        {
            return position.distance(circle.position) - circle.radius <= radius;
        }

        inline constexpr bool contains(VecParam point) const
        {
            return position.distance(point) < radius;
        }

        inline constexpr bool contains(CircleParam circle) const
        {
            return position.distance(circle.position) + circle.radius < radius;
        }

        inline string_t str() const
        {
            return sprintTuple(position, radius);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseTuple(s, position, radius);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseTuple(is, position, radius);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Circle)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Circle)
    };

    typedef CircleN<real_t  > Circle;
    typedef CircleN<float   > FCircle;
    typedef CircleN<double  > DCircle;
    typedef CircleN<int     > ICircle;
    typedef CircleN<unsigned> UCircle;

    extern template struct CircleN<float   >;
    extern template struct CircleN<double  >;
    extern template struct CircleN<int     >;
    extern template struct CircleN<unsigned>;
}
