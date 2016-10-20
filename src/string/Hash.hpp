// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
namespace hash
{
    inline unsigned djb2(cstring_t s)
    {
        unsigned c;
        unsigned hash = 5381;

        while ( (c = *s++) )
            hash = ((hash << 5) + hash) + c;
        return hash;
    }

    inline unsigned djb2(string_t const& s)
    {
        return djb2(s.c_str());
    }
}
}
