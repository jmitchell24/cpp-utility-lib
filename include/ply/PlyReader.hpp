// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include "PlyDefinition.hpp"
#include "PlyDocument.hpp"

#include "../String.hpp"

namespace util
{
namespace ply
{
    class Reader
    {
    public:
        Reader(Segment const& segment);
        void read(Document& document) const;
        Document read() const;

        static Document readFile(SegString const& filename);
        static void readFile(SegString const& filename, Document& document);

    private:
        Segment const segment;

        Number tryGetNumber(Segment const& s, Segment::Location current, NumberType t) const;
        Number tryGetNumber(byte const*& begin, byte const* const end, Segment::Location const current, NumberType const t) const;
        ContentType readHeader(Document& document, Segment::Location& current) const;
        void readContentAscii(Document& document, Segment::Location const current) const;
        void readContentBin  (Document& document, Segment::Location const current) const;
        void readElementContentAscii(ElementBuffer& content, Segment::Location& current) const;
        byte const* readElementContentBin(ElementBuffer& content, Segment::Location const current, byte const* begin, byte const* end) const;
    };
}
}
