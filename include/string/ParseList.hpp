#pragma once

#include "Types.hpp"
#include "Parse.hpp"

namespace util
{
    template <typename C, typename T, size_t N> inline bool basicTryParseList(SegmentT<C> const& s, T(&array)[N]);
    template <typename C, typename T, size_t N> inline bool basicTryParseList(SegmentT<C> const& s, std::array<T,N>& array);

    template <typename C, typename T, size_t N> inline bool basicTryParseList(std::basic_istream<C>& is, T(&array)[N]);
    template <typename C, typename T, size_t N> inline bool basicTryParseList(std::basic_istream<C>& is, std::array<T,N>& array);

    template <typename T, size_t N> inline bool tryParseList(Segment const& s, T(&array)[N]);
    template <typename T, size_t N> inline bool tryParseList(Segment const& s, std::array<T,N>& array);

    template <typename T, size_t N> inline bool tryParseList(istream_t& is, T(&array)[N]);
    template <typename T, size_t N> inline bool tryParseList(istream_t& is, std::array<T,N>& array);
}

template <typename C, typename T, size_t N> bool util::basicTryParseList(std::basic_istream<C>& is, std::array<T,N>& array)
{
    C token;

    if (!(is >> token) || token != UTIL_DEFAULT_NEST)
        return false;

    size_t i = 0;
    std::array<T,N> copy;

    for (; i < N-1; ++i)
        if (!util::basicTryParse(is,copy[i]) || !(is >> token) || token != UTIL_DEFAULT_DELIM)
            return false;

    if (!util::basicTryParse(is,copy[i]) || !(is >> token) || token != UTIL_DEFAULT_UNNEST)
        return false;

    array = copy;
    return true;
}

template <typename C, typename T, size_t N> inline bool util::basicTryParseList(std::basic_istream<C>& is, T(&array)[N])
{
    C token;

    if (!(is >> token) || token != UTIL_DEFAULT_NEST)
        return false;

    size_t i = 0;
    T copy[N];

    for (; i < N-1; ++i)
        if (!util::basicTryParse(is,copy[i]) || !(is >> token) || token != UTIL_DEFAULT_DELIM)
            return false;

    if (!util::basicTryParse(is,copy[i]) || !(is >> token) || token != UTIL_DEFAULT_UNNEST)
        return false;

    std::copy(copy,copy+N,array);
    return true;
}

template <typename C, typename T, size_t N> bool util::basicTryParseList(SegmentT<C> const& s, std::array<T,N>& array)
{
    typename SegmentT<C>::StreamBuffer buf(s);
    std::basic_istream<C> is(&buf);
    return basicTryParseList(is, array);
}

template <typename C, typename T, size_t N> bool util::basicTryParseList(SegmentT<C> const& s, T(&array)[N])
{
    typename SegmentT<C>::StreamBuffer buf(s);
    std::basic_istream<C> is(&buf);
    return basicTryParseList(is, array);
}

template <typename T, size_t N> bool util::tryParseList(Segment const& s, std::array<T,N>& array)
{
    return basicTryParseList(s, array);
}

template <typename T, size_t N> bool util::tryParseList(Segment const& s, T(&array)[N])
{
    return basicTryParseList(s, array);
}

template <typename T, size_t N> inline bool util::tryParseList(istream_t& is, T(&array)[N])
{
    return basicTryParseList(is, array);
}

template <typename T, size_t N> inline bool util::tryParseList(istream_t& is, std::array<T,N>& array)
{
    return basicTryParseList(is, array);
}
