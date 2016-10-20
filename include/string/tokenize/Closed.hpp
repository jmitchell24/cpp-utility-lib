// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../Segment.hpp"

namespace util
{
namespace tokenize
{
    template <typename C> class ClosedT
    {
    public:
        typedef C                          Char;
        typedef SegmentT<Char>             Segment;
        typedef typename Segment::Location Location;

        ClosedT(Char delim_open  = UTIL_DEFAULT_NEST, Char delim_close = UTIL_DEFAULT_UNNEST)
            : delim_open(delim_open), delim_close(delim_close)
        {}

        template<typename Predicate>
        void tokenize(Segment const& s, Predicate predicate) const
        {
            int depth = 0;
            Location token_begin=s.begin;
            for (auto i = s.begin; i != s.end; ++i)
            {
                if (*i == delim_open && !depth++)
                    token_begin = i+1;
                else if (*i == delim_close && !--depth)
                    predicate(s.sub(token_begin,i));
            }
        }

        template<typename Predicate, typename PredicateFull>
        inline void tokenizeFull(Segment const& s, Predicate predicate, PredicateFull predicate_full) const
        {
            int depth = 0;
            Location token_begin=s.begin;
            Location token_last =s.begin;

            for (auto i = s.begin; i != s.end; ++i)
            {
                if (*i == delim_open && !depth++)
                    token_begin = i+1;
                else if (*i == delim_close && !--depth)
                {
                    auto const token = s.sub(token_begin,i);

                    predicate_full(s.sub(token_last, token_begin-1));
                    predicate(token);

                    token_last = i+1;
                }
            }

            if (token_last < s.end)
                predicate_full(s.sub(token_last, s.end));
        }

    private:
        Char const delim_open;
        Char const delim_close;
    };

    typedef ClosedT<char_t> Closed;
}
}
