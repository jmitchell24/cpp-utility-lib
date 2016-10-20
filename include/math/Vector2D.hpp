#pragma once

#include "Types.hpp"
#include "VectorND.hpp"

namespace util
{
    template <typename N> class VecND<N, 2>
    {
    public:
        typedef N               Num;
        typedef VecND<Num,2>    Vec;
        typedef Vec&            VecRef;
        typedef Num&            NumRef;
        typedef best_param<Vec> VecParam;
        typedef best_param<Num> NumParam;
        typedef N               Components[2];

        size_t static constexpr Size = 2;

        union
        {
            struct { Num x,y; };
            Components components;
        };

        inline constexpr VecND()
            : x((Num)0), y((Num)0)
        {}

        constexpr VecND(Vec&&)=default;
        constexpr VecND(Vec const&)=default;

        template <typename P>
        inline constexpr VecND(VecND<P,2> const& v)
            : x((Num)v.x), y((Num)v.y)
        {}

        inline constexpr explicit VecND(NumParam n)
            : x(n), y(n)
        {}

        inline constexpr VecND(NumParam x, NumParam y)
            : x(x), y(y)
        {}

        inline constexpr explicit VecND(Components const& components)
            : x(components[0]), y(components[1])
        {}

        inline Num sum       () const { return x + y; }
        inline Num sumSquared() const { return x*x + y*y; }
        inline Num length    () const { return std::sqrt(sumSquared()); }
        inline Num dot     (VecParam v) const { return x*v.x + y*v.y; }
        inline Num angle   (VecParam v) const { return (*this==v) ? (Num)0 : std::acos(dot(v) / (length()*v.length())); }
        inline Num distance(VecParam v) const { return ((*this) - v).length(); }

        inline Vec round() const { return Vec(std::round(x), std::round(y)); }
        inline Vec floor() const { return Vec(std::floor(x), std::floor(y)); }
        inline Vec ceil () const { return Vec(std::ceil (x), std::ceil (y)); }

        inline Vec round(NumParam n) const { return Vec(util::round(x,n), util::round(y,n)); }
        inline Vec floor(NumParam n) const { return Vec(util::floor(x,n), util::floor(y,n)); }
        inline Vec ceil (NumParam n) const { return Vec(util::ceil (x,n), util::ceil (y,n)); }

        inline Vec reverse() const { return Vec(y,x); }
        inline Vec normal () const { return *this / length(); }
        inline Vec project(VecParam base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        inline Vec reflect(VecParam normal) const { return *this + (normal * -(normal * *this) * (Num)2); }

        inline bool isNan() const { return std::isnan(x) || std::isnan(y); }
        inline bool isInf() const { return std::isinf(x) || std::isinf(y); }

        inline Vec operator - () const { return Vec(-x,-y); }

        inline Vec operator + (VecParam v) const { return Vec(x + v.x, y + v.y); }
        inline Vec operator - (VecParam v) const { return Vec(x - v.x, y - v.y); }
        inline Vec operator * (VecParam v) const { return Vec(x * v.x, y * v.y); }
        inline Vec operator / (VecParam v) const { return Vec(x / v.x, y / v.y); }

        inline VecRef operator =  (Vec&&)=default;
        inline VecRef operator =  (VecParam v) { x = v.x; y = v.y; return *this; }
        inline VecRef operator += (VecParam v) { return (*this = *this + v); }
        inline VecRef operator -= (VecParam v) { return (*this = *this - v); }
        inline VecRef operator *= (VecParam v) { return (*this = *this * v); }
        inline VecRef operator /= (VecParam v) { return (*this = *this / v); }

        inline Vec operator + (NumParam n) const { return Vec(x + n, y + n); }
        inline Vec operator - (NumParam n) const { return Vec(x - n, y - n); }
        inline Vec operator * (NumParam n) const { return Vec(x * n, y * n); }
        inline Vec operator / (NumParam n) const { return Vec(x / n, y / n); }

        inline VecRef operator =  (NumParam n) { x  = n; y  = n; return *this; }
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

    template <typename N> using Vec2N = VecND<N, 2>;

    typedef Vec2N<real_t>        Vec2;
    typedef Vec2N<float>         Vec2f;
    typedef Vec2N<double>        Vec2d;
    typedef Vec2N<int>           Vec2i;
    typedef Vec2N<unsigned>      Vec2u;
    typedef Vec2N<unsigned char> Vec2b;

    extern template struct VecND<float        , 2>;
    extern template struct VecND<double       , 2>;
    extern template struct VecND<int          , 2>;
    extern template struct VecND<unsigned     , 2>;
    extern template struct VecND<unsigned char, 2>;
}
