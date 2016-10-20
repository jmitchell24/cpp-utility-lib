// Copyright 2014, James Mitchell, All rights reserved.

#include "PlyDefinition.hpp"
using namespace util::ply;
using namespace std;

Endianness util::ply::getNativeEndianness()
{
    union
    {
        int i;
        char c[sizeof(int)];
    } u;

    u.i = 0x1;
    if (u.c[0] == 1)
        return LITTLE;
    return BIG;
}

Char const* util::ply::getNativeEndiannessString()
{
    if (getNativeEndianness() == LITTLE)
        return ("binary_little_endian");
    return ("binary_big_endian");
}

bool PropertyDefinition::isList() const
{
    return type_index != INVALID;
}

bool PropertyDefinition::isEqual(PropertyDefinition const& pd) const
{
    return name == pd.name && type == pd.type && type_index == pd.type_index;
}

bool PropertyDefinition::operator== (PropertyDefinition const& pd) const
{
    return isEqual(pd);
}

bool PropertyDefinition::operator!= (PropertyDefinition const& pd) const
{
    return !isEqual(pd);
}
