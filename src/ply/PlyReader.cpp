// Copyright 2014, James Mitchell, All rights reserved.

#include "PlyReader.hpp"
#include "../Assertion.hpp"
#include "../Exception.hpp"
using namespace util::ply;
using namespace std;

#define TOK_END_HEADER      ("end_header")
#define TOK_COMMENT         ("comment")
#define TOK_ELEMENT         ("element")
#define TOK_PROPERTY        ("property")
#define TOK_MAGIC           ("ply")
#define TOK_FORMAT          ("format")
#define TOK_FORMAT_ASCII    ("ascii")
#define TOK_VERSION         ("1.0")
#define TOK_LIST            ("list")

#define ERR_EMPTY_CONTENT  STR_LITERAL("no content")

#define ERR_NUMBER         STR_LITERAL("number is invalid")
#define ERR_DOCUMENT       STR_LITERAL("document is invalid")
#define ERR_FORMAT         STR_LITERAL("format is not defined")
#define ERR_FORMAT_TYPE    STR_LITERAL("format is unsupported")
#define ERR_FORMAT_VERSION STR_LITERAL("format version is unsupported")

#define ERR_HEADER                 STR_LITERAL("header is invalid")
#define ERR_PROPERTY_DEFINITION    STR_LITERAL("property definition is invalid")
#define ERR_ELEMENT_DEFINITION     STR_LITERAL("element definition is invalid")
#define ERR_NO_ELEMENT_DEFINITIONS STR_LITERAL("no element definitions in header")

#define ERR_LIST            STR_LITERAL("list is invalid")
#define ERR_ELEMENT         STR_LITERAL("element is invalid")
#define ERR_ELEMENT_MISSING STR_LITERAL("element(s) are missing from document")
#define ERR_MAX_BUFFER      STR_LITERAL("capacity of number buffer has been exceeded")
#define ERR_INVALID_BLOB    STR_LITERAL("blob is invalid")

#define DOC_NAME STR_LITERAL("ply")
#define EXCEPTION(err) DocumentException(err, DOC_NAME, DocumentException::getLineCount(segment, current))

unsigned getElementBufferSizeEstimate(ElementDefinition const& ed)
{
    unsigned static const ASSUMED_LIST_SIZE = 3;
    unsigned element_part_size = 0;
    for (unsigned i = 0; i < ed.properties.size(); ++i, ++element_part_size)
        if (ed.properties[i].isList())
            element_part_size += ASSUMED_LIST_SIZE;
    return element_part_size * ed.size;
}

