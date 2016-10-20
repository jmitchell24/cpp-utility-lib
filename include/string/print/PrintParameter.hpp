// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "PrintAlignment.hpp"

#include "../Segment.hpp"

namespace util
{
namespace detail
{
// Parameter represents a part of the string that will be filled in based on input arguments
    struct Parameter
    {
        string_t * value     = nullptr;
        Segment  * token     = nullptr;
        Alignment* alignment = nullptr;
        string_t * transform = nullptr;
    };
}
}
