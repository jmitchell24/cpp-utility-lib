// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Types.hpp"
#include "../string/Types.hpp"
#include "../string/Segment.hpp"
#include "../typetraits/CallTraits.hpp"

#include "Matrix4x4.hpp"
#include "Vector4D.hpp"

namespace util
{
    template <typename N> struct QuatN
    {
        typedef N               Num;
        typedef Num&            NumRef;
        typedef best_param<Num> NumParam;

        typedef Vec3N<Num>       Vec3;
        typedef Vec3&            Vec3Ref;
        typedef best_param<Vec3> Vec3Param;

        typedef Vec4N<Num>       Vec4;
        typedef Vec4&            Vec4Ref;
        typedef best_param<Vec4> Vec4Param;

        typedef Mat44N<Num>       Mat44;
        typedef Mat44&            Mat44Ref;
        typedef best_param<Mat44> Mat44Param;

        typedef QuatN<Num>       Quat;
        typedef Quat&            QuatRef;
        typedef best_param<Quat> QuatParam;

        typedef typename Vec4::Components Components;

        union
        {
            struct
            {
                union
                {
                    struct { Num x,y,z; };
                    Vec3 complex;
                };

                union { Num w, real; };
            };

            Components components;
            Vec4 vector;
        };

        inline QuatN()
            : vector()
        {}

        inline QuatN(Quat&& q)
            : vector(std::move(q.vector))
        {}

        inline QuatN(Quat const& q)
            : vector(q.vector)
        {}

        template <typename P>
        inline explicit QuatN(QuatN<P> const& q)
            : components(q.components)
        {}

        inline explicit QuatN(Vec4Param v)
            : vector(v)
        {}

        inline QuatN(Vec3Param complex, NumParam real)
            : complex(complex), real(real)
        {}

        inline QuatN(NumParam x, NumParam y, NumParam z, NumParam w)
            : x(x), y(y), z(z), w(w)
        {}

        inline Quat conjugate() const // conjugate
        {
            return Quat(-complex, real);
        }

        inline Num length()  const
        {
            return vector.length();
        }

        inline Quat normal()  const
        {
            return Quat(vector.normal());
        }

        inline Quat inverse() const
        {
            return conjugate() / normal();
        }

        inline static Quat euler(NumParam x, NumParam y, NumParam z)
        {
            Num const
            c1 = std::cos(z / (Num)2),c2 = std::cos(y / (Num)2),c3 = std::cos(x / (Num)2),
            s1 = std::sin(z / (Num)2),s2 = std::sin(y / (Num)2),s3 = std::sin(x / (Num)2);

            return Quat(
            c1*c2*s3 - s1*s2*c3,
            c1*s2*c3 + s1*c2*s3,
            s1*c2*c3 - c1*s2*s3,
            c1*c2*c3 + s1*s2*s3);
        }

        inline static Quat euler(Vec3Param v)
        {
            return euler(v.x, v.y, v.z);
        }

        inline Mat44 matrix() const
        {
            typename Mat44::Components const components =
            {
                +w,-z,+y,+x,
                +z,+w,-x,+y,
                -y,+x,+w,+z,
                -x,-y,-z,+w
            };

            return Mat44(components);
        }

        inline Mat44 rightMatrix() const
        {
            typename Mat44::Components const components =
            {
                +w,-z, y,-x,
                +z,+w,-x,-y,
                -y,+x,+w,-z,
                +x,+y,+z,+w
            };

            return Mat44(components);
        }

        inline Mat44 rotation() const
        {
            typename Mat44::Components const components =
            {
                (Num)1-(Num)2*y*y - (Num)2*z*z,        (Num)2*x*y - (Num)2*z*w,        (Num)2*x*z + (Num)2*y*w, (Num)0,
                       (Num)2*x*y + (Num)2*z*w, (Num)1-(Num)2*x*x - (Num)2*z*z,        (Num)2*y*z - (Num)2*x*w, (Num)0,
                       (Num)2*x*z - (Num)2*y*w,        (Num)2*y*z + (Num)2*x*w, (Num)1-(Num)2*x*x - (Num)2*y*y, (Num)0,
                (Num)0                        , (Num)0                     , (Num)0                           , (Num)1
            };

            return Mat44(components);
        }

        Vec3 rotatedVector(Vec3Param v) const
        {
            return (*this * Quat(v, Num()) * conjugate()).complex;
        }

        inline Quat operator* (NumParam s) const { return Quat(vector * s); }
        inline Quat operator/ (NumParam s) const { return Quat(vector / s); }
        inline Quat operator+ (NumParam s) const { return Quat(vector + s); }
        inline Quat operator- (NumParam s) const { return Quat(vector - s); }

        inline Quat operator+ (QuatParam q) const { return Quat(vector + q.vector); }
        inline Quat operator- (QuatParam q) const { return Quat(vector - q.vector); }
        inline Quat operator/ (QuatParam q) const { return Quat(vector / q.vector); }
        inline Quat operator* (QuatParam q) const
        {
            return Quat(y*q.z - z*q.y + x*q.w + w*q.x,
                        z*q.x - x*q.z + y*q.w + w*q.y,
                        x*q.y - y*q.x + z*q.w + w*q.z,
                        w*q.w - x*q.x - y*q.y - z*q.z);
        }

        inline Quat& operator = (QuatParam q) { vector = q.vector; return *this; }
        inline Quat& operator*= (QuatParam q) { return *this = *this * q; }
        inline Quat& operator/= (QuatParam q) { return *this = *this / q; }
        inline Quat& operator+= (QuatParam q) { return *this = *this + q; }
        inline Quat& operator-= (QuatParam q) { return *this = *this - q; }

        inline Quat& operator*= (NumParam n) { return *this = *this * n; }
        inline Quat& operator/= (NumParam n) { return *this = *this / n; }
        inline Quat& operator+= (NumParam n) { return *this = *this + n; }
        inline Quat& operator-= (NumParam n) { return *this = *this - n; }

        inline bool tryParse(Segment const& s)
        {
            return vector.tryParse(s);
        }

        inline string_t str() const
        {
            return vector.str();
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Quat)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Quat)
    };

    template <typename N> using QuatT = QuatN<N>;

    typedef QuatT<real_t> Quat;
    typedef QuatT<float>  Quatf;
    typedef QuatT<double> Quatd;

    extern template struct QuatN<float>;
    extern template struct QuatN<double>;
}