Number Reader::tryGetNumber(Segment const& s, Segment::Location current, NumberType t) const
{
	switch(t)
    {
        // parse as word and then cast so that istream doesn't treat the 1byte integer as a character symbol
        case CHAR   : { signed   x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number((typename ply::get_number_type<CHAR >::type)x); }
        case UCHAR  : { unsigned x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number((typename ply::get_number_type<UCHAR>::type)x); }
        case SHORT  : { typename ply::get_number_type<SHORT >::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
        case USHORT : { typename ply::get_number_type<USHORT>::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
        case INT    : { typename ply::get_number_type<INT   >::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
        case UINT   : { typename ply::get_number_type<UINT  >::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
        case FLOAT  : { typename ply::get_number_type<FLOAT >::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
        case DOUBLE : { typename ply::get_number_type<DOUBLE>::type x; if (!basicTryParse(s, x)) throw EXCEPTION(ERR_NUMBER); return Number(x); }
		case INVALID:
        default:break;
	}
    throw EXCEPTION(ERR_NUMBER);
}

Number Reader::tryGetNumber(byte const*& begin, byte const* end, Segment::Location current, NumberType t) const
{
    unsigned const size = getTypeSize(t);
    if (unsigned(end - begin) < size)
        throw EXCEPTION(ERR_NUMBER);

    byte const* const temp = begin;
    begin += size;
    return Number(t, temp, size);
}

inline static String toLower(Segment const& s)
{
    String str;
    str.resize(s.size());
    for (unsigned i = 0; i < str.size(); ++i)
        str[i] = std::tolower(s[i]);
    return str;
}

inline static bool seekPredicate(int c)
{ 
	return std::iscntrl(c) || std::isspace(c); 
}

inline static Segment::Location skipSpace(Segment::Location& current, Segment::Location end)
{
    while(seekPredicate(*current) && current != end)
        ++current;
    return current;
}

inline static Segment::Location skipNotSpace(Segment::Location& current, Segment::Location end)
{
    while(!seekPredicate(*current) && current != end)
        ++current;
    return current;
}

inline static Segment nextToken(Segment::Location& current, Segment::Location end)
{
    Segment::Location const a = skipSpace(current,end);
    return Segment(a, skipNotSpace(current,end));
}

inline static Segment nextLineToken(Segment::Location& current, Segment::Location end)
{
    while(*current != NEWLINE && current != end)
        ++current;
    return nextToken(current, end);
}

inline static Segment nextLine(Segment::Location& current, Segment::Location end)
{
    Segment::Location const l = current;
    while (*current != NEWLINE && current != end)
        ++current;
    return Segment(l,current++);
}

util::ply::Reader::Reader(Segment const& segment)
    : segment(segment)
{}

ContentType Reader::readHeader(Document& document, Segment::Location& current) const
{
    ElementDefinition* current_element = NULL;
    ContentType content_type;

    if (!nextToken(current, segment.end).same(TOK_MAGIC))
        throw EXCEPTION(ERR_DOCUMENT);

    if (!nextLineToken(current, segment.end).same(TOK_FORMAT))
        throw EXCEPTION(ERR_FORMAT);

    Segment const format = nextToken(current, segment.end);
    if (toLower(format) == TOK_FORMAT_ASCII)
        content_type = ASCII;
    else if (toLower(format) == ::getNativeEndiannessString())
        content_type = BINARY;
    else
       throw EXCEPTION(ERR_FORMAT_TYPE);

    if (!nextToken(current, segment.end).same(TOK_VERSION))
        throw EXCEPTION(ERR_FORMAT_VERSION);
    skipSpace(current, segment.end);

    do
    {
        Segment const line = nextLine(current, segment.end);

        if (line.empty())
            continue;

        Segment::Location line_current = line.begin;
        Segment const first_token = nextToken(line_current, line.end);

        if (first_token.same(TOK_END_HEADER))
        {
            if (document.definition.elements.empty())
                throw EXCEPTION(ERR_NO_ELEMENT_DEFINITIONS);
            break;
        }
        else if (first_token.same(TOK_COMMENT))
            continue;
        else if (first_token.same(TOK_ELEMENT))
        {
            if (current_element != NULL && current_element->properties.empty())
                throw EXCEPTION(ERR_ELEMENT_DEFINITION);

            ElementDefinition element;
            element.name = str_cast<util::char_t>(nextToken(line_current, line.end).str());
            if (element.name.empty())
                throw EXCEPTION(ERR_ELEMENT_DEFINITION);

            if (!basicTryParse(nextToken(line_current, line.end), element.size))
                throw EXCEPTION(ERR_ELEMENT_DEFINITION);

            document.definition.elements.push_back(element);
            current_element = &document.definition.elements.back();
        }
        else if (first_token.same(TOK_PROPERTY))
        {
            if (current_element == NULL)
                throw EXCEPTION(ERR_PROPERTY_DEFINITION);

            PropertyDefinition property;
            property.type_index = INVALID;

            Segment second_token = nextToken(line_current, line.end);
            if (second_token.same(TOK_LIST))
            {
                if (!tryParseNumberType(nextToken(line_current, line.end), property.type_index))
                    throw EXCEPTION(ERR_PROPERTY_DEFINITION);
                if (!tryParseNumberType(nextToken(line_current, line.end), property.type))
                    throw EXCEPTION(ERR_PROPERTY_DEFINITION);
            }
            else
                if (!tryParseNumberType(second_token, property.type))
                    throw EXCEPTION(ERR_PROPERTY_DEFINITION);

            property.name = str_cast<char_t>(nextToken(line_current, line.end).str());
            if (property.name.empty())
                throw EXCEPTION(ERR_PROPERTY_DEFINITION);

            current_element->properties.push_back(property);
        }
        else
            throw EXCEPTION(ERR_HEADER);
    }
    while(current != segment.end);
    return content_type;
}

util::byte const* Reader::readElementContentBin(ElementBuffer& content, Segment::Location const current, byte const* begin, byte const* const end) const
{
    unsigned const element_size  = content.definition->size;
    unsigned       element_count = 0;

    content.buffer.reserve(getElementBufferSizeEstimate(*content.definition));

    for (; element_count < element_size && begin < end; ++element_count)
    {
        ElementDefinition::Properties::const_iterator       property     = content.definition->properties.cbegin();
        ElementDefinition::Properties::const_iterator const property_end = content.definition->properties.cend();
        ElementBuffer::Index const element_begin = content.buffer.size();

        do
        {
            if (property->isList())
            {
                content.buffer.push_back(tryGetNumber(begin, end, current, property->type_index));
                unsigned const list_size  = content.buffer.back().castGet<unsigned>();
                unsigned       list_count = 0;

                do
                    content.buffer.push_back(tryGetNumber(begin, end, current, property->type));
                while (++list_count < list_size && begin < end);

                if (list_count != list_size)
                    throw EXCEPTION(ERR_LIST);
            }
            else
                content.buffer.push_back(tryGetNumber(begin, end, current, property->type));
        }
        while(++property != property_end && begin < end);

        if (property != property_end)
            throw EXCEPTION(ERR_ELEMENT);
        if (content.buffer.size() > ElementBuffer::ELEMENT_MAX)
            throw EXCEPTION(ERR_MAX_BUFFER);
        content.elements.push_back(ElementBuffer::Element(element_begin, content.buffer.size()));
    }

    unsigned temp = content.elements.size();
    if (content.definition->size != temp)
        throw EXCEPTION(ERR_ELEMENT_MISSING);
    return begin;
}

void Reader::readElementContentAscii(ElementBuffer& content, Segment::Location& current) const
{
    unsigned const element_size = content.definition->size;
    unsigned       element_count = 0;

    content.buffer.reserve(::getElementBufferSizeEstimate(*content.definition));
    do
    {
        Segment const line = nextLine(current, segment.end);

        if (line.empty())
            continue;
        ++element_count;

        Segment::Location line_current = line.begin;

        ElementDefinition::Properties::const_iterator       property     = content.definition->properties.cbegin();
        ElementDefinition::Properties::const_iterator const property_end = content.definition->properties.cend();
		ElementBuffer::Index const begin = content.buffer.size();

        do
        {
            Segment token = nextToken(line_current, line.end);

            if (property->isList())
            {
                content.buffer.push_back(tryGetNumber(token, current, property->type_index));
                unsigned const list_size  = content.buffer.back().castGet<unsigned>();
                unsigned       list_count = 0;

                do
                    content.buffer.push_back(tryGetNumber(nextToken(line_current, line.end), current, property->type));
                while (++list_count < list_size && line_current != line.end);

                if (list_count != list_size)
                    throw EXCEPTION(ERR_LIST);
            }
            else
                content.buffer.push_back(tryGetNumber(token, current, property->type));
        }
        while(++property < property_end && line_current != line.end);

        if (property != property_end || skipSpace(line_current,line.end) != line.end)
            throw EXCEPTION(ERR_ELEMENT);
		
		if (content.buffer.size() > ElementBuffer::ELEMENT_MAX)
			throw EXCEPTION(ERR_MAX_BUFFER);
		content.elements.push_back(ElementBuffer::Element(begin, content.buffer.size()));
    }
    while (element_count < element_size && current != segment.end);

    unsigned temp = content.elements.size();
    if (content.definition->size != temp)
        throw EXCEPTION(ERR_ELEMENT_MISSING);
}

void Reader::readContentAscii(Document& document, Segment::Location current) const
{
    for (unsigned i = 0; i < document.definition.elements.size(); ++i)
    {
        ElementBuffer element_content(&document.definition.elements[i]);
        document.content.push_back(element_content);
        readElementContentAscii(document.content.back(), current);
    }
}

void Reader::readContentBin(Document& document, Segment::Location current) const
{
    byte const*     begin = (byte const*)current;
    byte const* const end = (byte const*)segment.end;

    for (unsigned i = 0; i < document.definition.elements.size(); ++i)
    {
        ElementBuffer element_content(&document.definition.elements[i]);
        document.content.push_back(element_content);
        begin = readElementContentBin(document.content.back(), current, begin, end);
    }
}

Document Reader::read() const
{
    Document document;
    read(document);
    return document;
}

void Reader::read(Document& document) const
{
    Segment::Location current = segment.begin;
    if (segment.empty())
        throw DocumentException(STR_LITERAL("segment is empty"), STR_LITERAL("ply"), 0);

    if (readHeader(document, current) == ASCII)
        readContentAscii(document, current);
    else
        readContentBin(document, current);
}

void Reader::readFile(SegString const& filename, Document& document)
{
    Ifstream in(filename.begin, ios::binary);
    if (!in.is_open())
        throw Exception(sprintStd(STR_LITERAL("file could not be read: %s"), filename.begin));

    String content;
    basicStreamString(in, content);
    Reader(content).read(document);
}

Document Reader::readFile(SegString const& filename)
{
    Document document;
    readFile(filename, document);
    return document;
}
