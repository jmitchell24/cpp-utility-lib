// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "VectorND.hpp"
#include "Vector2D.hpp"


namespace util
{
    template <typename N> class VecND<N, 3>
    {
    public:
        typedef N               Num;
        typedef VecND<Num,3>    Vec;
        typedef Vec&            VecRef;
        typedef Num&            NumRef;
        typedef best_param<Vec> VecParam;
        typedef best_param<Num> NumParam;
        typedef N               Components[3];

        size_t static constexpr Size = 3;

        union
        {
            struct { Num x,y,z; };
            struct { Num r,g,b; };

            VecND<N,2> xy;
            VecND<N,2> rg;

            Components components;
        };

        inline constexpr VecND()
            : x((Num)0), y((Num)0), z((Num)0)
        {}

        constexpr VecND(Vec&&)=default;
        constexpr VecND(Vec const&)=default;

        template <typename P>
        inline constexpr VecND(VecND<P,3> const& v)
            : x((Num)v.x), y((Num)v.y), z((Num)v.z)
        {}

        inline constexpr explicit VecND(NumParam n)
            : x(n), y(n), z(n)
        {}

        inline constexpr VecND(VecND<N,2> const& v, NumParam z)
            : x(v.x), y(v.y), z(z)
        {}

        inline constexpr VecND(NumParam x, NumParam y, NumParam z)
            : x(x), y(y), z(z)
        {}

        inline constexpr explicit VecND(Components const& components)
            : x(components[0]), y(components[1]), z(components[2])
        {}

        inline Num sum       () const { return x + y + z; }
        inline Num sumSquared() const { return x*x + y*y + z*z; }
        inline Num length    () const { return std::sqrt(sumSquared()); }
        inline Num dot     (VecParam v) const { return x*v.x + y*v.y + z*v.z; }
        inline Num angle   (VecParam v) const { return (*this==v) ? (Num)0 : std::acos(dot(v) / (length()*v.length())); }
        inline Num distance(VecParam v) const { return ((*this) - v).length(); }

        inline Vec round  () const { return Vec(std::round(x), std::round(y), std::round(z)); }
        inline Vec floor  () const { return Vec(std::floor(x), std::floor(y), std::floor(z)); }
        inline Vec ceil   () const { return Vec(std::ceil (x), std::ceil (y), std::ceil (z)); }

        inline Vec round(NumParam n) const { return Vec(util::round(x,n), util::round(y,n), util::round(z,n)); }
        inline Vec floor(NumParam n) const { return Vec(util::floor(x,n), util::floor(y,n), util::floor(z,n)); }
        inline Vec ceil (NumParam n) const { return Vec(util::ceil (x,n), util::ceil (y,n), util::ceil (z,n)); }

        inline Vec reverse() const { return Vec(z,y,x); }
        inline Vec normal () const { return *this / length(); }
        inline Vec cross  (VecParam v)      const { return Vec((y * v.z) - (v.y * z), (z * v.x) - (v.z * x), (x * v.y) - (v.x * y)); }
        inline Vec project(VecParam base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        inline Vec reflect(VecParam normal) const { return *this + (normal * -(normal * *this) * (Num)2); }

        inline bool isNan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
        inline bool isInf() const { return std::isinf(x) || std::isinf(y) || std::isinf(z); }

        inline Vec operator - () const { return Vec(-x,-y,-z); }

        inline Vec operator + (VecParam v) const { return Vec(x + v.x, y + v.y, z + v.z); }
        inline Vec operator - (VecParam v) const { return Vec(x - v.x, y - v.y, z - v.z); }
        inline Vec operator * (VecParam v) const { return Vec(x * v.x, y * v.y, z * v.z); }
        inline Vec operator / (VecParam v) const { return Vec(x / v.x, y / v.y, z / v.z); }

        inline VecRef operator =  (Vec&&)=default;
        inline VecRef operator =  (VecParam v) { x = v.x; y = v.y; z = v.z; return *this; }
        inline VecRef operator += (VecParam v) { return (*this = *this + v); }
        inline VecRef operator -= (VecParam v) { return (*this = *this - v); }
        inline VecRef operator *= (VecParam v) { return (*this = *this * v); }
        inline VecRef operator /= (VecParam v) { return (*this = *this / v); }

        inline Vec operator + (NumParam n) const { return Vec(x + n, y + n, z + n); }
        inline Vec operator - (NumParam n) const { return Vec(x - n, y - n, z - n); }
        inline Vec operator * (NumParam n) const { return Vec(x * n, y * n, z * n); }
        inline Vec operator / (NumParam n) const { return Vec(x / n, y / n, z / n); }

        inline VecRef operator =  (NumParam n) { x = n; y = n; z = n; return *this; }
        inline VecRef operator += (NumParam n) { return (*this = *this + n); }
        inline VecRef operator -= (NumParam n) { return (*this = *this - n); }
        inline VecRef operator *= (NumParam n) { return (*this = *this * n); }
        inline VecRef operator /= (NumParam n) { return (*this = *this / n); }

        inline bool operator== (VecParam v) const { return std::equal(begin(), end(), v.begin()); }
        inline bool operator!= (VecParam v) const { return !(*this == v); }
        inline bool operator<  (VecParam v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        inline bool operator>  (VecParam v) const { return *this < v; }
        inline bool operator<= (VecParam v) const { return !(*this < v); }
        inline bool operator>= (VecParam v) const { return !(v < *this); }

        inline NumParam operator[] (size_t const i) const { ASSERT(i < Size); return components[i]; }
        inline Num    & operator[] (size_t const i)       { ASSERT(i < Size); return components[i]; }

        inline Num*       begin()       { return std::begin(components); }
        inline Num const* begin() const { return std::begin(components); }

        inline Num*       end()       { return std::end(components); }
        inline Num const* end() const { return std::end(components); }

        inline string_t str() const { return sprintList(components); }
        inline bool tryParse(istream_t& is) { return tryParseList(is, components); }
        inline bool tryParse(Segment const& s) { return tryParseList(s, components); }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Vec)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Vec)
    };

    template <typename N> using Vec3N = VecND<N, 3>;

    typedef Vec3N<real_t>        Vec3;
    typedef Vec3N<float>         Vec3f;
    typedef Vec3N<double>        Vec3d;
    typedef Vec3N<int>           Vec3i;
    typedef Vec3N<unsigned>      Vec3u;
    typedef Vec3N<unsigned char> Vec3b;

    typedef Vec3N<real_t>        Color3;
    typedef Vec3N<float>         Color3f;
    typedef Vec3N<double>        Color3d;
    typedef Vec3N<int>           Color3i;
    typedef Vec3N<unsigned>      Color3u;
    typedef Vec3N<unsigned char> Color3b;

    extern template struct VecND<float        , 3>;
    extern template struct VecND<double       , 3>;
    extern template struct VecND<int          , 3>;
    extern template struct VecND<unsigned     , 3>;
    extern template struct VecND<unsigned char, 3>;
}
