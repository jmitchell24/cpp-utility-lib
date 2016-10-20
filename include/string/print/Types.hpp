// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#define ARG_INDEX            '@'
#define ARG_ANONYMOUS        '#'

#define PARAM_OPEN           '['
#define PARAM_CLOSE          ']'
#define PARAM_INDEX          '@'
#define PARAM_ANONYMOUS      '#'

#define PARAM_FORMAT         ';'
#define PARAM_ALIGNMENT      ','
#define PARAM_ALIGNMENT_FILL '.'

#include "../Types.hpp"

#define PART_LIMIT 50

namespace util
{
namespace detail
{
    struct Symbols
    {
        char_t const arg_index     = ARG_INDEX;
        char_t const arg_anonymous = ARG_ANONYMOUS;

        char_t const param_open      = PARAM_OPEN;
        char_t const param_close     = PARAM_CLOSE;
        char_t const param_index     = PARAM_INDEX;
        char_t const param_anonymous = PARAM_ANONYMOUS;

        char_t const param_format    = PARAM_FORMAT;
        char_t const param_alignment = PARAM_ALIGNMENT;
    };
}
}
