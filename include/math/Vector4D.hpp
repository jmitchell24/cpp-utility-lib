#pragma once

#include "Types.hpp"
#include "VectorND.hpp"
#include "Vector3D.hpp"

namespace util
{
    template <typename N> class VecND<N, 4>
    {
    public:
        typedef N               Num;
        typedef VecND<Num,4>    Vec;
        typedef Vec&            VecRef;
        typedef Num&            NumRef;
        typedef best_param<Vec> VecParam;
        typedef best_param<Num> NumParam;
        typedef N               Components[4];

        size_t static constexpr Size = 4;

        union
        {
            struct { Num x,y,z,w; };
            struct { Num r,g,b,a; };

            VecND<N,2> xy;
            VecND<N,2> rg;

            VecND<N,3> xyz;
            VecND<N,3> rgb;

            Components components;
        };

        inline constexpr VecND()
            : x((Num)0), y((Num)0), z((Num)0), w((Num)0)
        {}

        constexpr VecND(Vec&&)=default;
        constexpr VecND(Vec const&)=default;

        template <typename P>
        inline constexpr VecND(VecND<P,4> const& v)
            : x((Num)v.x), y((Num)v.y), z((Num)v.z), w((Num)v.w)
        {}

        inline constexpr explicit VecND(NumParam n)
            : x(n), y(n), z(n), w(n)
        {}

        inline constexpr VecND(VecND<N,3> const& v, NumParam w)
            : x(v.x), y(v.y), z(v.z), w(w)
        {}

        inline constexpr VecND(VecND<N,2> const& v, NumParam z, NumParam w)
            : x(v.x), y(v.y), z(z), w(w)
        {}

        inline constexpr VecND(NumParam x, NumParam y, NumParam z, NumParam w)
            : x(x), y(y), z(z), w(w)
        {}

        inline constexpr explicit VecND(Components const& components)
            : x(components[0]), y(components[1]), z(components[2]), w(components[3])
        {}

        inline Num sum       () const { return x + y + z + w; }
        inline Num sumSquared() const { return x*x + y*y + z*z + w*w; }
        inline Num length    () const { return std::sqrt(sumSquared()); }
        inline Num dot     (VecParam v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
        inline Num angle   (VecParam v) const { return (*this==v) ? (Num)0 : std::acos(dot(v) / (length()*v.length())); }
        inline Num distance(VecParam v) const { return ((*this) - v).length(); }

        inline Vec round  () const { return Vec(std::round(x), std::round(y), std::round(z), std::round(w)); }
        inline Vec floor  () const { return Vec(std::floor(x), std::floor(y), std::floor(z), std::floor(w)); }
        inline Vec ceil   () const { return Vec(std::ceil (x), std::ceil (y), std::ceil (z), std::ceil (w)); }

        inline Vec round(NumParam n) const { return Vec(util::round(x,n), util::round(y,n), util::round(z,n), util::round(w,n)); }
        inline Vec floor(NumParam n) const { return Vec(util::floor(x,n), util::floor(y,n), util::floor(z,n), util::floor(w,n)); }
        inline Vec ceil (NumParam n) const { return Vec(util::ceil (x,n), util::ceil (y,n), util::ceil (z,n), util::ceil (w,n)); }

        inline Vec reverse() const { return Vec(w,z,y,x); }
        inline Vec normal () const { return *this / length(); }
        inline Vec project(VecParam base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        inline Vec reflect(VecParam normal) const { return *this + (normal * -(normal * *this) * (Num)2); }

        inline bool isNan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w); }
        inline bool isInf() const { return std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w); }

        inline Vec operator - () const { return Vec(-x,-y,-z,-w); }

        inline Vec operator + (VecParam v) const { return Vec(x + v.x, y + v.y, z + v.z, w + v.w); }
        inline Vec operator - (VecParam v) const { return Vec(x - v.x, y - v.y, z - v.z, w - v.w); }
        inline Vec operator * (VecParam v) const { return Vec(x * v.x, y * v.y, z * v.z, w * v.w); }
        inline Vec operator / (VecParam v) const { return Vec(x / v.x, y / v.y, z / v.z, w / v.w); }

        inline VecRef operator =  (Vec&&)=default;
        inline VecRef operator =  (VecParam v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
        inline VecRef operator += (VecParam v) { return (*this = *this + v); }
        inline VecRef operator -= (VecParam v) { return (*this = *this - v); }
        inline VecRef operator *= (VecParam v) { return (*this = *this * v); }
        inline VecRef operator /= (VecParam v) { return (*this = *this / v); }

        inline Vec operator + (NumParam n) const { return Vec(x + n, y + n, z + n, w + n); }
        inline Vec operator - (NumParam n) const { return Vec(x - n, y - n, z - n, w - n); }
        inline Vec operator * (NumParam n) const { return Vec(x * n, y * n, z * n, w * n); }
        inline Vec operator / (NumParam n) const { return Vec(x / n, y / n, z / n, w / n); }

        inline VecRef operator =  (NumParam n) { x = n; y = n; z = n; w = n; return *this; }
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

    template <typename N> using Vec4N = VecND<N, 4>;

    typedef Vec4N<real_t>        Vec4;
    typedef Vec4N<float>         Vec4f;
    typedef Vec4N<double>        Vec4d;
    typedef Vec4N<int>           Vec4i;
    typedef Vec4N<unsigned>      Vec4u;
    typedef Vec4N<unsigned char> Vec4b;

    typedef Vec4N<real_t>        Color4;
    typedef Vec4N<float>         Color4f;
    typedef Vec4N<double>        Color4d;
    typedef Vec4N<int>           Color4i;
    typedef Vec4N<unsigned>      Color4u;
    typedef Vec4N<unsigned char> Color4b;

    extern template struct VecND<float        , 4>;
    extern template struct VecND<double       , 4>;
    extern template struct VecND<int          , 4>;
    extern template struct VecND<unsigned     , 4>;
    extern template struct VecND<unsigned char, 4>;
}
