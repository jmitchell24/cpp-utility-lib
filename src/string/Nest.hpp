// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "Trim.hpp"

#define QUOTATION_MARK '\"'

namespace util
{
    template <typename C> struct NestT
    {
        typedef C                             Char;
        typedef std::char_traits<C>           CharTraits;
        typedef SegmentT<C>                   Segment;
        typedef Segment const&                SegmentParam;
        typedef std::basic_string<C>          String;
        typedef String const&                 StringParam;
        typedef TrimT<C>                      Trim;

        typedef typename Segment::Location    Location;

        inline static bool is(SegmentParam s, Char const nest_open = QUOTATION_MARK, Char const nest_close = QUOTATION_MARK)
        { 
			if (s.empty()) 
				return false; 
			return *Trim::leftIter(s) == nest_open && *(Trim::rightIter(s)-1) == nest_close; 
		}

        inline static String nest(SegmentParam s, Char const nest_open = QUOTATION_MARK, Char const nest_close = QUOTATION_MARK)
        {
            String copy = s.str();
            copy.insert(copy.begin(), nest_open);
            copy.push_back(nest_close);
            return copy;
        }

        inline static Segment unnest(SegmentParam s, Char const nest_open = QUOTATION_MARK, Char const nest_close = QUOTATION_MARK)
        {
            Location const ltrim = Trim::leftIter(s);
            Location const rtrim = Trim::rightIter(s)-1;
            if (ltrim != s.end && rtrim != s.end && *ltrim == nest_open && *rtrim == nest_close)
                return Segment(ltrim+1, rtrim);
            return s;
        }

        inline static String unnest(StringParam s, Char const nest_open = QUOTATION_MARK, Char const nest_close = QUOTATION_MARK)
        { return unnest(Segment(s), nest_open, nest_close).str(); }
    };

    typedef NestT<char_t> Nest;
}
