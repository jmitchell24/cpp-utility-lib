// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
namespace tokenize
{
    template <typename C> class OpenT
    {
    public:
        typedef C                          Char;
        typedef SegmentT<Char>             Segment;
        typedef typename Segment::Location Location;

        OpenT(Char delim                   = UTIL_DEFAULT_DELIM,
              Char delim_nested_list_open  = UTIL_DEFAULT_NEST,
              Char delim_nested_list_close = UTIL_DEFAULT_UNNEST)
            : delim(delim), delim_nested_list_open(delim_nested_list_open), delim_nested_list_close(delim_nested_list_close)
        {}

        template<typename Predicate>
        inline void tokenize(Segment const& s, Predicate predicate) const
        {
            int  nest_depth  = 0;
            Location token_begin = s.begin;

            for (auto i = s.begin; i != s.end; ++i)
            {
                     if (*i == delim_nested_list_open ) ++nest_depth;
                else if (*i == delim_nested_list_close) --nest_depth;
                else if (*i == delim && (nest_depth == 0))
                {
                    predicate(s.sub(token_begin, i));
                    token_begin = i+1;
                }
            }

            if (nest_depth == 0)
                predicate(s.sub(token_begin, s.end));
        }

        template<typename Predicate, typename PredicateFull>
        inline void tokenizeFull(Segment const& s, Predicate predicate, PredicateFull predicate_full) const
        {
            int nest_depth  = 0;
            Location token_begin = s.first;

            for (auto i = s.first; i != s.last; ++i)
            {
                     if (*i == delim_nested_list_open ) ++nest_depth;
                else if (*i == delim_nested_list_close) --nest_depth;
                else if (*i == delim && (nest_depth == 0))
                {
                    Segment const token = s.sub(token_begin,i);

                    predicate(token);
                    predicate_full(s.sub(i, i+1));

                    token_begin = i+1;
                }
            }

            if (nest_depth == 0)
            {
                auto const token = s.sub(token_begin, s.last);
                predicate_full(token);
                predicate(token);
            }
        }
    private:
        Char const delim;
        Char const delim_nested_list_open;
        Char const delim_nested_list_close;
    };

    typedef OpenT<char_t> Open;
}
}
