// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Ray.hpp"
#include "../Vector3D.hpp"
#include "../../String.hpp"

namespace util
{
    template <typename N> class TTriangle3 : public IntersectsRay<N>
    {
    public:
        typedef N               Num;
        typedef Vec3N<Num>      Vec;
        typedef RayN<Num>       Ray;
        typedef TTriangle3<Num> Triangle;

        typedef best_param<Num>      NumParam;
        typedef best_param<Vec>      VecParam;
        typedef best_param<Ray>      RayParam;
        typedef best_param<Triangle> TriangleParam;

        union
        {
            struct { Vec a, b, c; };
            Vec vertices[3];
        };

        TTriangle3()
        : a(), b(), c()
        {}

        TTriangle3(VecParam a, VecParam b, VecParam c)
        : a(a), b(b), c(c)
        {}

        bool intersects(RayParam) const
        {
            // unimplemented
            return false;
        }

        bool intersects(RayParam, Vec&) const
        {
            // unimplemented
            return false;
        }

        bool intersects(RayParam, Vec&, Vec&) const
        {
            // unimplemented
            return false;
        }

        inline string_t str() const
        {
            return sprintList(vertices);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseList(s, vertices);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseList(is, vertices);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Triangle)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Triangle)
    };

    typedef TTriangle3<real_t> Triangle3;
}
