// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include "PlyDocument.hpp"

namespace util
{
namespace ply
{
    class Writer
    {
    public:
        typedef SegmentT<char>            Segment;
        typedef StringT<char>             String;
        typedef std::basic_ostream<char>  ostream_t;
        typedef std::basic_ofstream<char> ofstream_t;

        Writer(Document const& document);
        void write      (Ostream& os);
        void writeAscii (Ostream& os, bool const print_pretty = true, unsigned const precision = 6);
        void writeBinary(Ostream& os);
        void writeFileBinary(SegString const& filename);
        void writeFileAscii (SegString const& filename);
    private:
        Document const& document;
    };
}
}
