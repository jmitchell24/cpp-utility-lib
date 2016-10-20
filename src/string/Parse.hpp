// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "Nest.hpp"
#include "Tokenize.hpp"

namespace util
{
    template <typename C, typename T> inline T       basicParse(SegmentT<C> const& s);
    template <typename C, typename T> inline bool basicTryParse(SegmentT<C> const& s, T& t);

    template <typename C, typename T> inline T       basicParse(std::basic_istream<C>& is);
    template <typename C, typename T> inline bool basicTryParse(std::basic_istream<C>& is, T& t);

    template <typename T> inline T       parse(Segment const& s);
    template <typename T> inline bool tryParse(Segment const& s, T& t);

    template <typename T> inline T       parse(istream_t& is);
    template <typename T> inline bool tryParse(istream_t& is, T& t);

    template <typename C> inline string_t basicParse(SegmentT<C> const& s);
    template <typename C> inline bool  basicTryParse(SegmentT<C> const& s, std::basic_string<C>& t);
}

template <typename C, typename T> inline T util::basicParse(SegmentT<C> const& s)
{
    typename SegmentT<C>::StreamBuffer buf(s);
    std::basic_istream<C> is(&buf);
    return basicParse<C,T>(is);
}

template <typename C, typename T> inline bool util::basicTryParse(SegmentT<C> const& s, T& t)
{
    typename SegmentT<C>::StreamBuffer buf(s);
    std::basic_istream<C> is(&buf);
    return basicTryParse<C,T>(is, t);
}

template <typename C, typename T> inline T util::basicParse(std::basic_istream<C>& is)
{ //static_assert(has_extraction_operator<T>::value, "T has no overload for istream extraction operator");
    T t;
    is >> std::boolalpha >> t >> std::noboolalpha;
    return t;
}

template <typename C, typename T> inline bool util::basicTryParse(std::basic_istream<C>& is, T& t)
{ //static_assert(has_extraction_operator<T>::value, "T has no overload for istream extraction operator");
    return !(is >> std::boolalpha >> t >> std::noboolalpha).fail();
}

template <typename C> inline util::string_t util::basicParse(SegmentT<C> const& s)
{
    return s.str();
}

template <typename C> inline bool util::basicTryParse(SegmentT<C> const& s, std::basic_string<C>& t)
{
    if (s.empty())
        return false;
    t = s.str();
    return true;
}



template <typename T> T util::parse(Segment const& s)
{ return util::basicParse<char_t,T>(s); }

template <typename T> bool util::tryParse(Segment const& s, T& t)
{ return util::basicTryParse<char_t,T>(s,t); }

template <typename T> T util::parse(istream_t& is)
{ return util::basicParse<char_t,T>(is); }

template <typename T> bool util::tryParse(istream_t& is, T& t)
{ return util::basicTryParse<char_t,T>(is,t); }

/*
template <typename T, unsigned N> void util::parseList(Segment const& s, T(&array)[N])
{ return util::basicParseList<char_t,T,N>(s,array); }

template <typename T, unsigned N> bool util::tryParseList(Segment const& s, T(&array)[N])
{ return util::basicTryParseList<char_t,T,N>(s,array); }

template <typename T> T* util::parseList(Segment const& s, size_t const size, T* array)
{ return util::basicParseList<char_t,T>(s,size,array); }

template <typename T> bool util::tryParseList(Segment const& s, size_t const size, T* array)
{ return util::basicParseList<char_t,T>(s,size,array); }
*/

