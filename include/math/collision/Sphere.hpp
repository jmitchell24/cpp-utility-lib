// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Ray.hpp"
#include "../Vector3D.hpp"
#include "../../String.hpp"

namespace util
{
    template <typename N> struct SphereN : public IntersectsRay<N>
    {
    public:
        typedef N            Num;
        typedef Vec3N<Num>   Vec;
        typedef RayN<Num>    Ray;
        typedef SphereN<Num> Sphere;

        typedef best_param<Num>    NumParam;
        typedef best_param<Vec>    VecParam;
        typedef best_param<Ray>    RayParam;
        typedef best_param<Sphere> SphereParam;

        Vec position;
        Num   radius;

        inline constexpr SphereN()
            : position(), radius((Num)1)
        {}

        inline constexpr SphereN(VecParam position, NumParam radius)
            : position(position), radius(radius)
        {}

        inline constexpr SphereN(Sphere const& sphere)
            : position(sphere.position), radius(sphere.radius)
        {}

        template <typename P>
        inline explicit constexpr SphereN(SphereN<P> const& sphere)
            : position(sphere.position), radius(sphere.radius)
        {}

        inline constexpr bool intersects(VecParam point) const
        {
            return (point-position).length() <= radius;
        }

        inline constexpr bool intersects(SphereParam sphere) const
        {
            return (sphere.position-position).length() - sphere.radius <= radius;
        }

        inline bool intersects(RayParam ray) const override
        {
            Num a = (ray.position - position).length();
            Num b = ray.direction.normal().dot(position - ray.position);
            return std::sqrt( (a*a) - (b*b) ) < radius;
        }

        inline bool intersects(RayParam ray, Vec& location) const override
        {
            Vec t_origin(ray.position - position);

            Num a = ray.direction.dot(ray.direction);
            Num b = 2.0 * ray.direction.dot(t_origin);
            Num c = t_origin.dot(t_origin) - (radius * radius);

            Num disc = b * b - 4 * a * c;

            if (disc < 0)
                return false;

            Num distSqrt = std::sqrt(disc);

            Num q = (b < 0) ? (-b - distSqrt) / 2.0 : (-b + distSqrt) / 2.0;

            Num t0 = q / a;
            Num t1 = c / q;

            if (t0 > t1)
            {
                Num temp = t0;
                t0 = t1;
                t1 = temp;
            }

            if (t1 < 0)
                return false;

            location = (t0 < 0) ? ray.getPoint(t1) : ray.getPoint(t0);
            return true;
        }

        inline bool intersects(RayParam ray, Vec& location, Vec& normal) const override
        {
            Vec t_origin(ray.position - position);

            Num a = ray.direction.dot(ray.direction);
            Num b = 2.0 * ray.direction.dot(t_origin);
            Num c = t_origin.dot(t_origin) - (radius * radius);

            Num disc = b * b - 4 * a * c;

            if (disc < 0)
                return false;

            Num distSqrt = std::sqrt(disc);

            Num q = (b < 0) ? (-b - distSqrt) / 2.0:
                                (-b + distSqrt) / 2.0;

            Num t0 = q / a;
            Num t1 = c / q;

            if (t0 > t1)
            {
                Num temp = t0;
                t0 = t1;
                t1 = temp;
            }

            if (t1 < 0)
                return false;

            location = (t0 < 0) ? ray.getPoint(t1) : ray.getPoint(t0);
            normal   = (location - position).normal();
            return true;
        }

        inline constexpr bool contains(VecParam point) const
        {
            return (point-position).length() < radius;
        }

        inline constexpr bool contains(SphereParam sphere) const
        {
            return (sphere.position-position).length() + sphere.radius < radius;
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

        DEFINE_OBJECT_OSTREAM_OPERATOR(Sphere)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Sphere)
    };

    typedef SphereN<real_t  > Sphere;
    typedef SphereN<float   > FSphere;
    typedef SphereN<double  > DSphere;
    typedef SphereN<int     > ISphere;
    typedef SphereN<unsigned> USphere;

    extern template struct SphereN<float   >;
    extern template struct SphereN<double  >;
    extern template struct SphereN<int     >;
    extern template struct SphereN<unsigned>;
}
