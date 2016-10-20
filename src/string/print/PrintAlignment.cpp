// Copyright 2013, James Mitchell, All rights reserved.

#include "PrintAlignment.hpp"
#include "../../misc/Integer.hpp"
using namespace util;

#include <iomanip>
#include <iterator>
#include <algorithm>
using namespace std;

Alignment::Alignment()
    : justify(LEFT), size(0), fill(ALIGNMENT_DEFAULT_FILL)
{}

Alignment::Alignment(unsigned const size)
    : justify(LEFT), size(size), fill(ALIGNMENT_DEFAULT_FILL)
{}

Alignment::Alignment(unsigned const size, char_t const fill)
    : justify(LEFT), size(size), fill(fill)
{}

Alignment::Alignment(unsigned const size, Alignment::Justify const justify)
    : justify(justify), size(size), fill(ALIGNMENT_DEFAULT_FILL)
{}

Alignment::Alignment(unsigned const  size, Justify const justify, char_t const fill)
    : justify(justify), size(size), fill(fill)
{}

string_t Alignment::str() const
{
    string_t s = toString(size);
    if (fill != ALIGNMENT_DEFAULT_FILL)
        return s + fill;
    return s;
}

bool Alignment::tryParse(istream_t& is)
{
    return tryParse(streamString(is));
}

bool Alignment::tryParse(Segment const& s)
{
    if (s.empty())
        return false;

    Segment::Location front = s.begin;
    Segment::Location back  = s.end-1;
    char_t            fill  = ALIGNMENT_DEFAULT_FILL;

    if (!isUIntChar(*s.begin))
    {
        switch (*s.begin)
        {
            case ALIGNMENT_JUSTIFY_LEFT    : justify = LEFT    ; ++front; break;
            case ALIGNMENT_JUSTIFY_CENTER  : justify = CENTER  ; ++front; break;
            case ALIGNMENT_JUSTIFY_RIGHT   : justify = RIGHT   ; ++front; break;
            case ALIGNMENT_JUSTIFY_INTERNAL: justify = INTERNAL; ++front; break;
            default: return false;
        }
    }

    if (s.size() > 2 && *(back-1) == PARAM_ALIGNMENT_FILL)
        fill = *back--;
    else
        ++back;

    if (!parseUInt(Segment(front, back), size))
        return false;
    this->fill = fill;

    return true;
}

void Alignment::printCenterString(ostream_t& os, Segment const& s)
{
    unsigned const sz = s.size();

    if (sz >= size) return;

    unsigned const l = (size - sz) / 2;
    unsigned const r = size - sz - l;

    fill_n(ostream_iterator<char_t,char_t>(os), l, fill);
    fill_n(ostream_iterator<char_t,char_t>(os << s), r, fill);
}
