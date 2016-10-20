// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Vector3D.hpp"
#include "../../String.hpp"

namespace util
{
    template <typename N> struct RayN
    {
    public:
        typedef N          Num;
        typedef Vec3N<Num> Vec;
        typedef RayN<Num>  Ray;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Ray> RayParam;

        Vec position;
        Vec direction;

        inline constexpr RayN()
            : position(), direction()
        {}

        template <typename P>
        inline explicit constexpr RayN(RayN<P> const& ray)
            : position((Vec)ray.position), direction((Vec)ray.direction)
        {}

        inline constexpr RayN(VecParam position, VecParam direction)
            : position(position), direction(direction)
        {}

        inline constexpr Vec getPoint(NumParam distance) const
        {
            return position + (direction*distance);
        }

        inline constexpr Vec getClosestPoint(VecParam point) const
        {
            return position + direction * (direction.dot(point - position) / direction.dot(direction));
        }

        inline string_t str() const
        {
            return sprintTuple(position, direction);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseTuple(s, position, direction);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseTuple(is, position, direction);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Ray)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Ray)
    };

    typedef RayN<real_t  > Ray;
    typedef RayN<float   > FRay;
    typedef RayN<double  > DRay;
    typedef RayN<int     > IRay;
    typedef RayN<unsigned> URay;

    extern template struct RayN<float   >;
    extern template struct RayN<double  >;
    extern template struct RayN<int     >;
    extern template struct RayN<unsigned>;

    template <typename N> struct IntersectsRay
    {
    public:
        typedef Vec3N<N>            Vec;
        typedef best_param<RayN<N>> RayParam;

        virtual bool intersects(RayParam ray)                             const=0;
        virtual bool intersects(RayParam ray, Vec& location)              const=0;
        virtual bool intersects(RayParam ray, Vec& location, Vec& normal) const=0;
    };
}
