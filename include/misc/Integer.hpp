// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <cstdint>
#include "../string/Segment.hpp"

namespace util
{
    typedef uint8_t      byte;
    typedef unsigned int uint;

    typedef   int8_t i8;
    typedef  uint8_t u8;
    typedef  int16_t i16;
    typedef uint16_t u16;
    typedef  int32_t i32;
    typedef uint32_t u32;
    typedef  int64_t i64;
    typedef uint64_t u64;

    typedef i8  char8;
    typedef i16 char16;
    typedef i32 char32;

    bool isIntDigit(int c);
    bool isUIntChar(int c);
    bool isIntChar(int c);
    bool parseUInt(Segment const& s, unsigned& i);
    bool parseInt(Segment const& s, signed& i);
}
