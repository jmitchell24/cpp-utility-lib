// Copyright 2013, James Mitchell, All rights reserved.

#include "Assertion.hpp"
#include "Logger.hpp"

void util::_assert(cstring_t expression, cstring_t file, cstring_t function, unsigned line)
{
    logr.abort(STR_LITERAL("assertion: # file: # function: # line: #"), expression, file, function, line);
}
