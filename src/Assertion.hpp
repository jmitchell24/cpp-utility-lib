// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "string/Types.hpp"

#ifdef NDEBUG
#define ASSERT(exp)
#else
#define ASSERT(exp) ( exp ) ? (void)0 : util::_assert(STR_LITERAL(#exp), STR_CURRENT_FILE, STR_CURRENT_FUNCTION, __LINE__)
#endif


namespace util
{
    void _assert(cstring_t expression, cstring_t file, cstring_t function, unsigned line);
}
