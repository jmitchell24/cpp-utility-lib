// Copyright 2013, James Mitchell, All rights reserved.

#include "PrintFormat.hpp"
#include "../Trim.hpp"
#include "../Parse.hpp"
#include "../Tokenize.hpp"
#include "../../misc/Integer.hpp"
using namespace util;
using namespace util::detail;

#include <tuple>
using namespace std;

static tuple<Segment,Segment,Segment> splitParameter(Segment const& s)
{
    Segment::Location bi, ba, bf, ei, ea, ef;
    bi = ba = bf = ei = ea = ef = s.end;

    for (auto i = s.begin; i != s.end;)
    {
        char_t c = *++i;
        while (i!=s.end && isspace(c))
            c = *++i;
        if (c == PARAM_ALIGNMENT)
            ba = i;
        else if (c == PARAM_FORMAT)
            bf = i;
    }

    bi = s.begin;
    ei = min(ba, bf);

    if (ba != ea)
    {
        ba = ba+1;
        ea = (ba<bf) ? min(bf,s.end) : s.end;
    }

    if (bf != ef)
    {
        bf = bf+1;
        ef = (bf<ba) ? min(ba,s.end) : s.end;
    }

    return tuple<Segment,Segment,Segment>
    (Trim::trim(Segment(bi,ei)),Trim::trim(Segment(ba,ea)),Trim::trim(Segment(bf,ef)));
}

Format::Format(Segment const& fmt)
    : buffer(), tokens(), anonymous(0)
{
    parseFormat(fmt);
}

void Format::print(ostream_t& os) const
{
    for (auto t : tokens)
        os << t;
}

bool Format::parseAnonymousAndIndex(Segment const& s, unsigned& index)
{
    switch (*s.begin)
    {
        case PARAM_ANONYMOUS:
            if (s.size() == 1)
                index = anonymous++;
            else if (parseUInt(s.sub(s.begin+1), index))
                anonymous = index+1;
            else return false;
        break;

        case PARAM_INDEX:
            if (s.size() > 1 && !parseUInt(s.sub(s.begin+1), index))
                return false;
        break;

        default: return false;
    }
    return true;
}

Segment::Location Format::parseAnonymousAndIndexEx(Segment const& s, unsigned& index)
{
    Segment::Location end = nullptr;

    auto const seek = [&s, &end](Segment::Location c)
    {
        Segment::Location e = c;
        for (; isIntDigit(*e) && e != s.end; ++e);
        return s.sub(c,end=e);
    };

    switch (*s.begin)
    {
        case ARG_ANONYMOUS:
            if (!isIntDigit(*(s.begin+1)))
            {
                end = s.begin+1;
                index = anonymous++;
            }
            else if (parseUInt(seek(s.begin+1), index))
                anonymous = index+1;
            else return nullptr;
        break;

        case ARG_INDEX:
            if (s.size() > 1 && !parseUInt(seek(s.begin+1), index))
                return nullptr;
        break;

        default:break;
    }
    return end;
}

void Format::parseFormat(Segment const& s)
{
    tokenize::Closed(PARAM_OPEN, PARAM_CLOSE).tokenizeFull(s,
    [this](Segment const& s)
    {
        enum { PARAMETER=1, ALIGNMENT=1, FORMAT=2 };

        auto segments = splitParameter(s);
        auto& seg_parameter = std::get<0>(segments);
        auto& seg_alignment = std::get<1>(segments);
        auto& seg_format    = std::get<2>(segments);

        unsigned parameter_index;
        if (!seg_parameter.empty() && (parseAnonymousAndIndex(seg_parameter, parameter_index) || tryParse(seg_parameter, parameter_index)))
        {
            Parameter parameter;

            unsigned index;
            if (!seg_alignment.empty())
            {
                if (parseAnonymousAndIndex(seg_alignment, index))
                    parameter.alignment = alignments.addUnknown(index);
                else
                    parameter.alignment = alignments.addKnown(seg_alignment);
            }

            if (!seg_format.empty())
            {
                if (parseAnonymousAndIndex(seg_format, index))
                    parameter.transform = formats.addUnknown(index);
                else
                    parameter.transform = formats.addKnown(seg_format);
            }

            buffer.emplace_back();
            parameter.value = &buffer.back();
            tokens.emplace_back();
            parameter.token = &tokens.back();
            parameters.addUnknown(parameter_index,parameter);
        }
    },
    [this](Segment const& s)
    {
        auto last = s.begin;
        for (auto i = s.begin; i != s.end; ++i)
        {
            unsigned index;
            auto const end = parseAnonymousAndIndexEx(s.sub(i), index);

            if (end)
            {
                tokens.emplace_back(s.sub(last,i));
                last = end;

                Parameter parameter;
                buffer.emplace_back();
                parameter.value = &buffer.back();
                tokens.emplace_back();
                parameter.token = &tokens.back();
                parameters.addUnknown(index,parameter);
            }
        }

        tokens.emplace_back(last, s.end);
    });
}
