// Copyright 2014, James Mitchell, All rights reserved.

#include "PlyWriter.hpp"
#include "../Exception.hpp"
#include "../String.hpp"
using namespace util::ply;
using namespace std;

#define DEF_STR(str) ((str))

#define TOK_END_HEADER   ("end_header")
#define TOK_COMMENT      ("comment")
#define TOK_ELEMENT      ("element")
#define TOK_PROPERTY     ("property")
#define TOK_MAGIC        ("ply")
#define TOK_FORMAT       ("format")
#define TOK_FORMAT_ASCII ("ascii")
#define TOK_VERSION      ("1.0")
#define TOK_LIST         ("list")

Writer::Writer(Document const& document)
    : document(document)
{}

static void writeHeader(Document const& document, Ostream& os, ContentType type)
{
    os << TOK_MAGIC << NEWLINE;
    os << TOK_FORMAT << ' ';
    if (type == ASCII)
        os << TOK_FORMAT_ASCII;
    else
        os << getNativeEndiannessString();
    os << ' ' << TOK_VERSION << NEWLINE;
    os << TOK_COMMENT << " 'ply generated using James Mitchell's UtilityLib'" << NEWLINE;
    os << TOK_COMMENT << " 'for more information, email me at james.mitchell.dev@gmail.com'" << NEWLINE;

    for (HeaderDefinition::Elements::const_iterator e = document.definition.elements.begin(); e != document.definition.elements.end(); ++e)
    {
        os << TOK_ELEMENT << ' ' << util::str_cast<char>(e->name) << ' ' << e->size << NEWLINE;

        for (ElementDefinition::Properties::const_iterator p = e->properties.begin(); p != e->properties.end(); ++p)
        {
            if (p->type_index == INVALID)
                os << TOK_PROPERTY << ' ' << toStringNumberType(p->type) << ' ' << util::str_cast<char>(p->name) << NEWLINE;
            else
                os << TOK_PROPERTY << ' ' << TOK_LIST << ' ' << toStringNumberType(p->type_index) << ' ' << toStringNumberType(p->type) << ' ' << util::str_cast<char>(p->name) << NEWLINE;
        }
    }

    os << TOK_END_HEADER << NEWLINE;
}

void Writer::write(Ostream& os)
{
    writeAscii(os);
}

void Writer::writeAscii(Ostream& os, bool const print_pretty, unsigned const precision)
{
    writeHeader(document, os, ASCII);
    for (Document::Content::const_iterator eb = document.content.begin(); eb != document.content.end(); ++eb)
    {
        for (ElementBuffer::Elements::const_iterator ebd = eb->elements.begin(); ebd != eb->elements.end(); ++ebd)
        {
            for (ElementBuffer::Index n = ebd->begin; n != ebd->end; ++n)
                eb->buffer[n].outAscii(os, print_pretty, precision) << ' ';
            os << NEWLINE;
        }
    }
}

void Writer::writeBinary(Ostream& os)
{
    writeHeader(document, os, BINARY);
    for (Document::Content::const_iterator eb = document.content.begin(); eb != document.content.end(); ++eb)
        for (unsigned i = 0; i < eb->buffer.size(); ++i)
            eb->buffer[i].outBin(os);
}

void Writer::writeFileBinary(SegString const& filename)
{
    Ofstream out(filename.begin, ios::binary);
    if (!out.is_open())
        throw Exception(sprintStd(STR_LITERAL("file could not be written: %s"), filename.begin));
    writeBinary(out);
}

void Writer::writeFileAscii(SegString const& filename)
{
    Ofstream out(filename.begin, ios::binary);
    if (!out.is_open())
        throw Exception(sprintStd(STR_LITERAL("file could not be written: %s"), filename.begin));
    writeAscii(out);
}
