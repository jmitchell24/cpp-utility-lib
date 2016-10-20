// Copyright 2015, James Mitchell, All rights reserved.

#include "ColorRGBA.hpp"
using namespace util;
using namespace std;

ColorRGBA::ColorRGBA(Color3BParam c)
    : ColorRGBA(c.r, c.g, c.b)
{}

ColorRGBA::ColorRGBA(Color4BParam c)
    : ColorRGBA(c.r, c.g, c.b, c.a)
{}

ColorRGBA::ColorRGBA(Color3RParam c)
    : ColorRGBA(c.r, c.g, c.b)
{}

ColorRGBA::ColorRGBA(Color4RParam c)
    : ColorRGBA(c.r, c.g, c.b, c.a)
{}

ColorRGBA::operator Color3B() { return Color3B(r,g,b); }
ColorRGBA::operator Color4B() { return Color4B(r,g,b,a); }

ColorRGBA::operator Color3R() { return Color3R((Real)r/BYTE_MAX, (Real)g/BYTE_MAX, (Real)b/BYTE_MAX); }
ColorRGBA::operator Color4R() { return Color4R((Real)r/BYTE_MAX, (Real)g/BYTE_MAX, (Real)b/BYTE_MAX, (Real)a/BYTE_MAX); }

string_t ColorRGBA::str() const
{
    return sprintStd("%.2X%.2X%.2X%.2X",r,g,b,a);
}

static int getHexDigit(char_t const c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

static bool setHexDigit(char_t const d1, ColorRGBA::Byte& b)
{
    int const c = getHexDigit(d1);
    if (c == -1)
        return false;
    b = c * 16;
    return true;
}

static bool setHexDigit(char_t const d1, char_t const d0, ColorRGBA::Byte& b)
{
    int const c0 = getHexDigit(d0);
    int const c1 = getHexDigit(d1);

    if (c0 == -1 || c1 == -1)
        return false;
    b = c0 + c1 * 16;
    return true;
}

bool ColorRGBA::tryParse(istream_t& is)
{
    string_t s;
    is >> s;
    return tryParse(s); // reimplement this!!!
}

bool ColorRGBA::tryParse(Segment const& s)
{
    ColorRGBA color;
    if (!s.empty())
    {
        Segment::Location l = s.begin;
        Segment::Location const e = s.end;
        for (unsigned i = 0; i < 4; ++i, l += 2)
        {
            Segment::Location const nextOne = l+1;
            Segment::Location const nextTwo = l+2;
            Segment::Char     const value   = *l;

            if (nextOne == e)
                return setHexDigit(value, color.components[i]);
            if (!setHexDigit(value, *nextOne, color.components[i]))
                break;
            if (nextTwo == e)
            {
                *this = color;
                return true;
            }
        }
    }
    return false;
}
