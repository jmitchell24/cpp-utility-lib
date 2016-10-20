// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
//ostream_t& os, Segment const&, T const& t
namespace util
{
    void formatToString(ostream_t& os, Segment const& format, unsigned char      i);
    void formatToString(ostream_t& os, Segment const& format, unsigned short     i);
    void formatToString(ostream_t& os, Segment const& format, unsigned int       i);
    void formatToString(ostream_t& os, Segment const& format, unsigned long      i);
    void formatToString(ostream_t& os, Segment const& format, unsigned long long i);

    void formatToString(ostream_t& os, Segment const& format, char      i);
    void formatToString(ostream_t& os, Segment const& format, short     i);
    void formatToString(ostream_t& os, Segment const& format, int       i);
    void formatToString(ostream_t& os, Segment const& format, long      i);
    void formatToString(ostream_t& os, Segment const& format, long long i);

    void formatToString(ostream_t& os, Segment const& format, float       r);
    void formatToString(ostream_t& os, Segment const& format, double      r);
    void formatToString(ostream_t& os, Segment const& format, long double r);
}
