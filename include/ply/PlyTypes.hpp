#pragma once

#include "../string/Segment.hpp"

#include <iostream>

namespace util
{
namespace ply
{
    typedef char                      Char;
    typedef std::basic_string  <Char> String;
    typedef std::basic_ostream <Char> Ostream;
    typedef std::basic_ofstream<Char> Ofstream;
    typedef std::basic_ifstream<Char> Ifstream;
    typedef SegmentT           <Char> Segment;
    typedef StringT            <Char> SegString;
}
}
