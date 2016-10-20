// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../Types.hpp"
#include "../Parse.hpp"

#define ALIGNMENT_DEFAULT_FILL     ' '
#define ALIGNMENT_JUSTIFY_LEFT     'l'
#define ALIGNMENT_JUSTIFY_CENTER   'c'
#define ALIGNMENT_JUSTIFY_RIGHT    'r'
#define ALIGNMENT_JUSTIFY_INTERNAL 'i'

#include <iomanip>
#include <iterator>

namespace util
{
    struct Alignment
    {
        //char_t constexpr static DEFAULT_FILL = ' ';
        enum Justify { LEFT, CENTER, RIGHT, INTERNAL } justify;

        unsigned size;
        char_t   fill;

        Alignment();
        Alignment(unsigned const size);
        Alignment(unsigned const size, char_t const fill);
        Alignment(unsigned const size, Justify const justify);
        Alignment(unsigned const size, Justify const justify, char_t const fill);

        template <typename T> void apply(ostream_t& os, T const& t)
        {
            if (size > 0)
            {
                switch (justify)
                {
                    case LEFT    : os << std::setfill(fill) << std::setw(size) << std::left     << t; break;
                    case RIGHT   : os << std::setfill(fill) << std::setw(size) << std::right    << t; break;
                    case INTERNAL: os << std::setfill(fill) << std::setw(size) << std::internal << t; break;
                    case CENTER  : printCenterString(os, toString(t)); break;
                    default:break;
                }
            }
        }

        template <typename T> void applyAndTransform(ostream_t& os, Segment const& transform, T const& t)
        {
            if (size > 0)
            {
                switch (justify)
                {
                    case LEFT    : os << std::setfill(fill) << std::setw(size) << std::left;     transformToStream(os, transform, t); break;
                    case RIGHT   : os << std::setfill(fill) << std::setw(size) << std::right;    transformToStream(os, transform, t); break;
                    case INTERNAL: os << std::setfill(fill) << std::setw(size) << std::internal; transformToStream(os, transform, t); break;
                    case CENTER  : printCenterString(os, transformToString(transform, t)); break;
                    default:break;
                }
            }
        }

        string_t str() const;
        bool tryParse(istream_t& is);
        bool tryParse(Segment const& s);

        DEFINE_OBJECT_OSTREAM_OPERATOR(Alignment)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Alignment)

        private:
            void printCenterString(ostream_t& os, Segment const& s);
    };
}
