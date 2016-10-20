// Copyright 2013, James Mitchell, All rights reserved.

#include "Integer.hpp"
#include "../string/Segment.hpp"
using namespace util;

bool util::isIntDigit(int c)
{
    return c >= '0' && c <= '9';
}

bool util::isIntChar(int c)
{
    return (c >= '0' && c <= '9') || c == '+' || c == '-';
}

bool util::isUIntChar(int c)
{
    return (c >= '0' && c <= '9') || c == '+';
}

bool util::parseUInt(Segment const& s, unsigned& i)
{
    char_t const* str = (*s.begin == '+') ? s.begin+1 : s.begin;

    unsigned value = 0;
    while (str != s.end)
    {
        char_t const c = *str++;
        if (!isIntDigit(c))
            return false; // non-digit, or overflow
        value *= 10;
        value += c - '0';
    }
    i = value;
    return true;
}

bool util::parseInt(Segment const& s, signed& i)
{
    int sign = 1;
    char_t const* str = s.begin;

    switch (*str)
    { // handle leading +/-
        case '-': ++str; sign = -1; break;
        case '+': ++str; break;
    }

    unsigned value = 0;
    if (parseUInt(s.sub(str, s.end), value))
    {
        i = value * sign;
        return true;
    }
    return false;
}
