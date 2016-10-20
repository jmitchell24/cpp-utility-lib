// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "tokenize/Types.hpp"
#include "tokenize/Open.hpp"
#include "tokenize/Closed.hpp"
#include "tokenize/Tokenizer.hpp"

namespace util
{
namespace tokenize
{
    template <typename T> inline auto makeTokenizer(T const& t) -> decltype(Tokenizer<T>(t))
    {
        return Tokenizer<T>(t);
    }

    template <typename C> inline auto basicMakeTokenizerOpen(C delim                   = UTIL_DEFAULT_DELIM,
                                                             C delim_nested_list_open  = UTIL_DEFAULT_NEST,
                                                             C delim_nested_list_close = UTIL_DEFAULT_UNNEST)
        -> decltype(makeTokenizer(OpenT<C>(delim, delim_nested_list_open, delim_nested_list_close)))
    {
        return makeTokenizer(OpenT<C>(delim, delim_nested_list_open, delim_nested_list_close));
    }

    template <typename C> inline auto basicMakeTokenizerClosed(C delim_open  = UTIL_DEFAULT_NEST,
                                                               C delim_close = UTIL_DEFAULT_UNNEST)
        -> decltype(makeTokenizer(ClosedT<C>(delim_open, delim_close)))
    {
        return makeTokenizer(ClosedT<C>(delim_open, delim_close));
    }

    inline auto makeTokenizerOpen(char_t delim                   = UTIL_DEFAULT_DELIM,
                                  char_t delim_nested_list_open  = UTIL_DEFAULT_NEST,
                                  char_t delim_nested_list_close = UTIL_DEFAULT_UNNEST)
        -> decltype(makeTokenizer(Open(delim, delim_nested_list_open, delim_nested_list_close)))
    {
        return makeTokenizer(Open(delim, delim_nested_list_open, delim_nested_list_close));
    }

    inline auto makeTokenizerClosed(char_t delim_open  = UTIL_DEFAULT_NEST,
                                    char_t delim_close = UTIL_DEFAULT_UNNEST)
        -> decltype(makeTokenizer(Closed(delim_open, delim_close)))
    {
        return makeTokenizer(Closed(delim_open, delim_close));
    }
}
}
