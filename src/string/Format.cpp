// Copyright 2013, James Mitchell, All rights reserved.

#include "Format.hpp"
#include "Segment.hpp"

#include <map>
using namespace std;
using namespace util;

typedef map<Segment,ios_base&(*)(ios_base&)> Formatters;

static Formatters integral_formatters = []()
{
    Formatters f;
    f[STR_LITERAL("hex")] = hex;
    f[STR_LITERAL("dec")] = dec;
    f[STR_LITERAL("oct")] = oct;
    return f;
}();

static Formatters real_formatters = []()
{
    Formatters f;
    f[STR_LITERAL("fixed")]      = fixed;
    f[STR_LITERAL("fix")]        = fixed;
    f[STR_LITERAL("scientific")] = scientific;
    f[STR_LITERAL("sci")]        = scientific;

    return f;
}();

template <typename T> inline void formatStreamIntegral(ostream_t& os, Segment const& format, T t)
{
    if (!format.empty())
    {
        auto it = integral_formatters.find(format);
        if (it != integral_formatters.end())
            os << it->second << t;
    }
}

template <typename T> inline void formatStreamReal(ostream_t& os, Segment const& format, T t)
{
    if (!format.empty())
    {
        auto it = real_formatters.find(format);
        if (it != real_formatters.end())
            os << it->second << t;
    }
}

void util::formatToString(ostream_t& os, Segment const& format, unsigned char      i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, unsigned short     i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, unsigned int       i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, unsigned long      i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, unsigned long long i) { formatStreamIntegral(os,format,i); }

void util::formatToString(ostream_t& os, Segment const& format, char      i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, short     i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, int       i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, long      i) { formatStreamIntegral(os,format,i); }
void util::formatToString(ostream_t& os, Segment const& format, long long i) { formatStreamIntegral(os,format,i); }

void util::formatToString(ostream_t& os, Segment const& format, float       r) { formatStreamReal(os,format,r); }
void util::formatToString(ostream_t& os, Segment const& format, double      r) { formatStreamReal(os,format,r); }
void util::formatToString(ostream_t& os, Segment const& format, long double r) { formatStreamReal(os,format,r); }
