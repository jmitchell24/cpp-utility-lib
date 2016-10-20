// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include "PlyTypes.hpp"
#include "PlyNumber.hpp"

namespace util
{
namespace ply
{
    struct ElementDefinition;
    struct PropertyDefinition;

    struct PropertyDefinition
    {
        string_t   name;
        NumberType type;
        NumberType type_index;

        bool isList() const;
        bool isEqual(PropertyDefinition const& pd) const;
        bool operator== (PropertyDefinition const& pd) const;
        bool operator!= (PropertyDefinition const& pd) const;
    };

    struct ElementDefinition
    {
		typedef std::vector<PropertyDefinition> Properties;

        string_t   name;
        unsigned   size;
        Properties properties;
    };

    struct HeaderDefinition
    {
        typedef std::vector<ElementDefinition> Elements;
        Elements elements;
    };

    enum ContentType { BINARY, ASCII };
    enum Endianness  { BIG, LITTLE };

    Endianness getNativeEndianness();
    Char const* getNativeEndiannessString();
}
}
