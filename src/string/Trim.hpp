// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "Segment.hpp"

#include <locale>
#include <algorithm>

namespace util
{
    template <typename C> struct TrimT
    {
        typedef C                             Char;
        typedef std::char_traits<C>           CharTraits;
        typedef SegmentT<C>                   Segment;
        typedef Segment const&                SegmentParam;
        typedef std::basic_string<C>          String;
        typedef String const&                 StringParam;

        typedef typename Segment::Location    Location;

        static Location leftIter(SegmentParam s)
        {
            Location i = s.begin;
            while (i!=s.end && isspace(*i))
                ++i;
            return i;
        }

        static Location rightIter(Segment const& s)
        {
            Location i = s.end-1;
            Location const end = s.begin-1;
            while (i!=end && isspace(*i))
                --i;
            return (i==end) ? s.end : i+1;
        }

        inline static Segment leftTrim(SegmentParam s) { return Segment(leftIter(s), s.last); }
        inline static String  leftTrim(StringParam s ) { return leftTrim(Segment(s)).str(); }
        inline static bool    leftCan (SegmentParam s) { if (s.empty()) return false; return std::isspace(*s.first); }
        inline static bool    leftCan (StringParam s ) { if (s.empty()) return false; return std::isspace(*s.begin()); }

        inline static Segment rightTrim(SegmentParam s) { return Segment(s.first, rightIter(s)); }
        inline static String  rightTrim(StringParam s ) { return rightTrim(Segment(s)).str(); }
        inline static bool    rightCan (SegmentParam s) { if (s.empty()) return false; return std::isspace(*(s.last-1)); }
        inline static bool    rightCan (StringParam s ) { if (s.empty()) return false; return std::isspace(*(s.end()-1)); }

        inline static Segment trim(SegmentParam s) { return Segment(leftIter(s), rightIter(s)); }
        inline static String  trim(StringParam s) { return trim(Segment(s)).str(); }

        inline static bool can(StringParam s) { return leftCan(s) || rightCan(s); }
        inline static bool can(SegmentParam s) { return leftCan(s) || rightCan(s); }
    };

    typedef TrimT<char_t> Trim;
}
