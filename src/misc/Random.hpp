// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Timer.hpp"

#include <random>
#include <type_traits>

/*
template <typename G> inline void setRand(G& g, type& t)                     { i = std::uniform_int_distribution<type>(numeric_limits<type>::min(),numeric_limits<type>::max())(g); }
template <typename G> inline void setRand(G& g, type& t, type max)           { i = std::uniform_int_distribution<type>(0,max)(g); }
template <typename G> inline void setRand(G& g, type& t, type min, type max) { i = std::uniform_int_distribution<type>(min,max)(g); }
*/

namespace util
{
#define DECLARE_SETRAND_INTEGER(type) \
template <typename G> inline void setRand(G& g, type&          i) { i = std::uniform_int_distribution<type>(std::numeric_limits<type>::min(),std::numeric_limits<type>::max())(g); } \
template <typename G> inline void setRand(G& g, unsigned type& i) { i = std::uniform_int_distribution<type>(std::numeric_limits<type>::min(),std::numeric_limits<type>::max())(g); } \
template <typename G> inline void setRand(G& g, type&          i, type          max) { i = std::uniform_int_distribution<type>(0,max)(g); } \
template <typename G> inline void setRand(G& g, unsigned type& i, unsigned type max) { i = std::uniform_int_distribution<type>(0,max)(g); } \
template <typename G> inline void setRand(G& g, type&          i, type          min, type          max) { i = std::uniform_int_distribution<type>(min,max)(g); } \
template <typename G> inline void setRand(G& g, unsigned type& i, unsigned type min, unsigned type max) { i = std::uniform_int_distribution<type>(min,max)(g); }
        DECLARE_SETRAND_INTEGER(char)
        DECLARE_SETRAND_INTEGER(short)
        DECLARE_SETRAND_INTEGER(int)
        DECLARE_SETRAND_INTEGER(long)
        DECLARE_SETRAND_INTEGER(long long)
#undef DECLARE_SETRAND_INTEGER

#define DECLARE_SETRAND_FLOAT(type) \
template <typename G> inline void setRand(G& g, type& i)                     { i = std::uniform_real_distribution<type>(std::numeric_limits<type>::min(),std::numeric_limits<type>::max())(g); } \
template <typename G> inline void setRand(G& g, type& i, type max)           { i = std::uniform_real_distribution<type>(0,max)(g); } \
template <typename G> inline void setRand(G& g, type& i, type min, type max) { i = std::uniform_real_distribution<type>(min,max)(g); }
        DECLARE_SETRAND_FLOAT(float)
        DECLARE_SETRAND_FLOAT(double)
        DECLARE_SETRAND_FLOAT(long double)
#undef DECLARE_SETRAND_FLOAT

        template <typename G> inline void setRand(G& g, bool& i) { i = g() % 2; }

    template <typename Generator> class RandomT
    {
    public:
        RandomT()
            : generator(Timer::getTimeSinceEpoch().seconds())
        {}

        RandomT(unsigned seed)
            : generator(seed)
        {}

        template <typename T> inline T get()
        {
            T t;
            setRand(generator,t);
            return t;
        }

        template <typename T> inline T get(T max)
        {
            T t;
            setRand(generator,t,max);
            return t;
        }

        template <typename T> inline T get(T min, T max)
        {
            T t;
            setRand(generator,t,min,max);
            return t;
        }

        template <typename T> inline std::vector<T> getList(unsigned const size)
        {
            std::vector<T> t(size);
            for (unsigned i = 0; i < size; ++i)
                setRand(generator,t[i]);
            return t;
        }

        template <typename T> inline std::vector<T> getList(unsigned const size, T max)
        {
            std::vector<T> t(size);
            for (unsigned i = 0; i < size; ++i)
                setRand(generator,t[i],max);
            return t;
        }

        template <typename T> inline std::vector<T> getList(const unsigned size, T min, T max)
        {
            std::vector<T> t(size);
            for (unsigned i = 0; i < size; ++i)
                setRand(generator,t[i],min,max);
            return t;
        }

    private:
        Generator generator;
    };

    typedef RandomT<std::mt19937_64> Random;
}
