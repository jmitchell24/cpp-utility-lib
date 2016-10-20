// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
namespace tokenize
{
    template <typename T> class Tokenizer
    {
    public:
        typedef typename T::Char     Char;
        typedef SegmentT<Char>       Segment;
        typedef std::vector<Segment> Tokens;

        inline Tokenizer(T const& tokenizer)
            : tokenizer(tokenizer)
        {}

        inline Tokens tokenize(Segment const& s) const
        {
            Tokens tokens;
            tokenize(s, tokens);
            return tokens;
        }

        inline void tokenize(Segment const& s, Tokens& tokens) const
        { AddToken at(tokens); tokenizer.tokenize(s, at); }

        inline void tokenize(Segment const& s, Tokens& tokens, Tokens& tokens_full) const
        { AddToken at(tokens), atfull(tokens_full); tokenizer.tokenizeFull(s, at, atfull); }

    private:
        struct AddToken
        {
            Tokens& tokens;
            AddToken(Tokens& tokens)
                : tokens(tokens)
            {}

            void operator() (Segment const& s)
            {
                tokens.emplace_back(TrimT<Char>::trim(s));
                if (tokens.back().empty())
                    tokens.pop_back();
            }
        };

        T tokenizer;
    };
}
}
