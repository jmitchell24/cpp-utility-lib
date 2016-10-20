#pragma once

#include <cmath>
#include <numeric>

namespace util
{
// see here for details: https://randomascii.wordpress.com/2012/01/23/stupid-float-tricks-2/
    union FloatUtility
    {
        typedef std::int32_t int_type;
        typedef float        float_type;

        std::int32_t i;
        float        f;

        inline constexpr FloatUtility(float_type const f) : f(f) {}
        inline constexpr FloatUtility(int_type   const i) : i(i) {}

        inline constexpr bool     negative() const { return (i >> 31) != 0; }
        inline constexpr int_type mantissa() const { return i & ((1 << 23) - 1); }
        inline constexpr int_type exponent() const { return (i >> 23) & 0xFF; }

        inline constexpr FloatUtility next() const { return FloatUtility(i+1); }
        inline constexpr FloatUtility prev() const { return FloatUtility(i-1); }
    };

    template <typename N> inline N clamp(N min, N max, N value)
    {
        if (value >= max) return max;
        if (value <= min) return min;
        return value;
    }

    template <typename N> inline N clampMin(N min, N value)
    {
        if (value <= min) return min;
        return value;
    }

    template <typename N> inline N clampMax(N max, N value)
    {
        if (value >= max) return max;
        return value;
    }

    template <typename N> inline bool intersects(N min, N max, N value)
    {
        return value >= min && value <= max;
    }

    template <typename N> inline bool intersects(N min, N max, N min_value, N max_value)
    {
        return min_value <= max && max_value >= min;
    }

    template <typename N> inline bool contains(N min, N max, N value)
    {
        return value > min && value < max;
    }

    template <typename N> inline bool contains(N min, N max, N min_value, N max_value)
    {
        return min_value > min && max_value < max;
    }

    template <typename N, typename R = N> inline R convertRange(N old_start, N old_end, N new_start, N new_end, N value)
    {
        return new_start + ((value - old_start) * ((new_end - new_start) / (old_end - old_start)));
    }

    template<typename N> inline bool carefulEquality(N a, N b)
    {
        return a == b;
    }

    inline bool carefulEquality(float a, float b, int d = 2)
    {
         // see here for details: http://www.altdevblogaday.com/2012/02/22/comparing-floating-point-numbers-2012-edition/
        typedef int I;
        if (std::signbit(a) != std::signbit(b))
            return a == b;

        I _a=0, _b=0;
        memcpy(&_a, &a, sizeof(I));
        memcpy(&_b, &b, sizeof(I));
        return std::abs(_a - _b) <= d;
    }

    inline bool carefulEquality(double a, double b, int d = 2)
    {
        typedef long int I;
        if (std::signbit(a) != std::signbit(b))
            return a == b;

        I _a=0, _b=0;
        memcpy(&_a, &a, sizeof(I));
        memcpy(&_b, &b, sizeof(I));
        return std::abs(_a - _b) <= d;
    }

    template <typename N> inline bool carefulEqualityZero(N n)
    {
        return n == (N)0;
    }

    inline bool carefulEqualityZero(float a, float e = std::numeric_limits<float>::epsilon())
    {
         // see here for details: http://www.altdevblogaday.com/2012/02/22/comparing-floating-point-numbers-2012-edition/
        return std::abs(a) <= e;
    }

    inline bool carefulEqualityZero(double a, double e = std::numeric_limits<double>::epsilon())
    {
        return std::abs(a) <= e;
    }

    template <typename N>
    inline N           mod(N           const x, N           const y) { return x % y; }
    inline float       mod(float       const x, float       const y) { return std::fmod(x,y); }
    inline double      mod(double      const x, double      const y) { return std::fmod(x,y); }
    inline long double mod(long double const x, long double const y) { return std::fmod(x,y); }

    // floor has a bug (might cause problems)
    template <typename N> inline N floor(N const x, N const y) { return (x<(N)0) ? x-util::mod(x,y)-y : x-util::mod(x,y); }
    template <typename N> inline N round(N const x, N const y) { return util::floor<N>(x+y/(N)2,y); }

namespace detail
{
    template <typename N> inline N ceilFloor   (N const x, N const y) { return x-util::mod(x,y); }
    template <typename N> inline N ceilPositive(N const x, N const y) { N const f = ceilFloor<N>(x,y); return (x-f) > 0 ? ceilFloor<N>(x+y,y) : f; }
    template <typename N> inline N ceilNegative(N const x, N const y) { N const f = ceilFloor<N>(x,y); return (x-f) > 0 ? ceilFloor<N>(x-y,y) : f; }
}
    template <typename N> inline N ceil (N const x, N const y) { return (x>(N)0) ? detail::ceilPositive(x,y) : detail::ceilNegative(x,y); }
}
