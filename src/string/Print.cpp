// Copyright 2013, James Mitchell, All rights reserved.

#include "Print.hpp"
using namespace util;

#include <cstdarg>

#define BUFFER_SIZE 512

void _print(ostream_t& os, String const& fmt, va_list args)
{
    char_t buffer[BUFFER_SIZE];
    os.write(buffer, s_vprintf(buffer, BUFFER_SIZE, fmt.ptr, args));
}

void _println(ostream_t& os, String const& fmt, va_list args)
{
    char_t buffer[BUFFER_SIZE];
    os.write(buffer, s_vprintf(buffer, BUFFER_SIZE, fmt.ptr, args));
    os << NEWLINE;
}

void util::printStd(String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _print(standardout, fmt, args);
    va_end(args);
}

void util::printStdln(String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _println(standardout, fmt, args);
    va_end(args);
}

void util::errorStd(String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _print(standarderr, fmt, args);
    va_end(args);
}

void util::errorStdln(String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _println(standarderr, fmt, args);
    va_end(args);
}

string_t util::sprintStd(String const fmt, ...)
{
    ostringstream_t oss;

    va_list args;
    va_start(args, fmt);
    _print(oss, fmt, args);
    va_end(args);

    return oss.str();
}

string_t util::sprintStdln(String const fmt, ...)
{
    ostringstream_t oss;

    va_list args;
    va_start(args, fmt);
    _println(oss, fmt, args);
    va_end(args);

    return oss.str();
}

void util::fprintStd(ostream_t& os, String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _print(os, fmt, args);
    va_end(args);
}

void util::fprintStdln(ostream_t& os, String const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _println(os, fmt, args);
    va_end(args);
}
