// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Ray.hpp"
#include "../Vector2D.hpp"
#include "../../String.hpp"

namespace util
{
    template <typename N> struct TPlane
    {
    public:
        typedef Vec3N<N>            Vec;
        typedef best_param<RayN<N>> RayParam;

        typedef TPlane<N> Plane;

        typedef N                 Num;
        typedef best_param<Num>   NumParam;
        typedef best_param<Vec>   VecParam;
        typedef best_param<Plane> PlaneParam;

        Vec position;
        Vec normal;

        inline constexpr TPlane()
            : position(), normal()
        {}

        inline constexpr TPlane(VecParam position, VecParam normal)
            : position(position), normal(normal)
        {}

        inline constexpr TPlane(Plane const& plane)
            : position(plane.position), normal(plane.normal)
        {}

        template <typename P>
        inline constexpr TPlane(TPlane<P> const& plane)
            : position((Vec)plane.position), normal((Vec)plane.normal)
        {}

        inline Vec2N<N> getPoint(VecParam v, VecParam up) const
        {
            ASSERT(carefulEqualityZero(up.dot(normal)));
            Vec const right = up.cross(normal);
            return Vec2N<N>( right.dot(v), up.dot(v) );
        }

        bool intersects(RayParam ray) const
        {
            if ( carefulEqualityZero(normal.dot(ray.direction)) )                 // if ray/plane are parallel
                return carefulEqualityZero(normal.dot(ray.position - position));  // either ray is on plane or will never intersect
            return ((ray.position - position).dot(-normal) / ray.direction.dot(normal)) > (N)0;
        }

        bool intersects(RayParam ray, Vec& location) const
        {
            if ( carefulEqualityZero(normal.dot(ray.direction)) )                 // if ray/plane are parallel
                return carefulEqualityZero(normal.dot(ray.position - position));  // either ray is on plane or will never intersect
            N const s = (ray.position - position).dot(-normal) / ray.direction.dot(normal);

            if (s < (N)0)
                return false;
            location = ray.getPoint(s);
            return true;
        }

        bool intersects(RayParam ray, Vec& location, Vec& normal) const
        {
            if ( carefulEqualityZero(normal.dot(ray.direction)) )                 // if ray/plane are parallel
                return carefulEqualityZero(normal.dot(ray.position - position));  // either ray is on plane or will never intersect
            N const s = (ray.position - position).dot(-this->normal) / ray.direction.dot(this->normal);

            if (s < (N)0)
                return false;
            location = ray.getPoint(s);
            normal   = this->normal;
            return true;
        }

        inline string_t str() const
        {
            return sprintTuple(position, normal);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseTuple(s, position, normal);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseTuple(is, position, normal);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Plane)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Plane)
    };

    typedef TPlane<real_t  > Plane;
    typedef TPlane<float   > FPlane;
    typedef TPlane<double  > DPlane;
    typedef TPlane<int     > IPlane;
    typedef TPlane<unsigned> UPlane;

    extern template struct TPlane<float   >;
    extern template struct TPlane<double  >;
    extern template struct TPlane<int     >;
    extern template struct TPlane<unsigned>;
}
