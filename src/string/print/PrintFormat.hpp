// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "PrintPart.hpp"
#include "PrintParameter.hpp"
#include "PrintAlignment.hpp"

namespace util
{
    class Format
    {
        detail::Part<detail::Parameter> parameters;
        detail::Part<Alignment>         alignments;
        detail::Part<string_t>          formats;

        StaticBuffer<string_t,PART_LIMIT> buffer;
        StaticBuffer<Segment,PART_LIMIT>  tokens;

        unsigned anonymous;

        bool parseAnonymousAndIndex(Segment const& s, unsigned& index);
        Segment::Location parseAnonymousAndIndexEx(Segment const& s, unsigned& index);
        void parseFormat(Segment const& s);

    public:
        Format(Segment const& s);

        void print(ostream_t& os) const;

        Format& params()
        { return *this; }

        template <typename... Params> Format& params(Params&&... params)
        {
            if (!formats.index.empty()   ) formats.params   (params...);
            if (!alignments.index.empty()) alignments.params(params...);
            if (!parameters.index.empty()) parameters.params(params...);
            return *this;
        }

        friend inline ostream_t& operator<< (ostream_t& os, Format const& f)
        {
            f.print(os);
            return os;
        }

        friend inline istream_t& operator>> (istream_t& is, Format& f)
        {
            f = Format(streamString(is));
            return is;
        }
    };
}
