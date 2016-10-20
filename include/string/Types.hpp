// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#define UTIL_DEFAULT_DELIM  ','
#define UTIL_DEFAULT_NEST   '('
#define UTIL_DEFAULT_UNNEST ')'

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include <cstring>

#define DEFINE_OBJECT_OSTREAM_OPERATOR(__type__)                                         \
friend inline util::ostream_t& operator << (util::ostream_t& os, __type__ const& object) \
{                                                                                        \
    return os << object.str();                                                           \
}

#define DEFINE_OBJECT_ISTREAM_OPERATOR(__type__)                                   \
friend inline util::istream_t& operator >> (util::istream_t& is, __type__& object) \
{                                                                                  \
    if (!object.tryParse(streamString(is)))                                        \
        is.setstate(std::ios::failbit);                                            \
    return is;                                                                     \
}

#ifdef UNICODE
#define UTILITYLIB_USE_UNICODE
#endif

namespace util
{
#ifdef UTILITYLIB_USE_UNICODE
#define CONCAT(A, B) A##B
#define STR_LITERAL(A) CONCAT(L,A)
#define STR_CURRENT_FILE     STR_LITERAL(__FILE__)
#define STR_CURRENT_FUNCTION L"no wide str available"

#define s_ftime   wcsftime
#define s_vprintf _vsnwprintf
#define s_sprintf wsprintf

    typedef wchar_t char_t;
    static std::basic_istream<char_t>& standardin  = std::wcin;
    static std::basic_ostream<char_t>& standardout = std::wcout;
    static std::basic_ostream<char_t>& standarderr = std::wcerr;
#else
#define STR_LITERAL(A) A
#define STR_CURRENT_FILE     STR_LITERAL(__FILE__)
#define STR_CURRENT_FUNCTION __FUNCTION__

#define s_ftime   strftime
#define s_vprintf vsnprintf
#define s_sprintf sprintf

    typedef char char_t;
    static std::basic_istream<char_t>& standardin  = std::cin;
    static std::basic_ostream<char_t>& standardout = std::cout;
    static std::basic_ostream<char_t>& standarderr = std::cerr;
#endif // UTILITYLIB_USE_UNICODE

    typedef std::basic_string       <char_t> string_t;
    typedef std::basic_ostream      <char_t> ostream_t;
    typedef std::basic_ofstream     <char_t> ofstream_t;
    typedef std::basic_ostringstream<char_t> ostringstream_t;

    typedef std::basic_istream      <char_t> istream_t;
    typedef std::basic_ifstream     <char_t> ifstream_t;
    typedef std::basic_istringstream<char_t> istringstream_t;
    typedef std::basic_streambuf    <char_t> streambuf_t;

    typedef string_t::iterator                 s_iterator;
    typedef string_t::const_iterator          sc_iterator;
    typedef string_t::reverse_iterator        sr_iterator;
    typedef string_t::const_reverse_iterator scr_iterator;

    template <typename CReturn, typename CArg>
    inline typename std::enable_if<!std::is_same<CReturn,CArg>::value, std::basic_string<CReturn> >::type
    str_cast(std::basic_string<CArg> const& s)
    { return std::basic_string<CReturn>(s.begin(), s.end()); }

    template <typename C> std::basic_string<C> const& str_cast(std::basic_string<C> const& s)
    { return s; }

    typedef char_t const* cstring_t;

    template <typename C> class SegmentT;
    template <typename C> class StringT;

    typedef SegmentT<char_t> Segment;
    typedef StringT<char_t>   String;

    template <typename C, typename T> inline std::basic_string<C> basicToString(T const& t)
    {
        std::basic_ostringstream<C> oss;
        oss << std::boolalpha << t;
        return oss.str();
    }

    template <typename T> inline string_t toString(T const& t)
    { return basicToString<char_t,T>(t); }

    template <typename C> inline void basicStreamString(std::basic_istream<C>& in, std::basic_string<C>& out)
    {
        std::istreambuf_iterator<C> static const end;
        if (!in.fail())
            out.assign(std::istreambuf_iterator<C>(in), end);
    }

    template <typename C> inline std::basic_string<C> basicStreamString(std::basic_istream<C>& in)
    {
        std::basic_string<C> out;
        basicStreamString(in, out);
        return out;
    }

    inline void basicStreamString(istream_t& in, string_t& out)
    { basicStreamString<char_t>(in, out); }

    inline string_t streamString(istream_t& in)
    { return basicStreamString(in); }

    template <typename T> inline void transformToStream(ostream_t& os, Segment const& /*transform*/, T const& t)
    { os << t; }

    template <typename T> inline string_t transformToString(Segment const& transform, T const& t)
    {
        ostringstream_t oss;
        transformToStream(oss, transform, t);
        return oss.str();
    }

    template <typename C> inline SegmentT<C> getDirectory(SegmentT<C> const& fullpath)
    {
        typename SegmentT<C>::Location const location_fslash = fullpath.findLast('/');
        typename SegmentT<C>::Location const location_bslash = fullpath.findLast('\\');
        if (location_fslash != fullpath.end)
            return fullpath.sub(fullpath.begin, location_fslash);
        if (location_bslash != fullpath.end)
            return fullpath.sub(fullpath.begin, location_bslash);
        return fullpath;
    }

    template <typename C> inline SegmentT<C> getFilename(SegmentT<C> const& fullpath)
    {
        typename SegmentT<C>::Location const location_fslash = fullpath.findLast('/');
        typename SegmentT<C>::Location const location_bslash = fullpath.findLast('\\');
        if (location_fslash != fullpath.end)
            return fullpath.sub(location_fslash+1);
        if (location_bslash != fullpath.end)
            return fullpath.sub(location_bslash+1);
        return fullpath;
    }

    template <typename C> inline SegmentT<C> getExtension(SegmentT<C> const& fullpath)
    {
        typename SegmentT<C>::Location const location = fullpath.findLast('.');
        if (location != fullpath.end)
            return fullpath.sub(location+1);
        return SegmentT<C>();
    }
}
